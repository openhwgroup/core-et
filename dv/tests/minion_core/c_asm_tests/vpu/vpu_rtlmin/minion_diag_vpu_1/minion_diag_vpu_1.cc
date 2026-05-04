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
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00000
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x0e000007,                                                  // Trailing 1s:                                /// 00008
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0000c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00010
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00014
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00018
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0001c
			 0x55555555,                                                  // 4 random values                             /// 00020
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00024
			 0x0c400000,                                                  // Leading 1s:                                 /// 00028
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0002c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00030
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00034
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00038
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0003c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00040
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00044
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00048
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0004c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00054
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00058
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0005c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00060
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00064
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00068
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0006c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00070
			 0x0c780000,                                                  // Leading 1s:                                 /// 00074
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00078
			 0x0e000003,                                                  // Trailing 1s:                                /// 0007c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00084
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00088
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00090
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00094
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x0e000007,                                                  // Trailing 1s:                                /// 0009c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 000a0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a4
			 0x7f800000,                                                  // inf                                         /// 000a8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 000b0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000b4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 000b8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000bc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 000c0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 000c4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000cc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 000d0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000d4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000d8
			 0x0e000001,                                                  // Trailing 1s:                                /// 000dc
			 0xffc00001,                                                  // -signaling NaN                              /// 000e0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000e4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000e8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ec
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 000f0
			 0x0e000003,                                                  // Trailing 1s:                                /// 000f4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f8
			 0x0e000003,                                                  // Trailing 1s:                                /// 000fc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00100
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00104
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00108
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0010c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00110
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00114
			 0x0e000007,                                                  // Trailing 1s:                                /// 00118
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0011c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00120
			 0x0c600000,                                                  // Leading 1s:                                 /// 00124
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00128
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0012c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00130
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00138
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0013c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00140
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00144
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0014c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00150
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00154
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00158
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0015c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00160
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00164
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00168
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00174
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00178
			 0x7fc00001,                                                  // signaling NaN                               /// 0017c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00180
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00184
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00188
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0018c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00190
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00198
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0019c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001a0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001a4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 001a8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ac
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 001b0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 001b8
			 0xbf028f5c,                                                  // -0.51                                       /// 001bc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001c0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001c4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001c8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 001cc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001d4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d8
			 0x55555555,                                                  // 4 random values                             /// 001dc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001e0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001e4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 001ec
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001f4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001f8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001fc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00200
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00204
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00208
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0020c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00210
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00214
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0021c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00220
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00224
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00228
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00230
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00234
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00238
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0023c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00240
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00244
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00248
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0024c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00250
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00254
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00258
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0025c
			 0x3f028f5c,                                                  // 0.51                                        /// 00260
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00264
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0026c
			 0xffc00001,                                                  // -signaling NaN                              /// 00270
			 0x0e000001,                                                  // Trailing 1s:                                /// 00274
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00278
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0027c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00280
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00284
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00288
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00290
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00298
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0029c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002a0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002a8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 002ac
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002b0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002b4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 002b8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002bc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002c0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 002c4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002c8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002cc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 002d4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002dc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002e4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002e8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 002ec
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002f4
			 0x3f028f5c,                                                  // 0.51                                        /// 002f8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002fc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00300
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00304
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00308
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0030c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00310
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00314
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00318
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0031c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00320
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00324
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00328
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0032c
			 0x7fc00001,                                                  // signaling NaN                               /// 00330
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00334
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00338
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0033c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00344
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00348
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0xffc00001,                                                  // -signaling NaN                              /// 00350
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00354
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00358
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0035c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00364
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00368
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0036c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00370
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00374
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00378
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0037c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00380
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00384
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00388
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0038c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00390
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 003a0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003a8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b0
			 0x0c700000,                                                  // Leading 1s:                                 /// 003b4
			 0x0c400000,                                                  // Leading 1s:                                 /// 003b8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 003c0
			 0xcb000000,                                                  // -8388608.0                                  /// 003c4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 003c8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003d4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003dc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003e0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 003e4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003e8
			 0x0c600000,                                                  // Leading 1s:                                 /// 003ec
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003f0
			 0xff800000,                                                  // -inf                                        /// 003f4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 003f8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003fc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00400
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00404
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00408
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0040c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00410
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00414
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00418
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0041c
			 0xff800000,                                                  // -inf                                        /// 00420
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00424
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00428
			 0x00011111,                                                  // 9.7958E-41                                  /// 0042c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00430
			 0xff800000,                                                  // -inf                                        /// 00434
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00438
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00440
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00444
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00448
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0044c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00450
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00454
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00458
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0045c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00460
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00464
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00468
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0046c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00470
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00474
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00478
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0047c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00480
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00484
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00488
			 0x0e000007,                                                  // Trailing 1s:                                /// 0048c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00490
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00494
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00498
			 0xbf028f5c,                                                  // -0.51                                       /// 0049c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 004a4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004a8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004ac
			 0x0e00001f,                                                  // Trailing 1s:                                /// 004b0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004b4
			 0x80000000,                                                  // -zero                                       /// 004b8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 004bc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004c8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004cc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004d0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004d4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 004d8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004dc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004e0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004e4
			 0x80011111,                                                  // -9.7958E-41                                 /// 004e8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 004f8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00500
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00504
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00508
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0050c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00510
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00514
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00518
			 0xffc00001,                                                  // -signaling NaN                              /// 0051c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00520
			 0x0e000007,                                                  // Trailing 1s:                                /// 00524
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00528
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00530
			 0x0e000003,                                                  // Trailing 1s:                                /// 00534
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00538
			 0x00011111,                                                  // 9.7958E-41                                  /// 0053c
			 0x80000000,                                                  // -zero                                       /// 00540
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00544
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00548
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0054c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00550
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00554
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00558
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0055c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00560
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00564
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00568
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0056c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00570
			 0xffc00001,                                                  // -signaling NaN                              /// 00574
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00578
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0057c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00580
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00584
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00588
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0058c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00590
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00594
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00598
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0059c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005a0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005a4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005a8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 005ac
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005b4
			 0x0e000003,                                                  // Trailing 1s:                                /// 005b8
			 0x0c700000,                                                  // Leading 1s:                                 /// 005bc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 005c4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005c8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 005cc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005d0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005d4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005d8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005dc
			 0x0e000003,                                                  // Trailing 1s:                                /// 005e0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 005e8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005f0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 005f4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 005f8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 005fc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00600
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00604
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00608
			 0x55555555,                                                  // 4 random values                             /// 0060c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00610
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00614
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00618
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00620
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00624
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0062c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00630
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00634
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00638
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0063c
			 0xffc00001,                                                  // -signaling NaN                              /// 00640
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00644
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00648
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0064c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00650
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00654
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00658
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0065c
			 0xcb000000,                                                  // -8388608.0                                  /// 00660
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00664
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00668
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0066c
			 0x4b000000,                                                  // 8388608.0                                   /// 00670
			 0x0c400000,                                                  // Leading 1s:                                 /// 00674
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0067c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00680
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00684
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00688
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0068c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00690
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00694
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00698
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0069c
			 0x7f800000,                                                  // inf                                         /// 006a0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a4
			 0x0c600000,                                                  // Leading 1s:                                 /// 006a8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006ac
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006b0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 006b4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006b8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006bc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0x0c600000,                                                  // Leading 1s:                                 /// 006c8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006cc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006d0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 006d4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006d8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006dc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006e0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006e4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006e8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006ec
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 006f0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006f4
			 0x0c400000,                                                  // Leading 1s:                                 /// 006f8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 006fc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x33333333,                                                  // 4 random values                             /// 00704
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00708
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0070c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00710
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00718
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0071c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00720
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00728
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0072c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00730
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00734
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x0e000003,                                                  // Trailing 1s:                                /// 0073c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00740
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00744
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00748
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0074c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00750
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00754
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00758
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0075c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00764
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00768
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0076c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00770
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00774
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00778
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0077c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00780
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0x0e000003,                                                  // Trailing 1s:                                /// 00788
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00790
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00794
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00798
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0079c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007a0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007a4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007a8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007ac
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007b8
			 0x80011111,                                                  // -9.7958E-41                                 /// 007bc
			 0x0e000007,                                                  // Trailing 1s:                                /// 007c0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007c8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 007cc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 007d0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007dc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e0
			 0xcb000000,                                                  // -8388608.0                                  /// 007e4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007ec
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 007f0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007f4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 007f8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00800
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00804
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00808
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0080c
			 0x55555555,                                                  // 4 random values                             /// 00810
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00814
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00818
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0081c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00820
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00824
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00828
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0082c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00830
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00834
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00838
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0083c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00840
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0xcb000000,                                                  // -8388608.0                                  /// 00848
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0084c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00850
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00854
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00858
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0085c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00860
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00864
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00868
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0086c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00870
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00874
			 0x0c780000,                                                  // Leading 1s:                                 /// 00878
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0087c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00884
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00888
			 0x0e000007,                                                  // Trailing 1s:                                /// 0088c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00894
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00898
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0089c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008a0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008a8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008b8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008c4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008c8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008cc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 008d8
			 0x0e000003,                                                  // Trailing 1s:                                /// 008dc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 008e0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008e8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008ec
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008f0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 008f4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008f8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 008fc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00900
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00904
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00908
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00910
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00918
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0091c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00920
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00924
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00928
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0092c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00930
			 0x80011111,                                                  // -9.7958E-41                                 /// 00934
			 0x0e000001,                                                  // Trailing 1s:                                /// 00938
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0093c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00940
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00944
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00948
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0094c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00950
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00954
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0095c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00960
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00964
			 0x0e000003,                                                  // Trailing 1s:                                /// 00968
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0096c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00970
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00974
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0097c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00980
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00984
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00988
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0098c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00990
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00998
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0099c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009a0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 009a4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 009a8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 009ac
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009b0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 009b4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 009b8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009bc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009c0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009c4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 009c8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009d0
			 0xbf028f5c,                                                  // -0.51                                       /// 009d4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 009d8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 009dc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009e0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009e4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009e8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009ec
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 009f4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009f8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 009fc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a04
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a08
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a0c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a10
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a18
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a1c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a20
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a24
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a28
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a2c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a30
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a38
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a3c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a40
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a44
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a48
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a4c
			 0x7f800000,                                                  // inf                                         /// 00a50
			 0x4b000000,                                                  // 8388608.0                                   /// 00a54
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a5c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a60
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a6c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a70
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a74
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a78
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a7c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a84
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a88
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a8c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a90
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a94
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a98
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a9c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00aa0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00aa4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00aa8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00aac
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0xffc00001,                                                  // -signaling NaN                              /// 00ab4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ab8
			 0x00000000,                                                  // zero                                        /// 00abc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ac4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ac8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00acc
			 0xcb000000,                                                  // -8388608.0                                  /// 00ad0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ad4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00adc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ae8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0x0c780000,                                                  // Leading 1s:                                 /// 00af0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00af4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00af8
			 0x33333333,                                                  // 4 random values                             /// 00afc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b04
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b08
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b0c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b10
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b14
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b18
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b1c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b20
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b24
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b28
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b30
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b34
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b3c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b40
			 0x55555555,                                                  // 4 random values                             /// 00b44
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b48
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b4c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b50
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b58
			 0xff800000,                                                  // -inf                                        /// 00b5c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b60
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b64
			 0xff800000,                                                  // -inf                                        /// 00b68
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b6c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b74
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b78
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b7c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b84
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b88
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b8c
			 0x7f800000,                                                  // inf                                         /// 00b90
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b94
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b98
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b9c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ba0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ba4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ba8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00bb0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00bb4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00bbc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bc0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00bc4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00bc8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00bcc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00bd0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00bd4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00bd8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00bdc
			 0x33333333,                                                  // 4 random values                             /// 00be0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00be4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00be8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bec
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00bf0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00bf4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00bf8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00bfc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c00
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c08
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c0c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c10
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c18
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c1c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c20
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c24
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c28
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c2c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c30
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c34
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c38
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c3c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c40
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c44
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c48
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c4c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c50
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c54
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c58
			 0x3f028f5c,                                                  // 0.51                                        /// 00c5c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c64
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c68
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c6c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c70
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c74
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c78
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c7c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c80
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c84
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c8c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c90
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c94
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c98
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c9c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ca0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ca8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00cac
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00cb4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00cb8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cbc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00cc0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00cc4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00cc8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00cd4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ce0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ce4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ce8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00cec
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00cf0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00cf4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cf8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00cfc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d00
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d04
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d08
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d0c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d10
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d14
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d18
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d1c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d20
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d24
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d28
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d2c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d30
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d34
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d38
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d40
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d48
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d4c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d50
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d58
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d5c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d60
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d64
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d68
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d6c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d70
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d74
			 0x55555555,                                                  // 4 random values                             /// 00d78
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d7c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d80
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d84
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d88
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d8c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d94
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d98
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d9c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00da8
			 0xffc00001,                                                  // -signaling NaN                              /// 00dac
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00db0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00db4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00db8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00dbc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00dc0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00dc4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00dc8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00dcc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00dd0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00dd4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00dd8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ddc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00df0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00df4
			 0x7fc00001,                                                  // signaling NaN                               /// 00df8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00dfc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e00
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e04
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e08
			 0x7fc00001,                                                  // signaling NaN                               /// 00e0c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e10
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e14
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e18
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e20
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e24
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e28
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e2c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e30
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e34
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e38
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e40
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e44
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e48
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e4c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e50
			 0x33333333,                                                  // 4 random values                             /// 00e54
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e58
			 0xbf028f5c,                                                  // -0.51                                       /// 00e5c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e60
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e64
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e68
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e74
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e78
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e7c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e84
			 0x80000000,                                                  // -zero                                       /// 00e88
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e8c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e90
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e94
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e98
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e9c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ea8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00eb0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00eb4
			 0x33333333,                                                  // 4 random values                             /// 00eb8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ebc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ec0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ec4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ec8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ecc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ed0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ed4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00edc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ee0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ee4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ee8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00eec
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ef0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ef4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00efc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f00
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f04
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f08
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f0c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f10
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f14
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f18
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f1c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f24
			 0x3f028f5c,                                                  // 0.51                                        /// 00f28
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f2c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f30
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f40
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f44
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f48
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f4c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f50
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f54
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f58
			 0x33333333,                                                  // 4 random values                             /// 00f5c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f60
			 0x7fc00001,                                                  // signaling NaN                               /// 00f64
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f68
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f6c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f70
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f74
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f78
			 0x3f028f5c,                                                  // 0.51                                        /// 00f7c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f80
			 0xcb000000,                                                  // -8388608.0                                  /// 00f84
			 0xcb000000,                                                  // -8388608.0                                  /// 00f88
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f8c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f94
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f98
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00fa0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fa8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fac
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fb0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fb4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fbc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00fc0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00fc4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00fc8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00fcc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00fd0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00fd4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00fdc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00fe0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fe4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fe8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ff0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ff4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ff8
			 0x0f7ffffe                                                  // all bit of mantissa set upto -3ulp          /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x3bc55e7c, /// 0x0
			 0xe3c095ee, /// 0x4
			 0xcb918a9e, /// 0x8
			 0x10145050, /// 0xc
			 0xbc5892f8, /// 0x10
			 0x1e0b49b8, /// 0x14
			 0xc808d8c4, /// 0x18
			 0xdf7548b0, /// 0x1c
			 0x085ac830, /// 0x20
			 0xff2c575f, /// 0x24
			 0xa4941d35, /// 0x28
			 0xb2d2e668, /// 0x2c
			 0xa0d26064, /// 0x30
			 0x6b995856, /// 0x34
			 0x4cb2f2fa, /// 0x38
			 0x2490a1aa, /// 0x3c
			 0x6214bbd5, /// 0x40
			 0x4aa679bb, /// 0x44
			 0xe0e982ab, /// 0x48
			 0xfb99a6e0, /// 0x4c
			 0xb9b19797, /// 0x50
			 0x4adebe7c, /// 0x54
			 0x7b769a59, /// 0x58
			 0x45e9a2d0, /// 0x5c
			 0xbf538ce7, /// 0x60
			 0x514beb58, /// 0x64
			 0x6f01d443, /// 0x68
			 0x29455e23, /// 0x6c
			 0x813f14e8, /// 0x70
			 0x949024cd, /// 0x74
			 0x17e138b6, /// 0x78
			 0xfd4d7b0d, /// 0x7c
			 0x59048617, /// 0x80
			 0x0d755123, /// 0x84
			 0xb4ab0d92, /// 0x88
			 0xf77ff9b7, /// 0x8c
			 0x08aec121, /// 0x90
			 0xce253b73, /// 0x94
			 0x2b8bea59, /// 0x98
			 0xa37cc0ff, /// 0x9c
			 0xa9e2b086, /// 0xa0
			 0xf363b0f1, /// 0xa4
			 0x9bef2135, /// 0xa8
			 0xb6de7c5a, /// 0xac
			 0xb1407bf0, /// 0xb0
			 0xe9904979, /// 0xb4
			 0x07fed03f, /// 0xb8
			 0x35710dbd, /// 0xbc
			 0xb85ac01b, /// 0xc0
			 0x55030416, /// 0xc4
			 0x6ae6cbab, /// 0xc8
			 0x75fe5115, /// 0xcc
			 0xd5949813, /// 0xd0
			 0xbb8335c1, /// 0xd4
			 0xb7dd61b8, /// 0xd8
			 0xb6024c46, /// 0xdc
			 0xb40b3500, /// 0xe0
			 0xfe88ee4d, /// 0xe4
			 0x191210d1, /// 0xe8
			 0xebc81091, /// 0xec
			 0xc36f7076, /// 0xf0
			 0xf4912ff9, /// 0xf4
			 0xead5e76c, /// 0xf8
			 0x060f0a87, /// 0xfc
			 0x2c8b6af4, /// 0x100
			 0x4c898568, /// 0x104
			 0x1674ae1e, /// 0x108
			 0x49eb6e32, /// 0x10c
			 0x2ace63b5, /// 0x110
			 0xe0e7f798, /// 0x114
			 0x9e6ddd61, /// 0x118
			 0x57878288, /// 0x11c
			 0xf2e07ed5, /// 0x120
			 0xef1f2e84, /// 0x124
			 0x743c8621, /// 0x128
			 0x0824cf6f, /// 0x12c
			 0xeb240b0c, /// 0x130
			 0x7aea7bd8, /// 0x134
			 0x203cd3ab, /// 0x138
			 0x31c26b81, /// 0x13c
			 0x443969c9, /// 0x140
			 0xbfd7bf74, /// 0x144
			 0x2e3bf48c, /// 0x148
			 0xfc762b1c, /// 0x14c
			 0x8bd7aedf, /// 0x150
			 0x7372ba7c, /// 0x154
			 0x09ec7dae, /// 0x158
			 0xcb1dad5b, /// 0x15c
			 0x4bf24d44, /// 0x160
			 0x4b414002, /// 0x164
			 0xfe001997, /// 0x168
			 0x2a8ff154, /// 0x16c
			 0x7d88e6a9, /// 0x170
			 0x47f3e69c, /// 0x174
			 0x0b1885bb, /// 0x178
			 0x01280b5c, /// 0x17c
			 0xaf313ff4, /// 0x180
			 0xee36b908, /// 0x184
			 0x1549c10f, /// 0x188
			 0x814f2e27, /// 0x18c
			 0xa14d4e94, /// 0x190
			 0xa41bb802, /// 0x194
			 0x0f68c8ce, /// 0x198
			 0xd3c5ea71, /// 0x19c
			 0xa36c7775, /// 0x1a0
			 0xc8d3a14e, /// 0x1a4
			 0xb2f351ea, /// 0x1a8
			 0x5ba030ed, /// 0x1ac
			 0x16ebc3bf, /// 0x1b0
			 0x521dc5c0, /// 0x1b4
			 0xa1915a28, /// 0x1b8
			 0x372c28ec, /// 0x1bc
			 0xe0261f77, /// 0x1c0
			 0x2733685d, /// 0x1c4
			 0xbb0002ae, /// 0x1c8
			 0x8eb925e3, /// 0x1cc
			 0x54c386e8, /// 0x1d0
			 0x104dc0bf, /// 0x1d4
			 0x497d4f67, /// 0x1d8
			 0x70ce777b, /// 0x1dc
			 0xce15c274, /// 0x1e0
			 0x2681099c, /// 0x1e4
			 0x2fe10ae8, /// 0x1e8
			 0x75ed9a8b, /// 0x1ec
			 0xb946765f, /// 0x1f0
			 0x0b481e7d, /// 0x1f4
			 0xe74945d6, /// 0x1f8
			 0xcd66e7e5, /// 0x1fc
			 0x5eae29df, /// 0x200
			 0x22cf7254, /// 0x204
			 0x85dce330, /// 0x208
			 0x3f196c56, /// 0x20c
			 0xbccb0f9d, /// 0x210
			 0x9d9b73d2, /// 0x214
			 0xb2102a90, /// 0x218
			 0x43f15c57, /// 0x21c
			 0x55c0a7c1, /// 0x220
			 0xc16ba926, /// 0x224
			 0x8eb00d33, /// 0x228
			 0x4ad55957, /// 0x22c
			 0xe302af3e, /// 0x230
			 0x8c332d17, /// 0x234
			 0x34ce22c7, /// 0x238
			 0x435b4245, /// 0x23c
			 0x4069a3cc, /// 0x240
			 0x9dc42e0b, /// 0x244
			 0xc14bcb6b, /// 0x248
			 0x0d8b95e5, /// 0x24c
			 0x0827b157, /// 0x250
			 0xcf33ed71, /// 0x254
			 0x8d005443, /// 0x258
			 0x84e4a524, /// 0x25c
			 0xbda069e1, /// 0x260
			 0x1a110e98, /// 0x264
			 0xb083b2ec, /// 0x268
			 0x3741761b, /// 0x26c
			 0xf4e9624a, /// 0x270
			 0x3d6a93a2, /// 0x274
			 0x7d76da12, /// 0x278
			 0xb1f45ef0, /// 0x27c
			 0xfd2c0e2c, /// 0x280
			 0x3b016be0, /// 0x284
			 0xed1af6f4, /// 0x288
			 0x92295bfa, /// 0x28c
			 0xb39beda4, /// 0x290
			 0x0b85b223, /// 0x294
			 0x3158428f, /// 0x298
			 0x6f546099, /// 0x29c
			 0x413bac6c, /// 0x2a0
			 0xe11f9f32, /// 0x2a4
			 0x16f74d13, /// 0x2a8
			 0x3ad8e7eb, /// 0x2ac
			 0x4d74b7de, /// 0x2b0
			 0xf1fcf309, /// 0x2b4
			 0x5b30993d, /// 0x2b8
			 0x399b8442, /// 0x2bc
			 0x26c640ea, /// 0x2c0
			 0xf1bed36e, /// 0x2c4
			 0x73a5de44, /// 0x2c8
			 0xbe073daa, /// 0x2cc
			 0xcbc26f9d, /// 0x2d0
			 0xbcc4ffca, /// 0x2d4
			 0x60140e6d, /// 0x2d8
			 0x38ed6e06, /// 0x2dc
			 0x532847d3, /// 0x2e0
			 0x46541c84, /// 0x2e4
			 0xd62a1484, /// 0x2e8
			 0xf44e7061, /// 0x2ec
			 0x18a903df, /// 0x2f0
			 0xb1900569, /// 0x2f4
			 0xafae6401, /// 0x2f8
			 0x7925ba91, /// 0x2fc
			 0xcb86bd27, /// 0x300
			 0xb9cbf888, /// 0x304
			 0xe2ffc386, /// 0x308
			 0x396d6149, /// 0x30c
			 0xce061213, /// 0x310
			 0x23d634e7, /// 0x314
			 0xfe16b002, /// 0x318
			 0x60af3ad8, /// 0x31c
			 0xdd5f6868, /// 0x320
			 0x9e61aae0, /// 0x324
			 0x6551d461, /// 0x328
			 0xb300793d, /// 0x32c
			 0xf05a4af6, /// 0x330
			 0xf153e522, /// 0x334
			 0x25943dd3, /// 0x338
			 0xd7e033ca, /// 0x33c
			 0x2407eb6e, /// 0x340
			 0xe9a5a3ca, /// 0x344
			 0xf538425b, /// 0x348
			 0xd77e1098, /// 0x34c
			 0xf4d33e8a, /// 0x350
			 0xb8752d91, /// 0x354
			 0xe7b5e2b3, /// 0x358
			 0xd5c76652, /// 0x35c
			 0x04d41878, /// 0x360
			 0xea4e44a5, /// 0x364
			 0x422bb9df, /// 0x368
			 0xa9841e3e, /// 0x36c
			 0x9b2f82d8, /// 0x370
			 0x0a21cb2c, /// 0x374
			 0xceeafb32, /// 0x378
			 0xe7c8b9d4, /// 0x37c
			 0x38e2b2b7, /// 0x380
			 0xa0c28f2e, /// 0x384
			 0xfe1da9fd, /// 0x388
			 0xacd40268, /// 0x38c
			 0xcc188036, /// 0x390
			 0x6b2350d5, /// 0x394
			 0xa7d835bf, /// 0x398
			 0xd9c3d56b, /// 0x39c
			 0xdc93f02c, /// 0x3a0
			 0xde2ace07, /// 0x3a4
			 0xfb22a2a4, /// 0x3a8
			 0xb95d8c4d, /// 0x3ac
			 0x35ae6000, /// 0x3b0
			 0xd11b10c0, /// 0x3b4
			 0x6cc33917, /// 0x3b8
			 0x60fc0f30, /// 0x3bc
			 0x9a12f1a8, /// 0x3c0
			 0x769fd5ff, /// 0x3c4
			 0xec90c2cc, /// 0x3c8
			 0xd7ae0e58, /// 0x3cc
			 0x4ca26355, /// 0x3d0
			 0xef6d9da9, /// 0x3d4
			 0xeb5fa4f9, /// 0x3d8
			 0x43bd3a82, /// 0x3dc
			 0x34143920, /// 0x3e0
			 0xfb75c4ff, /// 0x3e4
			 0x9f58e609, /// 0x3e8
			 0x8509c658, /// 0x3ec
			 0x7b625fa0, /// 0x3f0
			 0x2c701411, /// 0x3f4
			 0x35d50f47, /// 0x3f8
			 0xb9331beb, /// 0x3fc
			 0x839b0114, /// 0x400
			 0x1a00150d, /// 0x404
			 0x8aa65931, /// 0x408
			 0x107cea68, /// 0x40c
			 0x8862136f, /// 0x410
			 0xe639bb19, /// 0x414
			 0x29400e7f, /// 0x418
			 0xcfdc8730, /// 0x41c
			 0x489d7baa, /// 0x420
			 0x657842a8, /// 0x424
			 0x48e68c87, /// 0x428
			 0x010e78c0, /// 0x42c
			 0xf1a8a233, /// 0x430
			 0x0dd1506d, /// 0x434
			 0xb7751d43, /// 0x438
			 0xcc464a84, /// 0x43c
			 0x8f8fd659, /// 0x440
			 0xc0ceedfb, /// 0x444
			 0xd240e18c, /// 0x448
			 0x8f66a399, /// 0x44c
			 0x93780e03, /// 0x450
			 0x6642acaf, /// 0x454
			 0xbc928d15, /// 0x458
			 0x48edc700, /// 0x45c
			 0x3ee3f1a0, /// 0x460
			 0x3d68977d, /// 0x464
			 0x863c9604, /// 0x468
			 0x97edbc07, /// 0x46c
			 0x9b9b5cbb, /// 0x470
			 0xccf221fe, /// 0x474
			 0xf1d72bb9, /// 0x478
			 0x39a78a37, /// 0x47c
			 0x8341c66c, /// 0x480
			 0x814e7818, /// 0x484
			 0x9a9d4d2c, /// 0x488
			 0xf4145889, /// 0x48c
			 0xbf769ef6, /// 0x490
			 0x3bdbea37, /// 0x494
			 0xcac7c1b7, /// 0x498
			 0xcd6fd4de, /// 0x49c
			 0x459ea55f, /// 0x4a0
			 0xc9497655, /// 0x4a4
			 0x6a51e8f0, /// 0x4a8
			 0x0ba52275, /// 0x4ac
			 0x3d4a9f1b, /// 0x4b0
			 0x51a7af1a, /// 0x4b4
			 0x65eb9214, /// 0x4b8
			 0xf6214816, /// 0x4bc
			 0xf695be06, /// 0x4c0
			 0xc0ebb453, /// 0x4c4
			 0xc55f3ff3, /// 0x4c8
			 0x097bb3b3, /// 0x4cc
			 0xe9a76ec8, /// 0x4d0
			 0xef1fe575, /// 0x4d4
			 0xce167686, /// 0x4d8
			 0xe03eb009, /// 0x4dc
			 0xa31943c1, /// 0x4e0
			 0x92de5cfa, /// 0x4e4
			 0xe8abc87a, /// 0x4e8
			 0x35cef6ae, /// 0x4ec
			 0x24b34faa, /// 0x4f0
			 0x38d5545c, /// 0x4f4
			 0xa97c4e14, /// 0x4f8
			 0xbb0c4af3, /// 0x4fc
			 0x1fcb8a4c, /// 0x500
			 0x4d41e472, /// 0x504
			 0xb1c9b564, /// 0x508
			 0xdd980c74, /// 0x50c
			 0xe7059fe1, /// 0x510
			 0xdfcfb944, /// 0x514
			 0xc227f4a9, /// 0x518
			 0x1e4ad198, /// 0x51c
			 0x4d48bc72, /// 0x520
			 0x2228151d, /// 0x524
			 0x178d85ae, /// 0x528
			 0xabe1bb33, /// 0x52c
			 0x7fc69b2a, /// 0x530
			 0x10b9306b, /// 0x534
			 0x2b89ea58, /// 0x538
			 0x7bfcab33, /// 0x53c
			 0x37e18e42, /// 0x540
			 0x3813be84, /// 0x544
			 0xc1381617, /// 0x548
			 0xf069a30a, /// 0x54c
			 0xb3244ee9, /// 0x550
			 0x8b855cbd, /// 0x554
			 0xae528c7c, /// 0x558
			 0x1562d3a9, /// 0x55c
			 0x920a76b7, /// 0x560
			 0xc2f19dc9, /// 0x564
			 0xe0e0c202, /// 0x568
			 0x41d3b81e, /// 0x56c
			 0x2919645f, /// 0x570
			 0xb96106b5, /// 0x574
			 0xc2c67a03, /// 0x578
			 0xb13396b3, /// 0x57c
			 0xaa5efc7a, /// 0x580
			 0xc0cb73f2, /// 0x584
			 0x36cd6fa2, /// 0x588
			 0xe6225da2, /// 0x58c
			 0x2a3bfa86, /// 0x590
			 0x118ba422, /// 0x594
			 0xec8fe160, /// 0x598
			 0x5331672b, /// 0x59c
			 0x2819d219, /// 0x5a0
			 0xe48ce057, /// 0x5a4
			 0xca8cfa07, /// 0x5a8
			 0x0562d010, /// 0x5ac
			 0x982dc971, /// 0x5b0
			 0x85650d84, /// 0x5b4
			 0x994aec4a, /// 0x5b8
			 0x889c475f, /// 0x5bc
			 0x7c0822bc, /// 0x5c0
			 0x47be2f0b, /// 0x5c4
			 0xff7ac635, /// 0x5c8
			 0x62a6c316, /// 0x5cc
			 0x02500a19, /// 0x5d0
			 0x2511bc64, /// 0x5d4
			 0x338cd3fc, /// 0x5d8
			 0x1a1ebcf7, /// 0x5dc
			 0x88710c98, /// 0x5e0
			 0x84f0b084, /// 0x5e4
			 0xe869d0a5, /// 0x5e8
			 0xd7950759, /// 0x5ec
			 0x47ac2e85, /// 0x5f0
			 0x85b54ad5, /// 0x5f4
			 0xe9e377c2, /// 0x5f8
			 0x713f887d, /// 0x5fc
			 0x8df01785, /// 0x600
			 0x2ac8087d, /// 0x604
			 0x00fa039f, /// 0x608
			 0xfa4f0890, /// 0x60c
			 0x51799337, /// 0x610
			 0xee2142d4, /// 0x614
			 0x595cce6a, /// 0x618
			 0xd0675769, /// 0x61c
			 0x900463e1, /// 0x620
			 0xb6a6c63e, /// 0x624
			 0xd37edef7, /// 0x628
			 0x5a436de1, /// 0x62c
			 0x05ac4887, /// 0x630
			 0xb2be30ad, /// 0x634
			 0x66d92dd9, /// 0x638
			 0x64a2c05f, /// 0x63c
			 0x4f304426, /// 0x640
			 0x614463b5, /// 0x644
			 0x4a9b021f, /// 0x648
			 0xc56b7a79, /// 0x64c
			 0x91eaa6eb, /// 0x650
			 0x670e10e3, /// 0x654
			 0x2144523e, /// 0x658
			 0x7d418202, /// 0x65c
			 0x3cc4f83f, /// 0x660
			 0x7b9ccc77, /// 0x664
			 0xfd9c95b7, /// 0x668
			 0x684a6fbc, /// 0x66c
			 0x328925b2, /// 0x670
			 0x34a832bb, /// 0x674
			 0xafdb2500, /// 0x678
			 0x00880456, /// 0x67c
			 0x58cc7601, /// 0x680
			 0x77ee98b9, /// 0x684
			 0x824abd9a, /// 0x688
			 0xeebcd179, /// 0x68c
			 0x06af34e4, /// 0x690
			 0x32bb3250, /// 0x694
			 0x25fae828, /// 0x698
			 0x5b6b6ab2, /// 0x69c
			 0x600e3650, /// 0x6a0
			 0x965b9089, /// 0x6a4
			 0x9214e2be, /// 0x6a8
			 0x640fed77, /// 0x6ac
			 0xcc9a1c6a, /// 0x6b0
			 0x101a5264, /// 0x6b4
			 0xed6526e6, /// 0x6b8
			 0xa6d69523, /// 0x6bc
			 0xc97d6e3c, /// 0x6c0
			 0x7842a995, /// 0x6c4
			 0x776146ca, /// 0x6c8
			 0x4099011d, /// 0x6cc
			 0x6f4b4b61, /// 0x6d0
			 0x94ed19a2, /// 0x6d4
			 0xb91dd9f0, /// 0x6d8
			 0x4b159d94, /// 0x6dc
			 0x992e3070, /// 0x6e0
			 0x1a73e47e, /// 0x6e4
			 0xa1b8499b, /// 0x6e8
			 0x215846cb, /// 0x6ec
			 0xe715be84, /// 0x6f0
			 0x7b9a2714, /// 0x6f4
			 0x9e47c2f5, /// 0x6f8
			 0xe8736ac3, /// 0x6fc
			 0x133e0211, /// 0x700
			 0x22bd6738, /// 0x704
			 0x8b319f69, /// 0x708
			 0x0d945ee8, /// 0x70c
			 0xe6828c01, /// 0x710
			 0xf728a0cd, /// 0x714
			 0x0a2128b4, /// 0x718
			 0x6f052a77, /// 0x71c
			 0x219fe8e0, /// 0x720
			 0xf407b902, /// 0x724
			 0xa30d645d, /// 0x728
			 0x3ce9579e, /// 0x72c
			 0xbe24b4b9, /// 0x730
			 0xf10e62a4, /// 0x734
			 0x3698d9a2, /// 0x738
			 0xc01700b9, /// 0x73c
			 0xe55946b2, /// 0x740
			 0x14824a7a, /// 0x744
			 0x2fb6b9f6, /// 0x748
			 0x74cf5f6f, /// 0x74c
			 0x6bc3fc80, /// 0x750
			 0xae26d12f, /// 0x754
			 0x052ad80d, /// 0x758
			 0x650b0adc, /// 0x75c
			 0xf37f3505, /// 0x760
			 0x41af3a59, /// 0x764
			 0x2e496658, /// 0x768
			 0xa5216ff7, /// 0x76c
			 0xb01ffb69, /// 0x770
			 0x0206aae1, /// 0x774
			 0x3663abbd, /// 0x778
			 0xc7182533, /// 0x77c
			 0xf70270f4, /// 0x780
			 0x51487248, /// 0x784
			 0xa675fbdf, /// 0x788
			 0x07cb27e2, /// 0x78c
			 0xf1361658, /// 0x790
			 0x183a0f5d, /// 0x794
			 0x46d39b42, /// 0x798
			 0x2f3c9c47, /// 0x79c
			 0x44ca3b58, /// 0x7a0
			 0x8a6ceea7, /// 0x7a4
			 0x963501bb, /// 0x7a8
			 0x3b05e975, /// 0x7ac
			 0x0ffea161, /// 0x7b0
			 0xf0399a74, /// 0x7b4
			 0x487ad76a, /// 0x7b8
			 0x97aa208c, /// 0x7bc
			 0xa3c707e2, /// 0x7c0
			 0xebc12cdb, /// 0x7c4
			 0x2366ce33, /// 0x7c8
			 0x9a0ee0d7, /// 0x7cc
			 0x400f5920, /// 0x7d0
			 0xf74e0480, /// 0x7d4
			 0x5a6d7340, /// 0x7d8
			 0x603eee55, /// 0x7dc
			 0x54ab9cd0, /// 0x7e0
			 0x3b6ed626, /// 0x7e4
			 0x03ddcf8e, /// 0x7e8
			 0x5b1bf70c, /// 0x7ec
			 0xd46b12ce, /// 0x7f0
			 0x3431f4fe, /// 0x7f4
			 0xbd537417, /// 0x7f8
			 0x27f998fa, /// 0x7fc
			 0xceb70d49, /// 0x800
			 0xe13e098f, /// 0x804
			 0x233c1512, /// 0x808
			 0x7f2f9d0d, /// 0x80c
			 0xb6c03684, /// 0x810
			 0xa665be3f, /// 0x814
			 0x854fbb69, /// 0x818
			 0x9ed4bc87, /// 0x81c
			 0x3b72dac3, /// 0x820
			 0x79f95330, /// 0x824
			 0x080fc0a8, /// 0x828
			 0x536569af, /// 0x82c
			 0xac345e9c, /// 0x830
			 0x26cd095e, /// 0x834
			 0xb203c60c, /// 0x838
			 0x33eb073f, /// 0x83c
			 0x1d5366ff, /// 0x840
			 0xbe5ac809, /// 0x844
			 0x58be3a03, /// 0x848
			 0x76ea9565, /// 0x84c
			 0xe2fd6d42, /// 0x850
			 0xc6d1d59b, /// 0x854
			 0xd91d2886, /// 0x858
			 0x2332898c, /// 0x85c
			 0xeffa2945, /// 0x860
			 0xeb107710, /// 0x864
			 0xf9ae8bfe, /// 0x868
			 0x2df2c33f, /// 0x86c
			 0x64f9f284, /// 0x870
			 0x17029c8d, /// 0x874
			 0xced2fbf0, /// 0x878
			 0x210fb75f, /// 0x87c
			 0xc364a8aa, /// 0x880
			 0xe31bf31e, /// 0x884
			 0x3ad2b933, /// 0x888
			 0x1ba11faa, /// 0x88c
			 0x39d14dac, /// 0x890
			 0xac5ce4a0, /// 0x894
			 0x6d95f2b4, /// 0x898
			 0x22a14a92, /// 0x89c
			 0xffa2e01b, /// 0x8a0
			 0x476e1166, /// 0x8a4
			 0x3eb18a15, /// 0x8a8
			 0x67a0ef98, /// 0x8ac
			 0xf388c532, /// 0x8b0
			 0x005c6255, /// 0x8b4
			 0xe31d60fe, /// 0x8b8
			 0xb31bfbdf, /// 0x8bc
			 0x5be34178, /// 0x8c0
			 0x2b2971b0, /// 0x8c4
			 0x69062d35, /// 0x8c8
			 0x4a26da17, /// 0x8cc
			 0x8b9edfb3, /// 0x8d0
			 0x7c6cbadf, /// 0x8d4
			 0x230c8612, /// 0x8d8
			 0xdb216752, /// 0x8dc
			 0x09e48586, /// 0x8e0
			 0xdb89b05f, /// 0x8e4
			 0xf4ffa223, /// 0x8e8
			 0x16c0ba51, /// 0x8ec
			 0xc4302807, /// 0x8f0
			 0x6d830bfe, /// 0x8f4
			 0x19d2da9b, /// 0x8f8
			 0xa7072344, /// 0x8fc
			 0x96fd9dda, /// 0x900
			 0x82393610, /// 0x904
			 0xc3e6af23, /// 0x908
			 0x73ede391, /// 0x90c
			 0x093e6c93, /// 0x910
			 0xfbdb38ed, /// 0x914
			 0xd7001297, /// 0x918
			 0xc423f9cd, /// 0x91c
			 0xc8e62ba1, /// 0x920
			 0xe836837d, /// 0x924
			 0xe004d462, /// 0x928
			 0x3b53495d, /// 0x92c
			 0x1780af30, /// 0x930
			 0xd41f38f2, /// 0x934
			 0x2a1567b2, /// 0x938
			 0x0592ebfc, /// 0x93c
			 0x51f6533b, /// 0x940
			 0xbf0aca3f, /// 0x944
			 0x0b78d7c1, /// 0x948
			 0xd5afb24c, /// 0x94c
			 0xbb86ad5a, /// 0x950
			 0x8ea321a1, /// 0x954
			 0x21c4db88, /// 0x958
			 0x729f8bdb, /// 0x95c
			 0x5c9d02ff, /// 0x960
			 0x353c8f9f, /// 0x964
			 0x161ddfbf, /// 0x968
			 0x5e9b52cb, /// 0x96c
			 0x60eac688, /// 0x970
			 0x84adf7ab, /// 0x974
			 0x44f801e5, /// 0x978
			 0x22129812, /// 0x97c
			 0xdcc5f1eb, /// 0x980
			 0x9aef0eab, /// 0x984
			 0x9f36f91f, /// 0x988
			 0x3caa22f1, /// 0x98c
			 0x2a1a3836, /// 0x990
			 0xfe6e4f56, /// 0x994
			 0x599b885c, /// 0x998
			 0xabb1154b, /// 0x99c
			 0x06c0314c, /// 0x9a0
			 0x4405b972, /// 0x9a4
			 0x294c2496, /// 0x9a8
			 0x30308546, /// 0x9ac
			 0x41b1bab6, /// 0x9b0
			 0x132c6a89, /// 0x9b4
			 0x8eaa6392, /// 0x9b8
			 0xe6799fbb, /// 0x9bc
			 0x147632c6, /// 0x9c0
			 0x62cb9b39, /// 0x9c4
			 0x64cd90f5, /// 0x9c8
			 0xbcd071ac, /// 0x9cc
			 0xc8799cde, /// 0x9d0
			 0xc9b15917, /// 0x9d4
			 0x2a8f217b, /// 0x9d8
			 0x9ed539dc, /// 0x9dc
			 0x3b39df8a, /// 0x9e0
			 0x8c0db428, /// 0x9e4
			 0x01888509, /// 0x9e8
			 0x7e3ef197, /// 0x9ec
			 0xff9361ed, /// 0x9f0
			 0x04c38603, /// 0x9f4
			 0xefe042f0, /// 0x9f8
			 0x9f6add35, /// 0x9fc
			 0x93305286, /// 0xa00
			 0x1626f202, /// 0xa04
			 0xe4499414, /// 0xa08
			 0x5ff14b43, /// 0xa0c
			 0xed2a394d, /// 0xa10
			 0xdad0dc2b, /// 0xa14
			 0xca6c83fc, /// 0xa18
			 0xae4863f0, /// 0xa1c
			 0x927c6c21, /// 0xa20
			 0x62d0ebaa, /// 0xa24
			 0xc5a1a4a6, /// 0xa28
			 0x5325af2a, /// 0xa2c
			 0x0f377843, /// 0xa30
			 0x7e700a41, /// 0xa34
			 0xf34c0302, /// 0xa38
			 0x5d4d3bff, /// 0xa3c
			 0x32a4f1bd, /// 0xa40
			 0x21e0e6f3, /// 0xa44
			 0x10535645, /// 0xa48
			 0x5ec99021, /// 0xa4c
			 0x8ee888a2, /// 0xa50
			 0x7d8ef0b7, /// 0xa54
			 0x721074f2, /// 0xa58
			 0x7e954b22, /// 0xa5c
			 0xd308b6e7, /// 0xa60
			 0x4e73e461, /// 0xa64
			 0xc8009428, /// 0xa68
			 0x8c2ccb17, /// 0xa6c
			 0xc87997e8, /// 0xa70
			 0x80be7294, /// 0xa74
			 0x388b5c3b, /// 0xa78
			 0xf412d6a1, /// 0xa7c
			 0xc02354a1, /// 0xa80
			 0x6c577895, /// 0xa84
			 0xc8d75355, /// 0xa88
			 0x251d7449, /// 0xa8c
			 0xf1619ece, /// 0xa90
			 0xdc7fee3a, /// 0xa94
			 0x416f28d5, /// 0xa98
			 0x5c38c0a1, /// 0xa9c
			 0xdc452ad7, /// 0xaa0
			 0x7ebb8687, /// 0xaa4
			 0xd0866aa6, /// 0xaa8
			 0x0ea29232, /// 0xaac
			 0xa3c9b587, /// 0xab0
			 0xb26c009d, /// 0xab4
			 0x3fae5a7c, /// 0xab8
			 0x3506234c, /// 0xabc
			 0x27b3da98, /// 0xac0
			 0x8edd6f74, /// 0xac4
			 0x479d040e, /// 0xac8
			 0x4d8a4941, /// 0xacc
			 0xa353d19b, /// 0xad0
			 0x33de9af0, /// 0xad4
			 0x46957f46, /// 0xad8
			 0xfe8b4e80, /// 0xadc
			 0x0a7435b2, /// 0xae0
			 0xd0563506, /// 0xae4
			 0xb0567fd5, /// 0xae8
			 0x00554482, /// 0xaec
			 0x60072c66, /// 0xaf0
			 0x17260eeb, /// 0xaf4
			 0xcfcb0849, /// 0xaf8
			 0x1a45d8ae, /// 0xafc
			 0x125b7b97, /// 0xb00
			 0x1419d63d, /// 0xb04
			 0xd89727f0, /// 0xb08
			 0xf4d3aef1, /// 0xb0c
			 0xe547008f, /// 0xb10
			 0xdcbf310a, /// 0xb14
			 0xd9c48ab1, /// 0xb18
			 0xa3a885bc, /// 0xb1c
			 0xfd8ba326, /// 0xb20
			 0xea00892e, /// 0xb24
			 0xa4463ae4, /// 0xb28
			 0x6bdf9a16, /// 0xb2c
			 0xd6255824, /// 0xb30
			 0x0d3d991c, /// 0xb34
			 0xf572b11f, /// 0xb38
			 0x47c2ac44, /// 0xb3c
			 0x37cb2ba3, /// 0xb40
			 0xf18fee6c, /// 0xb44
			 0x28d20a07, /// 0xb48
			 0xcebf0d61, /// 0xb4c
			 0xf3b7733f, /// 0xb50
			 0x14d50b48, /// 0xb54
			 0x1429d30b, /// 0xb58
			 0x9de692c2, /// 0xb5c
			 0x084cfb77, /// 0xb60
			 0x586d6b28, /// 0xb64
			 0x74680420, /// 0xb68
			 0xe4c0842e, /// 0xb6c
			 0x731c76e7, /// 0xb70
			 0x45b5bf98, /// 0xb74
			 0x27978aba, /// 0xb78
			 0x60352815, /// 0xb7c
			 0x29bc0148, /// 0xb80
			 0xfbe2ca3c, /// 0xb84
			 0x70b1725e, /// 0xb88
			 0xacc334db, /// 0xb8c
			 0xe295f0e1, /// 0xb90
			 0x0ba46826, /// 0xb94
			 0xe4f0b6da, /// 0xb98
			 0xd2d1f56b, /// 0xb9c
			 0x09b7b1f3, /// 0xba0
			 0xff32aa90, /// 0xba4
			 0x1c0f640c, /// 0xba8
			 0x75a1dc83, /// 0xbac
			 0xaddd7f5a, /// 0xbb0
			 0x581097f8, /// 0xbb4
			 0xb5433bde, /// 0xbb8
			 0x7c48238d, /// 0xbbc
			 0x21c9b251, /// 0xbc0
			 0x6893f5ae, /// 0xbc4
			 0x3cd48ad0, /// 0xbc8
			 0x8124bb4d, /// 0xbcc
			 0xeb72be54, /// 0xbd0
			 0x63107cd0, /// 0xbd4
			 0x883b762a, /// 0xbd8
			 0x3a92055b, /// 0xbdc
			 0xc9f9063e, /// 0xbe0
			 0x7ba6c986, /// 0xbe4
			 0xab50e392, /// 0xbe8
			 0xda31c243, /// 0xbec
			 0xf329b7d8, /// 0xbf0
			 0xb27df6e5, /// 0xbf4
			 0x7ca9ca90, /// 0xbf8
			 0x26266838, /// 0xbfc
			 0xcc9f0dce, /// 0xc00
			 0x44948220, /// 0xc04
			 0xcf438c01, /// 0xc08
			 0x39d956f7, /// 0xc0c
			 0x59fcc0e7, /// 0xc10
			 0x3bef78c7, /// 0xc14
			 0xd9cad75b, /// 0xc18
			 0x9a312347, /// 0xc1c
			 0xad3a1733, /// 0xc20
			 0xfc6779f2, /// 0xc24
			 0x111f2dc0, /// 0xc28
			 0x39d3f7a0, /// 0xc2c
			 0x93a785f7, /// 0xc30
			 0xfa253679, /// 0xc34
			 0xf241a8e5, /// 0xc38
			 0xd12decb9, /// 0xc3c
			 0x8b8571ae, /// 0xc40
			 0xc55c39ee, /// 0xc44
			 0xc99ba21a, /// 0xc48
			 0xd0a0441e, /// 0xc4c
			 0x491462fb, /// 0xc50
			 0x96aa6c7b, /// 0xc54
			 0xa521f897, /// 0xc58
			 0x67367658, /// 0xc5c
			 0x9c8fed57, /// 0xc60
			 0xb9e2c983, /// 0xc64
			 0xebbff0c3, /// 0xc68
			 0x140259ad, /// 0xc6c
			 0x864aa453, /// 0xc70
			 0x8ef51850, /// 0xc74
			 0xc476df26, /// 0xc78
			 0x238199d9, /// 0xc7c
			 0x6015a377, /// 0xc80
			 0x14011366, /// 0xc84
			 0xb1ee39f2, /// 0xc88
			 0x4cb26d04, /// 0xc8c
			 0xbbfae01b, /// 0xc90
			 0x9d1b131a, /// 0xc94
			 0x702a9603, /// 0xc98
			 0x9f96d9f0, /// 0xc9c
			 0xa03608dc, /// 0xca0
			 0x081c4949, /// 0xca4
			 0xe2eb82f7, /// 0xca8
			 0xa19cd5e9, /// 0xcac
			 0x42bf53e7, /// 0xcb0
			 0x557a8689, /// 0xcb4
			 0x0049dda4, /// 0xcb8
			 0xdfd27294, /// 0xcbc
			 0x96dd54e9, /// 0xcc0
			 0x2e1fd51b, /// 0xcc4
			 0x8c2500b6, /// 0xcc8
			 0x4c381cfe, /// 0xccc
			 0x5fc409d8, /// 0xcd0
			 0x9a8af16b, /// 0xcd4
			 0x406dedbc, /// 0xcd8
			 0x5d0002f0, /// 0xcdc
			 0x50a2c48c, /// 0xce0
			 0xed445af0, /// 0xce4
			 0x7689c6d1, /// 0xce8
			 0x7c7f4348, /// 0xcec
			 0x46e9e6fe, /// 0xcf0
			 0xdaf7f514, /// 0xcf4
			 0x79eb9c8d, /// 0xcf8
			 0x9fed9507, /// 0xcfc
			 0x07837759, /// 0xd00
			 0x58a5b92f, /// 0xd04
			 0x8ec37fb5, /// 0xd08
			 0xd09b5994, /// 0xd0c
			 0x2f7a502a, /// 0xd10
			 0x69219aac, /// 0xd14
			 0x00ef29e2, /// 0xd18
			 0xb3f66367, /// 0xd1c
			 0x658db82c, /// 0xd20
			 0x5b2031ad, /// 0xd24
			 0x8cac6ea4, /// 0xd28
			 0x6e556253, /// 0xd2c
			 0x59a4ed81, /// 0xd30
			 0xa2082778, /// 0xd34
			 0xdaecc447, /// 0xd38
			 0x7d68f317, /// 0xd3c
			 0x983589e4, /// 0xd40
			 0xddadeee8, /// 0xd44
			 0x6eed4932, /// 0xd48
			 0x8e14dc30, /// 0xd4c
			 0x1a13b211, /// 0xd50
			 0xeee4abb6, /// 0xd54
			 0xe069b89b, /// 0xd58
			 0xf6ab7a7b, /// 0xd5c
			 0xfe96837d, /// 0xd60
			 0x883a67d3, /// 0xd64
			 0x37012ce6, /// 0xd68
			 0xb22d00f5, /// 0xd6c
			 0x01249eef, /// 0xd70
			 0x22586f12, /// 0xd74
			 0xd342ae3e, /// 0xd78
			 0xd51bb857, /// 0xd7c
			 0xc3617775, /// 0xd80
			 0x990e5b56, /// 0xd84
			 0x6df110fb, /// 0xd88
			 0x5e324659, /// 0xd8c
			 0xbf92760f, /// 0xd90
			 0x7ead76d6, /// 0xd94
			 0x30f647e3, /// 0xd98
			 0x4a405e9f, /// 0xd9c
			 0x62f95662, /// 0xda0
			 0x46f295c5, /// 0xda4
			 0x6b3ffcbf, /// 0xda8
			 0xc445b5b0, /// 0xdac
			 0xe9578716, /// 0xdb0
			 0xcc925b02, /// 0xdb4
			 0x4fdb3e3a, /// 0xdb8
			 0xc560f5b5, /// 0xdbc
			 0x7082ee6b, /// 0xdc0
			 0xe343af49, /// 0xdc4
			 0x9c7f7bb9, /// 0xdc8
			 0x24b1b35f, /// 0xdcc
			 0xc052159c, /// 0xdd0
			 0x7c995c7a, /// 0xdd4
			 0xefedba87, /// 0xdd8
			 0x566e5b50, /// 0xddc
			 0xe5be0b2f, /// 0xde0
			 0x61ae47bf, /// 0xde4
			 0x66583500, /// 0xde8
			 0xcf5f6603, /// 0xdec
			 0x100fdae3, /// 0xdf0
			 0x36bb55b8, /// 0xdf4
			 0xde7bef4e, /// 0xdf8
			 0xd72e2dde, /// 0xdfc
			 0xd86c11ca, /// 0xe00
			 0x983a37e6, /// 0xe04
			 0x9aed263a, /// 0xe08
			 0x838b3a5a, /// 0xe0c
			 0x4df8fca8, /// 0xe10
			 0x4eef93d7, /// 0xe14
			 0xceaa1b48, /// 0xe18
			 0xb54d45f1, /// 0xe1c
			 0x05ddc51b, /// 0xe20
			 0xaa57194b, /// 0xe24
			 0x3dc28c85, /// 0xe28
			 0x2bdce679, /// 0xe2c
			 0xb35eddd0, /// 0xe30
			 0x15f9749b, /// 0xe34
			 0x1e4cc353, /// 0xe38
			 0x965d8721, /// 0xe3c
			 0x244eb625, /// 0xe40
			 0xf8895cb4, /// 0xe44
			 0xf2e71bdf, /// 0xe48
			 0x7c474891, /// 0xe4c
			 0x979bde4b, /// 0xe50
			 0x020f295a, /// 0xe54
			 0x4622815b, /// 0xe58
			 0x109203ad, /// 0xe5c
			 0xbf72a3a2, /// 0xe60
			 0x4bc953da, /// 0xe64
			 0x1d1e67be, /// 0xe68
			 0x49d43963, /// 0xe6c
			 0x02bdb6be, /// 0xe70
			 0x278178b6, /// 0xe74
			 0xc8d96064, /// 0xe78
			 0x88744157, /// 0xe7c
			 0x784e7c76, /// 0xe80
			 0x852ef86c, /// 0xe84
			 0x10f5ef39, /// 0xe88
			 0x14aad820, /// 0xe8c
			 0x24776b01, /// 0xe90
			 0x7f206942, /// 0xe94
			 0xb585f3a1, /// 0xe98
			 0xffec7d74, /// 0xe9c
			 0xe6ee2254, /// 0xea0
			 0x04b58b7f, /// 0xea4
			 0x753b0611, /// 0xea8
			 0xd45273ca, /// 0xeac
			 0xb250b2a9, /// 0xeb0
			 0x29dc4576, /// 0xeb4
			 0x59659810, /// 0xeb8
			 0xae43091a, /// 0xebc
			 0xe6f331bb, /// 0xec0
			 0x5181a87f, /// 0xec4
			 0xf250c958, /// 0xec8
			 0xa4ac58e4, /// 0xecc
			 0x4385f183, /// 0xed0
			 0xb76cae1f, /// 0xed4
			 0xee26f986, /// 0xed8
			 0xacd9f018, /// 0xedc
			 0x3d1fd753, /// 0xee0
			 0x3a9b0a26, /// 0xee4
			 0x0232473e, /// 0xee8
			 0xfea3a80b, /// 0xeec
			 0xfe3d89b4, /// 0xef0
			 0x333e8f80, /// 0xef4
			 0x126c395e, /// 0xef8
			 0xa794f1c6, /// 0xefc
			 0x66c928f3, /// 0xf00
			 0x6e3c9cd5, /// 0xf04
			 0x9047d2d1, /// 0xf08
			 0x8eeb924f, /// 0xf0c
			 0xcf49d3c5, /// 0xf10
			 0x6ed7979d, /// 0xf14
			 0x06ca0955, /// 0xf18
			 0xd625e0e2, /// 0xf1c
			 0x368e7ea2, /// 0xf20
			 0xb69779c5, /// 0xf24
			 0xd5405698, /// 0xf28
			 0xd68f4bc5, /// 0xf2c
			 0xcf4bdeb5, /// 0xf30
			 0xdd58c6c9, /// 0xf34
			 0x8e00ef59, /// 0xf38
			 0x08d0df59, /// 0xf3c
			 0xe8f34311, /// 0xf40
			 0x4d70a852, /// 0xf44
			 0x690fea81, /// 0xf48
			 0x6758f81c, /// 0xf4c
			 0x372d466c, /// 0xf50
			 0x28768ac4, /// 0xf54
			 0x820c6d9a, /// 0xf58
			 0xb85425e0, /// 0xf5c
			 0x5c220edb, /// 0xf60
			 0x908f1ae6, /// 0xf64
			 0xf99f9260, /// 0xf68
			 0x2cd2c9cc, /// 0xf6c
			 0x87be34ae, /// 0xf70
			 0x90401471, /// 0xf74
			 0x2381519a, /// 0xf78
			 0x723e2967, /// 0xf7c
			 0xf0d16ed0, /// 0xf80
			 0x1296bd39, /// 0xf84
			 0x17753966, /// 0xf88
			 0xafab9ce4, /// 0xf8c
			 0x1f01d2e4, /// 0xf90
			 0x1362fd4f, /// 0xf94
			 0x30b3e79a, /// 0xf98
			 0x743fc6d7, /// 0xf9c
			 0x6b70a865, /// 0xfa0
			 0x375a0f20, /// 0xfa4
			 0x6730f1d7, /// 0xfa8
			 0xb05f6d94, /// 0xfac
			 0xbfa4bd7b, /// 0xfb0
			 0x46bdb55c, /// 0xfb4
			 0x9d99659e, /// 0xfb8
			 0xa06fa2bb, /// 0xfbc
			 0xeb68046e, /// 0xfc0
			 0x1a81664b, /// 0xfc4
			 0xe7d54806, /// 0xfc8
			 0x5ed3895d, /// 0xfcc
			 0xd42b0039, /// 0xfd0
			 0x7fca8953, /// 0xfd4
			 0x76d39f8c, /// 0xfd8
			 0x68f472bc, /// 0xfdc
			 0x897d4c9b, /// 0xfe0
			 0xd88bacc4, /// 0xfe4
			 0x1ce83127, /// 0xfe8
			 0x94758493, /// 0xfec
			 0xccd61870, /// 0xff0
			 0xa1d3488f, /// 0xff4
			 0x26b02b12, /// 0xff8
			 0x84dcd7f4 /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x86f076b9, /// 0x0
			 0x59494ea3, /// 0x4
			 0xa154629a, /// 0x8
			 0xf9705fd4, /// 0xc
			 0x0c2ba2e2, /// 0x10
			 0x5ba3c085, /// 0x14
			 0xf232510c, /// 0x18
			 0x36edbf75, /// 0x1c
			 0x2a70430a, /// 0x20
			 0x44947e82, /// 0x24
			 0x8d888633, /// 0x28
			 0xf3eb16e5, /// 0x2c
			 0x730beb40, /// 0x30
			 0x35fc7f03, /// 0x34
			 0x5827aec0, /// 0x38
			 0x1cd3b70e, /// 0x3c
			 0x0e6eb277, /// 0x40
			 0x97f5ff05, /// 0x44
			 0xc3b8a24c, /// 0x48
			 0x8497bcd7, /// 0x4c
			 0x26bc0e38, /// 0x50
			 0x4b27e0f7, /// 0x54
			 0x097529d2, /// 0x58
			 0x84fb700f, /// 0x5c
			 0xfd89bf2e, /// 0x60
			 0xe0c641b5, /// 0x64
			 0xa2ce97dd, /// 0x68
			 0x2fc56d61, /// 0x6c
			 0xef105e70, /// 0x70
			 0x7d527c4e, /// 0x74
			 0x3a0aaa17, /// 0x78
			 0xf2c0b635, /// 0x7c
			 0x41d0d3dd, /// 0x80
			 0x24a887ec, /// 0x84
			 0x08f16406, /// 0x88
			 0x6eff6a6f, /// 0x8c
			 0x479e00ad, /// 0x90
			 0x37449e15, /// 0x94
			 0x070dca1f, /// 0x98
			 0x12f70637, /// 0x9c
			 0x00e685fd, /// 0xa0
			 0xf8742de6, /// 0xa4
			 0x91c2b7f9, /// 0xa8
			 0x85291020, /// 0xac
			 0xdb9c1912, /// 0xb0
			 0xd179fe83, /// 0xb4
			 0x0dfe3779, /// 0xb8
			 0x84d0b627, /// 0xbc
			 0x55b1ff49, /// 0xc0
			 0x6cc00a25, /// 0xc4
			 0x74231c24, /// 0xc8
			 0x382dd873, /// 0xcc
			 0x28839bea, /// 0xd0
			 0x75c8c4c1, /// 0xd4
			 0x3e59b064, /// 0xd8
			 0x5b9d9d43, /// 0xdc
			 0xebeb327f, /// 0xe0
			 0x48cb0420, /// 0xe4
			 0xaa88da97, /// 0xe8
			 0xbf53467a, /// 0xec
			 0x13ed7a05, /// 0xf0
			 0xb4c7e79f, /// 0xf4
			 0x12a55f5a, /// 0xf8
			 0x07450358, /// 0xfc
			 0x964a78dc, /// 0x100
			 0xffa4733a, /// 0x104
			 0x32118b09, /// 0x108
			 0x02f9a5ee, /// 0x10c
			 0x3c2678aa, /// 0x110
			 0xdc6b9d25, /// 0x114
			 0x72c6e934, /// 0x118
			 0x22916d61, /// 0x11c
			 0xd3f07f49, /// 0x120
			 0xae54c505, /// 0x124
			 0x418a73c0, /// 0x128
			 0xfefffa3f, /// 0x12c
			 0x1ca04f6f, /// 0x130
			 0xb3f6e48d, /// 0x134
			 0x99697647, /// 0x138
			 0x9d1d446d, /// 0x13c
			 0x746e7948, /// 0x140
			 0x0ca94555, /// 0x144
			 0xb183936e, /// 0x148
			 0x59526bf7, /// 0x14c
			 0xbc0366d7, /// 0x150
			 0x1825e157, /// 0x154
			 0x22d0fa7a, /// 0x158
			 0x14903171, /// 0x15c
			 0x42ced288, /// 0x160
			 0x5f3e08fb, /// 0x164
			 0x42c94446, /// 0x168
			 0x3441aa69, /// 0x16c
			 0x7793910b, /// 0x170
			 0x5c7d8af6, /// 0x174
			 0x83460766, /// 0x178
			 0x5a416e46, /// 0x17c
			 0x10777706, /// 0x180
			 0x5c8e1674, /// 0x184
			 0x519e060c, /// 0x188
			 0xbb2f312e, /// 0x18c
			 0xbf75e43d, /// 0x190
			 0x620aa575, /// 0x194
			 0x16798712, /// 0x198
			 0xc0f3286d, /// 0x19c
			 0x6a0674e5, /// 0x1a0
			 0x25fce1bd, /// 0x1a4
			 0x54d960ff, /// 0x1a8
			 0x9a37f2a3, /// 0x1ac
			 0xcb497f53, /// 0x1b0
			 0xa3054edc, /// 0x1b4
			 0x7c2869d5, /// 0x1b8
			 0x16aa01e8, /// 0x1bc
			 0x0b1d90c1, /// 0x1c0
			 0x99a37221, /// 0x1c4
			 0x8b429884, /// 0x1c8
			 0xc12fdea8, /// 0x1cc
			 0xade954d7, /// 0x1d0
			 0xcbc57017, /// 0x1d4
			 0x5c206458, /// 0x1d8
			 0x21734470, /// 0x1dc
			 0xc3b04a55, /// 0x1e0
			 0xbcb76a0a, /// 0x1e4
			 0x747d2d9d, /// 0x1e8
			 0xfeb58a7f, /// 0x1ec
			 0xc63c1782, /// 0x1f0
			 0x572d95ac, /// 0x1f4
			 0xf1371a3d, /// 0x1f8
			 0xca195cff, /// 0x1fc
			 0xc76e003c, /// 0x200
			 0x5de2ce6f, /// 0x204
			 0x7772eb0a, /// 0x208
			 0x86b3774c, /// 0x20c
			 0xa6dbaf49, /// 0x210
			 0x6b68d19f, /// 0x214
			 0x38cd7678, /// 0x218
			 0x68de3f36, /// 0x21c
			 0x3a9e5b8a, /// 0x220
			 0xa908d411, /// 0x224
			 0x98172407, /// 0x228
			 0x31474814, /// 0x22c
			 0x84969493, /// 0x230
			 0x6ba5ac81, /// 0x234
			 0x2a1a445f, /// 0x238
			 0x4f0ef02a, /// 0x23c
			 0xb187fb2c, /// 0x240
			 0x10ed59fd, /// 0x244
			 0xc78f4b1c, /// 0x248
			 0x46fed946, /// 0x24c
			 0x443ac481, /// 0x250
			 0xdd649c1a, /// 0x254
			 0x464420ad, /// 0x258
			 0x455c6d3a, /// 0x25c
			 0x73dfcd00, /// 0x260
			 0x99e06435, /// 0x264
			 0xd5cb921f, /// 0x268
			 0x38a76228, /// 0x26c
			 0x37bc3478, /// 0x270
			 0x7f9fe57d, /// 0x274
			 0xad53f7eb, /// 0x278
			 0x52eda870, /// 0x27c
			 0xb5441c31, /// 0x280
			 0x2b9767b0, /// 0x284
			 0x3744aee3, /// 0x288
			 0xc79f6045, /// 0x28c
			 0x1bf960b7, /// 0x290
			 0xae65986b, /// 0x294
			 0x960bde3e, /// 0x298
			 0x8ee741e2, /// 0x29c
			 0xecf16f78, /// 0x2a0
			 0xa18ce0d0, /// 0x2a4
			 0xfd181c6e, /// 0x2a8
			 0xb5f19f7e, /// 0x2ac
			 0x52892a7b, /// 0x2b0
			 0xacdc1cbd, /// 0x2b4
			 0x474d2326, /// 0x2b8
			 0xbb9718ac, /// 0x2bc
			 0xa178a1bb, /// 0x2c0
			 0x3990a128, /// 0x2c4
			 0x74fdffc7, /// 0x2c8
			 0x99b1dedd, /// 0x2cc
			 0x041f0c98, /// 0x2d0
			 0xe69ec86a, /// 0x2d4
			 0xd7ce0e00, /// 0x2d8
			 0xf99c32df, /// 0x2dc
			 0xb66e1656, /// 0x2e0
			 0x854f63ae, /// 0x2e4
			 0x83372612, /// 0x2e8
			 0xc1e80670, /// 0x2ec
			 0xc64836f5, /// 0x2f0
			 0x8ab6b4a7, /// 0x2f4
			 0x1f2c7ce4, /// 0x2f8
			 0xb4e84808, /// 0x2fc
			 0x916e8e70, /// 0x300
			 0xd5c97fc9, /// 0x304
			 0x43e958d6, /// 0x308
			 0xcae9a572, /// 0x30c
			 0x4eb824d8, /// 0x310
			 0xb2ff17d2, /// 0x314
			 0x008178d4, /// 0x318
			 0xa4d89532, /// 0x31c
			 0x5a803b7b, /// 0x320
			 0x85b685ed, /// 0x324
			 0x3a8c3b18, /// 0x328
			 0xf9d4a97a, /// 0x32c
			 0xed7b1ff4, /// 0x330
			 0x377ab187, /// 0x334
			 0x796fe61a, /// 0x338
			 0xb3ac79f8, /// 0x33c
			 0xf20572a6, /// 0x340
			 0x83c91f3b, /// 0x344
			 0xbc0a536a, /// 0x348
			 0xcddc9f1c, /// 0x34c
			 0x56dde06a, /// 0x350
			 0x4f57e1df, /// 0x354
			 0xad6d11a8, /// 0x358
			 0x9928cc47, /// 0x35c
			 0xf0967ef8, /// 0x360
			 0x0bb56c46, /// 0x364
			 0xf40a05bc, /// 0x368
			 0x09af4200, /// 0x36c
			 0x493771b9, /// 0x370
			 0x0b335561, /// 0x374
			 0x731965a7, /// 0x378
			 0x7e423b07, /// 0x37c
			 0xe4612117, /// 0x380
			 0x3fd43dfd, /// 0x384
			 0xf3f394cd, /// 0x388
			 0xc1a53924, /// 0x38c
			 0xed75b14a, /// 0x390
			 0x6330adef, /// 0x394
			 0xda4f3011, /// 0x398
			 0x55c3dbb8, /// 0x39c
			 0x7c450c32, /// 0x3a0
			 0xce39766d, /// 0x3a4
			 0xac156e6a, /// 0x3a8
			 0xc959ab96, /// 0x3ac
			 0xe0792354, /// 0x3b0
			 0x99b3a09d, /// 0x3b4
			 0x52e6423d, /// 0x3b8
			 0xfe5b4b5f, /// 0x3bc
			 0x2d0ef64e, /// 0x3c0
			 0x514024a6, /// 0x3c4
			 0xb664f379, /// 0x3c8
			 0x7f45814b, /// 0x3cc
			 0xf488770b, /// 0x3d0
			 0x585d63ed, /// 0x3d4
			 0xae2e95fd, /// 0x3d8
			 0x5bcf9b6c, /// 0x3dc
			 0xa27ea721, /// 0x3e0
			 0xd4aabfae, /// 0x3e4
			 0x0ac7d1e8, /// 0x3e8
			 0x61d7655f, /// 0x3ec
			 0x00c32296, /// 0x3f0
			 0x4cd3d1c0, /// 0x3f4
			 0x46e63cd8, /// 0x3f8
			 0x39d78674, /// 0x3fc
			 0x5abec1b3, /// 0x400
			 0x62ba05f1, /// 0x404
			 0xd27d88b6, /// 0x408
			 0xf1f07ec0, /// 0x40c
			 0xdffd389a, /// 0x410
			 0x54e2e530, /// 0x414
			 0x91c041ce, /// 0x418
			 0x00816331, /// 0x41c
			 0xd59b5aa1, /// 0x420
			 0xd24e6a0b, /// 0x424
			 0xc960a30f, /// 0x428
			 0x2bb609a1, /// 0x42c
			 0x6a4b9570, /// 0x430
			 0x649149e8, /// 0x434
			 0x4576bdc6, /// 0x438
			 0xc0a006fa, /// 0x43c
			 0xfedb02fb, /// 0x440
			 0x86a29fa8, /// 0x444
			 0xbeac4ddf, /// 0x448
			 0x4d474588, /// 0x44c
			 0x343401b2, /// 0x450
			 0xc76090c0, /// 0x454
			 0x132f6e06, /// 0x458
			 0xb3107842, /// 0x45c
			 0x2a8afe1e, /// 0x460
			 0x79259129, /// 0x464
			 0x33ae68e0, /// 0x468
			 0xffc63dc6, /// 0x46c
			 0x8b0e5ad7, /// 0x470
			 0xf3e8b592, /// 0x474
			 0xfdec556c, /// 0x478
			 0x3f473a05, /// 0x47c
			 0x9da78dd0, /// 0x480
			 0x40f58b37, /// 0x484
			 0xf3c39673, /// 0x488
			 0x80a95dd2, /// 0x48c
			 0x46ae6e88, /// 0x490
			 0x0d13ad3b, /// 0x494
			 0xe7e90240, /// 0x498
			 0x48a72d81, /// 0x49c
			 0x89fc7595, /// 0x4a0
			 0xa00e5240, /// 0x4a4
			 0xdfc5836c, /// 0x4a8
			 0x3fef1ac9, /// 0x4ac
			 0x3eafcf38, /// 0x4b0
			 0x79269ecd, /// 0x4b4
			 0x7e874d3b, /// 0x4b8
			 0x412e32ac, /// 0x4bc
			 0x65d43b69, /// 0x4c0
			 0x0b63d11c, /// 0x4c4
			 0xb3625a83, /// 0x4c8
			 0x290f2cdc, /// 0x4cc
			 0xb5c88199, /// 0x4d0
			 0x3d23b0e6, /// 0x4d4
			 0x617bee43, /// 0x4d8
			 0x4f727125, /// 0x4dc
			 0x705a32ba, /// 0x4e0
			 0xdd3d8ad7, /// 0x4e4
			 0xb28633a8, /// 0x4e8
			 0x85d63f88, /// 0x4ec
			 0x3a30aab8, /// 0x4f0
			 0x114e0d7f, /// 0x4f4
			 0xe8d6aebe, /// 0x4f8
			 0x28f3d45e, /// 0x4fc
			 0xeef6dc35, /// 0x500
			 0x54de23fc, /// 0x504
			 0x53b14a1e, /// 0x508
			 0xb93092ac, /// 0x50c
			 0x9bc93786, /// 0x510
			 0x84866489, /// 0x514
			 0x3c768089, /// 0x518
			 0xa0428cd4, /// 0x51c
			 0x184add95, /// 0x520
			 0x5af21f2e, /// 0x524
			 0xcbffab97, /// 0x528
			 0xa8298d5f, /// 0x52c
			 0x94433ff7, /// 0x530
			 0xc6ee55d5, /// 0x534
			 0xc1cd1372, /// 0x538
			 0x200b11ba, /// 0x53c
			 0x17c37de9, /// 0x540
			 0xc9ce42de, /// 0x544
			 0x8b80cbe3, /// 0x548
			 0x9e2e4355, /// 0x54c
			 0xec1eeb82, /// 0x550
			 0x3a72fc86, /// 0x554
			 0x950887db, /// 0x558
			 0xe2f6a8cc, /// 0x55c
			 0x0a18e75f, /// 0x560
			 0xdc3ef5d7, /// 0x564
			 0x236ccf3c, /// 0x568
			 0x273dfbcc, /// 0x56c
			 0xb591f720, /// 0x570
			 0x08f3270d, /// 0x574
			 0xa3182865, /// 0x578
			 0xa385b454, /// 0x57c
			 0xf19ee542, /// 0x580
			 0x4c36dae3, /// 0x584
			 0x17c0a058, /// 0x588
			 0x5c74ba2c, /// 0x58c
			 0x3bcbdd94, /// 0x590
			 0x387aae96, /// 0x594
			 0xc455c57a, /// 0x598
			 0x5ce9d526, /// 0x59c
			 0xa659f557, /// 0x5a0
			 0x615bb59e, /// 0x5a4
			 0xb6c6c4ea, /// 0x5a8
			 0x75de2474, /// 0x5ac
			 0xfcf6a2a1, /// 0x5b0
			 0x01e25302, /// 0x5b4
			 0xf554bc4f, /// 0x5b8
			 0x97dd1676, /// 0x5bc
			 0xcb8f68ec, /// 0x5c0
			 0xab66e63e, /// 0x5c4
			 0xdea51b75, /// 0x5c8
			 0x0009876c, /// 0x5cc
			 0xb16d9b75, /// 0x5d0
			 0x6189f454, /// 0x5d4
			 0xf839d825, /// 0x5d8
			 0xe6bcce3a, /// 0x5dc
			 0x38801c92, /// 0x5e0
			 0xbecd88fa, /// 0x5e4
			 0x6ffe52ff, /// 0x5e8
			 0x11447bce, /// 0x5ec
			 0x0b401c07, /// 0x5f0
			 0xeaf23870, /// 0x5f4
			 0xf2024c23, /// 0x5f8
			 0x29663867, /// 0x5fc
			 0xb3f016d8, /// 0x600
			 0x0a97ee75, /// 0x604
			 0xc6798b33, /// 0x608
			 0xd87960b6, /// 0x60c
			 0x26c80b2b, /// 0x610
			 0x7cf28e4e, /// 0x614
			 0x4b7e54c5, /// 0x618
			 0x4253bc26, /// 0x61c
			 0xca85a599, /// 0x620
			 0xb5c52b5d, /// 0x624
			 0x148a4717, /// 0x628
			 0x8cc3028b, /// 0x62c
			 0x418a5f4f, /// 0x630
			 0xc49b8360, /// 0x634
			 0xabb9bf8a, /// 0x638
			 0x55b99ded, /// 0x63c
			 0xe982194f, /// 0x640
			 0xe58d7a81, /// 0x644
			 0xc92a1b23, /// 0x648
			 0x56b7b7d9, /// 0x64c
			 0xab060125, /// 0x650
			 0xbb1687ee, /// 0x654
			 0x4fff19f5, /// 0x658
			 0x0347037d, /// 0x65c
			 0xb146c098, /// 0x660
			 0x39c8a6ea, /// 0x664
			 0xe9b87750, /// 0x668
			 0x6a885555, /// 0x66c
			 0x11df3259, /// 0x670
			 0xbd62414a, /// 0x674
			 0xb273a314, /// 0x678
			 0x4e337b5c, /// 0x67c
			 0x50e3988a, /// 0x680
			 0x9335043f, /// 0x684
			 0x6e4ed4a8, /// 0x688
			 0x527763f1, /// 0x68c
			 0xae2c515c, /// 0x690
			 0x4c013c53, /// 0x694
			 0xa407b710, /// 0x698
			 0x4af60098, /// 0x69c
			 0x9107f9ec, /// 0x6a0
			 0x08a56911, /// 0x6a4
			 0x12d5904b, /// 0x6a8
			 0x566aa80b, /// 0x6ac
			 0xbc42de92, /// 0x6b0
			 0x2ec02324, /// 0x6b4
			 0x610e5d5e, /// 0x6b8
			 0x123544b3, /// 0x6bc
			 0xa42e57ba, /// 0x6c0
			 0xc75a3d7d, /// 0x6c4
			 0x7b7ff30b, /// 0x6c8
			 0x6a661dcb, /// 0x6cc
			 0x79f7846f, /// 0x6d0
			 0x5f7376c7, /// 0x6d4
			 0xcdcc044e, /// 0x6d8
			 0xacfa2a5b, /// 0x6dc
			 0xe71b7d13, /// 0x6e0
			 0x3f31e3c0, /// 0x6e4
			 0xd7681aeb, /// 0x6e8
			 0xee8e158a, /// 0x6ec
			 0x60f57c57, /// 0x6f0
			 0xac11db1b, /// 0x6f4
			 0x24e2f571, /// 0x6f8
			 0x99fd68f0, /// 0x6fc
			 0xa2a9306e, /// 0x700
			 0x928ddc80, /// 0x704
			 0xf67f832e, /// 0x708
			 0xe261c7fb, /// 0x70c
			 0x6ede9a44, /// 0x710
			 0x45cb8cf2, /// 0x714
			 0x78b8f5c5, /// 0x718
			 0x258cb02e, /// 0x71c
			 0x7641e891, /// 0x720
			 0x2f675ce0, /// 0x724
			 0x7bd2a216, /// 0x728
			 0x57839dbf, /// 0x72c
			 0x6dc174e0, /// 0x730
			 0xd574e02c, /// 0x734
			 0xac90dd04, /// 0x738
			 0x099fe585, /// 0x73c
			 0xe6a00eda, /// 0x740
			 0xab7183fb, /// 0x744
			 0x52fac1bf, /// 0x748
			 0x054ca74d, /// 0x74c
			 0xe86ac43c, /// 0x750
			 0xc437e331, /// 0x754
			 0xae4fbef2, /// 0x758
			 0xe387483f, /// 0x75c
			 0x8555f3f9, /// 0x760
			 0xf63c9ef0, /// 0x764
			 0x6f6bad60, /// 0x768
			 0xeeaaa320, /// 0x76c
			 0x60a9b0f5, /// 0x770
			 0x2af64170, /// 0x774
			 0x726e2a99, /// 0x778
			 0xd6689fb3, /// 0x77c
			 0x59f5f018, /// 0x780
			 0xbb4337d8, /// 0x784
			 0xe47e7856, /// 0x788
			 0x6f5db28e, /// 0x78c
			 0xebff6f34, /// 0x790
			 0x9398e18c, /// 0x794
			 0xea0fdfd0, /// 0x798
			 0xdd701ce2, /// 0x79c
			 0x2bfa1a8f, /// 0x7a0
			 0x1c376200, /// 0x7a4
			 0x46510ab3, /// 0x7a8
			 0xf1653ee7, /// 0x7ac
			 0xf0cd9591, /// 0x7b0
			 0xe7bcc0b3, /// 0x7b4
			 0x69b6e853, /// 0x7b8
			 0x1a3b722d, /// 0x7bc
			 0x10279d40, /// 0x7c0
			 0x80337f2e, /// 0x7c4
			 0xbfaa0cf0, /// 0x7c8
			 0xf890e6ec, /// 0x7cc
			 0x859d3a4b, /// 0x7d0
			 0x6a5e9ffd, /// 0x7d4
			 0x8afda167, /// 0x7d8
			 0x5570b8e5, /// 0x7dc
			 0xaf33e48f, /// 0x7e0
			 0x35fb72be, /// 0x7e4
			 0xaa2b557a, /// 0x7e8
			 0xc5689414, /// 0x7ec
			 0x2a437704, /// 0x7f0
			 0xedb6ec58, /// 0x7f4
			 0xd64a1209, /// 0x7f8
			 0xe48c613b, /// 0x7fc
			 0x3bb408d3, /// 0x800
			 0xe3f8ef65, /// 0x804
			 0xc5d4e9a1, /// 0x808
			 0x20bd23ac, /// 0x80c
			 0x620225a8, /// 0x810
			 0xf647ed07, /// 0x814
			 0xdc1791ca, /// 0x818
			 0xe13e9226, /// 0x81c
			 0xe4af3036, /// 0x820
			 0x21f5a56b, /// 0x824
			 0xde239c7e, /// 0x828
			 0x7837a820, /// 0x82c
			 0xa32ee709, /// 0x830
			 0x3e3da8b1, /// 0x834
			 0xa2b917e7, /// 0x838
			 0x0dbb25a4, /// 0x83c
			 0xa95990f6, /// 0x840
			 0xd8f78d96, /// 0x844
			 0xe05c1cc4, /// 0x848
			 0x64fdd69f, /// 0x84c
			 0xdbd8d619, /// 0x850
			 0xff789f23, /// 0x854
			 0x4aa9f3ab, /// 0x858
			 0x03a80b13, /// 0x85c
			 0x9795af3b, /// 0x860
			 0x1b36ad66, /// 0x864
			 0xe4841518, /// 0x868
			 0xee1571de, /// 0x86c
			 0x371b5025, /// 0x870
			 0xb832a483, /// 0x874
			 0x46b97142, /// 0x878
			 0xdeab185c, /// 0x87c
			 0x4f95ba55, /// 0x880
			 0x9a55189e, /// 0x884
			 0x323f4a42, /// 0x888
			 0xc0728040, /// 0x88c
			 0x2acc4fb0, /// 0x890
			 0x168cd036, /// 0x894
			 0xeef4bd6d, /// 0x898
			 0xfdccf566, /// 0x89c
			 0x3d4efcb2, /// 0x8a0
			 0xc9427711, /// 0x8a4
			 0xeb5eeced, /// 0x8a8
			 0xb9cdd594, /// 0x8ac
			 0xe8b3c60e, /// 0x8b0
			 0x22bd559e, /// 0x8b4
			 0x482d56f3, /// 0x8b8
			 0x6b22c472, /// 0x8bc
			 0x4f86fbef, /// 0x8c0
			 0x2e11782b, /// 0x8c4
			 0x9fe99c5e, /// 0x8c8
			 0x02c500a0, /// 0x8cc
			 0xf2946e31, /// 0x8d0
			 0x857411ca, /// 0x8d4
			 0x7f3a1136, /// 0x8d8
			 0x1a5a32cd, /// 0x8dc
			 0xf3069836, /// 0x8e0
			 0x294f370e, /// 0x8e4
			 0xa84cacbb, /// 0x8e8
			 0x76da4fe1, /// 0x8ec
			 0x2445cdfd, /// 0x8f0
			 0xc6f16180, /// 0x8f4
			 0x59e42e7f, /// 0x8f8
			 0x2d2eac17, /// 0x8fc
			 0x950807ad, /// 0x900
			 0xe280d448, /// 0x904
			 0xa7c4ca6c, /// 0x908
			 0xefd40b99, /// 0x90c
			 0x6d1f2b0c, /// 0x910
			 0xbfb5236e, /// 0x914
			 0x3e9485ad, /// 0x918
			 0xbaa3c7a7, /// 0x91c
			 0x25b038fd, /// 0x920
			 0x99722278, /// 0x924
			 0x95a9161d, /// 0x928
			 0x07985fff, /// 0x92c
			 0x095372ce, /// 0x930
			 0x13ba270b, /// 0x934
			 0x0316fd79, /// 0x938
			 0xe93d3df5, /// 0x93c
			 0xdc48065a, /// 0x940
			 0xe0a77b7c, /// 0x944
			 0xbe7182a8, /// 0x948
			 0x5d539c06, /// 0x94c
			 0xde11bd2e, /// 0x950
			 0x479913bd, /// 0x954
			 0x5338afa2, /// 0x958
			 0xbab1fda0, /// 0x95c
			 0x8d54c8df, /// 0x960
			 0x5db515da, /// 0x964
			 0x18cbba17, /// 0x968
			 0x93ea99aa, /// 0x96c
			 0xac39779b, /// 0x970
			 0xbb8231c5, /// 0x974
			 0xe45078cd, /// 0x978
			 0x67081f25, /// 0x97c
			 0xf281d10a, /// 0x980
			 0x1dce3361, /// 0x984
			 0x463c5330, /// 0x988
			 0xea29510a, /// 0x98c
			 0x9fe96a37, /// 0x990
			 0xa9591071, /// 0x994
			 0xfd9f0a1b, /// 0x998
			 0xa9308655, /// 0x99c
			 0x88c43068, /// 0x9a0
			 0x1902f50b, /// 0x9a4
			 0xcb847bd6, /// 0x9a8
			 0xb2120114, /// 0x9ac
			 0xd304794b, /// 0x9b0
			 0x57ecf0cb, /// 0x9b4
			 0x034f3536, /// 0x9b8
			 0x46d24c49, /// 0x9bc
			 0xdeee8e61, /// 0x9c0
			 0x2cdad064, /// 0x9c4
			 0x9da094e1, /// 0x9c8
			 0xb5126480, /// 0x9cc
			 0x32469867, /// 0x9d0
			 0x51d82bf9, /// 0x9d4
			 0x97769f7c, /// 0x9d8
			 0xcdc76b8f, /// 0x9dc
			 0xf960d752, /// 0x9e0
			 0x89470d14, /// 0x9e4
			 0x9479ade9, /// 0x9e8
			 0x28027639, /// 0x9ec
			 0x4763fe90, /// 0x9f0
			 0x6588ea99, /// 0x9f4
			 0xa7c0da58, /// 0x9f8
			 0x147c8b46, /// 0x9fc
			 0x2ba442a8, /// 0xa00
			 0x237021a6, /// 0xa04
			 0xea4062fb, /// 0xa08
			 0xa3d30f59, /// 0xa0c
			 0x4912420f, /// 0xa10
			 0x55960110, /// 0xa14
			 0xd32817f6, /// 0xa18
			 0x5a20aec5, /// 0xa1c
			 0x62e26bcc, /// 0xa20
			 0x1531b3d0, /// 0xa24
			 0x74bc6e1d, /// 0xa28
			 0x81de0a98, /// 0xa2c
			 0x7ff5825b, /// 0xa30
			 0x64523c4e, /// 0xa34
			 0x89c7cec8, /// 0xa38
			 0xefd38e83, /// 0xa3c
			 0xbcac4145, /// 0xa40
			 0x798c03e1, /// 0xa44
			 0xf879166e, /// 0xa48
			 0xee778072, /// 0xa4c
			 0x7a428c80, /// 0xa50
			 0x73440ce7, /// 0xa54
			 0x57e9ab61, /// 0xa58
			 0x428b6cfd, /// 0xa5c
			 0xb7e0713d, /// 0xa60
			 0xac20b172, /// 0xa64
			 0x96c26925, /// 0xa68
			 0xa00328fa, /// 0xa6c
			 0xa4c3b8ea, /// 0xa70
			 0x3e927825, /// 0xa74
			 0x925ae4e4, /// 0xa78
			 0x9f12e8cd, /// 0xa7c
			 0x8e5ecf10, /// 0xa80
			 0x8287efbd, /// 0xa84
			 0xba16bb5f, /// 0xa88
			 0x1b595988, /// 0xa8c
			 0xf1224002, /// 0xa90
			 0x069c0aaf, /// 0xa94
			 0x229c36f8, /// 0xa98
			 0xeb2a0817, /// 0xa9c
			 0x519a9fe4, /// 0xaa0
			 0x4fc0d424, /// 0xaa4
			 0xcad0e89c, /// 0xaa8
			 0x38355190, /// 0xaac
			 0x27cfedce, /// 0xab0
			 0xd3390638, /// 0xab4
			 0xcd6b0d9e, /// 0xab8
			 0x21d0c040, /// 0xabc
			 0x72ccc0da, /// 0xac0
			 0x4564562a, /// 0xac4
			 0x25d82c58, /// 0xac8
			 0xaa927be1, /// 0xacc
			 0xdec334c7, /// 0xad0
			 0xc85d852c, /// 0xad4
			 0x1d623fc5, /// 0xad8
			 0x678166ac, /// 0xadc
			 0x5caf4742, /// 0xae0
			 0x5addc332, /// 0xae4
			 0x14e4e410, /// 0xae8
			 0xfc9fca9f, /// 0xaec
			 0x0f588f85, /// 0xaf0
			 0xfd95dd29, /// 0xaf4
			 0x093c6c1a, /// 0xaf8
			 0xb5db83a4, /// 0xafc
			 0xd06b30ca, /// 0xb00
			 0x7f161c10, /// 0xb04
			 0xcd95adfa, /// 0xb08
			 0xb94011ad, /// 0xb0c
			 0x775d64bf, /// 0xb10
			 0x81df89d6, /// 0xb14
			 0xabc130fc, /// 0xb18
			 0xd8ee6b58, /// 0xb1c
			 0x7c0cccb2, /// 0xb20
			 0x65364fb2, /// 0xb24
			 0x8883bc2f, /// 0xb28
			 0x9d590f2b, /// 0xb2c
			 0x44f2c3be, /// 0xb30
			 0x3c8c77c1, /// 0xb34
			 0x6cf40103, /// 0xb38
			 0x2365fb5a, /// 0xb3c
			 0x8afa808f, /// 0xb40
			 0xcecdca06, /// 0xb44
			 0x77f5d840, /// 0xb48
			 0x80a38ffe, /// 0xb4c
			 0x9679e32b, /// 0xb50
			 0x1e64d94f, /// 0xb54
			 0xc4cc79e7, /// 0xb58
			 0x9fe6f1fc, /// 0xb5c
			 0x54f01ff0, /// 0xb60
			 0xcee2651d, /// 0xb64
			 0x22df0f29, /// 0xb68
			 0x07ed201d, /// 0xb6c
			 0xd80c2827, /// 0xb70
			 0x15d7106c, /// 0xb74
			 0xa46d6230, /// 0xb78
			 0xfdca5633, /// 0xb7c
			 0x02116343, /// 0xb80
			 0x53703ebc, /// 0xb84
			 0xf5208983, /// 0xb88
			 0x7e6bfdfb, /// 0xb8c
			 0x2e166773, /// 0xb90
			 0x1eed616b, /// 0xb94
			 0x93e7ef10, /// 0xb98
			 0x1ebc2efa, /// 0xb9c
			 0x2a183789, /// 0xba0
			 0x37f2ed3d, /// 0xba4
			 0x361cf252, /// 0xba8
			 0x830025f4, /// 0xbac
			 0xff840a5e, /// 0xbb0
			 0xc1f3d404, /// 0xbb4
			 0xbe9b8a87, /// 0xbb8
			 0x9ea144cd, /// 0xbbc
			 0x18d5527f, /// 0xbc0
			 0x69f4d1a3, /// 0xbc4
			 0x23ef5389, /// 0xbc8
			 0x1a3ac637, /// 0xbcc
			 0x405ebae3, /// 0xbd0
			 0x65f51932, /// 0xbd4
			 0xa1fd9744, /// 0xbd8
			 0x5856953f, /// 0xbdc
			 0x6beefc68, /// 0xbe0
			 0xf6859e87, /// 0xbe4
			 0xf24e609a, /// 0xbe8
			 0x635b8f7e, /// 0xbec
			 0x489200bc, /// 0xbf0
			 0xc65be351, /// 0xbf4
			 0x7d074bf1, /// 0xbf8
			 0x9649baa7, /// 0xbfc
			 0x139e9bd3, /// 0xc00
			 0xa1f3bfc6, /// 0xc04
			 0xd64a7c04, /// 0xc08
			 0x20097589, /// 0xc0c
			 0xf78ead13, /// 0xc10
			 0x679f51bb, /// 0xc14
			 0x1faacce1, /// 0xc18
			 0x826fe38c, /// 0xc1c
			 0x83a0ee47, /// 0xc20
			 0x24dd772d, /// 0xc24
			 0x36be459e, /// 0xc28
			 0x844cee72, /// 0xc2c
			 0x5a96fa5a, /// 0xc30
			 0xe40c22a0, /// 0xc34
			 0xce7fe4be, /// 0xc38
			 0x959c8396, /// 0xc3c
			 0x05a55632, /// 0xc40
			 0x31df6263, /// 0xc44
			 0x5de6c449, /// 0xc48
			 0xee0dc82b, /// 0xc4c
			 0x0293f59e, /// 0xc50
			 0xb83fc77c, /// 0xc54
			 0x6dfe6f8c, /// 0xc58
			 0x9af487b0, /// 0xc5c
			 0x73f0858f, /// 0xc60
			 0x9603aa55, /// 0xc64
			 0x3af79fad, /// 0xc68
			 0x99b61abf, /// 0xc6c
			 0x7a031411, /// 0xc70
			 0x91679805, /// 0xc74
			 0xf90ee1af, /// 0xc78
			 0xeb42ac58, /// 0xc7c
			 0x9bf8d8b5, /// 0xc80
			 0x7fbd4dfc, /// 0xc84
			 0xe4d1d757, /// 0xc88
			 0x5fdf9c06, /// 0xc8c
			 0x10694cfe, /// 0xc90
			 0xe42fc38e, /// 0xc94
			 0x11b7be80, /// 0xc98
			 0xa07cb39d, /// 0xc9c
			 0xfe9728cf, /// 0xca0
			 0x6934cfbd, /// 0xca4
			 0xf4030c04, /// 0xca8
			 0xb558e55d, /// 0xcac
			 0x5646696e, /// 0xcb0
			 0xd037ece0, /// 0xcb4
			 0x6376b938, /// 0xcb8
			 0xbec5c6b9, /// 0xcbc
			 0xe9fee6a5, /// 0xcc0
			 0x055235ba, /// 0xcc4
			 0xc7f1477d, /// 0xcc8
			 0xe056e01b, /// 0xccc
			 0x0c8e3572, /// 0xcd0
			 0xc83b8cf9, /// 0xcd4
			 0x869d5eda, /// 0xcd8
			 0x887e97a0, /// 0xcdc
			 0xd6e39c22, /// 0xce0
			 0x54a3736f, /// 0xce4
			 0x5072dd9b, /// 0xce8
			 0xdbb3e782, /// 0xcec
			 0xa0efe879, /// 0xcf0
			 0x1bd492ee, /// 0xcf4
			 0xfbb58a4a, /// 0xcf8
			 0x94363e4c, /// 0xcfc
			 0xd345ef96, /// 0xd00
			 0xa67c9e89, /// 0xd04
			 0x76170682, /// 0xd08
			 0x1f14571a, /// 0xd0c
			 0xbfe95444, /// 0xd10
			 0xefb76c14, /// 0xd14
			 0x189202a4, /// 0xd18
			 0x5331238c, /// 0xd1c
			 0x561068cb, /// 0xd20
			 0x02a4921f, /// 0xd24
			 0x87258fbd, /// 0xd28
			 0x6a66f229, /// 0xd2c
			 0x2b2ea938, /// 0xd30
			 0xf84811b1, /// 0xd34
			 0x537d726b, /// 0xd38
			 0x71a0bfae, /// 0xd3c
			 0xf87211c9, /// 0xd40
			 0x3fe0b3df, /// 0xd44
			 0x1db7bad9, /// 0xd48
			 0xd4a55149, /// 0xd4c
			 0xd1f44be2, /// 0xd50
			 0x657b077b, /// 0xd54
			 0x7756cfb3, /// 0xd58
			 0xb0f955b1, /// 0xd5c
			 0x53e56262, /// 0xd60
			 0xdb400a35, /// 0xd64
			 0x1c01937e, /// 0xd68
			 0xa3cbc869, /// 0xd6c
			 0xca1b0ad7, /// 0xd70
			 0x6d1b55bc, /// 0xd74
			 0x41d1b71f, /// 0xd78
			 0x394ca67f, /// 0xd7c
			 0x109e7828, /// 0xd80
			 0xf5dd4b4a, /// 0xd84
			 0xc84735d9, /// 0xd88
			 0xd8f66f7a, /// 0xd8c
			 0xc041bc86, /// 0xd90
			 0x0d428422, /// 0xd94
			 0x4382f9dc, /// 0xd98
			 0x29c9bc1a, /// 0xd9c
			 0x222c0bd0, /// 0xda0
			 0x31015f0d, /// 0xda4
			 0xab331e3d, /// 0xda8
			 0x67928ba2, /// 0xdac
			 0xc0443cdb, /// 0xdb0
			 0x90f90906, /// 0xdb4
			 0xba6b2a3c, /// 0xdb8
			 0x3675250b, /// 0xdbc
			 0xe1317499, /// 0xdc0
			 0x5b8fa8c2, /// 0xdc4
			 0xa379dd18, /// 0xdc8
			 0xa70d012d, /// 0xdcc
			 0xa89f6273, /// 0xdd0
			 0xbf3e3054, /// 0xdd4
			 0x6304cb90, /// 0xdd8
			 0xf04384ac, /// 0xddc
			 0xbe527881, /// 0xde0
			 0x2857aeec, /// 0xde4
			 0x90e8dc14, /// 0xde8
			 0xd5b9322b, /// 0xdec
			 0x89697cf2, /// 0xdf0
			 0x1c71b91b, /// 0xdf4
			 0x88359c43, /// 0xdf8
			 0x3fc10a82, /// 0xdfc
			 0x72434c3c, /// 0xe00
			 0x3be64ba5, /// 0xe04
			 0xef3c1b19, /// 0xe08
			 0xa77b95b4, /// 0xe0c
			 0x3bc3d1e5, /// 0xe10
			 0xdc3b59b1, /// 0xe14
			 0x429a0aad, /// 0xe18
			 0x1b3a58ef, /// 0xe1c
			 0x1f28761c, /// 0xe20
			 0x6c42c693, /// 0xe24
			 0x2afbdb7c, /// 0xe28
			 0x50adf5e2, /// 0xe2c
			 0x4baa541e, /// 0xe30
			 0x160635bc, /// 0xe34
			 0x25b5fc4e, /// 0xe38
			 0x9e293afb, /// 0xe3c
			 0x545468e1, /// 0xe40
			 0x0f812784, /// 0xe44
			 0x05babf44, /// 0xe48
			 0x2b8c8434, /// 0xe4c
			 0xc6557daa, /// 0xe50
			 0x6343fd7a, /// 0xe54
			 0x4fd92d1c, /// 0xe58
			 0xf5975519, /// 0xe5c
			 0xd7df52c0, /// 0xe60
			 0x87f76e26, /// 0xe64
			 0x6aa77308, /// 0xe68
			 0x02ef3947, /// 0xe6c
			 0xddb9e0ab, /// 0xe70
			 0x2b6ebd1f, /// 0xe74
			 0xc924c97d, /// 0xe78
			 0x95e8a6df, /// 0xe7c
			 0xea91aee6, /// 0xe80
			 0xfb0302d5, /// 0xe84
			 0x72b7c450, /// 0xe88
			 0x7d7b9270, /// 0xe8c
			 0x8476f954, /// 0xe90
			 0x27b7de18, /// 0xe94
			 0x1260cd4a, /// 0xe98
			 0x25461b2b, /// 0xe9c
			 0xa78ba96a, /// 0xea0
			 0x318a488a, /// 0xea4
			 0x5c2bca06, /// 0xea8
			 0xdfbc1b50, /// 0xeac
			 0x53096a15, /// 0xeb0
			 0x7f3aa4ee, /// 0xeb4
			 0xa813a479, /// 0xeb8
			 0xb4f24816, /// 0xebc
			 0xd35b481d, /// 0xec0
			 0x1ed63f64, /// 0xec4
			 0x97aafd93, /// 0xec8
			 0x7a2f0e13, /// 0xecc
			 0x022737f5, /// 0xed0
			 0xd51e42a3, /// 0xed4
			 0x69288ebe, /// 0xed8
			 0xbac4a512, /// 0xedc
			 0x9ce1cacb, /// 0xee0
			 0x18f0ede9, /// 0xee4
			 0x69b4488c, /// 0xee8
			 0xe0b87b29, /// 0xeec
			 0x81eb2332, /// 0xef0
			 0x7da5310d, /// 0xef4
			 0xa9444fc7, /// 0xef8
			 0x4d45f7bd, /// 0xefc
			 0xe8d01e3c, /// 0xf00
			 0x43422556, /// 0xf04
			 0xd7b558e2, /// 0xf08
			 0x5c218d2f, /// 0xf0c
			 0x64a57db0, /// 0xf10
			 0x95252b9f, /// 0xf14
			 0x3dc0afa8, /// 0xf18
			 0x7f785677, /// 0xf1c
			 0x7e1a5657, /// 0xf20
			 0x83b9772d, /// 0xf24
			 0xf08bd004, /// 0xf28
			 0x8e27b7b8, /// 0xf2c
			 0xb9c60b99, /// 0xf30
			 0x48993626, /// 0xf34
			 0x3201c1db, /// 0xf38
			 0x9ca2eeb6, /// 0xf3c
			 0x044d60f5, /// 0xf40
			 0x2a9c745e, /// 0xf44
			 0xd16ad11a, /// 0xf48
			 0x436f6880, /// 0xf4c
			 0x7fd090fd, /// 0xf50
			 0x17b13f3c, /// 0xf54
			 0xd9976454, /// 0xf58
			 0x8574220b, /// 0xf5c
			 0xbbfb4f81, /// 0xf60
			 0xde9e1c6f, /// 0xf64
			 0x720d64f5, /// 0xf68
			 0xd1d0ea78, /// 0xf6c
			 0x6addb489, /// 0xf70
			 0xd4b59a3d, /// 0xf74
			 0xd4b8d740, /// 0xf78
			 0x09f2d928, /// 0xf7c
			 0x12b87667, /// 0xf80
			 0x8506c42a, /// 0xf84
			 0x9f3aea34, /// 0xf88
			 0x75714ea1, /// 0xf8c
			 0x838f0f91, /// 0xf90
			 0xbe9814e8, /// 0xf94
			 0x681a3755, /// 0xf98
			 0x664e33d9, /// 0xf9c
			 0xdf926e50, /// 0xfa0
			 0x208cdc9b, /// 0xfa4
			 0x65454051, /// 0xfa8
			 0xeb7b297f, /// 0xfac
			 0x013dc533, /// 0xfb0
			 0xedc812eb, /// 0xfb4
			 0x3ea0244c, /// 0xfb8
			 0xeac7d686, /// 0xfbc
			 0x200a9d50, /// 0xfc0
			 0x14dba92b, /// 0xfc4
			 0xee588ac4, /// 0xfc8
			 0x22e4152f, /// 0xfcc
			 0xa684d0f1, /// 0xfd0
			 0x0ec9e675, /// 0xfd4
			 0x25eab19a, /// 0xfd8
			 0x6a8b7892, /// 0xfdc
			 0x67acaa3c, /// 0xfe0
			 0x37791d00, /// 0xfe4
			 0xdbb49b74, /// 0xfe8
			 0x62706d68, /// 0xfec
			 0xfea49072, /// 0xff0
			 0x3e9962b3, /// 0xff4
			 0xa46765a8, /// 0xff8
			 0xca75f9d8 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00000
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00004
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00008
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0000c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00010
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00018
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0001c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00020
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00024
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00028
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0002c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00030
			 0x55555555,                                                  // 4 random values                             /// 00034
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00038
			 0x3f028f5c,                                                  // 0.51                                        /// 0003c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00040
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00044
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00048
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0004c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00054
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00058
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0005c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00060
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00064
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00068
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0006c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00070
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00074
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00078
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0007c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00080
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00084
			 0x0e000007,                                                  // Trailing 1s:                                /// 00088
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0008c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00090
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00098
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0009c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 000a0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000a4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000a8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000ac
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000b4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 000bc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000c0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000c4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 000c8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 000cc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000d0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000d8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 000e0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000e4
			 0x7fc00001,                                                  // signaling NaN                               /// 000e8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000ec
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000f0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000fc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00104
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00108
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00114
			 0x00000000,                                                  // zero                                        /// 00118
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0011c
			 0x55555555,                                                  // 4 random values                             /// 00120
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00124
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00128
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0012c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00130
			 0x33333333,                                                  // 4 random values                             /// 00134
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00138
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0013c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00144
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00148
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0014c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00150
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00154
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00158
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0015c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00160
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00164
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00168
			 0x80011111,                                                  // -9.7958E-41                                 /// 0016c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00170
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00174
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00178
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0017c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00184
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00188
			 0x7fc00001,                                                  // signaling NaN                               /// 0018c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00190
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00194
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00198
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0019c
			 0x0c400000,                                                  // Leading 1s:                                 /// 001a0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001a4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001b0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 001b4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001b8
			 0xff800000,                                                  // -inf                                        /// 001bc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001c0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001c4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001c8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 001cc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x80011111,                                                  // -9.7958E-41                                 /// 001d4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 001d8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001dc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001e8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001ec
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001f0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 001f8
			 0xbf028f5c,                                                  // -0.51                                       /// 001fc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00200
			 0x0e000003,                                                  // Trailing 1s:                                /// 00204
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0020c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00210
			 0x0e000007,                                                  // Trailing 1s:                                /// 00214
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00218
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0021c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00220
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00224
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00228
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0022c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00230
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00234
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00238
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00240
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00244
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0024c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00250
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00254
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00258
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00260
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00264
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00268
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0026c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00270
			 0x33333333,                                                  // 4 random values                             /// 00274
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00278
			 0x4b000000,                                                  // 8388608.0                                   /// 0027c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00280
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00284
			 0x0c700000,                                                  // Leading 1s:                                 /// 00288
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0028c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00290
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00294
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00298
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0029c
			 0x4b000000,                                                  // 8388608.0                                   /// 002a0
			 0x00011111,                                                  // 9.7958E-41                                  /// 002a4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 002ac
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002b0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002b4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002bc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002c4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 002c8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002cc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002d0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002d4
			 0x4b000000,                                                  // 8388608.0                                   /// 002d8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 002dc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002e4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002e8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002ec
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002f0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002f8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002fc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00300
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00304
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00308
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00310
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00314
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00318
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0031c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00320
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00324
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00328
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0032c
			 0xbf028f5c,                                                  // -0.51                                       /// 00330
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00334
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00338
			 0x0e000003,                                                  // Trailing 1s:                                /// 0033c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00340
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00344
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00348
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0034c
			 0xff800000,                                                  // -inf                                        /// 00350
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00354
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00358
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0035c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00360
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00368
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00370
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00374
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0037c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00380
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0038c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00390
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00394
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0039c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003a0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003b4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 003bc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003c0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003c4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003cc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 003d0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 003d8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003e4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003e8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003ec
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 003f0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003f4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00400
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00404
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00408
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0040c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00414
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00418
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0041c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00420
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00424
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00428
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0042c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00430
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00434
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00438
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0043c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00440
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00444
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00448
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0044c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00450
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00454
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0045c
			 0xff800000,                                                  // -inf                                        /// 00460
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00468
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0046c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00470
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00474
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00478
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0047c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00480
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00484
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00488
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0048c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00490
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0x00000000,                                                  // zero                                        /// 00498
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0049c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 004a0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004a4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004a8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004ac
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 004b0
			 0x33333333,                                                  // 4 random values                             /// 004b4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004b8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 004bc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0x0e000007,                                                  // Trailing 1s:                                /// 004c4
			 0x0c400000,                                                  // Leading 1s:                                 /// 004c8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 004cc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004d0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004d4
			 0x55555555,                                                  // 4 random values                             /// 004d8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 004e0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 004f0
			 0x00011111,                                                  // 9.7958E-41                                  /// 004f4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004f8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 004fc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00500
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00504
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00508
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0050c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00510
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00514
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00518
			 0xbf028f5c,                                                  // -0.51                                       /// 0051c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0x00000000,                                                  // zero                                        /// 00524
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00528
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0052c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00530
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00534
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00538
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0053c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00540
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00544
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0054c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00550
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00554
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00558
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0055c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00560
			 0x0c400000,                                                  // Leading 1s:                                 /// 00564
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00568
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0056c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00570
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00574
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00578
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0057c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00580
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00584
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00588
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0058c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00590
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00598
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0059c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 005a0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 005a4
			 0x00000000,                                                  // zero                                        /// 005a8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0x0e00003f,                                                  // Trailing 1s:                                /// 005b0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 005b4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005bc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005c0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005c8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005cc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d0
			 0xbf028f5c,                                                  // -0.51                                       /// 005d4
			 0x0c400000,                                                  // Leading 1s:                                 /// 005d8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005dc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 005e0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 005f4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005f8
			 0x4b000000,                                                  // 8388608.0                                   /// 005fc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00600
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00604
			 0x33333333,                                                  // 4 random values                             /// 00608
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0060c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00610
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00614
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00618
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0061c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00620
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00624
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00628
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0062c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00630
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00634
			 0x0e000001,                                                  // Trailing 1s:                                /// 00638
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0063c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00640
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00644
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0064c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00650
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00654
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00658
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0065c
			 0xbf028f5c,                                                  // -0.51                                       /// 00660
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00664
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0066c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00670
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00678
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0067c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00680
			 0x00011111,                                                  // 9.7958E-41                                  /// 00684
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0068c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00694
			 0xffc00001,                                                  // -signaling NaN                              /// 00698
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0069c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 006a4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 006b0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 006b4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006b8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006bc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006c0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006c4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006cc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006d0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006d4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006d8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 006dc
			 0x4b000000,                                                  // 8388608.0                                   /// 006e0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 006e4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006e8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006ec
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 006f0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006f4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006f8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006fc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00704
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0070c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00710
			 0xffc00001,                                                  // -signaling NaN                              /// 00714
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0071c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00720
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00724
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00728
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0072c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00730
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00734
			 0x4b000000,                                                  // 8388608.0                                   /// 00738
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0073c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00740
			 0x80011111,                                                  // -9.7958E-41                                 /// 00744
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00748
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0074c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00750
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00754
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00758
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0075c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00764
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00768
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0076c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00774
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00778
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0077c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00780
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00784
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0078c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00790
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00798
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0079c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 007a0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007a4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ac
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007b0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007b4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 007bc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007c0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007c4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 007c8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007cc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007d0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007d8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 007dc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 007e0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 007e4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007e8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007ec
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007f0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007f4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 007f8
			 0xbf028f5c,                                                  // -0.51                                       /// 007fc
			 0x4b000000,                                                  // 8388608.0                                   /// 00800
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00804
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00808
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0080c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00810
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00814
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0081c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00820
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00824
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00828
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0082c
			 0xffc00001,                                                  // -signaling NaN                              /// 00830
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00834
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00838
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0083c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00840
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00844
			 0x3f028f5c,                                                  // 0.51                                        /// 00848
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0084c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00854
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00858
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0085c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00860
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00864
			 0x3f028f5c,                                                  // 0.51                                        /// 00868
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0086c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00870
			 0x7f800000,                                                  // inf                                         /// 00874
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0087c
			 0xbf028f5c,                                                  // -0.51                                       /// 00880
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00884
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0x0c700000,                                                  // Leading 1s:                                 /// 0088c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00890
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00894
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00898
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0089c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 008a0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 008a8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 008ac
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 008b0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 008b4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008b8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008bc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008c0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008c4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 008c8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 008cc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x7fc00001,                                                  // signaling NaN                               /// 008d4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008d8
			 0xff800000,                                                  // -inf                                        /// 008dc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008e0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 008e4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008e8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008ec
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 008f0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008f8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 008fc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00900
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00904
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00908
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00910
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0x0e000001,                                                  // Trailing 1s:                                /// 00918
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0091c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00920
			 0x3f028f5c,                                                  // 0.51                                        /// 00924
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00928
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0092c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x00000000,                                                  // zero                                        /// 00934
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00938
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0093c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00940
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00944
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00948
			 0x0c400000,                                                  // Leading 1s:                                 /// 0094c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x80000000,                                                  // -zero                                       /// 00954
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00958
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0095c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00960
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00964
			 0x0e000003,                                                  // Trailing 1s:                                /// 00968
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0096c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00970
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00978
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0097c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00980
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00984
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00988
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0098c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00990
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00994
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00998
			 0x0c600000,                                                  // Leading 1s:                                 /// 0099c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 009a4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009a8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009ac
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0xbf028f5c,                                                  // -0.51                                       /// 009b4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009bc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009c0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 009c4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 009c8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009d0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009dc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009e0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 009e8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009ec
			 0xbf028f5c,                                                  // -0.51                                       /// 009f0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 009f4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009f8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009fc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a00
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a08
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a0c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a10
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a14
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a18
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a1c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a24
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a28
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a2c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a30
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a34
			 0x4b000000,                                                  // 8388608.0                                   /// 00a38
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a3c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a44
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a48
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a4c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a50
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a54
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a58
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a5c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a64
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a68
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a6c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a70
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a74
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a78
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a7c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a80
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a84
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a88
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a8c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a90
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a94
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a98
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a9c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ab0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ab4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ab8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00abc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ac0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ac4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00acc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ad0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ad4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ad8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00adc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ae0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ae4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ae8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0x0c600000,                                                  // Leading 1s:                                 /// 00af0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00af4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00afc
			 0xffc00001,                                                  // -signaling NaN                              /// 00b00
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b04
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b08
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b0c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b10
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b14
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b18
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b1c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b20
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x7f800000,                                                  // inf                                         /// 00b2c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b30
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b34
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b40
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b44
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b48
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b50
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b54
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b58
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b5c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b60
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b64
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b68
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b6c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b74
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b7c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b80
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b84
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b88
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b8c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b90
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b94
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b9c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ba0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ba4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ba8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00bac
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00bb0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00bb4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00bc4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00bc8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00bcc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bd0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00bd8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00bdc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00be0
			 0x00000000,                                                  // zero                                        /// 00be4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00bec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00bf0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00bf4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00bfc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c00
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c04
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c08
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c0c
			 0xbf028f5c,                                                  // -0.51                                       /// 00c10
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c1c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c28
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c2c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c30
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c34
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c38
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c40
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c48
			 0xff800000,                                                  // -inf                                        /// 00c4c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c50
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c54
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c58
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c5c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c60
			 0x4b000000,                                                  // 8388608.0                                   /// 00c64
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c68
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c6c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c70
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c74
			 0x4b000000,                                                  // 8388608.0                                   /// 00c78
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c7c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c80
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c84
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c88
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c8c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c90
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c94
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c98
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c9c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x33333333,                                                  // 4 random values                             /// 00ca4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ca8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00cac
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00cb0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00cb4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00cb8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00cbc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00cc0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00cc4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ccc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00cd0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00cd4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00cd8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cdc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ce0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ce8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00cec
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00cf0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00cf4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00cfc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d00
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d04
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d08
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d0c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d10
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d14
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d18
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0x80000000,                                                  // -zero                                       /// 00d20
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d24
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d2c
			 0x4b000000,                                                  // 8388608.0                                   /// 00d30
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d34
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d38
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d3c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d40
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d48
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d4c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d50
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d5c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d60
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d64
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d68
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d6c
			 0x7f800000,                                                  // inf                                         /// 00d70
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d78
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d7c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d80
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d84
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d88
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d8c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d94
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d98
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d9c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00da4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00da8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00dac
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00db4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00db8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00dbc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00dc0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00dc4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00dc8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dcc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00dd0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00dd4
			 0x00000000,                                                  // zero                                        /// 00dd8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ddc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00de0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00de4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00de8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00dec
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00df0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00df4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00df8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00dfc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e00
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e04
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e08
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e0c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e10
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e14
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e18
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e1c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e24
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e28
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e30
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e34
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e3c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e40
			 0x7f800000,                                                  // inf                                         /// 00e44
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e4c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e50
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e58
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e5c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e64
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e6c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e78
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e7c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e84
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e88
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e8c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e90
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e94
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e98
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e9c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ea0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ea4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00eac
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00eb4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ebc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ec0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ec4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ecc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ed4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ed8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00edc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ee0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ee4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ee8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00eec
			 0x00000000,                                                  // zero                                        /// 00ef0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ef8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00efc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f00
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f04
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f08
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f0c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f10
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f20
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f28
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f2c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f30
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f34
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f38
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f3c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0x55555555,                                                  // 4 random values                             /// 00f48
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f4c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f50
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f54
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f58
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f5c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f60
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f64
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f68
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f6c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f74
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f78
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f7c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f80
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f84
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f8c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f90
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00fa0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00fa4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fa8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fac
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fb4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00fbc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00fc0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fc4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00fc8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00fcc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00fd0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fd8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00fdc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00fe0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00fe4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fec
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ff0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ff4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ff8
			 0x80800001 // min norm + 1ulp                               // subnormals -ve                              /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x238dbe3c, /// 0x0
			 0xfd1181f2, /// 0x4
			 0x241de139, /// 0x8
			 0xdd0f1d6b, /// 0xc
			 0xe8ba3df9, /// 0x10
			 0x5d16112f, /// 0x14
			 0xe7189b32, /// 0x18
			 0xb3cc3b69, /// 0x1c
			 0xaf71f4af, /// 0x20
			 0x73b03acd, /// 0x24
			 0x5676c16e, /// 0x28
			 0x430c2498, /// 0x2c
			 0x331a33fe, /// 0x30
			 0xe67e3c92, /// 0x34
			 0xec95ed6b, /// 0x38
			 0x6ca3de24, /// 0x3c
			 0x29e20082, /// 0x40
			 0xcaea2edc, /// 0x44
			 0x937a745d, /// 0x48
			 0x3efc129f, /// 0x4c
			 0xd7346b30, /// 0x50
			 0xcc906a52, /// 0x54
			 0x942592cc, /// 0x58
			 0x5148025f, /// 0x5c
			 0xc1ed77a3, /// 0x60
			 0xac344462, /// 0x64
			 0xf3650a79, /// 0x68
			 0xda17781d, /// 0x6c
			 0x2645bc01, /// 0x70
			 0x301da901, /// 0x74
			 0x820f7429, /// 0x78
			 0xaca6cc43, /// 0x7c
			 0x7bc38411, /// 0x80
			 0x9debd5d3, /// 0x84
			 0x3479c575, /// 0x88
			 0x5532cddd, /// 0x8c
			 0x91a27dbb, /// 0x90
			 0x6287fe9d, /// 0x94
			 0xff0181d5, /// 0x98
			 0x7e5780ed, /// 0x9c
			 0x9b459394, /// 0xa0
			 0x99a31552, /// 0xa4
			 0xf50be115, /// 0xa8
			 0x5514c48a, /// 0xac
			 0x11d02208, /// 0xb0
			 0x3f5a7805, /// 0xb4
			 0x5ab4cd35, /// 0xb8
			 0xade506d2, /// 0xbc
			 0xb9cd97d0, /// 0xc0
			 0x0368fb0a, /// 0xc4
			 0xc436974a, /// 0xc8
			 0x21198ca0, /// 0xcc
			 0x2681bc47, /// 0xd0
			 0x672e6421, /// 0xd4
			 0x37968630, /// 0xd8
			 0xf4879bca, /// 0xdc
			 0xf5c02eb9, /// 0xe0
			 0x6b5aac7f, /// 0xe4
			 0x78b07585, /// 0xe8
			 0xada8808f, /// 0xec
			 0x206de191, /// 0xf0
			 0x4bf16e01, /// 0xf4
			 0x08e608fe, /// 0xf8
			 0xf5c8567f, /// 0xfc
			 0xba547a1e, /// 0x100
			 0x3016387a, /// 0x104
			 0x0c045fc6, /// 0x108
			 0x2612e884, /// 0x10c
			 0x840b8584, /// 0x110
			 0xed395c95, /// 0x114
			 0xe8bf2451, /// 0x118
			 0x6fe23b70, /// 0x11c
			 0x54e4e7e7, /// 0x120
			 0x49bd8c99, /// 0x124
			 0xea2b89de, /// 0x128
			 0x0390f421, /// 0x12c
			 0x2883ae64, /// 0x130
			 0x6f04f603, /// 0x134
			 0xc516d995, /// 0x138
			 0x1582884b, /// 0x13c
			 0x1cba1673, /// 0x140
			 0x63c66b06, /// 0x144
			 0x8850b8e3, /// 0x148
			 0x475ee9c7, /// 0x14c
			 0xa7a98b2f, /// 0x150
			 0x81f7d123, /// 0x154
			 0x2444dc5f, /// 0x158
			 0xb705be27, /// 0x15c
			 0xc42419dc, /// 0x160
			 0xf21678d9, /// 0x164
			 0x83332362, /// 0x168
			 0x6004014d, /// 0x16c
			 0xdcf16940, /// 0x170
			 0x133e06ec, /// 0x174
			 0xfdb71042, /// 0x178
			 0xed4b25ee, /// 0x17c
			 0x3b94f47d, /// 0x180
			 0x599aa2b8, /// 0x184
			 0xdf8755ca, /// 0x188
			 0xba58e575, /// 0x18c
			 0x3983d74f, /// 0x190
			 0x417f3558, /// 0x194
			 0xe1e488eb, /// 0x198
			 0x6deec32d, /// 0x19c
			 0x2ee248c8, /// 0x1a0
			 0x748f385f, /// 0x1a4
			 0x5850fc6c, /// 0x1a8
			 0x63db644f, /// 0x1ac
			 0x6605ed86, /// 0x1b0
			 0x3158cbd6, /// 0x1b4
			 0x29c3861c, /// 0x1b8
			 0x5735f45a, /// 0x1bc
			 0x7cfc35a5, /// 0x1c0
			 0x5392c9be, /// 0x1c4
			 0x02508ad3, /// 0x1c8
			 0x1a8e4561, /// 0x1cc
			 0x9f6483a7, /// 0x1d0
			 0xac82ea70, /// 0x1d4
			 0xa484b3a6, /// 0x1d8
			 0x410bf559, /// 0x1dc
			 0x5b40ad37, /// 0x1e0
			 0xe639a3b5, /// 0x1e4
			 0x62179a9d, /// 0x1e8
			 0xa76397c4, /// 0x1ec
			 0x68cb32f9, /// 0x1f0
			 0x75ddea8e, /// 0x1f4
			 0xc869bbe5, /// 0x1f8
			 0x67f37dae, /// 0x1fc
			 0x7b886ecc, /// 0x200
			 0x60a4a84e, /// 0x204
			 0x86884707, /// 0x208
			 0xb4c5f443, /// 0x20c
			 0xf25b4a74, /// 0x210
			 0xa5eccde6, /// 0x214
			 0x9174480a, /// 0x218
			 0x148299a8, /// 0x21c
			 0xedaf2a0a, /// 0x220
			 0x4964e81a, /// 0x224
			 0x57172286, /// 0x228
			 0xeaf329d7, /// 0x22c
			 0xddeb057a, /// 0x230
			 0xed6adebd, /// 0x234
			 0xacf349ac, /// 0x238
			 0xf544fd5b, /// 0x23c
			 0xa61695a3, /// 0x240
			 0x1401bbe9, /// 0x244
			 0xec962272, /// 0x248
			 0x3f415c57, /// 0x24c
			 0x2d4ba0d7, /// 0x250
			 0xb80df65d, /// 0x254
			 0x052fd676, /// 0x258
			 0x85158bf1, /// 0x25c
			 0xa9853c9d, /// 0x260
			 0xdbfca92f, /// 0x264
			 0x6e4fa80a, /// 0x268
			 0x36a5f396, /// 0x26c
			 0xd7f3683b, /// 0x270
			 0xed6e7eab, /// 0x274
			 0x8f7dbbe7, /// 0x278
			 0x5dfd68b2, /// 0x27c
			 0x9678d326, /// 0x280
			 0x39b76c3c, /// 0x284
			 0x815e6967, /// 0x288
			 0x7b1d5bc9, /// 0x28c
			 0xcfef6b29, /// 0x290
			 0xc4d5f902, /// 0x294
			 0xc4385958, /// 0x298
			 0x2d3d1875, /// 0x29c
			 0x75601c18, /// 0x2a0
			 0x69fcd88e, /// 0x2a4
			 0x4202e889, /// 0x2a8
			 0x7d51667f, /// 0x2ac
			 0x47b27eb9, /// 0x2b0
			 0x56060050, /// 0x2b4
			 0x7726eed1, /// 0x2b8
			 0x73a26958, /// 0x2bc
			 0x8fe865e5, /// 0x2c0
			 0x2cee951a, /// 0x2c4
			 0x547f3efb, /// 0x2c8
			 0x1968157f, /// 0x2cc
			 0xf185dac1, /// 0x2d0
			 0x92a6848a, /// 0x2d4
			 0xc2e29a81, /// 0x2d8
			 0xde5bb889, /// 0x2dc
			 0x70b70507, /// 0x2e0
			 0x64a81bc9, /// 0x2e4
			 0x6896a63f, /// 0x2e8
			 0x8afb35f4, /// 0x2ec
			 0xc5ec10dc, /// 0x2f0
			 0x24c3bec2, /// 0x2f4
			 0xd723e10d, /// 0x2f8
			 0xacb43841, /// 0x2fc
			 0xd56c7512, /// 0x300
			 0x6805c309, /// 0x304
			 0x30802097, /// 0x308
			 0xf89bd0ea, /// 0x30c
			 0x427b0c8d, /// 0x310
			 0x00e36a6b, /// 0x314
			 0x99fee1d9, /// 0x318
			 0xf34ec99c, /// 0x31c
			 0x01bdeb1a, /// 0x320
			 0xb496312f, /// 0x324
			 0x0719d37f, /// 0x328
			 0x744e5fab, /// 0x32c
			 0x91d8e85a, /// 0x330
			 0x68723531, /// 0x334
			 0xe67db79e, /// 0x338
			 0x65124272, /// 0x33c
			 0x2f3a3ba2, /// 0x340
			 0xb5aab7fa, /// 0x344
			 0x4072e080, /// 0x348
			 0xdcbc91bb, /// 0x34c
			 0x1b135be9, /// 0x350
			 0x5ac9a34c, /// 0x354
			 0x434c0400, /// 0x358
			 0x570a28cb, /// 0x35c
			 0x14b5ab7a, /// 0x360
			 0x409bcf7e, /// 0x364
			 0x389b9f63, /// 0x368
			 0xe4cc96e6, /// 0x36c
			 0x324e6623, /// 0x370
			 0xe50e9436, /// 0x374
			 0xa5512450, /// 0x378
			 0xbb195535, /// 0x37c
			 0x4995b17e, /// 0x380
			 0x7d149bf8, /// 0x384
			 0x4ac9f099, /// 0x388
			 0x63f8e302, /// 0x38c
			 0x8055ebcc, /// 0x390
			 0x39ef81ad, /// 0x394
			 0x7fdfead4, /// 0x398
			 0xc1646461, /// 0x39c
			 0x1846d757, /// 0x3a0
			 0xed6582cf, /// 0x3a4
			 0x1ae7b48a, /// 0x3a8
			 0x12acdd2e, /// 0x3ac
			 0x87bc397b, /// 0x3b0
			 0xf21fb00f, /// 0x3b4
			 0x1c68d42f, /// 0x3b8
			 0x5cc51280, /// 0x3bc
			 0xaa9fa903, /// 0x3c0
			 0x9d8df525, /// 0x3c4
			 0xdaa8a97c, /// 0x3c8
			 0x0012e09f, /// 0x3cc
			 0x4ab3d254, /// 0x3d0
			 0x709313fe, /// 0x3d4
			 0xe90ae9a0, /// 0x3d8
			 0x876e7cf3, /// 0x3dc
			 0x061b791d, /// 0x3e0
			 0xc358555f, /// 0x3e4
			 0x1b8e3941, /// 0x3e8
			 0x3362affa, /// 0x3ec
			 0xb9887eea, /// 0x3f0
			 0x9a487e35, /// 0x3f4
			 0x3c398a2c, /// 0x3f8
			 0x864f425d, /// 0x3fc
			 0x72afa2e4, /// 0x400
			 0xda6ad7e6, /// 0x404
			 0x7b36ea75, /// 0x408
			 0x4008efd7, /// 0x40c
			 0x420445c0, /// 0x410
			 0xb35e280f, /// 0x414
			 0xb2a6f18a, /// 0x418
			 0xe3e70f4a, /// 0x41c
			 0xbfd9b18a, /// 0x420
			 0xc1cf8772, /// 0x424
			 0x13e27044, /// 0x428
			 0x3c440879, /// 0x42c
			 0x71b0d871, /// 0x430
			 0x2a2715ed, /// 0x434
			 0xe9846759, /// 0x438
			 0x25cd26b6, /// 0x43c
			 0x53111905, /// 0x440
			 0x81a083bf, /// 0x444
			 0xf1d36fbe, /// 0x448
			 0xceb6f419, /// 0x44c
			 0xe94a0062, /// 0x450
			 0xf4ab5884, /// 0x454
			 0x339caefc, /// 0x458
			 0xab51f785, /// 0x45c
			 0x14d4021a, /// 0x460
			 0x1eec4c0f, /// 0x464
			 0x25571f29, /// 0x468
			 0xc2d232b7, /// 0x46c
			 0x177d5f48, /// 0x470
			 0x644fadf9, /// 0x474
			 0xc332bfd8, /// 0x478
			 0x859af4b9, /// 0x47c
			 0x48f46a95, /// 0x480
			 0x6d6467be, /// 0x484
			 0xad18419a, /// 0x488
			 0x99bb3a2a, /// 0x48c
			 0xe978cab0, /// 0x490
			 0xfe926452, /// 0x494
			 0x932afc5f, /// 0x498
			 0x74074360, /// 0x49c
			 0x95013dce, /// 0x4a0
			 0xfcb5fb72, /// 0x4a4
			 0xe3aa843d, /// 0x4a8
			 0x5d6d9e5c, /// 0x4ac
			 0x64442782, /// 0x4b0
			 0xccfb934a, /// 0x4b4
			 0xcca1913b, /// 0x4b8
			 0x429734ce, /// 0x4bc
			 0xe81495c5, /// 0x4c0
			 0xbdf5e874, /// 0x4c4
			 0x2dea5fb5, /// 0x4c8
			 0x2e35e6a8, /// 0x4cc
			 0x449c6395, /// 0x4d0
			 0x19b34f43, /// 0x4d4
			 0xecfd88ba, /// 0x4d8
			 0x5303e7c5, /// 0x4dc
			 0xd0f3a550, /// 0x4e0
			 0xa7531de6, /// 0x4e4
			 0x3668be1b, /// 0x4e8
			 0x2ec3948a, /// 0x4ec
			 0x839bb5a7, /// 0x4f0
			 0xa10cff7b, /// 0x4f4
			 0xef9e73d8, /// 0x4f8
			 0x18c56e3c, /// 0x4fc
			 0x525135f4, /// 0x500
			 0x7a2e9974, /// 0x504
			 0x629c94db, /// 0x508
			 0x6d3817b6, /// 0x50c
			 0xdb79adb8, /// 0x510
			 0xe5ab94c2, /// 0x514
			 0x7f20a9a9, /// 0x518
			 0x7b8779cf, /// 0x51c
			 0x22717453, /// 0x520
			 0xe79dd72e, /// 0x524
			 0xa8ef8432, /// 0x528
			 0xddeb8cac, /// 0x52c
			 0x45ecd45a, /// 0x530
			 0xfa812db4, /// 0x534
			 0x78746092, /// 0x538
			 0xd278dd16, /// 0x53c
			 0x7fa5939a, /// 0x540
			 0xfe56371f, /// 0x544
			 0x9497157e, /// 0x548
			 0xd93bea49, /// 0x54c
			 0xbeb3f9f0, /// 0x550
			 0x2758e3d5, /// 0x554
			 0xa59d5c07, /// 0x558
			 0xa4611795, /// 0x55c
			 0x24332990, /// 0x560
			 0xe8e2674f, /// 0x564
			 0x38c9d225, /// 0x568
			 0x563d3514, /// 0x56c
			 0xa4fe7cff, /// 0x570
			 0xfd437de4, /// 0x574
			 0x2ded5abd, /// 0x578
			 0xec00007e, /// 0x57c
			 0x2e183e5c, /// 0x580
			 0xa82b66d8, /// 0x584
			 0x38ab6de9, /// 0x588
			 0xe16ea289, /// 0x58c
			 0xd533df73, /// 0x590
			 0x03842909, /// 0x594
			 0x4bc09120, /// 0x598
			 0x8a2f5676, /// 0x59c
			 0xf722a18a, /// 0x5a0
			 0x607ab295, /// 0x5a4
			 0x11722049, /// 0x5a8
			 0xb9eb9280, /// 0x5ac
			 0x3be5b390, /// 0x5b0
			 0xed366ab8, /// 0x5b4
			 0x14792625, /// 0x5b8
			 0x1e81c04f, /// 0x5bc
			 0x48fc9a73, /// 0x5c0
			 0xbf923b93, /// 0x5c4
			 0x3f569085, /// 0x5c8
			 0x09e745de, /// 0x5cc
			 0xbf5ca47d, /// 0x5d0
			 0x1428625c, /// 0x5d4
			 0xcccf0976, /// 0x5d8
			 0xf3f7ef26, /// 0x5dc
			 0xaa9f3837, /// 0x5e0
			 0x344dd334, /// 0x5e4
			 0x6a9f1a5c, /// 0x5e8
			 0x36deea4a, /// 0x5ec
			 0x9dfc77eb, /// 0x5f0
			 0xf9a99eba, /// 0x5f4
			 0xe363fc0a, /// 0x5f8
			 0xe5e37826, /// 0x5fc
			 0x56dfbffe, /// 0x600
			 0xfe8c9c11, /// 0x604
			 0xf9cc1d43, /// 0x608
			 0xc5ea3854, /// 0x60c
			 0x31033ead, /// 0x610
			 0xe655173a, /// 0x614
			 0xe3831217, /// 0x618
			 0xa57726f8, /// 0x61c
			 0x6db6fcb3, /// 0x620
			 0xae1beb11, /// 0x624
			 0x51fe8056, /// 0x628
			 0x598f31fa, /// 0x62c
			 0xcf01bd2f, /// 0x630
			 0x9c14c213, /// 0x634
			 0x5ec7475f, /// 0x638
			 0x79833c66, /// 0x63c
			 0x906f742f, /// 0x640
			 0x6ffb13eb, /// 0x644
			 0x294715b5, /// 0x648
			 0xf876efef, /// 0x64c
			 0xa04c06fb, /// 0x650
			 0xe6b3e939, /// 0x654
			 0x794ef47f, /// 0x658
			 0xdcc8abe5, /// 0x65c
			 0x026f0d77, /// 0x660
			 0x529222b5, /// 0x664
			 0xff22ab63, /// 0x668
			 0x089db230, /// 0x66c
			 0x970989c3, /// 0x670
			 0x1341e3e6, /// 0x674
			 0xfbe2a573, /// 0x678
			 0xf7fac6c1, /// 0x67c
			 0xe1f9f406, /// 0x680
			 0xee0925ab, /// 0x684
			 0xc25d314e, /// 0x688
			 0x811bbb0a, /// 0x68c
			 0xf86fb733, /// 0x690
			 0xff2546f7, /// 0x694
			 0xdd1af930, /// 0x698
			 0x64b39a89, /// 0x69c
			 0x17837415, /// 0x6a0
			 0x231445f5, /// 0x6a4
			 0xbe836bde, /// 0x6a8
			 0xe570d474, /// 0x6ac
			 0x606b3be0, /// 0x6b0
			 0xe34fb6ef, /// 0x6b4
			 0x8a7ce164, /// 0x6b8
			 0x18e0ecc1, /// 0x6bc
			 0x90c83e88, /// 0x6c0
			 0xfffc1c90, /// 0x6c4
			 0xd3a0eb4c, /// 0x6c8
			 0xa9ab20d6, /// 0x6cc
			 0xdcbd7022, /// 0x6d0
			 0x373a59d3, /// 0x6d4
			 0x32b5f414, /// 0x6d8
			 0xc5630512, /// 0x6dc
			 0x6f0cba6b, /// 0x6e0
			 0xe9da23c8, /// 0x6e4
			 0xf164cb0e, /// 0x6e8
			 0x1fddb41d, /// 0x6ec
			 0x09a99026, /// 0x6f0
			 0x56ea8b0c, /// 0x6f4
			 0x22b2f777, /// 0x6f8
			 0x72c9fdd4, /// 0x6fc
			 0xf4e2512f, /// 0x700
			 0xca8c45ee, /// 0x704
			 0x21cc6d71, /// 0x708
			 0x71caaa0e, /// 0x70c
			 0xcc87998a, /// 0x710
			 0xfe4df7fb, /// 0x714
			 0x35848c29, /// 0x718
			 0x341a741f, /// 0x71c
			 0xff0056ed, /// 0x720
			 0x49e81f7c, /// 0x724
			 0xb1b27a03, /// 0x728
			 0x859582e1, /// 0x72c
			 0xa173d77f, /// 0x730
			 0x575ad65e, /// 0x734
			 0x2d75f178, /// 0x738
			 0xd565b3f5, /// 0x73c
			 0xfe6419d2, /// 0x740
			 0x27b10791, /// 0x744
			 0x23d75d7c, /// 0x748
			 0x48bfeb46, /// 0x74c
			 0xd52d728e, /// 0x750
			 0xdc83e9d2, /// 0x754
			 0x73684ced, /// 0x758
			 0xd5b1d35c, /// 0x75c
			 0xdf8a3e48, /// 0x760
			 0x528a96e6, /// 0x764
			 0x65041822, /// 0x768
			 0x2710bbec, /// 0x76c
			 0xa80197f9, /// 0x770
			 0x244fe7fe, /// 0x774
			 0xc20e67b6, /// 0x778
			 0xc7f5a200, /// 0x77c
			 0xbfe8db8b, /// 0x780
			 0xf6d6d6df, /// 0x784
			 0x371da231, /// 0x788
			 0xb1707c31, /// 0x78c
			 0x1db82cb9, /// 0x790
			 0xc113390a, /// 0x794
			 0xd097d4f7, /// 0x798
			 0x627583f5, /// 0x79c
			 0x2f4c0e05, /// 0x7a0
			 0x5eb9a228, /// 0x7a4
			 0x38961bc6, /// 0x7a8
			 0xa4ad7e85, /// 0x7ac
			 0xe79e1b6d, /// 0x7b0
			 0x0a831877, /// 0x7b4
			 0x80695635, /// 0x7b8
			 0xfa772111, /// 0x7bc
			 0x10494ccf, /// 0x7c0
			 0xc3c69bae, /// 0x7c4
			 0x50a2c588, /// 0x7c8
			 0x25b86993, /// 0x7cc
			 0x22ec9600, /// 0x7d0
			 0x70fe43a0, /// 0x7d4
			 0xbd7a38f6, /// 0x7d8
			 0xcc629f17, /// 0x7dc
			 0x89afe161, /// 0x7e0
			 0xaebbddf0, /// 0x7e4
			 0x035e1bd9, /// 0x7e8
			 0x17a39757, /// 0x7ec
			 0x3ed9769a, /// 0x7f0
			 0x572fd998, /// 0x7f4
			 0x14b656b1, /// 0x7f8
			 0xbff82997, /// 0x7fc
			 0xaa32ac30, /// 0x800
			 0xeec7efa4, /// 0x804
			 0xfb70aa30, /// 0x808
			 0x1f0f2d12, /// 0x80c
			 0xa648437d, /// 0x810
			 0xf925376e, /// 0x814
			 0xf31bf4a2, /// 0x818
			 0xc6b316a4, /// 0x81c
			 0xdc0eb66d, /// 0x820
			 0x95468b43, /// 0x824
			 0x8fa3abcc, /// 0x828
			 0xd88cdc20, /// 0x82c
			 0x84b41a84, /// 0x830
			 0xc0d8a33a, /// 0x834
			 0x3356d570, /// 0x838
			 0xd7c2d325, /// 0x83c
			 0x0cf51a8c, /// 0x840
			 0x087aa84f, /// 0x844
			 0xc8b5db26, /// 0x848
			 0x8e81cda9, /// 0x84c
			 0xb5253f07, /// 0x850
			 0xcc976b9b, /// 0x854
			 0x3e575964, /// 0x858
			 0x24d0b139, /// 0x85c
			 0xae349080, /// 0x860
			 0x518baa4c, /// 0x864
			 0xa25fecf9, /// 0x868
			 0x7875f206, /// 0x86c
			 0x57d50de1, /// 0x870
			 0xe6e7bf97, /// 0x874
			 0x7d5360e6, /// 0x878
			 0x30da6a32, /// 0x87c
			 0x6b48785b, /// 0x880
			 0x715d4da3, /// 0x884
			 0x25c3ed75, /// 0x888
			 0xd9245c9d, /// 0x88c
			 0x1cacf520, /// 0x890
			 0x38ea786e, /// 0x894
			 0xa8f9c312, /// 0x898
			 0xd142a48b, /// 0x89c
			 0xc11801e9, /// 0x8a0
			 0xe594fd4d, /// 0x8a4
			 0x7d21a341, /// 0x8a8
			 0xe86e5e73, /// 0x8ac
			 0xb3f1a3fb, /// 0x8b0
			 0x6df3b4b0, /// 0x8b4
			 0x9ed93d89, /// 0x8b8
			 0x95ef298d, /// 0x8bc
			 0x0a381ab0, /// 0x8c0
			 0xda5eac50, /// 0x8c4
			 0x24f222e9, /// 0x8c8
			 0x06e15cda, /// 0x8cc
			 0xbbcfcbb6, /// 0x8d0
			 0x5a99c324, /// 0x8d4
			 0x06eba608, /// 0x8d8
			 0xf554f894, /// 0x8dc
			 0x63076adc, /// 0x8e0
			 0xab7145e6, /// 0x8e4
			 0x393b7beb, /// 0x8e8
			 0xee9beb05, /// 0x8ec
			 0xc77f4f34, /// 0x8f0
			 0x67df26ff, /// 0x8f4
			 0x6c950e02, /// 0x8f8
			 0xa7a984a9, /// 0x8fc
			 0x2b785cec, /// 0x900
			 0xac77403c, /// 0x904
			 0xd018aaab, /// 0x908
			 0x4bb0e888, /// 0x90c
			 0xdc163448, /// 0x910
			 0xd342e08f, /// 0x914
			 0x1fc61363, /// 0x918
			 0x7bf512c9, /// 0x91c
			 0xb084f7df, /// 0x920
			 0x7418f43c, /// 0x924
			 0x364bd505, /// 0x928
			 0xb0b0a5e1, /// 0x92c
			 0x68f11db5, /// 0x930
			 0x5456b4ee, /// 0x934
			 0x6e77d820, /// 0x938
			 0x0917a96a, /// 0x93c
			 0x64bd52f4, /// 0x940
			 0xbcafb859, /// 0x944
			 0x933828b4, /// 0x948
			 0xeb620435, /// 0x94c
			 0x00fefb48, /// 0x950
			 0x6dd16026, /// 0x954
			 0xe228cf53, /// 0x958
			 0x03b2f3b7, /// 0x95c
			 0xa7a41ed2, /// 0x960
			 0xfb93bfd0, /// 0x964
			 0x48a09382, /// 0x968
			 0xf09f4410, /// 0x96c
			 0xc92dbf81, /// 0x970
			 0x0637afac, /// 0x974
			 0xe1e9d707, /// 0x978
			 0x1b09d7f0, /// 0x97c
			 0xf07791a4, /// 0x980
			 0x5b4bf65b, /// 0x984
			 0x9e860952, /// 0x988
			 0xe27a4525, /// 0x98c
			 0xf06e0eb8, /// 0x990
			 0xcc28a504, /// 0x994
			 0x218250bc, /// 0x998
			 0x08c10262, /// 0x99c
			 0xe2f8d066, /// 0x9a0
			 0xf03f5b65, /// 0x9a4
			 0x1d61ae3b, /// 0x9a8
			 0xbdf89a2e, /// 0x9ac
			 0xedf34468, /// 0x9b0
			 0x9a12fabb, /// 0x9b4
			 0x9260c491, /// 0x9b8
			 0xc5c9dd88, /// 0x9bc
			 0x9da52091, /// 0x9c0
			 0x18f2ccc9, /// 0x9c4
			 0x3bdf5931, /// 0x9c8
			 0xe6f3b7c9, /// 0x9cc
			 0xc04feb8b, /// 0x9d0
			 0xf2354adc, /// 0x9d4
			 0x8913da69, /// 0x9d8
			 0xde79ff40, /// 0x9dc
			 0xf53431c9, /// 0x9e0
			 0x7d1641b2, /// 0x9e4
			 0xc386ee36, /// 0x9e8
			 0x7d4df925, /// 0x9ec
			 0x0ed33059, /// 0x9f0
			 0x93493c5a, /// 0x9f4
			 0x4b005210, /// 0x9f8
			 0x665c3031, /// 0x9fc
			 0xd7db91c8, /// 0xa00
			 0x568ee559, /// 0xa04
			 0x3e6e8d42, /// 0xa08
			 0xbabe6035, /// 0xa0c
			 0x90684115, /// 0xa10
			 0x0d0e32b5, /// 0xa14
			 0x626dacf4, /// 0xa18
			 0x4e15849f, /// 0xa1c
			 0x0c0d9e2e, /// 0xa20
			 0x8cfae663, /// 0xa24
			 0xadbd38cf, /// 0xa28
			 0xc8ffd6b1, /// 0xa2c
			 0x4c8de6ae, /// 0xa30
			 0x3ec869a1, /// 0xa34
			 0x7b652807, /// 0xa38
			 0xe6ddb47f, /// 0xa3c
			 0xc58b91f4, /// 0xa40
			 0xf3ded2cf, /// 0xa44
			 0x4d1423bb, /// 0xa48
			 0x80ed3a51, /// 0xa4c
			 0x4653966b, /// 0xa50
			 0x0a22d185, /// 0xa54
			 0x70fbc09d, /// 0xa58
			 0xe33ffebc, /// 0xa5c
			 0x9125ac2a, /// 0xa60
			 0xe16cca69, /// 0xa64
			 0xfa71e75d, /// 0xa68
			 0x58d6181c, /// 0xa6c
			 0xfe1d67fc, /// 0xa70
			 0xdb6aa9cf, /// 0xa74
			 0xc0d2c07d, /// 0xa78
			 0x23920d73, /// 0xa7c
			 0x208060bf, /// 0xa80
			 0x49d0b60b, /// 0xa84
			 0xf8a87fa2, /// 0xa88
			 0x34813e27, /// 0xa8c
			 0x7af0b689, /// 0xa90
			 0x83cfbb04, /// 0xa94
			 0x3ae51a1c, /// 0xa98
			 0xd1a3dc5b, /// 0xa9c
			 0x4329af1e, /// 0xaa0
			 0xb162e0a1, /// 0xaa4
			 0xd066fe15, /// 0xaa8
			 0x2c2dfa14, /// 0xaac
			 0xdee5a3b8, /// 0xab0
			 0x91066ce4, /// 0xab4
			 0x3431acfc, /// 0xab8
			 0x5aaf99f3, /// 0xabc
			 0x731c2e91, /// 0xac0
			 0x0b4fb815, /// 0xac4
			 0x094802c6, /// 0xac8
			 0x76b815b8, /// 0xacc
			 0xc3e37a39, /// 0xad0
			 0x1625abdc, /// 0xad4
			 0x07beefc0, /// 0xad8
			 0x6ade7f11, /// 0xadc
			 0xe4cea6c6, /// 0xae0
			 0xb22bad2a, /// 0xae4
			 0x4bad09e8, /// 0xae8
			 0x47c16335, /// 0xaec
			 0xea067393, /// 0xaf0
			 0x3678ea9a, /// 0xaf4
			 0x91ef57f3, /// 0xaf8
			 0xe04f6d78, /// 0xafc
			 0x4f1d63b9, /// 0xb00
			 0x37f612a7, /// 0xb04
			 0xc29d32cc, /// 0xb08
			 0x36e34adf, /// 0xb0c
			 0xe2168a4e, /// 0xb10
			 0x01a41fbf, /// 0xb14
			 0xce344d7d, /// 0xb18
			 0xbfa9f448, /// 0xb1c
			 0xf2cbd089, /// 0xb20
			 0x33993704, /// 0xb24
			 0x79590230, /// 0xb28
			 0x74dd9dbe, /// 0xb2c
			 0x1cfa40e9, /// 0xb30
			 0x39cd0a06, /// 0xb34
			 0x37965662, /// 0xb38
			 0x2ea7c3a9, /// 0xb3c
			 0x9613f241, /// 0xb40
			 0xf01383d9, /// 0xb44
			 0x494ec46b, /// 0xb48
			 0xf3c67935, /// 0xb4c
			 0x0ec1c6e1, /// 0xb50
			 0xcb64a5ee, /// 0xb54
			 0x41f05bde, /// 0xb58
			 0x828cd83f, /// 0xb5c
			 0x8fd56a01, /// 0xb60
			 0x74752bd4, /// 0xb64
			 0xb2f102bf, /// 0xb68
			 0xa71ac9b5, /// 0xb6c
			 0xcf47845e, /// 0xb70
			 0x13d9e4df, /// 0xb74
			 0x7c8952e1, /// 0xb78
			 0x24126d42, /// 0xb7c
			 0xdf28db58, /// 0xb80
			 0xc1068175, /// 0xb84
			 0x912bcded, /// 0xb88
			 0x79ae6116, /// 0xb8c
			 0x77bc8494, /// 0xb90
			 0xa422bacb, /// 0xb94
			 0xf387e751, /// 0xb98
			 0xc5a4bc1d, /// 0xb9c
			 0x56bcedb9, /// 0xba0
			 0x422f677c, /// 0xba4
			 0x7d7c7d31, /// 0xba8
			 0x256ec675, /// 0xbac
			 0x7cf7b7b4, /// 0xbb0
			 0x5d7d8ff3, /// 0xbb4
			 0x70474180, /// 0xbb8
			 0x6c8ab50c, /// 0xbbc
			 0xf5089087, /// 0xbc0
			 0x8660e175, /// 0xbc4
			 0x69560977, /// 0xbc8
			 0x6044cb4b, /// 0xbcc
			 0xddca10f3, /// 0xbd0
			 0x8729b9d2, /// 0xbd4
			 0x307ea3a6, /// 0xbd8
			 0x03416121, /// 0xbdc
			 0x21672667, /// 0xbe0
			 0x3d8925a4, /// 0xbe4
			 0x7d304dff, /// 0xbe8
			 0x6dc163a2, /// 0xbec
			 0x23dcb78c, /// 0xbf0
			 0xc9fdbe75, /// 0xbf4
			 0x5159d140, /// 0xbf8
			 0xbdaae2ca, /// 0xbfc
			 0xba245b86, /// 0xc00
			 0xade4fc08, /// 0xc04
			 0x0d974b9d, /// 0xc08
			 0x40aa2e8f, /// 0xc0c
			 0x18289e2d, /// 0xc10
			 0x38269af7, /// 0xc14
			 0x5c416a74, /// 0xc18
			 0xabcd3b77, /// 0xc1c
			 0x114ef4b0, /// 0xc20
			 0xb43894df, /// 0xc24
			 0x9914f823, /// 0xc28
			 0xf8578255, /// 0xc2c
			 0xf397712d, /// 0xc30
			 0x4e8866cf, /// 0xc34
			 0xeb48cb37, /// 0xc38
			 0x2c7d4690, /// 0xc3c
			 0xfd649ec9, /// 0xc40
			 0xd05fca3b, /// 0xc44
			 0x9648ce8e, /// 0xc48
			 0xcfa0cddc, /// 0xc4c
			 0xa258dc11, /// 0xc50
			 0x213dd248, /// 0xc54
			 0x0174c4da, /// 0xc58
			 0xe8fab533, /// 0xc5c
			 0x33d9ab64, /// 0xc60
			 0xf3602a7c, /// 0xc64
			 0x892789c8, /// 0xc68
			 0x1f5c16a7, /// 0xc6c
			 0xcb735a44, /// 0xc70
			 0x9b490125, /// 0xc74
			 0xf3706e96, /// 0xc78
			 0x523e1b28, /// 0xc7c
			 0xb8a48e98, /// 0xc80
			 0x8e9ee8e5, /// 0xc84
			 0xe8d95921, /// 0xc88
			 0xfb998ac7, /// 0xc8c
			 0xbac37438, /// 0xc90
			 0x1fa502da, /// 0xc94
			 0xf90472e4, /// 0xc98
			 0x3b02d56d, /// 0xc9c
			 0x507fc7e7, /// 0xca0
			 0x7961f398, /// 0xca4
			 0x06368224, /// 0xca8
			 0xf6dadfdf, /// 0xcac
			 0x49720dfa, /// 0xcb0
			 0xa97a7955, /// 0xcb4
			 0xd867b118, /// 0xcb8
			 0x0b39424a, /// 0xcbc
			 0xac6b53bf, /// 0xcc0
			 0x432c7371, /// 0xcc4
			 0x0dae8e7b, /// 0xcc8
			 0x31cdb2df, /// 0xccc
			 0xacafdbbf, /// 0xcd0
			 0xa06ba676, /// 0xcd4
			 0x63fab28e, /// 0xcd8
			 0x4ee9df5f, /// 0xcdc
			 0xd5430b13, /// 0xce0
			 0x4a6922bc, /// 0xce4
			 0x1e05b117, /// 0xce8
			 0x8cd7b319, /// 0xcec
			 0xd2fc94a8, /// 0xcf0
			 0xcb383212, /// 0xcf4
			 0x9c2f0aab, /// 0xcf8
			 0x007195b2, /// 0xcfc
			 0x1df85731, /// 0xd00
			 0x3c4840a6, /// 0xd04
			 0xa7019839, /// 0xd08
			 0x4f4e0068, /// 0xd0c
			 0xc1728852, /// 0xd10
			 0xc88b0447, /// 0xd14
			 0xa12f5576, /// 0xd18
			 0x2d9cf626, /// 0xd1c
			 0x68b76db1, /// 0xd20
			 0x217de38d, /// 0xd24
			 0x3cf9e0bf, /// 0xd28
			 0x3a2c4810, /// 0xd2c
			 0x16d3bacb, /// 0xd30
			 0xa2ba758c, /// 0xd34
			 0x004e64b0, /// 0xd38
			 0xa6e9141b, /// 0xd3c
			 0x29adeb39, /// 0xd40
			 0xc95b3fba, /// 0xd44
			 0x416015d7, /// 0xd48
			 0x8f13c662, /// 0xd4c
			 0x747466ec, /// 0xd50
			 0x38b1a033, /// 0xd54
			 0xb05f1c7d, /// 0xd58
			 0x3a634446, /// 0xd5c
			 0x6e3fc907, /// 0xd60
			 0xd99ae15c, /// 0xd64
			 0x397185aa, /// 0xd68
			 0x4987b9d5, /// 0xd6c
			 0xddf57f63, /// 0xd70
			 0x6ea58b38, /// 0xd74
			 0x16be7838, /// 0xd78
			 0xb662aabd, /// 0xd7c
			 0x773bbb51, /// 0xd80
			 0x12383e8e, /// 0xd84
			 0xd2aceff1, /// 0xd88
			 0x0658c031, /// 0xd8c
			 0x22550491, /// 0xd90
			 0x7fbaef87, /// 0xd94
			 0x58031624, /// 0xd98
			 0x002e8cf9, /// 0xd9c
			 0xfdb8ee05, /// 0xda0
			 0xdd483763, /// 0xda4
			 0x8a4398d7, /// 0xda8
			 0x4f1d576e, /// 0xdac
			 0x4ca75e6f, /// 0xdb0
			 0x958a8d39, /// 0xdb4
			 0x1ff5061f, /// 0xdb8
			 0x93c4b7ed, /// 0xdbc
			 0xe09a581c, /// 0xdc0
			 0xf5ef1341, /// 0xdc4
			 0x00539eaf, /// 0xdc8
			 0x2ea38c6d, /// 0xdcc
			 0x1c42c763, /// 0xdd0
			 0x454457c4, /// 0xdd4
			 0x6a3076e1, /// 0xdd8
			 0x0fa2debb, /// 0xddc
			 0x5938ffea, /// 0xde0
			 0xdce31422, /// 0xde4
			 0x48584883, /// 0xde8
			 0x18b3427f, /// 0xdec
			 0x56cdf5e5, /// 0xdf0
			 0xde2bd00c, /// 0xdf4
			 0xe21a7274, /// 0xdf8
			 0x1efe6487, /// 0xdfc
			 0x6a0375f6, /// 0xe00
			 0x4ca87dba, /// 0xe04
			 0xe99185d4, /// 0xe08
			 0x816ca717, /// 0xe0c
			 0xb6130899, /// 0xe10
			 0xa18ae708, /// 0xe14
			 0x37bb04f7, /// 0xe18
			 0x7fdb2c62, /// 0xe1c
			 0x43b92854, /// 0xe20
			 0x0846573b, /// 0xe24
			 0xf3a78a19, /// 0xe28
			 0x8eaaaa42, /// 0xe2c
			 0x35a4921b, /// 0xe30
			 0xd1531156, /// 0xe34
			 0xd942623d, /// 0xe38
			 0xd08b86a7, /// 0xe3c
			 0x99c3ae38, /// 0xe40
			 0x3d67772c, /// 0xe44
			 0x0d21a11d, /// 0xe48
			 0x6931171f, /// 0xe4c
			 0x938b5c83, /// 0xe50
			 0x13ff9df3, /// 0xe54
			 0x3c25c081, /// 0xe58
			 0x87cb13ca, /// 0xe5c
			 0xaff7c71f, /// 0xe60
			 0xd0d67e54, /// 0xe64
			 0x4ee815b9, /// 0xe68
			 0x3a09a3ec, /// 0xe6c
			 0xd42adbc5, /// 0xe70
			 0x6b02e91a, /// 0xe74
			 0x7a971f4d, /// 0xe78
			 0x013dfbe3, /// 0xe7c
			 0x8230ad1c, /// 0xe80
			 0x8416a09c, /// 0xe84
			 0x2989b62f, /// 0xe88
			 0x3c8b7ad5, /// 0xe8c
			 0x8d19c3dd, /// 0xe90
			 0xdec099cc, /// 0xe94
			 0xc4e50732, /// 0xe98
			 0x73bd215f, /// 0xe9c
			 0x58b70e9d, /// 0xea0
			 0xe8dd5329, /// 0xea4
			 0xed1edb24, /// 0xea8
			 0x2bc2c33e, /// 0xeac
			 0xb85635e7, /// 0xeb0
			 0x3d445192, /// 0xeb4
			 0xe4d45dc7, /// 0xeb8
			 0x51162e85, /// 0xebc
			 0xb76afe47, /// 0xec0
			 0x4c2c7477, /// 0xec4
			 0x23851c5e, /// 0xec8
			 0x2e73eacb, /// 0xecc
			 0xcf02b8b4, /// 0xed0
			 0x7d5ce557, /// 0xed4
			 0x21baa00e, /// 0xed8
			 0xe428276d, /// 0xedc
			 0xd3dd545e, /// 0xee0
			 0x9d1a2c44, /// 0xee4
			 0xedb9090f, /// 0xee8
			 0xabaa3802, /// 0xeec
			 0xd4b2a36f, /// 0xef0
			 0x689aabfe, /// 0xef4
			 0xff53e25d, /// 0xef8
			 0x7b7bb9ea, /// 0xefc
			 0x044ab7c3, /// 0xf00
			 0x05839185, /// 0xf04
			 0x354d19ad, /// 0xf08
			 0xb6017a70, /// 0xf0c
			 0x2d424510, /// 0xf10
			 0xa7938a75, /// 0xf14
			 0xfb488d6d, /// 0xf18
			 0xcb46203f, /// 0xf1c
			 0x51b40d8d, /// 0xf20
			 0x194339ac, /// 0xf24
			 0x851b1d7e, /// 0xf28
			 0xa4a1f006, /// 0xf2c
			 0x7c6afbd4, /// 0xf30
			 0xc510ae19, /// 0xf34
			 0x95ab9ab4, /// 0xf38
			 0x4d4d3ece, /// 0xf3c
			 0x60508501, /// 0xf40
			 0x22be9beb, /// 0xf44
			 0x6f3773fa, /// 0xf48
			 0x165fcd6e, /// 0xf4c
			 0xff7bfdc9, /// 0xf50
			 0xf05c233a, /// 0xf54
			 0xfb2603b9, /// 0xf58
			 0x10f85a4a, /// 0xf5c
			 0xd81da70e, /// 0xf60
			 0x8dd79962, /// 0xf64
			 0x42bd6dee, /// 0xf68
			 0x4baea4fd, /// 0xf6c
			 0xa1511ee0, /// 0xf70
			 0x79af3208, /// 0xf74
			 0x34ff36ba, /// 0xf78
			 0xc5d3a72f, /// 0xf7c
			 0xeca36593, /// 0xf80
			 0x03b0b2a4, /// 0xf84
			 0xfe158684, /// 0xf88
			 0x6772d8d6, /// 0xf8c
			 0xed959293, /// 0xf90
			 0xd9f2362c, /// 0xf94
			 0x1791392b, /// 0xf98
			 0x94341745, /// 0xf9c
			 0xe09a962f, /// 0xfa0
			 0x541e9f21, /// 0xfa4
			 0x436892e1, /// 0xfa8
			 0x3121b811, /// 0xfac
			 0xfd9e2b60, /// 0xfb0
			 0x5b7f03c1, /// 0xfb4
			 0x0107e196, /// 0xfb8
			 0x94f9030b, /// 0xfbc
			 0x3e179867, /// 0xfc0
			 0xa6f7e9a1, /// 0xfc4
			 0xf1d53200, /// 0xfc8
			 0x93a6617c, /// 0xfcc
			 0x0d8637f4, /// 0xfd0
			 0xb2af5f6d, /// 0xfd4
			 0x686618f9, /// 0xfd8
			 0x900ba60a, /// 0xfdc
			 0x756d1183, /// 0xfe0
			 0x3a20ccf2, /// 0xfe4
			 0x8c092785, /// 0xfe8
			 0xdc36f675, /// 0xfec
			 0x365771f7, /// 0xff0
			 0x4590c8b3, /// 0xff4
			 0x4beb3331, /// 0xff8
			 0xfda7ee98 /// last
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
			 0x0000032c,
			 0x000003e0,
			 0x00000544,
			 0x000003e4,
			 0x00000528,
			 0x000000b8,
			 0x000002bc,
			 0x000003f4,

			 /// vpu register f2
			 0x41b80000,
			 0x41c80000,
			 0x41c00000,
			 0x40e00000,
			 0x41800000,
			 0x41880000,
			 0x41f80000,
			 0x41a00000,

			 /// vpu register f3
			 0x41000000,
			 0x40e00000,
			 0x41a00000,
			 0x40a00000,
			 0x41a80000,
			 0x41500000,
			 0x41700000,
			 0x40e00000,

			 /// vpu register f4
			 0x41200000,
			 0x41400000,
			 0x40e00000,
			 0x41900000,
			 0x41f00000,
			 0x41200000,
			 0x41c00000,
			 0x41c00000,

			 /// vpu register f5
			 0x40e00000,
			 0x41000000,
			 0x41a00000,
			 0x41b00000,
			 0x41a00000,
			 0x41100000,
			 0x41a80000,
			 0x41f00000,

			 /// vpu register f6
			 0x41c00000,
			 0x41200000,
			 0x41300000,
			 0x41a80000,
			 0x41880000,
			 0x41d00000,
			 0x40c00000,
			 0x41200000,

			 /// vpu register f7
			 0x41a00000,
			 0x40400000,
			 0x41a80000,
			 0x40c00000,
			 0x42000000,
			 0x41f00000,
			 0x41800000,
			 0x41600000,

			 /// vpu register f8
			 0x40800000,
			 0x41e80000,
			 0x41e00000,
			 0x41600000,
			 0x41100000,
			 0x40400000,
			 0x41b00000,
			 0x41d00000,

			 /// vpu register f9
			 0x41a80000,
			 0x41c80000,
			 0x41200000,
			 0x41500000,
			 0x41400000,
			 0x41c80000,
			 0x41f80000,
			 0x40400000,

			 /// vpu register f10
			 0x41300000,
			 0x41300000,
			 0x41600000,
			 0x41400000,
			 0x40a00000,
			 0x41e00000,
			 0x41500000,
			 0x41b80000,

			 /// vpu register f11
			 0x41800000,
			 0x42000000,
			 0x40000000,
			 0x41000000,
			 0x41980000,
			 0x41b80000,
			 0x41800000,
			 0x41500000,

			 /// vpu register f12
			 0x41e80000,
			 0x41400000,
			 0x3f800000,
			 0x41e80000,
			 0x41980000,
			 0x3f800000,
			 0x40e00000,
			 0x41b00000,

			 /// vpu register f13
			 0x41b80000,
			 0x41980000,
			 0x41c80000,
			 0x41e00000,
			 0x41c00000,
			 0x41200000,
			 0x41f80000,
			 0x41300000,

			 /// vpu register f14
			 0x40400000,
			 0x41a80000,
			 0x41980000,
			 0x41400000,
			 0x41b80000,
			 0x41900000,
			 0x3f800000,
			 0x41f80000,

			 /// vpu register f15
			 0x41700000,
			 0x41900000,
			 0x41f00000,
			 0x40800000,
			 0x40a00000,
			 0x41c80000,
			 0x41f80000,
			 0x41b80000,

			 /// vpu register f16
			 0x42000000,
			 0x41e00000,
			 0x41a00000,
			 0x41d00000,
			 0x41e80000,
			 0x41700000,
			 0x41a00000,
			 0x41e00000,

			 /// vpu register f17
			 0x41e80000,
			 0x41a00000,
			 0x40000000,
			 0x3f800000,
			 0x41300000,
			 0x41900000,
			 0x41980000,
			 0x41000000,

			 /// vpu register f18
			 0x41800000,
			 0x40a00000,
			 0x41b00000,
			 0x41d00000,
			 0x41e80000,
			 0x41600000,
			 0x3f800000,
			 0x42000000,

			 /// vpu register f19
			 0x40800000,
			 0x41c00000,
			 0x41200000,
			 0x41400000,
			 0x41d80000,
			 0x42000000,
			 0x3f800000,
			 0x41500000,

			 /// vpu register f20
			 0x40e00000,
			 0x40a00000,
			 0x41c00000,
			 0x3f800000,
			 0x41600000,
			 0x40800000,
			 0x41e00000,
			 0x41200000,

			 /// vpu register f21
			 0x41900000,
			 0x41f80000,
			 0x41700000,
			 0x41c80000,
			 0x41d80000,
			 0x40c00000,
			 0x41b80000,
			 0x40800000,

			 /// vpu register f22
			 0x41c00000,
			 0x41a00000,
			 0x41300000,
			 0x41000000,
			 0x41d00000,
			 0x41e80000,
			 0x40e00000,
			 0x41980000,

			 /// vpu register f23
			 0x41900000,
			 0x41f00000,
			 0x41600000,
			 0x41c80000,
			 0x40800000,
			 0x41a00000,
			 0x41980000,
			 0x40c00000,

			 /// vpu register f24
			 0x41e00000,
			 0x41700000,
			 0x40e00000,
			 0x41400000,
			 0x40a00000,
			 0x40000000,
			 0x41f00000,
			 0x40400000,

			 /// vpu register f25
			 0x41000000,
			 0x41900000,
			 0x40000000,
			 0x41800000,
			 0x41100000,
			 0x41600000,
			 0x41000000,
			 0x41a80000,

			 /// vpu register f26
			 0x41600000,
			 0x41b80000,
			 0x41f80000,
			 0x3f800000,
			 0x41a00000,
			 0x41500000,
			 0x40a00000,
			 0x40400000,

			 /// vpu register f27
			 0x41b80000,
			 0x3f800000,
			 0x41d00000,
			 0x42000000,
			 0x41800000,
			 0x40e00000,
			 0x41500000,
			 0x40e00000,

			 /// vpu register f28
			 0x41a80000,
			 0x41300000,
			 0x41c00000,
			 0x41800000,
			 0x41a80000,
			 0x41980000,
			 0x41100000,
			 0x41900000,

			 /// vpu register f29
			 0x41c80000,
			 0x42000000,
			 0x41d80000,
			 0x41700000,
			 0x41a00000,
			 0x3f800000,
			 0x41f00000,
			 0x41600000,

			 /// vpu register f30
			 0x41a80000,
			 0x41000000,
			 0x41e80000,
			 0x41f00000,
			 0x41880000,
			 0x41000000,
			 0x41f00000,
			 0x42000000,

			 /// vpu register f31
			 0x41d80000,
			 0x41000000,
			 0x41f80000,
			 0x41a00000,
			 0x3f800000,
			 0x41100000,
			 0x40000000,
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
		"maskor m4, m7, m3\n"                                 ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m6, m2, m4\n"                                 ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m5, m7, m4\n"                                 ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m1, m4, m6\n"                                 ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m2, m6, m5\n"                                 ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m5, m7, m0\n"                                 ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m6, m2, m6\n"                                 ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m7, m3, m0\n"                                 ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m3, m2, m0\n"                                 ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m5, m6, m7\n"                                 ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m3, m2, m2\n"                                 ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m6, m4, m2\n"                                 ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m0, m0, m7\n"                                 ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m7, m7, m5\n"                                 ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m4, m7, m1\n"                                 ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m1, m5, m4\n"                                 ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m2, m1, m5\n"                                 ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m6, m5, m1\n"                                 ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m4, m1, m1\n"                                 ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m3, m2, m7\n"                                 ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m7, m3, m2\n"                                 ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m6, m6, m4\n"                                 ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m6, m1, m2\n"                                 ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m5, m0, m4\n"                                 ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m1, m0, m0\n"                                 ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m1, m2, m5\n"                                 ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m7, m6, m6\n"                                 ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m6, m1, m1\n"                                 ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m0, m3, m6\n"                                 ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m1, m3, m7\n"                                 ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m5, m3, m1\n"                                 ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m5, m7, m3\n"                                 ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m0, m6, m2\n"                                 ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m0, m3, m2\n"                                 ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m4, m1, m5\n"                                 ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m1, m4, m1\n"                                 ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m1, m6, m5\n"                                 ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m0, m2, m6\n"                                 ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m7, m1, m7\n"                                 ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m0, m1, m3\n"                                 ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m6, m1, m2\n"                                 ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m6, m7, m3\n"                                 ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m6, m2, m0\n"                                 ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m6, m1, m6\n"                                 ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m4, m1, m6\n"                                 ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m1, m7, m3\n"                                 ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m3, m6, m4\n"                                 ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m7, m2, m2\n"                                 ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m0, m4, m7\n"                                 ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m7, m2, m5\n"                                 ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m1, m6, m7\n"                                 ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m6, m2, m0\n"                                 ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m4, m3, m6\n"                                 ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m0, m6, m6\n"                                 ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m5, m6, m2\n"                                 ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m2, m5, m7\n"                                 ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m2, m1, m0\n"                                 ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m1, m3, m6\n"                                 ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m6, m0, m4\n"                                 ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m7, m1, m3\n"                                 ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m1, m5, m3\n"                                 ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m6, m6, m1\n"                                 ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m2, m3, m4\n"                                 ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m5, m7, m1\n"                                 ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m1, m7, m4\n"                                 ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m6, m1, m7\n"                                 ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m5, m1, m3\n"                                 ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m5, m5, m2\n"                                 ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m7, m0, m6\n"                                 ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m3, m0, m5\n"                                 ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m7, m2, m6\n"                                 ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m2, m4, m6\n"                                 ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m1, m0, m4\n"                                 ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m2, m5, m4\n"                                 ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m3, m6, m6\n"                                 ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m6, m3, m1\n"                                 ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m7, m6, m1\n"                                 ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m0, m4, m0\n"                                 ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m3, m5, m6\n"                                 ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m1, m7, m7\n"                                 ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m7, m7, m6\n"                                 ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m0, m4, m6\n"                                 ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m7, m7, m4\n"                                 ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m6, m1, m3\n"                                 ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m4, m0, m6\n"                                 ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m3, m4, m3\n"                                 ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m6, m7, m3\n"                                 ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m0, m3, m3\n"                                 ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m5, m4, m6\n"                                 ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m7, m1, m3\n"                                 ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m5, m5, m6\n"                                 ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m4, m7, m5\n"                                 ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m1, m7, m5\n"                                 ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m0, m0, m0\n"                                 ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m6, m1, m3\n"                                 ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m2, m2, m3\n"                                 ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m3, m2, m2\n"                                 ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m0, m7, m7\n"                                 ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskor m4, m5, m2\n"                                 ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
