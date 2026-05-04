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
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00000
			 0x7fc00001,                                                  // signaling NaN                               /// 00004
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00008
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0000c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00010
			 0x7fc00001,                                                  // signaling NaN                               /// 00014
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00018
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0001c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00020
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0002c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00030
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00034
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x0e000007,                                                  // Trailing 1s:                                /// 0003c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00040
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00044
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00048
			 0xffc00001,                                                  // -signaling NaN                              /// 0004c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00050
			 0x0c400000,                                                  // Leading 1s:                                 /// 00054
			 0x80011111,                                                  // -9.7958E-41                                 /// 00058
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0005c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00060
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0006c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00070
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00074
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00078
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0007c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00080
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00084
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00088
			 0x7fc00001,                                                  // signaling NaN                               /// 0008c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00090
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00094
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0x0c600000,                                                  // Leading 1s:                                 /// 0009c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 000a0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000a4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000a8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ac
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000b0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000b4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 000b8
			 0x0e000003,                                                  // Trailing 1s:                                /// 000bc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000c0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000c4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 000c8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000cc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 000d4
			 0x33333333,                                                  // 4 random values                             /// 000d8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000e4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000e8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 000ec
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 000f0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000f8
			 0xffc00001,                                                  // -signaling NaN                              /// 000fc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00100
			 0x0e000007,                                                  // Trailing 1s:                                /// 00104
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00108
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0010c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00110
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00114
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00118
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0011c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00120
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00124
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0012c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00130
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00138
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0013c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00140
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00144
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00148
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0014c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00150
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00154
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00158
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0015c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00160
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00164
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00168
			 0x55555555,                                                  // 4 random values                             /// 0016c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00170
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00174
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00178
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0017c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00180
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00184
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00188
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0018c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00190
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00198
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0019c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 001a4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 001ac
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001b0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 001b8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 001bc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 001c0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001c4
			 0x7f800000,                                                  // inf                                         /// 001c8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001d0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001d4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 001dc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001e0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001e4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 001e8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ec
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f0
			 0xff800000,                                                  // -inf                                        /// 001f4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001f8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001fc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00200
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00204
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00208
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00210
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00214
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0021c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00220
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00224
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00228
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0022c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00230
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00234
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00238
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0023c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00240
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00244
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00248
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0024c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00250
			 0xbf028f5c,                                                  // -0.51                                       /// 00254
			 0x0e000003,                                                  // Trailing 1s:                                /// 00258
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0025c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00260
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00264
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00268
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0026c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00270
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00274
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0027c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00280
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00284
			 0x00000000,                                                  // zero                                        /// 00288
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0028c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00294
			 0x0c400000,                                                  // Leading 1s:                                 /// 00298
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0029c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 002a4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ac
			 0x55555555,                                                  // 4 random values                             /// 002b0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002bc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002c0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002c4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 002c8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002cc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 002d4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002dc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002e0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 002e4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ec
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 002f0
			 0x3f028f5c,                                                  // 0.51                                        /// 002f4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00300
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00304
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00308
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0030c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00310
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00314
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00318
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0031c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00320
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00324
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00328
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0032c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00330
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00334
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00338
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0033c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00340
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00344
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00348
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0034c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00354
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00358
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0035c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00360
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00364
			 0x7fc00001,                                                  // signaling NaN                               /// 00368
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0036c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00370
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00374
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00378
			 0x00000000,                                                  // zero                                        /// 0037c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00380
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00384
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0x0c400000,                                                  // Leading 1s:                                 /// 0038c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00390
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00398
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0039c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003a0
			 0x00011111,                                                  // 9.7958E-41                                  /// 003a4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003a8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 003ac
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003b0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003b8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003bc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 003c4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 003c8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 003d0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003d4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003d8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003dc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003e0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e4
			 0x0c780000,                                                  // Leading 1s:                                 /// 003e8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003ec
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003f0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 003f4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 003f8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003fc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00400
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00404
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00408
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0040c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00414
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00418
			 0xbf028f5c,                                                  // -0.51                                       /// 0041c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00420
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00424
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00428
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0042c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00430
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00434
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00438
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0043c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00444
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00448
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0044c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00450
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00454
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00458
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0045c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00460
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00464
			 0x4b000000,                                                  // 8388608.0                                   /// 00468
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0046c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00470
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00474
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00478
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0047c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00480
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00484
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00488
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0048c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00490
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00494
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00498
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0049c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004a0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004a8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ac
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004b0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004b4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 004b8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 004bc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 004c0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004c4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004c8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004cc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 004dc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004e0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004e4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004e8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004ec
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004f0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004f4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004f8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00500
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00504
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0050c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00510
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00514
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00518
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0051c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00520
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00524
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00528
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0052c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00530
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00534
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00538
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00540
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00544
			 0x7f800000,                                                  // inf                                         /// 00548
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0054c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00550
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00554
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00558
			 0x0c700000,                                                  // Leading 1s:                                 /// 0055c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00560
			 0x00011111,                                                  // 9.7958E-41                                  /// 00564
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00568
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0056c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00570
			 0x0e000003,                                                  // Trailing 1s:                                /// 00574
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00578
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0057c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00580
			 0x0c600000,                                                  // Leading 1s:                                 /// 00584
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00588
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0058c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00590
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00594
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00598
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0059c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 005a0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005a4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005ac
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005b0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005b4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 005b8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x0e000003,                                                  // Trailing 1s:                                /// 005c0
			 0xff800000,                                                  // -inf                                        /// 005c4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005c8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005cc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005d4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005d8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 005e0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005e4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005e8
			 0x0c700000,                                                  // Leading 1s:                                 /// 005ec
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005f0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005f4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 005f8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005fc
			 0xcb000000,                                                  // -8388608.0                                  /// 00600
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00604
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0060c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00610
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00614
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00618
			 0x0c600000,                                                  // Leading 1s:                                 /// 0061c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00620
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00624
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00628
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0062c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00634
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00638
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0063c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00640
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00644
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00648
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0064c
			 0xff800000,                                                  // -inf                                        /// 00650
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00654
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00658
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0065c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00660
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00664
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00668
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0066c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00670
			 0x0c700000,                                                  // Leading 1s:                                 /// 00674
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00678
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0067c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00680
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00684
			 0x0e000007,                                                  // Trailing 1s:                                /// 00688
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0068c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00694
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00698
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0069c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006a4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006a8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0x0e000fff,                                                  // Trailing 1s:                                /// 006b0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 006b4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006b8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006c4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 006c8
			 0x33333333,                                                  // 4 random values                             /// 006cc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006d0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006d4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006d8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006dc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 006e0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006e8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006ec
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006f0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 006f4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 006fc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00704
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00708
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0070c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00710
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00714
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00718
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0071c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00720
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00724
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00728
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00730
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00734
			 0xffc00001,                                                  // -signaling NaN                              /// 00738
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0073c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0x0e000001,                                                  // Trailing 1s:                                /// 00744
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00748
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0074c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00750
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00754
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00758
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0075c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00764
			 0x0e000007,                                                  // Trailing 1s:                                /// 00768
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0076c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00770
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00774
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00778
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0077c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00780
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00784
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00788
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0078c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00790
			 0x00011111,                                                  // 9.7958E-41                                  /// 00794
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00798
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0079c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 007a4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 007ac
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007b4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007bc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007c0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007c4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007c8
			 0x0c780000,                                                  // Leading 1s:                                 /// 007cc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007d0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007d4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d8
			 0x0c700000,                                                  // Leading 1s:                                 /// 007dc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 007e0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007e4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 007e8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007f4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 007f8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007fc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00800
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00804
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00808
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0080c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00810
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00818
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0081c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00820
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00824
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00828
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0082c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00834
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00838
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0083c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00840
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00844
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00848
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0084c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00850
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00854
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00858
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0085c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00860
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00864
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00868
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0086c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00870
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00874
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00878
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0087c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00880
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00884
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00888
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0088c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00890
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00894
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0089c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 008a4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008a8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008ac
			 0x0e000007,                                                  // Trailing 1s:                                /// 008b0
			 0x7fc00001,                                                  // signaling NaN                               /// 008b4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008b8
			 0x80011111,                                                  // -9.7958E-41                                 /// 008bc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008c0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 008cc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008d0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008d4
			 0x3f028f5c,                                                  // 0.51                                        /// 008d8
			 0x7fc00001,                                                  // signaling NaN                               /// 008dc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008e0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008e4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ec
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f0
			 0x0c400000,                                                  // Leading 1s:                                 /// 008f4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 008f8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008fc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00900
			 0x00011111,                                                  // 9.7958E-41                                  /// 00904
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00908
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00910
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00914
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00918
			 0x00011111,                                                  // 9.7958E-41                                  /// 0091c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00920
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00924
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00928
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0092c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00930
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00934
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00938
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0093c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00940
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00948
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0094c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00954
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00958
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0095c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00960
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00964
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00968
			 0x0e000001,                                                  // Trailing 1s:                                /// 0096c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00970
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00974
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00978
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00980
			 0x55555555,                                                  // 4 random values                             /// 00984
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00988
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0098c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00990
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00994
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00998
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0099c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009a0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009a8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009ac
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009b0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 009b8
			 0x00011111,                                                  // 9.7958E-41                                  /// 009bc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009c0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 009cc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d4
			 0x4b000000,                                                  // 8388608.0                                   /// 009d8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 009dc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 009e0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ec
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009f0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009f8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009fc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a00
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a08
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a0c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a10
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a14
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a18
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a1c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a20
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a24
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a28
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a2c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a30
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a34
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a38
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a3c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a48
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a4c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0x55555555,                                                  // 4 random values                             /// 00a54
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a5c
			 0xcb000000,                                                  // -8388608.0                                  /// 00a60
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a68
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a6c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a70
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a74
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a78
			 0x7f800000,                                                  // inf                                         /// 00a7c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a84
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a88
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a8c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a90
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a94
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a98
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a9c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00aa0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00aa4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00aa8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00aac
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ab0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ab4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ab8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00acc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ad0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ad4
			 0xffc00001,                                                  // -signaling NaN                              /// 00ad8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00adc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ae0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ae8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00af0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00afc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b00
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b08
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b0c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b10
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b14
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b18
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b1c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b20
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b24
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b28
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b30
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b34
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b38
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b3c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b40
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b44
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b4c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b50
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b54
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b58
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b5c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b60
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b64
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b68
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b70
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b74
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0x4b000000,                                                  // 8388608.0                                   /// 00b7c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b80
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b8c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b90
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b94
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b98
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ba0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ba4
			 0xcb000000,                                                  // -8388608.0                                  /// 00ba8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00bb0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00bb4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00bb8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00bc0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00bc8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00bcc
			 0xcb000000,                                                  // -8388608.0                                  /// 00bd0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00bd8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00bdc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00be0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00be8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00bec
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00bf4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c00
			 0xcb000000,                                                  // -8388608.0                                  /// 00c04
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c08
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c0c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c10
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c14
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c18
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c1c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c20
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c24
			 0x4b000000,                                                  // 8388608.0                                   /// 00c28
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c2c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c30
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c34
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c38
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c3c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c44
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c48
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c4c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c50
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c54
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c58
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c5c
			 0x00000000,                                                  // zero                                        /// 00c60
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c64
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c68
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c70
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c74
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c78
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c7c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c84
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c8c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c90
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c94
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c9c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ca4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00cac
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00cb0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00cb4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00cb8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00cbc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00cc4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ccc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00cd0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00cd4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00cd8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00cdc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ce8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00cec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00cf4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00cfc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d00
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d04
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d08
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d0c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d10
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d14
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d18
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d20
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d24
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d28
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d2c
			 0xcb000000,                                                  // -8388608.0                                  /// 00d30
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d34
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d38
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d40
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d44
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d48
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d4c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d50
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d54
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d5c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d60
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d64
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d68
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d6c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d70
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d74
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d78
			 0x55555555,                                                  // 4 random values                             /// 00d7c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d80
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d84
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d88
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d8c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d90
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d94
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d98
			 0x55555555,                                                  // 4 random values                             /// 00d9c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00da0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00da4
			 0xff800000,                                                  // -inf                                        /// 00da8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00dac
			 0x0e000001,                                                  // Trailing 1s:                                /// 00db0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00db4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00db8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00dbc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00dc0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00dc8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00dcc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00dd0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00dd4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00dd8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ddc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00de0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0x7fc00001,                                                  // signaling NaN                               /// 00de8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00dec
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00df0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dfc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e00
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e04
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e08
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e0c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e10
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e14
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e18
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e1c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e24
			 0x7f800000,                                                  // inf                                         /// 00e28
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e2c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e30
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e34
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e38
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e3c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e40
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e44
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e48
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e4c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e50
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e58
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e5c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e60
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e64
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e68
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e6c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e74
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e7c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e80
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e84
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e88
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e8c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e90
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e98
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e9c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ea4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00eac
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00eb0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00eb8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ebc
			 0x00000000,                                                  // zero                                        /// 00ec0
			 0x7f800000,                                                  // inf                                         /// 00ec4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ec8
			 0x7f800000,                                                  // inf                                         /// 00ecc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ed0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ed4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ed8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00edc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ee0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ee8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eec
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ef0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ef4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ef8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00efc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f00
			 0x7f800000,                                                  // inf                                         /// 00f04
			 0xbf028f5c,                                                  // -0.51                                       /// 00f08
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f0c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f10
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f14
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f18
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f1c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f20
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f24
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f28
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f2c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f30
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f34
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f40
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f44
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f48
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f4c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f50
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f54
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f58
			 0x4b000000,                                                  // 8388608.0                                   /// 00f5c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f60
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f64
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f68
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f6c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f70
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f74
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f78
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f7c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f84
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f88
			 0xff800000,                                                  // -inf                                        /// 00f8c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f90
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f94
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f98
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fa0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fa4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00fa8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00fac
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00fb0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00fb8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00fbc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00fc0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00fc4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00fc8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00fcc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00fd0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00fd8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fdc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00fe0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00fe8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00fec
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff0
			 0x7fc00001,                                                  // signaling NaN                               /// 00ff4
			 0x7f800000,                                                  // inf                                         /// 00ff8
			 0x80000000                                                  // -zero                                       /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0xd33eefb2, /// 0x0
			 0x0fda88fd, /// 0x4
			 0x9f98edbe, /// 0x8
			 0xfb09a4f4, /// 0xc
			 0xcc2fe579, /// 0x10
			 0x182bde44, /// 0x14
			 0xd8d75862, /// 0x18
			 0x6d935369, /// 0x1c
			 0xddbd22aa, /// 0x20
			 0x95fa18a0, /// 0x24
			 0xccbfbfe7, /// 0x28
			 0x4e442f95, /// 0x2c
			 0xf4cce775, /// 0x30
			 0xef37e9e8, /// 0x34
			 0x78db18e4, /// 0x38
			 0x5f8df6f2, /// 0x3c
			 0x11b9b15b, /// 0x40
			 0x360e89ce, /// 0x44
			 0x69223270, /// 0x48
			 0x862ffac8, /// 0x4c
			 0x5c8129fa, /// 0x50
			 0x7dee7a44, /// 0x54
			 0x336e6f7a, /// 0x58
			 0x4e0034c4, /// 0x5c
			 0x54022759, /// 0x60
			 0xf71092bb, /// 0x64
			 0xef985ac4, /// 0x68
			 0x5c37be6c, /// 0x6c
			 0x062e1e50, /// 0x70
			 0x563b51e6, /// 0x74
			 0x70010fe4, /// 0x78
			 0x562da1e0, /// 0x7c
			 0xf1b1f738, /// 0x80
			 0x1d1895f4, /// 0x84
			 0xe9b50e45, /// 0x88
			 0x4dd2b047, /// 0x8c
			 0x7f566b2b, /// 0x90
			 0xb3dd026c, /// 0x94
			 0xf3903ac1, /// 0x98
			 0xec599807, /// 0x9c
			 0x9c443ac9, /// 0xa0
			 0xf1ac803c, /// 0xa4
			 0x0eb01155, /// 0xa8
			 0x3b3de513, /// 0xac
			 0x01c2cc7e, /// 0xb0
			 0xb53c9457, /// 0xb4
			 0x57b20d33, /// 0xb8
			 0x9b609a28, /// 0xbc
			 0xb0ee8d86, /// 0xc0
			 0xf538271f, /// 0xc4
			 0xfa32306a, /// 0xc8
			 0xd032a33f, /// 0xcc
			 0x965d6df4, /// 0xd0
			 0x43ae0b85, /// 0xd4
			 0xdcf502cc, /// 0xd8
			 0xe821081e, /// 0xdc
			 0xdef6447e, /// 0xe0
			 0xa4aea52e, /// 0xe4
			 0xf06fa06c, /// 0xe8
			 0x124b8bbc, /// 0xec
			 0x59a68979, /// 0xf0
			 0xcf4ccb02, /// 0xf4
			 0x5db2e86a, /// 0xf8
			 0x9ed4d111, /// 0xfc
			 0x45de580d, /// 0x100
			 0xd95ab6ca, /// 0x104
			 0xb8728158, /// 0x108
			 0x394d63fd, /// 0x10c
			 0x318f111c, /// 0x110
			 0x1fc49fc4, /// 0x114
			 0x9e155b9c, /// 0x118
			 0xbba43eb3, /// 0x11c
			 0x66fbb90a, /// 0x120
			 0xb61cea14, /// 0x124
			 0xa6665a35, /// 0x128
			 0x114eba96, /// 0x12c
			 0x30505069, /// 0x130
			 0xf566d851, /// 0x134
			 0x7f502cc3, /// 0x138
			 0x034a31e7, /// 0x13c
			 0xb5a54585, /// 0x140
			 0x81d3ea4f, /// 0x144
			 0x205acada, /// 0x148
			 0x034a7081, /// 0x14c
			 0x8f78ca6a, /// 0x150
			 0xc7aac661, /// 0x154
			 0x15893e82, /// 0x158
			 0x5f38839e, /// 0x15c
			 0x25615db5, /// 0x160
			 0xf196ad80, /// 0x164
			 0x808f228e, /// 0x168
			 0xada6721f, /// 0x16c
			 0x1c6ebc13, /// 0x170
			 0x0617da55, /// 0x174
			 0x0e5df73b, /// 0x178
			 0x3aaef550, /// 0x17c
			 0xb08085a1, /// 0x180
			 0x051b01f8, /// 0x184
			 0x82f23d78, /// 0x188
			 0xd1b28828, /// 0x18c
			 0x12924c04, /// 0x190
			 0x713dbc41, /// 0x194
			 0x78c969c9, /// 0x198
			 0x53e04c05, /// 0x19c
			 0xb537d170, /// 0x1a0
			 0x64bb0322, /// 0x1a4
			 0x8fefbf0e, /// 0x1a8
			 0xc4eb9fdf, /// 0x1ac
			 0x6a9b49e6, /// 0x1b0
			 0x0ec457a7, /// 0x1b4
			 0x4a1f9de5, /// 0x1b8
			 0xb058e914, /// 0x1bc
			 0xab1493c1, /// 0x1c0
			 0xcb1134b8, /// 0x1c4
			 0xd276bfa1, /// 0x1c8
			 0x8c2ed8cc, /// 0x1cc
			 0xd4770fc2, /// 0x1d0
			 0x38a5b2f7, /// 0x1d4
			 0x8f8c7cc9, /// 0x1d8
			 0x60b68dbb, /// 0x1dc
			 0xd513736b, /// 0x1e0
			 0x3c78f3e0, /// 0x1e4
			 0x637c89c7, /// 0x1e8
			 0xce41e912, /// 0x1ec
			 0xf0a835c8, /// 0x1f0
			 0x07e37e8c, /// 0x1f4
			 0x16d5a733, /// 0x1f8
			 0x350f8250, /// 0x1fc
			 0x8a4ee6d2, /// 0x200
			 0x05472cc7, /// 0x204
			 0x03f5461c, /// 0x208
			 0x5cd24d9f, /// 0x20c
			 0x24696490, /// 0x210
			 0xf6a1744d, /// 0x214
			 0x84e45bc0, /// 0x218
			 0x0a9cdbed, /// 0x21c
			 0x65132887, /// 0x220
			 0x56496eff, /// 0x224
			 0x4c73c961, /// 0x228
			 0xc119a0d7, /// 0x22c
			 0xce0fedea, /// 0x230
			 0x71ea641a, /// 0x234
			 0x9e0bffd3, /// 0x238
			 0xdad3d5ef, /// 0x23c
			 0x8069b96b, /// 0x240
			 0x730d4800, /// 0x244
			 0x16f64c16, /// 0x248
			 0x12e6e225, /// 0x24c
			 0x86ee6fb5, /// 0x250
			 0x290761d9, /// 0x254
			 0x7b6da06a, /// 0x258
			 0xce1c52d4, /// 0x25c
			 0x859072e7, /// 0x260
			 0x6078a7b0, /// 0x264
			 0x9d1a0fc8, /// 0x268
			 0x20784b6d, /// 0x26c
			 0xa9773fc2, /// 0x270
			 0x15b62912, /// 0x274
			 0x70c2144f, /// 0x278
			 0x84d9e88e, /// 0x27c
			 0x7cc67d7e, /// 0x280
			 0x0b72fc58, /// 0x284
			 0xc381c79b, /// 0x288
			 0xbab54c69, /// 0x28c
			 0xc0b1ac30, /// 0x290
			 0x5f22908c, /// 0x294
			 0xb54f24c9, /// 0x298
			 0xd3dab4f2, /// 0x29c
			 0x1cbc9256, /// 0x2a0
			 0xc6d6a4e7, /// 0x2a4
			 0x58138e0d, /// 0x2a8
			 0xa00314c6, /// 0x2ac
			 0xc3c0f40a, /// 0x2b0
			 0x50adac4f, /// 0x2b4
			 0x2a4af861, /// 0x2b8
			 0x82a9ece4, /// 0x2bc
			 0xfdf4bb3f, /// 0x2c0
			 0x541cefa1, /// 0x2c4
			 0xaece5399, /// 0x2c8
			 0xf500fd5f, /// 0x2cc
			 0xafa0fbf0, /// 0x2d0
			 0x7230e39d, /// 0x2d4
			 0x04e6e207, /// 0x2d8
			 0x8da066b9, /// 0x2dc
			 0x75318f6a, /// 0x2e0
			 0x37ec7034, /// 0x2e4
			 0x32f6659a, /// 0x2e8
			 0xff98bc2d, /// 0x2ec
			 0xdfe1c75d, /// 0x2f0
			 0x55259ec2, /// 0x2f4
			 0xa3407d0d, /// 0x2f8
			 0x64de485b, /// 0x2fc
			 0xbd2ef416, /// 0x300
			 0x0416a296, /// 0x304
			 0x2566777f, /// 0x308
			 0xaf078fab, /// 0x30c
			 0x81289279, /// 0x310
			 0x5eba5610, /// 0x314
			 0x71f0b537, /// 0x318
			 0xc755bb94, /// 0x31c
			 0x7d94bc69, /// 0x320
			 0x2d1ab0b0, /// 0x324
			 0x0da3090b, /// 0x328
			 0xb2aeb0e1, /// 0x32c
			 0x50d03a84, /// 0x330
			 0x2328eb5d, /// 0x334
			 0x18160521, /// 0x338
			 0x3b3f4c63, /// 0x33c
			 0x87eb060f, /// 0x340
			 0xf387a1f0, /// 0x344
			 0xa8ec2575, /// 0x348
			 0xc0612de2, /// 0x34c
			 0x3b25e795, /// 0x350
			 0x0b2b244a, /// 0x354
			 0x5bfd4a19, /// 0x358
			 0xa8360dce, /// 0x35c
			 0xd4995256, /// 0x360
			 0xbd82941b, /// 0x364
			 0x53d50696, /// 0x368
			 0x2412aaf0, /// 0x36c
			 0xa1d1d27e, /// 0x370
			 0x7b23531d, /// 0x374
			 0xc57cefe6, /// 0x378
			 0xd2fff2cf, /// 0x37c
			 0xb55fd597, /// 0x380
			 0xcca729b7, /// 0x384
			 0x1ab9901d, /// 0x388
			 0xc92d39e3, /// 0x38c
			 0xf3526ff4, /// 0x390
			 0xccbfe3cc, /// 0x394
			 0x8e5fa43d, /// 0x398
			 0xfe1e1410, /// 0x39c
			 0x7228e632, /// 0x3a0
			 0xb4e5e849, /// 0x3a4
			 0xdce202fc, /// 0x3a8
			 0x267d1daa, /// 0x3ac
			 0x1613f681, /// 0x3b0
			 0xd7fd36d4, /// 0x3b4
			 0xf6faf258, /// 0x3b8
			 0xc82c97f5, /// 0x3bc
			 0x23e9081d, /// 0x3c0
			 0x05d37147, /// 0x3c4
			 0x251fa771, /// 0x3c8
			 0x9fb8d517, /// 0x3cc
			 0xeae5d68c, /// 0x3d0
			 0x3eea548e, /// 0x3d4
			 0xa93dc616, /// 0x3d8
			 0xf74ca547, /// 0x3dc
			 0x9ecb89a0, /// 0x3e0
			 0x11cc37f0, /// 0x3e4
			 0xce6464a7, /// 0x3e8
			 0x1958fcbd, /// 0x3ec
			 0x40e26cee, /// 0x3f0
			 0x52eb082a, /// 0x3f4
			 0x6b49de01, /// 0x3f8
			 0x90edbf30, /// 0x3fc
			 0x129ced42, /// 0x400
			 0xc50dde69, /// 0x404
			 0xe94cdf17, /// 0x408
			 0xbeb9c24f, /// 0x40c
			 0x407c1ddb, /// 0x410
			 0xf3a8ada2, /// 0x414
			 0xcf09e35b, /// 0x418
			 0xbe030f4e, /// 0x41c
			 0x32b6351a, /// 0x420
			 0xf234be9a, /// 0x424
			 0xbaac4e64, /// 0x428
			 0x044cf63b, /// 0x42c
			 0x7dbb4493, /// 0x430
			 0x7fd0c8df, /// 0x434
			 0x0a75ca5c, /// 0x438
			 0x6499b673, /// 0x43c
			 0x6ec12a39, /// 0x440
			 0xaa5efcfc, /// 0x444
			 0x62bb3f09, /// 0x448
			 0x371f2854, /// 0x44c
			 0x981691bd, /// 0x450
			 0x928bb6a1, /// 0x454
			 0xb1df4bce, /// 0x458
			 0x2a2187c1, /// 0x45c
			 0x45c7ec81, /// 0x460
			 0x59c8c7f1, /// 0x464
			 0x96f89897, /// 0x468
			 0x17f11b71, /// 0x46c
			 0x3c4fdf7b, /// 0x470
			 0x4762e073, /// 0x474
			 0xde9f212d, /// 0x478
			 0xd3a13837, /// 0x47c
			 0x311de322, /// 0x480
			 0x826c0ff4, /// 0x484
			 0xb65e7ceb, /// 0x488
			 0x0e029808, /// 0x48c
			 0xcb34d58b, /// 0x490
			 0x0790691e, /// 0x494
			 0xb2fb007d, /// 0x498
			 0xb309104c, /// 0x49c
			 0xf5f08f73, /// 0x4a0
			 0x52b12c43, /// 0x4a4
			 0x91f664fa, /// 0x4a8
			 0x15957578, /// 0x4ac
			 0x9226d6d2, /// 0x4b0
			 0x90609f9c, /// 0x4b4
			 0xf8dcbd6c, /// 0x4b8
			 0x54317641, /// 0x4bc
			 0x45e11aba, /// 0x4c0
			 0x8572ea68, /// 0x4c4
			 0xc393fbea, /// 0x4c8
			 0xbb157884, /// 0x4cc
			 0x123951c7, /// 0x4d0
			 0xb3cb4ae4, /// 0x4d4
			 0x56b86664, /// 0x4d8
			 0x23159214, /// 0x4dc
			 0xb1b2d23b, /// 0x4e0
			 0x28fee250, /// 0x4e4
			 0xccc4ee74, /// 0x4e8
			 0xc49e54ca, /// 0x4ec
			 0x0bc3991a, /// 0x4f0
			 0x7eb06782, /// 0x4f4
			 0xeed4067f, /// 0x4f8
			 0xbc2c235e, /// 0x4fc
			 0xea155659, /// 0x500
			 0x72987d55, /// 0x504
			 0x472ce0b4, /// 0x508
			 0x2f0bc6a2, /// 0x50c
			 0xbafd6f7f, /// 0x510
			 0x27f36f8e, /// 0x514
			 0xa552f430, /// 0x518
			 0x30ebfe14, /// 0x51c
			 0x4c044fc2, /// 0x520
			 0xfcedede5, /// 0x524
			 0x9f98efab, /// 0x528
			 0x3b78ce6d, /// 0x52c
			 0xd86fdaa0, /// 0x530
			 0xe2039a89, /// 0x534
			 0xb76f00da, /// 0x538
			 0x4d7a1fb8, /// 0x53c
			 0xa146b870, /// 0x540
			 0xc338c0c5, /// 0x544
			 0xb51ed7d7, /// 0x548
			 0xaa795e15, /// 0x54c
			 0x7933b5d2, /// 0x550
			 0xccf12fe7, /// 0x554
			 0x915cd51e, /// 0x558
			 0x9a89ad1c, /// 0x55c
			 0x76619952, /// 0x560
			 0x36f12fa5, /// 0x564
			 0xea111ed1, /// 0x568
			 0x74385483, /// 0x56c
			 0x4ded70e6, /// 0x570
			 0x5197b80d, /// 0x574
			 0xba6fc3d9, /// 0x578
			 0xaf77538c, /// 0x57c
			 0xfd865498, /// 0x580
			 0xc8b4045b, /// 0x584
			 0x27b37ab2, /// 0x588
			 0x6743c330, /// 0x58c
			 0x2ba09201, /// 0x590
			 0x145865cc, /// 0x594
			 0xfc8ecbb9, /// 0x598
			 0x27329d8a, /// 0x59c
			 0x6987e90f, /// 0x5a0
			 0x472cf796, /// 0x5a4
			 0x5508c680, /// 0x5a8
			 0x56c73a30, /// 0x5ac
			 0x41e5d813, /// 0x5b0
			 0xbd024fd9, /// 0x5b4
			 0x8b0a8f6c, /// 0x5b8
			 0xc3a4da97, /// 0x5bc
			 0x805e0580, /// 0x5c0
			 0xf328601c, /// 0x5c4
			 0x26c20ac0, /// 0x5c8
			 0x0e32e0f4, /// 0x5cc
			 0xb7ae7d26, /// 0x5d0
			 0x00cee9b0, /// 0x5d4
			 0xf3c3d07c, /// 0x5d8
			 0xce1871c9, /// 0x5dc
			 0x5c122ab5, /// 0x5e0
			 0xc3a278ae, /// 0x5e4
			 0xaf9a5bad, /// 0x5e8
			 0x7d0c6f0b, /// 0x5ec
			 0x92129597, /// 0x5f0
			 0xe18aeefb, /// 0x5f4
			 0x1e619107, /// 0x5f8
			 0xf133dc46, /// 0x5fc
			 0x9d7fff59, /// 0x600
			 0x7b030bb9, /// 0x604
			 0x4342ba8d, /// 0x608
			 0xbd0ffdca, /// 0x60c
			 0xb049e79f, /// 0x610
			 0xf93c792b, /// 0x614
			 0x58e25f63, /// 0x618
			 0x32a22de9, /// 0x61c
			 0xd7394220, /// 0x620
			 0xfb2c9816, /// 0x624
			 0xd2ea1385, /// 0x628
			 0xad31c024, /// 0x62c
			 0x7220aaf6, /// 0x630
			 0xec494e20, /// 0x634
			 0x51a2b66a, /// 0x638
			 0xab9f28a7, /// 0x63c
			 0x9b6fedb5, /// 0x640
			 0xc2881b0f, /// 0x644
			 0xbf3e26ab, /// 0x648
			 0x0547c035, /// 0x64c
			 0xb0537d18, /// 0x650
			 0xb828e5d7, /// 0x654
			 0x057a7e68, /// 0x658
			 0xa4b7d3e4, /// 0x65c
			 0xf3afbe8c, /// 0x660
			 0x30bbc3e4, /// 0x664
			 0x425fa8f8, /// 0x668
			 0xcf0d2e21, /// 0x66c
			 0xddcaa858, /// 0x670
			 0x6a0ed566, /// 0x674
			 0x175e3911, /// 0x678
			 0x99ce69de, /// 0x67c
			 0x6584e205, /// 0x680
			 0x7b5fe4a1, /// 0x684
			 0x5c5dd164, /// 0x688
			 0xec06c02e, /// 0x68c
			 0x1d6ea4df, /// 0x690
			 0x52a4c677, /// 0x694
			 0x9c05018e, /// 0x698
			 0xf0051f59, /// 0x69c
			 0x522aac9f, /// 0x6a0
			 0x6c460003, /// 0x6a4
			 0xb99312ab, /// 0x6a8
			 0x24d0910b, /// 0x6ac
			 0xa68c89ce, /// 0x6b0
			 0x4560b415, /// 0x6b4
			 0x338777c1, /// 0x6b8
			 0xf61d9c2f, /// 0x6bc
			 0x31576806, /// 0x6c0
			 0x3deeea7c, /// 0x6c4
			 0x720afcac, /// 0x6c8
			 0xad04fd56, /// 0x6cc
			 0xa503611c, /// 0x6d0
			 0x797d76bd, /// 0x6d4
			 0xcea37c57, /// 0x6d8
			 0xdd798c52, /// 0x6dc
			 0x87ef1426, /// 0x6e0
			 0x2d6f856a, /// 0x6e4
			 0x022b1825, /// 0x6e8
			 0x52378083, /// 0x6ec
			 0x8751535a, /// 0x6f0
			 0x97c056a0, /// 0x6f4
			 0xe0a83ab3, /// 0x6f8
			 0xa8354846, /// 0x6fc
			 0xbd72d90c, /// 0x700
			 0x89b06754, /// 0x704
			 0x284e0143, /// 0x708
			 0x88ee094b, /// 0x70c
			 0x20ebc4fd, /// 0x710
			 0xd9dc6c94, /// 0x714
			 0xdb1f88f6, /// 0x718
			 0xe4995a87, /// 0x71c
			 0x13093447, /// 0x720
			 0xc39ac769, /// 0x724
			 0xa43a0596, /// 0x728
			 0x0be1b3c3, /// 0x72c
			 0x46eb1cc5, /// 0x730
			 0x162ff3e8, /// 0x734
			 0xd7c8d67c, /// 0x738
			 0x7b797b8e, /// 0x73c
			 0x7447bca1, /// 0x740
			 0x56ff258c, /// 0x744
			 0xd4cd51fd, /// 0x748
			 0x9fb97dc7, /// 0x74c
			 0x9e6f97a1, /// 0x750
			 0xb642cf4b, /// 0x754
			 0x8692713b, /// 0x758
			 0x5e5e8444, /// 0x75c
			 0x07cf7616, /// 0x760
			 0x933f741c, /// 0x764
			 0xd88bd938, /// 0x768
			 0xab4f86c3, /// 0x76c
			 0xc9acf59d, /// 0x770
			 0x6bd32340, /// 0x774
			 0x1bce4878, /// 0x778
			 0x7ceafd16, /// 0x77c
			 0xfa52e52c, /// 0x780
			 0x116680b0, /// 0x784
			 0xa8abd929, /// 0x788
			 0x29175eec, /// 0x78c
			 0x88f02a8f, /// 0x790
			 0xbb6ce64a, /// 0x794
			 0xcb569218, /// 0x798
			 0xaa4e0d20, /// 0x79c
			 0x7d15451d, /// 0x7a0
			 0x3259b2f4, /// 0x7a4
			 0x129d7c67, /// 0x7a8
			 0x799e76d1, /// 0x7ac
			 0xf3d8ce52, /// 0x7b0
			 0x0303e581, /// 0x7b4
			 0xe349a691, /// 0x7b8
			 0x98e30c7d, /// 0x7bc
			 0x9ca263c1, /// 0x7c0
			 0x5860d45a, /// 0x7c4
			 0xa4e0f0a3, /// 0x7c8
			 0x25d6476c, /// 0x7cc
			 0x4eccb507, /// 0x7d0
			 0x74cc983f, /// 0x7d4
			 0x110d4a8e, /// 0x7d8
			 0x916f9ccb, /// 0x7dc
			 0x9781f27e, /// 0x7e0
			 0x2395aa9e, /// 0x7e4
			 0xa0b3f89b, /// 0x7e8
			 0x54396c45, /// 0x7ec
			 0xd1f3bd91, /// 0x7f0
			 0xb7f8396f, /// 0x7f4
			 0x6bf87ccc, /// 0x7f8
			 0xa67071d1, /// 0x7fc
			 0x007fd99b, /// 0x800
			 0xb7543f66, /// 0x804
			 0xdb5cc5a3, /// 0x808
			 0xe5dc80cb, /// 0x80c
			 0x48ba3f0e, /// 0x810
			 0x6f121f67, /// 0x814
			 0x2f82da23, /// 0x818
			 0xbf41c89a, /// 0x81c
			 0x1e7313c3, /// 0x820
			 0xd8204f85, /// 0x824
			 0x7b837585, /// 0x828
			 0x564bef24, /// 0x82c
			 0x5006b4f6, /// 0x830
			 0xcece2e75, /// 0x834
			 0xef78fd81, /// 0x838
			 0x9618bf11, /// 0x83c
			 0xafdc6fee, /// 0x840
			 0x47ecbd94, /// 0x844
			 0x40a2c4ff, /// 0x848
			 0xe22fc773, /// 0x84c
			 0xc3aecb70, /// 0x850
			 0x52b2766e, /// 0x854
			 0xca26cfe9, /// 0x858
			 0x7be415e7, /// 0x85c
			 0x9e32e834, /// 0x860
			 0xb6bfd0a5, /// 0x864
			 0xd89e89c4, /// 0x868
			 0x7ce1a6a2, /// 0x86c
			 0x5a5106c2, /// 0x870
			 0x29e5782f, /// 0x874
			 0x1a51902b, /// 0x878
			 0x35d6f038, /// 0x87c
			 0xa952afdb, /// 0x880
			 0x9e34e038, /// 0x884
			 0x23492477, /// 0x888
			 0x3308a19c, /// 0x88c
			 0x75fefc6d, /// 0x890
			 0xf08fc962, /// 0x894
			 0x379c0907, /// 0x898
			 0x6e1bdfd1, /// 0x89c
			 0x04df99bc, /// 0x8a0
			 0xbbd8cf5b, /// 0x8a4
			 0xe525a7b9, /// 0x8a8
			 0x99df3182, /// 0x8ac
			 0x50bb5424, /// 0x8b0
			 0x46e90887, /// 0x8b4
			 0xe8770e8d, /// 0x8b8
			 0xf307a526, /// 0x8bc
			 0x9ffee071, /// 0x8c0
			 0xf3cac568, /// 0x8c4
			 0x2e2e8005, /// 0x8c8
			 0x178c5c04, /// 0x8cc
			 0xab83f89f, /// 0x8d0
			 0xc50e1560, /// 0x8d4
			 0xcb20cb0a, /// 0x8d8
			 0x827f4813, /// 0x8dc
			 0xe070a12f, /// 0x8e0
			 0xa9bb5b25, /// 0x8e4
			 0x7c52b017, /// 0x8e8
			 0xf9830db0, /// 0x8ec
			 0xdac96249, /// 0x8f0
			 0x6463abe3, /// 0x8f4
			 0x79864ae3, /// 0x8f8
			 0xdc783604, /// 0x8fc
			 0xcd37f99c, /// 0x900
			 0xd7696da9, /// 0x904
			 0xcb9fee15, /// 0x908
			 0xea1d36cf, /// 0x90c
			 0xdb2c1c92, /// 0x910
			 0x4ff61b20, /// 0x914
			 0xe99ccda7, /// 0x918
			 0x26562fb2, /// 0x91c
			 0x9b6af4aa, /// 0x920
			 0x7faa1fd2, /// 0x924
			 0xf8267bce, /// 0x928
			 0x1772e5b0, /// 0x92c
			 0xa57d8fa5, /// 0x930
			 0xcb80563f, /// 0x934
			 0x8d3278a0, /// 0x938
			 0xd0e4d7ab, /// 0x93c
			 0x0c62bb83, /// 0x940
			 0xa2462ee7, /// 0x944
			 0xd8aed535, /// 0x948
			 0xdaa95a3c, /// 0x94c
			 0x0b25eb3f, /// 0x950
			 0x7d938765, /// 0x954
			 0x2562e52a, /// 0x958
			 0x40cd5ed4, /// 0x95c
			 0x462fde78, /// 0x960
			 0xd9290284, /// 0x964
			 0x25c6a22e, /// 0x968
			 0x3620fe83, /// 0x96c
			 0x27aacaed, /// 0x970
			 0x8985247b, /// 0x974
			 0xed773fb2, /// 0x978
			 0x9680dafc, /// 0x97c
			 0x7a581ed9, /// 0x980
			 0x130d3104, /// 0x984
			 0x95fbe363, /// 0x988
			 0xb3c23a87, /// 0x98c
			 0x44c3487e, /// 0x990
			 0x2faf7bfc, /// 0x994
			 0x366f3ce9, /// 0x998
			 0xfdc18a4b, /// 0x99c
			 0xbdcfb495, /// 0x9a0
			 0x7b6930b1, /// 0x9a4
			 0x59bda2cd, /// 0x9a8
			 0x184cb704, /// 0x9ac
			 0xebfbb815, /// 0x9b0
			 0x74444e35, /// 0x9b4
			 0x5c6e37da, /// 0x9b8
			 0x6e0bb2cf, /// 0x9bc
			 0xa98dfcf6, /// 0x9c0
			 0x465e7694, /// 0x9c4
			 0x0ab2f60b, /// 0x9c8
			 0x628779f5, /// 0x9cc
			 0xdd32429f, /// 0x9d0
			 0xd949109e, /// 0x9d4
			 0x8b5e833d, /// 0x9d8
			 0x1757ccc2, /// 0x9dc
			 0xf090916f, /// 0x9e0
			 0x1c9e7bc2, /// 0x9e4
			 0x49592d3f, /// 0x9e8
			 0xedb1ee1f, /// 0x9ec
			 0x275c3543, /// 0x9f0
			 0x13badeb2, /// 0x9f4
			 0x44d38edc, /// 0x9f8
			 0xcfbe9790, /// 0x9fc
			 0xcd14b9c1, /// 0xa00
			 0x7a84281b, /// 0xa04
			 0xc7369100, /// 0xa08
			 0xd08857c2, /// 0xa0c
			 0x1bb2b5df, /// 0xa10
			 0x28ceb1fd, /// 0xa14
			 0x382b38ce, /// 0xa18
			 0x8a6ee95e, /// 0xa1c
			 0x91a7d5e9, /// 0xa20
			 0x10f0ed2d, /// 0xa24
			 0x7d92d482, /// 0xa28
			 0x1cfa2419, /// 0xa2c
			 0x70bdcb1c, /// 0xa30
			 0xe87a1926, /// 0xa34
			 0xd27ee932, /// 0xa38
			 0xbdc4ba23, /// 0xa3c
			 0xb4b3dc2a, /// 0xa40
			 0x457699e3, /// 0xa44
			 0xc38baf13, /// 0xa48
			 0x01a4f648, /// 0xa4c
			 0xf4f1a5f1, /// 0xa50
			 0x94ff9e89, /// 0xa54
			 0x41e336c3, /// 0xa58
			 0x14c2eada, /// 0xa5c
			 0xab1a19c6, /// 0xa60
			 0x2235e0e4, /// 0xa64
			 0x60410a29, /// 0xa68
			 0x320b4ef3, /// 0xa6c
			 0xd45e2935, /// 0xa70
			 0x99b7f785, /// 0xa74
			 0x50e17875, /// 0xa78
			 0x9630acc4, /// 0xa7c
			 0x61040273, /// 0xa80
			 0x169bf560, /// 0xa84
			 0x604be017, /// 0xa88
			 0xdae1ea59, /// 0xa8c
			 0x3e01811e, /// 0xa90
			 0x041e78b6, /// 0xa94
			 0x44f7e1dc, /// 0xa98
			 0x0e4a8715, /// 0xa9c
			 0xac98b74b, /// 0xaa0
			 0x7583035c, /// 0xaa4
			 0xe9c569aa, /// 0xaa8
			 0xd04e67bd, /// 0xaac
			 0x36638aa5, /// 0xab0
			 0xee1ea6c3, /// 0xab4
			 0xd51cded0, /// 0xab8
			 0x0e6eaa17, /// 0xabc
			 0x0b831bcb, /// 0xac0
			 0xd885ae7b, /// 0xac4
			 0x976f784b, /// 0xac8
			 0xd5bca05d, /// 0xacc
			 0x33f7eba5, /// 0xad0
			 0xecbb5448, /// 0xad4
			 0x03d0c589, /// 0xad8
			 0xe741bb53, /// 0xadc
			 0xe900178a, /// 0xae0
			 0xf88491ba, /// 0xae4
			 0x5d54755e, /// 0xae8
			 0x4c525c5c, /// 0xaec
			 0xd2ff9c5f, /// 0xaf0
			 0x872b42cc, /// 0xaf4
			 0x78345129, /// 0xaf8
			 0x3bea6664, /// 0xafc
			 0x4e099ac6, /// 0xb00
			 0x1c0a9ee5, /// 0xb04
			 0xa9391144, /// 0xb08
			 0x357ecc3f, /// 0xb0c
			 0xd1df94ea, /// 0xb10
			 0xbd091d65, /// 0xb14
			 0x366f7d7a, /// 0xb18
			 0x21fc73d7, /// 0xb1c
			 0x60743443, /// 0xb20
			 0xfaa62751, /// 0xb24
			 0xdb49c337, /// 0xb28
			 0x315d00ad, /// 0xb2c
			 0xb5183717, /// 0xb30
			 0x44276f32, /// 0xb34
			 0xf1d095f9, /// 0xb38
			 0x3fc5155f, /// 0xb3c
			 0xfc841448, /// 0xb40
			 0x87ec6bdc, /// 0xb44
			 0xda1aa4c3, /// 0xb48
			 0x31ccdf37, /// 0xb4c
			 0xe31a1f3d, /// 0xb50
			 0x3f94a720, /// 0xb54
			 0xc99951c3, /// 0xb58
			 0x696c2467, /// 0xb5c
			 0xaacf5198, /// 0xb60
			 0xce9020ac, /// 0xb64
			 0xd638160f, /// 0xb68
			 0xf6e8a291, /// 0xb6c
			 0x3e7c2812, /// 0xb70
			 0x3d0d2585, /// 0xb74
			 0x2a91f75c, /// 0xb78
			 0xa53fe02e, /// 0xb7c
			 0x9b7bcea3, /// 0xb80
			 0x89ff088b, /// 0xb84
			 0x837362ec, /// 0xb88
			 0xf11322d2, /// 0xb8c
			 0xd25d967a, /// 0xb90
			 0x50ed002a, /// 0xb94
			 0x50bbc63b, /// 0xb98
			 0x625ef5aa, /// 0xb9c
			 0x419eed6a, /// 0xba0
			 0xcf5f77d2, /// 0xba4
			 0x3ab0b712, /// 0xba8
			 0xb7f57729, /// 0xbac
			 0x9e5e6b0b, /// 0xbb0
			 0xb99ab715, /// 0xbb4
			 0x2c0412ad, /// 0xbb8
			 0xcf260c7a, /// 0xbbc
			 0x1a2da7f8, /// 0xbc0
			 0x1aac15ca, /// 0xbc4
			 0x8f482723, /// 0xbc8
			 0x1c0860d8, /// 0xbcc
			 0xcc717441, /// 0xbd0
			 0x7c645f1c, /// 0xbd4
			 0x4876e486, /// 0xbd8
			 0x1c614045, /// 0xbdc
			 0xf5454485, /// 0xbe0
			 0x8fc19ed6, /// 0xbe4
			 0xc679b175, /// 0xbe8
			 0x1efc4590, /// 0xbec
			 0x6772e1db, /// 0xbf0
			 0x4208df61, /// 0xbf4
			 0x421b832b, /// 0xbf8
			 0x0f703027, /// 0xbfc
			 0xd41fcf24, /// 0xc00
			 0x153f441d, /// 0xc04
			 0x741a30e0, /// 0xc08
			 0x8db754ef, /// 0xc0c
			 0x0256d0f5, /// 0xc10
			 0xe7250894, /// 0xc14
			 0x7d185eb4, /// 0xc18
			 0x3f94e5b8, /// 0xc1c
			 0x52c0ce56, /// 0xc20
			 0xe9873812, /// 0xc24
			 0x046b00a2, /// 0xc28
			 0x3797f785, /// 0xc2c
			 0x283e2f26, /// 0xc30
			 0x707ca6a8, /// 0xc34
			 0x022ab9d7, /// 0xc38
			 0xd807f1c0, /// 0xc3c
			 0x6add24c8, /// 0xc40
			 0x75871093, /// 0xc44
			 0xab6fdf18, /// 0xc48
			 0x2a925877, /// 0xc4c
			 0x95154068, /// 0xc50
			 0xa2214106, /// 0xc54
			 0xd9f3f148, /// 0xc58
			 0x93c2a112, /// 0xc5c
			 0xf7a92e91, /// 0xc60
			 0x4b544580, /// 0xc64
			 0xe782ccc2, /// 0xc68
			 0x91047365, /// 0xc6c
			 0x36d984b1, /// 0xc70
			 0xe2cdd3a8, /// 0xc74
			 0x82b479d2, /// 0xc78
			 0xf53cac4e, /// 0xc7c
			 0x91c1d1fc, /// 0xc80
			 0xea4c5d11, /// 0xc84
			 0xa1cb0b1e, /// 0xc88
			 0xd4679064, /// 0xc8c
			 0xc0d45634, /// 0xc90
			 0x1fffb6f5, /// 0xc94
			 0x32a5f448, /// 0xc98
			 0x918a573e, /// 0xc9c
			 0xa47b6d02, /// 0xca0
			 0x621b4b3a, /// 0xca4
			 0xf36a366f, /// 0xca8
			 0xfd9d3940, /// 0xcac
			 0x3f704ce5, /// 0xcb0
			 0x48508c8b, /// 0xcb4
			 0xd69dc3d7, /// 0xcb8
			 0x636eb630, /// 0xcbc
			 0x8760dafc, /// 0xcc0
			 0x67a31a66, /// 0xcc4
			 0x4fafc86e, /// 0xcc8
			 0x48b1c35a, /// 0xccc
			 0xe93ce20a, /// 0xcd0
			 0x3d4f7e87, /// 0xcd4
			 0x2ab97e6e, /// 0xcd8
			 0xbb55a11a, /// 0xcdc
			 0x59fa93b0, /// 0xce0
			 0x5fc897a1, /// 0xce4
			 0xa847789b, /// 0xce8
			 0xfef51c0d, /// 0xcec
			 0x2e8ba81e, /// 0xcf0
			 0x8d50078e, /// 0xcf4
			 0xb8cea4ab, /// 0xcf8
			 0x1ea9353d, /// 0xcfc
			 0x5659fe77, /// 0xd00
			 0xf06ec233, /// 0xd04
			 0x475797e0, /// 0xd08
			 0xcebdc44b, /// 0xd0c
			 0x51e2f691, /// 0xd10
			 0xff382db5, /// 0xd14
			 0x4a0c28e3, /// 0xd18
			 0x371a3073, /// 0xd1c
			 0x3fb85da3, /// 0xd20
			 0x8b075319, /// 0xd24
			 0x18fb7792, /// 0xd28
			 0x94b99c89, /// 0xd2c
			 0xe10eeb36, /// 0xd30
			 0xa9e58382, /// 0xd34
			 0x494d8c48, /// 0xd38
			 0xa2b5c0de, /// 0xd3c
			 0xf5d3116f, /// 0xd40
			 0x109d34eb, /// 0xd44
			 0xe3a39bc6, /// 0xd48
			 0x20467d1e, /// 0xd4c
			 0xe39cb474, /// 0xd50
			 0x1cee0112, /// 0xd54
			 0xdef7d8d1, /// 0xd58
			 0x98b130e8, /// 0xd5c
			 0xe48f55ce, /// 0xd60
			 0x51ec9628, /// 0xd64
			 0xdad10e9c, /// 0xd68
			 0x560fb7eb, /// 0xd6c
			 0x5a4fa6ee, /// 0xd70
			 0x9c8c87a1, /// 0xd74
			 0x1e2de028, /// 0xd78
			 0x05a6070a, /// 0xd7c
			 0x2f5745ab, /// 0xd80
			 0x6ae28f36, /// 0xd84
			 0x7066f4db, /// 0xd88
			 0x35475852, /// 0xd8c
			 0xe2b58600, /// 0xd90
			 0xe42ca73d, /// 0xd94
			 0xfd45743f, /// 0xd98
			 0xf9deb843, /// 0xd9c
			 0x17c2c575, /// 0xda0
			 0x0f67d3ff, /// 0xda4
			 0x8d55794f, /// 0xda8
			 0xa5ee4e1b, /// 0xdac
			 0x69f83c42, /// 0xdb0
			 0x4697ea66, /// 0xdb4
			 0xd7b59182, /// 0xdb8
			 0x59c07bf4, /// 0xdbc
			 0x263d128b, /// 0xdc0
			 0x88b9fe64, /// 0xdc4
			 0x13bd75ec, /// 0xdc8
			 0x61dc1b01, /// 0xdcc
			 0xa55c37ae, /// 0xdd0
			 0x5c8f8105, /// 0xdd4
			 0xee29a7c3, /// 0xdd8
			 0x59af3e91, /// 0xddc
			 0x911aca7a, /// 0xde0
			 0xd1bec8ef, /// 0xde4
			 0x917a2630, /// 0xde8
			 0xf5dcb5d8, /// 0xdec
			 0x14222eff, /// 0xdf0
			 0xe3363076, /// 0xdf4
			 0xeffc5165, /// 0xdf8
			 0x9f4ecabe, /// 0xdfc
			 0x6edf94b2, /// 0xe00
			 0x5069a7bf, /// 0xe04
			 0x69146379, /// 0xe08
			 0x7a4fb426, /// 0xe0c
			 0x42c33eb3, /// 0xe10
			 0x772445b4, /// 0xe14
			 0x2a52dcdb, /// 0xe18
			 0xf736a62d, /// 0xe1c
			 0x55ca5796, /// 0xe20
			 0xf95c338a, /// 0xe24
			 0xc8bdcd8e, /// 0xe28
			 0x9c0edc91, /// 0xe2c
			 0x27639272, /// 0xe30
			 0x335443c9, /// 0xe34
			 0x1c067a77, /// 0xe38
			 0x5b4d72fe, /// 0xe3c
			 0xad72a5f2, /// 0xe40
			 0xfaafbf45, /// 0xe44
			 0x791c7c94, /// 0xe48
			 0x876e5cdf, /// 0xe4c
			 0x1213f993, /// 0xe50
			 0x867271a2, /// 0xe54
			 0xa5aba56e, /// 0xe58
			 0xb5d6720f, /// 0xe5c
			 0xb4231edf, /// 0xe60
			 0x228f48f3, /// 0xe64
			 0xba66cb11, /// 0xe68
			 0x63f09dba, /// 0xe6c
			 0x222bba52, /// 0xe70
			 0xbecca49a, /// 0xe74
			 0x68d2c1e4, /// 0xe78
			 0x86ce8470, /// 0xe7c
			 0xe44a90cd, /// 0xe80
			 0x4052a088, /// 0xe84
			 0x94c59a88, /// 0xe88
			 0x62db1b9a, /// 0xe8c
			 0x4b9eef3f, /// 0xe90
			 0x5b74229f, /// 0xe94
			 0x77ab86f4, /// 0xe98
			 0xecef13d2, /// 0xe9c
			 0x7395fcf7, /// 0xea0
			 0x13373174, /// 0xea4
			 0x551ca7d0, /// 0xea8
			 0x9667ee22, /// 0xeac
			 0x3f54a6ab, /// 0xeb0
			 0xc223857d, /// 0xeb4
			 0x0b5449ec, /// 0xeb8
			 0xffccac8b, /// 0xebc
			 0xe8f0fdfd, /// 0xec0
			 0xe6393654, /// 0xec4
			 0x1cb7ca60, /// 0xec8
			 0x076f8937, /// 0xecc
			 0x75e37002, /// 0xed0
			 0x4a24bd32, /// 0xed4
			 0xd7489a42, /// 0xed8
			 0x3903aa8a, /// 0xedc
			 0x84246ebd, /// 0xee0
			 0xed89cff2, /// 0xee4
			 0x29627ff3, /// 0xee8
			 0xd2ad4e6d, /// 0xeec
			 0x7bb321e9, /// 0xef0
			 0xf1d21175, /// 0xef4
			 0xf3eb7c89, /// 0xef8
			 0x3ba1a508, /// 0xefc
			 0xa192aab6, /// 0xf00
			 0xf78a6db8, /// 0xf04
			 0xa76460cf, /// 0xf08
			 0x7ab2166e, /// 0xf0c
			 0xd3513250, /// 0xf10
			 0x7a07cbd0, /// 0xf14
			 0x4e4b52df, /// 0xf18
			 0x95ab4faf, /// 0xf1c
			 0xb323a792, /// 0xf20
			 0x5595b88e, /// 0xf24
			 0x4fb6a30d, /// 0xf28
			 0x2f1c9019, /// 0xf2c
			 0x878c0951, /// 0xf30
			 0xe78e0e50, /// 0xf34
			 0xcbcda96a, /// 0xf38
			 0x6bfb2b22, /// 0xf3c
			 0xbf097cbd, /// 0xf40
			 0x3351fb27, /// 0xf44
			 0xade83458, /// 0xf48
			 0x53a77e7f, /// 0xf4c
			 0x89e64156, /// 0xf50
			 0x897326d0, /// 0xf54
			 0x55b551f8, /// 0xf58
			 0x84fcce35, /// 0xf5c
			 0x9174738c, /// 0xf60
			 0xb9b6418a, /// 0xf64
			 0x8c222c23, /// 0xf68
			 0xf6e468b7, /// 0xf6c
			 0x9de1168d, /// 0xf70
			 0x3597b60d, /// 0xf74
			 0x774189ee, /// 0xf78
			 0xd84d2107, /// 0xf7c
			 0x14891ef2, /// 0xf80
			 0x7b06ab5a, /// 0xf84
			 0x5c493ed1, /// 0xf88
			 0x95c949c8, /// 0xf8c
			 0x4db24474, /// 0xf90
			 0x24675298, /// 0xf94
			 0xe1e2bee4, /// 0xf98
			 0x1be33caa, /// 0xf9c
			 0x11fa2483, /// 0xfa0
			 0x818f26df, /// 0xfa4
			 0x5d9134a2, /// 0xfa8
			 0x192f8779, /// 0xfac
			 0x2d9bd347, /// 0xfb0
			 0x90f29d50, /// 0xfb4
			 0xfea41ff6, /// 0xfb8
			 0xd748f923, /// 0xfbc
			 0x98dce246, /// 0xfc0
			 0xc35eaac7, /// 0xfc4
			 0x71f07fed, /// 0xfc8
			 0x66497da7, /// 0xfcc
			 0x85086f20, /// 0xfd0
			 0xbaee3895, /// 0xfd4
			 0x8683b88e, /// 0xfd8
			 0xe24825b4, /// 0xfdc
			 0x553ee56a, /// 0xfe0
			 0x8925214b, /// 0xfe4
			 0x36092dbe, /// 0xfe8
			 0x5259c808, /// 0xfec
			 0x08af65f5, /// 0xff0
			 0x838d59ee, /// 0xff4
			 0x92875227, /// 0xff8
			 0x988faa22 /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00000
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00004
			 0x7f800000,                                                  // inf                                         /// 00008
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0000c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00010
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00014
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00018
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0001c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00020
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00024
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00028
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0002c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00030
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00038
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0003c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00044
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00048
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0004c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00050
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00054
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00058
			 0x0e000003,                                                  // Trailing 1s:                                /// 0005c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00060
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00064
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00068
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0006c
			 0x7fc00001,                                                  // signaling NaN                               /// 00070
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00074
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00078
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00080
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00084
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00088
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0008c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00090
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00094
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0009c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 000a0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000a4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000ac
			 0x80800000, // min norm                                      // subnormals -ve                              /// 000b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000b8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 000bc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000c0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0x00011111,                                                  // 9.7958E-41                                  /// 000c8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 000cc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000d0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 000d4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 000dc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000e4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ec
			 0x7fc00000,                                                  // cquiet NaN                                  /// 000f0
			 0x80011111,                                                  // -9.7958E-41                                 /// 000f4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000f8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000fc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00104
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00108
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00110
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00114
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00118
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0011c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00120
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00124
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00128
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0012c
			 0xbf028f5c,                                                  // -0.51                                       /// 00130
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00134
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00138
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0013c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00140
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00144
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00148
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0014c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00150
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00154
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00158
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0015c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00160
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00164
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00168
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0016c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00170
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00174
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00178
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0017c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00180
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00184
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00188
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00190
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00194
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00198
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0019c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 001a0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 001a4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ac
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001b4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001b8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001bc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 001c0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001c4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001cc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001d0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001d4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 001d8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001dc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 001e0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001e4
			 0x55555555,                                                  // 4 random values                             /// 001e8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001ec
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001f8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001fc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00200
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00204
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0020c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00210
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00214
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00218
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0021c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00220
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00224
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00228
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0022c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00230
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00234
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00238
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0023c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00240
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00244
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00248
			 0x3f028f5c,                                                  // 0.51                                        /// 0024c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00250
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00254
			 0x33333333,                                                  // 4 random values                             /// 00258
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0025c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00260
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00264
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00270
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00274
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00278
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0027c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00280
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00284
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00288
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0028c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00290
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00294
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00298
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0029c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002a0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002a4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002a8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002b8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 002bc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002c0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002c4
			 0x80011111,                                                  // -9.7958E-41                                 /// 002c8
			 0x80000000,                                                  // -zero                                       /// 002cc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002d0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002d8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002dc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002e0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002e4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002e8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 002ec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002f4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002f8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 002fc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0x0c600000,                                                  // Leading 1s:                                 /// 00304
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00308
			 0xbf028f5c,                                                  // -0.51                                       /// 0030c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00310
			 0x80000000,                                                  // -zero                                       /// 00314
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00318
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0031c
			 0x4b000000,                                                  // 8388608.0                                   /// 00320
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00324
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00328
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00330
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00340
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00344
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00348
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0034c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00350
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00354
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00358
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0035c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00360
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00364
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00370
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00378
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00380
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00384
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00388
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0038c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00390
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00394
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00398
			 0xcb000000,                                                  // -8388608.0                                  /// 0039c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 003a0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 003a4
			 0x00000000,                                                  // zero                                        /// 003a8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0x0e000fff,                                                  // Trailing 1s:                                /// 003b0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 003b4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003b8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003bc
			 0x0c700000,                                                  // Leading 1s:                                 /// 003c0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003c4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 003c8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003cc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003d8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003dc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003e0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 003e4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003e8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ec
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003f0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003f4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 003f8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 003fc
			 0xffc00001,                                                  // -signaling NaN                              /// 00400
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00404
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00408
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0040c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00414
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00418
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0041c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00420
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00424
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00428
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0042c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00430
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00434
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00438
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0043c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00440
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00444
			 0x7f800000,                                                  // inf                                         /// 00448
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0044c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00450
			 0xbf028f5c,                                                  // -0.51                                       /// 00454
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00458
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0045c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00460
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00464
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00468
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00470
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00474
			 0x0c600000,                                                  // Leading 1s:                                 /// 00478
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0047c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00480
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00484
			 0xffc00001,                                                  // -signaling NaN                              /// 00488
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0048c
			 0x55555555,                                                  // 4 random values                             /// 00490
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00494
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00498
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0049c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 004a4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 004a8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ac
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 004b0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004b4
			 0x33333333,                                                  // 4 random values                             /// 004b8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 004bc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004c0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c4
			 0x0c600000,                                                  // Leading 1s:                                 /// 004c8
			 0x80000000,                                                  // -zero                                       /// 004cc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004d0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004d4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004d8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004dc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004e0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 004e4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004e8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004ec
			 0x7f800000,                                                  // inf                                         /// 004f0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004f4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 004f8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 004fc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00504
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00508
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0050c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00510
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00514
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00518
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0051c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00520
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00524
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00528
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0052c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00530
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00534
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00538
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0053c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00540
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00548
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0054c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00550
			 0x0c700000,                                                  // Leading 1s:                                 /// 00554
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00558
			 0x0e000001,                                                  // Trailing 1s:                                /// 0055c
			 0x4b000000,                                                  // 8388608.0                                   /// 00560
			 0xff800000,                                                  // -inf                                        /// 00564
			 0x33333333,                                                  // 4 random values                             /// 00568
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0056c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00570
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00574
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00578
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0057c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00580
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00584
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00588
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0058c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00590
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00594
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00598
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0059c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005a0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005a4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 005ac
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005b0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005b4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005b8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005bc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 005c0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005c4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005c8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 005cc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 005d0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005d4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005d8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 005dc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005e0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005e4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005e8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 005f0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 005f4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 005f8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005fc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00600
			 0x7f800000,                                                  // inf                                         /// 00604
			 0xcb000000,                                                  // -8388608.0                                  /// 00608
			 0x0c700000,                                                  // Leading 1s:                                 /// 0060c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00610
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00614
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00618
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0061c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00620
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00624
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00628
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0062c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00630
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00634
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00638
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0063c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00640
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00644
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00648
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0064c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00650
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00654
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00658
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0065c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00660
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00664
			 0x0c600000,                                                  // Leading 1s:                                 /// 00668
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0066c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00670
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00674
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00678
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0067c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00680
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00684
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00688
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00690
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00694
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00698
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0069c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006a0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 006a4
			 0x80011111,                                                  // -9.7958E-41                                 /// 006a8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 006ac
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006b0
			 0xff800000,                                                  // -inf                                        /// 006b4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006b8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006bc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006c4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006cc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006d4
			 0x0e000001,                                                  // Trailing 1s:                                /// 006d8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006e4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 006e8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006ec
			 0x0c700000,                                                  // Leading 1s:                                 /// 006f0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006f4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 006f8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006fc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00704
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0070c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00710
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00714
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00718
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0071c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00720
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00724
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00728
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0072c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00730
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00734
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00738
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0073c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00740
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00744
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00748
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0074c
			 0xff800000,                                                  // -inf                                        /// 00750
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00754
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00758
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0075c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00760
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00764
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00768
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0076c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00770
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00774
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00778
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0077c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00784
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00790
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00794
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007a4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007a8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007ac
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007b4
			 0x0e000001,                                                  // Trailing 1s:                                /// 007b8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 007bc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 007c0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 007c4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007c8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007cc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d0
			 0x00011111,                                                  // 9.7958E-41                                  /// 007d4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007dc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007e0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007e4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e8
			 0x0e000003,                                                  // Trailing 1s:                                /// 007ec
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007f0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 007f4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00800
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00804
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00808
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0080c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00810
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00814
			 0x7f800000,                                                  // inf                                         /// 00818
			 0x0e000003,                                                  // Trailing 1s:                                /// 0081c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00820
			 0x0e000007,                                                  // Trailing 1s:                                /// 00824
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00828
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0082c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00834
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00838
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0083c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00840
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00844
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00848
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00850
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00858
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0085c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00860
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00864
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00868
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0086c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00870
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00874
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00878
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0087c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00880
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00894
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00898
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0089c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008a0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x3f028f5c,                                                  // 0.51                                        /// 008ac
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 008b0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 008b4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 008bc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008c0
			 0xff800000,                                                  // -inf                                        /// 008c4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008cc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008d0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008d4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008d8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008e0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008e8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ec
			 0x0c780000,                                                  // Leading 1s:                                 /// 008f0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008f4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008f8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008fc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00900
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00904
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00908
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0090c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00918
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0091c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00924
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00928
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0092c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00930
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00934
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00938
			 0xbf028f5c,                                                  // -0.51                                       /// 0093c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00940
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00948
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0094c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00950
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00954
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00958
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0095c
			 0x7fc00001,                                                  // signaling NaN                               /// 00960
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00964
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00968
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0096c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00970
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00974
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00978
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0097c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00980
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00984
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00988
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0098c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00990
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00994
			 0x33333333,                                                  // 4 random values                             /// 00998
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0099c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009a0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009a8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 009b0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009bc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009c0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009c4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009c8
			 0x0e000003,                                                  // Trailing 1s:                                /// 009cc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009d4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009d8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009dc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 009e4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009e8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 009ec
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009f0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009f4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f8
			 0x00011111,                                                  // 9.7958E-41                                  /// 009fc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a08
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a0c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a10
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a14
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a18
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a1c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a20
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a24
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a28
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a30
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a34
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a38
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a3c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a40
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a44
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a48
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a4c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a50
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a54
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a58
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a5c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a60
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a64
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a6c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a70
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a74
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a78
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a7c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a80
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a84
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a8c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a90
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a94
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a98
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a9c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00aa0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00aac
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ab8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00abc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ac0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ac8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00acc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ad0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ad4
			 0x3f028f5c,                                                  // 0.51                                        /// 00ad8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00adc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ae0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ae8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00af0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00af4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00af8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00afc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b00
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b04
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b08
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b0c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b10
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b14
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b18
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b1c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b20
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b24
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b28
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b2c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b30
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b34
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b38
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b3c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b40
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b44
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b48
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b4c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b50
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b54
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b58
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b5c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b64
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b68
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b70
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b74
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b78
			 0x55555555,                                                  // 4 random values                             /// 00b7c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b80
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0x3f028f5c,                                                  // 0.51                                        /// 00b88
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b94
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b9c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ba4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ba8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00bac
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00bb0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00bb4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00bb8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00bc0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0xff800000,                                                  // -inf                                        /// 00bc8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00bcc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00bd0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00bd4
			 0x7f800000,                                                  // inf                                         /// 00bd8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00bdc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00be0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00be4
			 0x33333333,                                                  // 4 random values                             /// 00be8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00bec
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00bf0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00bf4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00bf8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00bfc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c00
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c04
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c0c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c10
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c14
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c18
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c1c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c24
			 0xcb000000,                                                  // -8388608.0                                  /// 00c28
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c2c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c30
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c34
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c38
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c3c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c40
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c44
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c48
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c4c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c50
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c54
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c58
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c5c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c60
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c64
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c68
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c6c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c70
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c74
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c78
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c7c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c80
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c84
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c88
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c8c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c90
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c94
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c98
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c9c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ca0
			 0x33333333,                                                  // 4 random values                             /// 00ca4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ca8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00cac
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cb4
			 0x55555555,                                                  // 4 random values                             /// 00cb8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00cbc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00cc0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00cc4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00cc8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ccc
			 0x3f028f5c,                                                  // 0.51                                        /// 00cd0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cd4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00cdc
			 0x7f800000,                                                  // inf                                         /// 00ce0
			 0xff800000,                                                  // -inf                                        /// 00ce4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ce8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00cec
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00cf4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d00
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d04
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d08
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d0c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d10
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d14
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d18
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0xcb000000,                                                  // -8388608.0                                  /// 00d24
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d28
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d2c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d30
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d34
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d38
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d40
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d44
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d48
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d4c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d50
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d54
			 0x55555555,                                                  // 4 random values                             /// 00d58
			 0xff800000,                                                  // -inf                                        /// 00d5c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d64
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d68
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d6c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d70
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d74
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d78
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d7c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d80
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d88
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d8c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d90
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d94
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d98
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d9c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00da0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00da4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00da8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00dac
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00db0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00db4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00db8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00dbc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00dc0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00dc4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00dc8
			 0x80000000,                                                  // -zero                                       /// 00dcc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd0
			 0xcb000000,                                                  // -8388608.0                                  /// 00dd4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00dd8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ddc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00de0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00de4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00de8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00dec
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00df0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00df8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00dfc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e00
			 0xcb000000,                                                  // -8388608.0                                  /// 00e04
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e08
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e0c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e10
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e14
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e18
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e20
			 0x4b000000,                                                  // 8388608.0                                   /// 00e24
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e28
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e2c
			 0xcb000000,                                                  // -8388608.0                                  /// 00e30
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e34
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e38
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e3c
			 0x00000000,                                                  // zero                                        /// 00e40
			 0x3f028f5c,                                                  // 0.51                                        /// 00e44
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e48
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e4c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e50
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e58
			 0x4b000000,                                                  // 8388608.0                                   /// 00e5c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e60
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e64
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e68
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e70
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e74
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e78
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e7c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e80
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e84
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e88
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e8c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e90
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e94
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e98
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e9c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ea0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ea4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00eac
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00eb4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ebc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ec0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ec4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ec8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ecc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ed4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00edc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ee0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ee8
			 0x4b000000,                                                  // 8388608.0                                   /// 00eec
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ef4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ef8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00efc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f00
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f04
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f08
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f0c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f10
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f14
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f18
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f1c
			 0x4b000000,                                                  // 8388608.0                                   /// 00f20
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f24
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f28
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f2c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f30
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f34
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0x7f800000,                                                  // inf                                         /// 00f3c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f40
			 0x7f800000,                                                  // inf                                         /// 00f44
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f48
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f4c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f50
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f58
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f5c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f60
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f64
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f68
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f6c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f70
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f78
			 0x55555555,                                                  // 4 random values                             /// 00f7c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f80
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f8c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f90
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f94
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f9c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00fa0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00fa4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00fac
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fb0
			 0x00000000,                                                  // zero                                        /// 00fb4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fb8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00fbc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00fc0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00fc4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00fc8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fcc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fd0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fd4
			 0x55555555,                                                  // 4 random values                             /// 00fd8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00fdc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00fe0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00fe4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fe8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00fec
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ff0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ff4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ff8
			 0x80800002                                                  // none of the mantissa set to +3ulp           /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0xa0b9efd3, /// 0x0
			 0xc4eaacee, /// 0x4
			 0x50ba39dd, /// 0x8
			 0xcd7f59a2, /// 0xc
			 0xaf579797, /// 0x10
			 0xe2a08ec1, /// 0x14
			 0x240ee4bc, /// 0x18
			 0xfe51e9e4, /// 0x1c
			 0xe927f545, /// 0x20
			 0xf9a57211, /// 0x24
			 0xe4da853e, /// 0x28
			 0xf4b1a3d0, /// 0x2c
			 0x6bf1a572, /// 0x30
			 0x0a2a3942, /// 0x34
			 0x4808abad, /// 0x38
			 0x00c292a7, /// 0x3c
			 0xfae3c5b3, /// 0x40
			 0xf7e0f157, /// 0x44
			 0xf6029b05, /// 0x48
			 0x5dc874fb, /// 0x4c
			 0x228c31ad, /// 0x50
			 0xfde9761f, /// 0x54
			 0x60c30e1d, /// 0x58
			 0xc3e18664, /// 0x5c
			 0x6da67622, /// 0x60
			 0x29090bd9, /// 0x64
			 0x68eda4da, /// 0x68
			 0xfe6e7d67, /// 0x6c
			 0xb2fe3bfc, /// 0x70
			 0xb98d5cd1, /// 0x74
			 0x9d2dec6b, /// 0x78
			 0x6b72a440, /// 0x7c
			 0xa93068cd, /// 0x80
			 0x23998f52, /// 0x84
			 0x832a8a70, /// 0x88
			 0x64ce0b82, /// 0x8c
			 0x5f8a37e4, /// 0x90
			 0x4d65529e, /// 0x94
			 0x6f597dfd, /// 0x98
			 0x9b317ee1, /// 0x9c
			 0x22a60e56, /// 0xa0
			 0xbb730cc4, /// 0xa4
			 0x33c26ac4, /// 0xa8
			 0xe77651f6, /// 0xac
			 0xf8934e17, /// 0xb0
			 0xa3a2e3c9, /// 0xb4
			 0xeb783075, /// 0xb8
			 0x5a43da56, /// 0xbc
			 0x819d3d86, /// 0xc0
			 0x49c7b996, /// 0xc4
			 0x26d112bb, /// 0xc8
			 0xac6349c1, /// 0xcc
			 0xba90debe, /// 0xd0
			 0xb430cb35, /// 0xd4
			 0x04601b50, /// 0xd8
			 0x67cd3fc2, /// 0xdc
			 0x318ee618, /// 0xe0
			 0xa8ba3b77, /// 0xe4
			 0x937cf38f, /// 0xe8
			 0x2ff9a551, /// 0xec
			 0x2d9c29b2, /// 0xf0
			 0x48295582, /// 0xf4
			 0x6917ac88, /// 0xf8
			 0x2e4ae509, /// 0xfc
			 0x07b26523, /// 0x100
			 0x822e4519, /// 0x104
			 0x841bb9ca, /// 0x108
			 0x433cc816, /// 0x10c
			 0x3c2cf92f, /// 0x110
			 0x9bdedbc7, /// 0x114
			 0x1295afd3, /// 0x118
			 0xba666d36, /// 0x11c
			 0x1030e852, /// 0x120
			 0xb8173fd0, /// 0x124
			 0x1b8e08cc, /// 0x128
			 0x0d238da7, /// 0x12c
			 0xaa8e8a9f, /// 0x130
			 0x354e453c, /// 0x134
			 0x1e5ad43a, /// 0x138
			 0xadec3ee9, /// 0x13c
			 0x780d4baf, /// 0x140
			 0xa5bded76, /// 0x144
			 0x4141824f, /// 0x148
			 0xc2d5a9b5, /// 0x14c
			 0x2a4a3efa, /// 0x150
			 0x4595f543, /// 0x154
			 0xa72b4508, /// 0x158
			 0x794f9d30, /// 0x15c
			 0xe47ec099, /// 0x160
			 0x4945c5a5, /// 0x164
			 0x9a164fb6, /// 0x168
			 0xdaa6a4a9, /// 0x16c
			 0x5e57a4d1, /// 0x170
			 0x870d07e4, /// 0x174
			 0xa1907331, /// 0x178
			 0x5f6691e0, /// 0x17c
			 0xf1340f47, /// 0x180
			 0xe6fad4e0, /// 0x184
			 0xe4e70f45, /// 0x188
			 0x380633d6, /// 0x18c
			 0xe1f694be, /// 0x190
			 0xcab2b641, /// 0x194
			 0x53887536, /// 0x198
			 0x164429db, /// 0x19c
			 0x1c187b63, /// 0x1a0
			 0xc97c52e5, /// 0x1a4
			 0x044a5297, /// 0x1a8
			 0xfd4c1b1b, /// 0x1ac
			 0x6721765c, /// 0x1b0
			 0xb2647724, /// 0x1b4
			 0x3f89f31a, /// 0x1b8
			 0x61eb0501, /// 0x1bc
			 0x4f2eef3d, /// 0x1c0
			 0xa96991e4, /// 0x1c4
			 0x3e2df70a, /// 0x1c8
			 0xb541b34a, /// 0x1cc
			 0xb9176b3e, /// 0x1d0
			 0x0d3c23d2, /// 0x1d4
			 0x5fe3bc66, /// 0x1d8
			 0x96b20a5e, /// 0x1dc
			 0x35a49e7f, /// 0x1e0
			 0x4fa42f4f, /// 0x1e4
			 0xd496f309, /// 0x1e8
			 0x6c669dc3, /// 0x1ec
			 0xc63fc274, /// 0x1f0
			 0xbed80055, /// 0x1f4
			 0x80ed0c3c, /// 0x1f8
			 0xbd2f4525, /// 0x1fc
			 0x6ae32e40, /// 0x200
			 0xe589851f, /// 0x204
			 0x680fc627, /// 0x208
			 0xdbfd768b, /// 0x20c
			 0xde79294d, /// 0x210
			 0x43149b49, /// 0x214
			 0x0f8f9af4, /// 0x218
			 0xd51f8b07, /// 0x21c
			 0x7d6a55f9, /// 0x220
			 0xf90ce622, /// 0x224
			 0x938020e3, /// 0x228
			 0xfe8c10c5, /// 0x22c
			 0x930e8d12, /// 0x230
			 0xc808fc99, /// 0x234
			 0x8badb59c, /// 0x238
			 0x1eae7597, /// 0x23c
			 0x106ebaae, /// 0x240
			 0x678b272e, /// 0x244
			 0xe0bdf2a8, /// 0x248
			 0x411ccc81, /// 0x24c
			 0x1540416a, /// 0x250
			 0x86a573ef, /// 0x254
			 0x1df28d51, /// 0x258
			 0xa448126b, /// 0x25c
			 0x7fa9f98f, /// 0x260
			 0xf386b606, /// 0x264
			 0xbbc87965, /// 0x268
			 0xb06804d4, /// 0x26c
			 0x34ebdffb, /// 0x270
			 0xfeaea511, /// 0x274
			 0xa4783a2f, /// 0x278
			 0x5e2cc3d0, /// 0x27c
			 0xf05170f8, /// 0x280
			 0x4547a12c, /// 0x284
			 0x9e6c4445, /// 0x288
			 0xa3b258d3, /// 0x28c
			 0xf4b24ebd, /// 0x290
			 0x53e4d78f, /// 0x294
			 0xf82f8741, /// 0x298
			 0x9e9a253f, /// 0x29c
			 0x11308158, /// 0x2a0
			 0x2c1a774f, /// 0x2a4
			 0x203b8c74, /// 0x2a8
			 0x80a35178, /// 0x2ac
			 0xffde68b7, /// 0x2b0
			 0xb44b5f83, /// 0x2b4
			 0x45d9e82d, /// 0x2b8
			 0xdb5466cf, /// 0x2bc
			 0xb0ef317f, /// 0x2c0
			 0x4538a9d9, /// 0x2c4
			 0x8030e038, /// 0x2c8
			 0x1d0ec87e, /// 0x2cc
			 0xb71f7252, /// 0x2d0
			 0xcedad2a6, /// 0x2d4
			 0x79de36e7, /// 0x2d8
			 0xfbc39e04, /// 0x2dc
			 0x6cea828b, /// 0x2e0
			 0xed0eb959, /// 0x2e4
			 0x54587bfd, /// 0x2e8
			 0x5d05bdca, /// 0x2ec
			 0xe24bb740, /// 0x2f0
			 0x538f4327, /// 0x2f4
			 0xf81e7c3d, /// 0x2f8
			 0xb6198193, /// 0x2fc
			 0x15908582, /// 0x300
			 0x0cdbf8d1, /// 0x304
			 0x82b72b49, /// 0x308
			 0x51005856, /// 0x30c
			 0xdbbde1d5, /// 0x310
			 0x33fd6a8c, /// 0x314
			 0x06383c59, /// 0x318
			 0x5feba92f, /// 0x31c
			 0x606c837e, /// 0x320
			 0xec8dba0e, /// 0x324
			 0xd559dd87, /// 0x328
			 0x6e02e573, /// 0x32c
			 0x497e38de, /// 0x330
			 0x69f9d38c, /// 0x334
			 0x542f87bc, /// 0x338
			 0xdf04c8b4, /// 0x33c
			 0xcd257643, /// 0x340
			 0x878d93fe, /// 0x344
			 0xaf945dec, /// 0x348
			 0x6f698eaf, /// 0x34c
			 0x5c763699, /// 0x350
			 0xf6746d1f, /// 0x354
			 0x706b35bb, /// 0x358
			 0xc23731f5, /// 0x35c
			 0xe0f65479, /// 0x360
			 0x009c1c2c, /// 0x364
			 0x0744bc32, /// 0x368
			 0x54fb9de9, /// 0x36c
			 0x10b2c7a0, /// 0x370
			 0x48882043, /// 0x374
			 0xd07596de, /// 0x378
			 0xac9be50d, /// 0x37c
			 0x7145dc5b, /// 0x380
			 0xada054f1, /// 0x384
			 0xa71088c9, /// 0x388
			 0x7e153e76, /// 0x38c
			 0xab0cb670, /// 0x390
			 0xa8be7707, /// 0x394
			 0x0a304d10, /// 0x398
			 0xc4ed027f, /// 0x39c
			 0xa1859963, /// 0x3a0
			 0xd7c17926, /// 0x3a4
			 0xe1df19e0, /// 0x3a8
			 0xbda1a1c2, /// 0x3ac
			 0xd7dc2e0b, /// 0x3b0
			 0xf5a5167e, /// 0x3b4
			 0x00c8bc22, /// 0x3b8
			 0x49d8cc24, /// 0x3bc
			 0xbedf9e1e, /// 0x3c0
			 0xc9192c76, /// 0x3c4
			 0xeb6e5cfc, /// 0x3c8
			 0x344cf647, /// 0x3cc
			 0x9ebac16f, /// 0x3d0
			 0x2df0caba, /// 0x3d4
			 0x9880bf02, /// 0x3d8
			 0xf1d8f92a, /// 0x3dc
			 0xf640d0cd, /// 0x3e0
			 0xcfe14513, /// 0x3e4
			 0x4f69c2da, /// 0x3e8
			 0xfbb278b5, /// 0x3ec
			 0xbb0f9944, /// 0x3f0
			 0x8b9fbdfa, /// 0x3f4
			 0x9419d4da, /// 0x3f8
			 0xb01318e8, /// 0x3fc
			 0xd3c73faf, /// 0x400
			 0x9d1d7f26, /// 0x404
			 0xf03aa208, /// 0x408
			 0xd4a4d2dd, /// 0x40c
			 0x6673ea49, /// 0x410
			 0x80ca0e37, /// 0x414
			 0xd00e5a68, /// 0x418
			 0x497fecf8, /// 0x41c
			 0x2457e24d, /// 0x420
			 0x73e1b7ca, /// 0x424
			 0x519eb968, /// 0x428
			 0x5b5fa4a7, /// 0x42c
			 0x250a1914, /// 0x430
			 0x4b69a18c, /// 0x434
			 0x2c3158ee, /// 0x438
			 0xbc7500ce, /// 0x43c
			 0xaca9ce7d, /// 0x440
			 0xbce70e2d, /// 0x444
			 0xe211fe4c, /// 0x448
			 0x922f8cdf, /// 0x44c
			 0x277de756, /// 0x450
			 0xefaff4a5, /// 0x454
			 0x485e8b30, /// 0x458
			 0x25f132d6, /// 0x45c
			 0xacd3c008, /// 0x460
			 0xc8604e8e, /// 0x464
			 0xf63c09d9, /// 0x468
			 0x156ad9d1, /// 0x46c
			 0x8fb9fcc6, /// 0x470
			 0x3e045c8c, /// 0x474
			 0xcb5c0bcf, /// 0x478
			 0xbe4ccd88, /// 0x47c
			 0xdb7cbf02, /// 0x480
			 0x12851761, /// 0x484
			 0xed092d72, /// 0x488
			 0x51331517, /// 0x48c
			 0x0fbac3c2, /// 0x490
			 0xb770ef0f, /// 0x494
			 0xed25d5e4, /// 0x498
			 0x5b902435, /// 0x49c
			 0xe89ec4ba, /// 0x4a0
			 0x1c44b00e, /// 0x4a4
			 0x89cda7e5, /// 0x4a8
			 0x8989c673, /// 0x4ac
			 0x6e3721ac, /// 0x4b0
			 0x25b60a08, /// 0x4b4
			 0x48fe723f, /// 0x4b8
			 0xc618d6ba, /// 0x4bc
			 0x9f1efeb4, /// 0x4c0
			 0x7ff361a7, /// 0x4c4
			 0x7721cc29, /// 0x4c8
			 0x1862fa16, /// 0x4cc
			 0xe20ca9cd, /// 0x4d0
			 0xaaac5a20, /// 0x4d4
			 0x2d4af837, /// 0x4d8
			 0xfbce6730, /// 0x4dc
			 0x50016bae, /// 0x4e0
			 0x97e902cd, /// 0x4e4
			 0xc8e82d22, /// 0x4e8
			 0x6bb12d59, /// 0x4ec
			 0x8c3fc9ee, /// 0x4f0
			 0xf22bbdc5, /// 0x4f4
			 0xbbb5be16, /// 0x4f8
			 0x57683797, /// 0x4fc
			 0x510e8812, /// 0x500
			 0x77eaff02, /// 0x504
			 0x388f6471, /// 0x508
			 0xbeb61e1f, /// 0x50c
			 0xdd0fea8a, /// 0x510
			 0xa6706551, /// 0x514
			 0xfcf2e161, /// 0x518
			 0x88840fb0, /// 0x51c
			 0x2e3ff667, /// 0x520
			 0x44c16a74, /// 0x524
			 0x4a1d0279, /// 0x528
			 0xd0384043, /// 0x52c
			 0xab9f9f19, /// 0x530
			 0x395720ee, /// 0x534
			 0xea24e9bd, /// 0x538
			 0xe6e7144c, /// 0x53c
			 0xb30bd639, /// 0x540
			 0x93888e7a, /// 0x544
			 0x833dd2ae, /// 0x548
			 0x07ba3bf8, /// 0x54c
			 0xb941fa6a, /// 0x550
			 0xe7652876, /// 0x554
			 0xdfe97b38, /// 0x558
			 0x879e9821, /// 0x55c
			 0xf4d4d8c8, /// 0x560
			 0x40b75bc0, /// 0x564
			 0x854dff22, /// 0x568
			 0x61158b23, /// 0x56c
			 0x9ba4cbf6, /// 0x570
			 0x6a663c8d, /// 0x574
			 0x21e9c691, /// 0x578
			 0xf3bb662d, /// 0x57c
			 0x6f023931, /// 0x580
			 0x94e11f0e, /// 0x584
			 0x448f4717, /// 0x588
			 0x15b4bd70, /// 0x58c
			 0x9a13c00d, /// 0x590
			 0xe0536678, /// 0x594
			 0x9064e9b2, /// 0x598
			 0xead8a349, /// 0x59c
			 0x41d2f6ef, /// 0x5a0
			 0x9e4d92aa, /// 0x5a4
			 0x429da0ac, /// 0x5a8
			 0xc5a9c319, /// 0x5ac
			 0x783f37e5, /// 0x5b0
			 0x7d9427f0, /// 0x5b4
			 0x29a726a3, /// 0x5b8
			 0xcb96d2bf, /// 0x5bc
			 0x3d5b1728, /// 0x5c0
			 0xd49a14c5, /// 0x5c4
			 0xd8b1e595, /// 0x5c8
			 0xf94149ac, /// 0x5cc
			 0xdfb6d71b, /// 0x5d0
			 0xdf4ceef8, /// 0x5d4
			 0xe12773ff, /// 0x5d8
			 0x556eddae, /// 0x5dc
			 0x473bede7, /// 0x5e0
			 0x7995d1c5, /// 0x5e4
			 0x3f9c6429, /// 0x5e8
			 0x49c63a43, /// 0x5ec
			 0xdb9d68c8, /// 0x5f0
			 0x27f42a57, /// 0x5f4
			 0x710ed30e, /// 0x5f8
			 0x37b75b8a, /// 0x5fc
			 0x5f001ce0, /// 0x600
			 0x6a56c484, /// 0x604
			 0xee37d79b, /// 0x608
			 0x30b0361b, /// 0x60c
			 0x7ec4ad1a, /// 0x610
			 0x4ae161a1, /// 0x614
			 0x2b7e0653, /// 0x618
			 0x08d02f9c, /// 0x61c
			 0xbf58fd05, /// 0x620
			 0xd3b1cb21, /// 0x624
			 0xcebaa805, /// 0x628
			 0xed982104, /// 0x62c
			 0x2fb110b7, /// 0x630
			 0x4f3cac75, /// 0x634
			 0xded7f4fe, /// 0x638
			 0x7c0130b1, /// 0x63c
			 0x883f60b9, /// 0x640
			 0xad8bee8e, /// 0x644
			 0xa9d872ae, /// 0x648
			 0x1830d926, /// 0x64c
			 0x33fee1f5, /// 0x650
			 0xc4cedad7, /// 0x654
			 0x115cf851, /// 0x658
			 0x52be69f4, /// 0x65c
			 0x20648ea0, /// 0x660
			 0x299c1138, /// 0x664
			 0x083f446b, /// 0x668
			 0x76a9d230, /// 0x66c
			 0x445352be, /// 0x670
			 0x479e630a, /// 0x674
			 0x1cdebfdd, /// 0x678
			 0x0743c252, /// 0x67c
			 0xca532ada, /// 0x680
			 0x9ea6b2f2, /// 0x684
			 0xd4fb5d38, /// 0x688
			 0x00834c07, /// 0x68c
			 0xbe4587f3, /// 0x690
			 0x9a2a983a, /// 0x694
			 0xb8d64aae, /// 0x698
			 0x80302f19, /// 0x69c
			 0xe9014f2e, /// 0x6a0
			 0x494d9eca, /// 0x6a4
			 0x5ce335bb, /// 0x6a8
			 0x4cd40ee1, /// 0x6ac
			 0x3233ee7d, /// 0x6b0
			 0xbd3eaaec, /// 0x6b4
			 0x5ef4e237, /// 0x6b8
			 0x6992f6f9, /// 0x6bc
			 0xc46d8755, /// 0x6c0
			 0xa74caee3, /// 0x6c4
			 0xaacb1aaf, /// 0x6c8
			 0x64189ea5, /// 0x6cc
			 0xa83f6bd2, /// 0x6d0
			 0x6d3306d5, /// 0x6d4
			 0xaf76c0cf, /// 0x6d8
			 0xa986c197, /// 0x6dc
			 0xd65fe691, /// 0x6e0
			 0xc2995c17, /// 0x6e4
			 0x4ceaf46c, /// 0x6e8
			 0x175c7498, /// 0x6ec
			 0x717fea30, /// 0x6f0
			 0x174ba8d1, /// 0x6f4
			 0x3d04f16d, /// 0x6f8
			 0x3751b426, /// 0x6fc
			 0x24790271, /// 0x700
			 0xb22c63cc, /// 0x704
			 0x0ba56638, /// 0x708
			 0x0cc49e8e, /// 0x70c
			 0x3629f2e8, /// 0x710
			 0x1f348f0f, /// 0x714
			 0x060af529, /// 0x718
			 0xfaea5e30, /// 0x71c
			 0xe43e2994, /// 0x720
			 0x0375192b, /// 0x724
			 0x46cc3a27, /// 0x728
			 0xf91afa6f, /// 0x72c
			 0x318218fe, /// 0x730
			 0x005a6c0a, /// 0x734
			 0x594efa83, /// 0x738
			 0x7847205f, /// 0x73c
			 0xadaf695a, /// 0x740
			 0x54dd29f6, /// 0x744
			 0x13c6d5e1, /// 0x748
			 0x7b79de3f, /// 0x74c
			 0xffde0fb0, /// 0x750
			 0x051fbd68, /// 0x754
			 0x7079abcb, /// 0x758
			 0x4116f71d, /// 0x75c
			 0xe9ad0544, /// 0x760
			 0x80d32245, /// 0x764
			 0x0b377fd2, /// 0x768
			 0xd84d9c7b, /// 0x76c
			 0x43ca886c, /// 0x770
			 0x2475a59e, /// 0x774
			 0xbba482d6, /// 0x778
			 0x82ba5b35, /// 0x77c
			 0x09008c50, /// 0x780
			 0x02587784, /// 0x784
			 0xda188b48, /// 0x788
			 0xdeb71554, /// 0x78c
			 0x0b90ac2c, /// 0x790
			 0x64733e19, /// 0x794
			 0x2401fde9, /// 0x798
			 0x7d21f05c, /// 0x79c
			 0xeb557483, /// 0x7a0
			 0x114387fa, /// 0x7a4
			 0xfb18e738, /// 0x7a8
			 0x4202da76, /// 0x7ac
			 0xba369264, /// 0x7b0
			 0x527dea73, /// 0x7b4
			 0xfee1449c, /// 0x7b8
			 0xf0caf456, /// 0x7bc
			 0xdbd31a61, /// 0x7c0
			 0xb3ee1f2a, /// 0x7c4
			 0xf511208a, /// 0x7c8
			 0x0bf69057, /// 0x7cc
			 0x2a7b6943, /// 0x7d0
			 0x6df4dfb6, /// 0x7d4
			 0x8df270ca, /// 0x7d8
			 0x1acaace3, /// 0x7dc
			 0x57326c09, /// 0x7e0
			 0x40aede98, /// 0x7e4
			 0xc9f12d93, /// 0x7e8
			 0xfe6384ed, /// 0x7ec
			 0x782b9a4a, /// 0x7f0
			 0xfdbb2307, /// 0x7f4
			 0xabe3a70b, /// 0x7f8
			 0xde2cd847, /// 0x7fc
			 0x60798574, /// 0x800
			 0x63b659b5, /// 0x804
			 0x6cee278c, /// 0x808
			 0xfdb42ebc, /// 0x80c
			 0xe02da966, /// 0x810
			 0x28964c6b, /// 0x814
			 0x222a2eb4, /// 0x818
			 0x824bef25, /// 0x81c
			 0xbeffdca0, /// 0x820
			 0x767abb94, /// 0x824
			 0x18eb1ff3, /// 0x828
			 0x8911fdbd, /// 0x82c
			 0x3e4484b0, /// 0x830
			 0x5c01d686, /// 0x834
			 0xbde02c5c, /// 0x838
			 0x1751bea4, /// 0x83c
			 0xbbfb766c, /// 0x840
			 0x4310f762, /// 0x844
			 0xb4ab6ce0, /// 0x848
			 0xa1afe7e9, /// 0x84c
			 0xeb2c5770, /// 0x850
			 0x58aed51a, /// 0x854
			 0xd0535ded, /// 0x858
			 0xbab65ca3, /// 0x85c
			 0x4a5dda32, /// 0x860
			 0xbd4e7e74, /// 0x864
			 0x715d94b1, /// 0x868
			 0x42928a8d, /// 0x86c
			 0xfba161ec, /// 0x870
			 0x584abf11, /// 0x874
			 0x99304372, /// 0x878
			 0xc5867d31, /// 0x87c
			 0xfbeea6c2, /// 0x880
			 0xfb57fb93, /// 0x884
			 0x5b246541, /// 0x888
			 0x179c2d6e, /// 0x88c
			 0x0694bf90, /// 0x890
			 0x887a98a8, /// 0x894
			 0xacc6401f, /// 0x898
			 0xd27e0458, /// 0x89c
			 0xbf02619d, /// 0x8a0
			 0xbd18cd79, /// 0x8a4
			 0x9ae0b043, /// 0x8a8
			 0x247e1004, /// 0x8ac
			 0x5cde00c2, /// 0x8b0
			 0xbc85d624, /// 0x8b4
			 0xecee3ff5, /// 0x8b8
			 0x1d6580a9, /// 0x8bc
			 0xe4ea8470, /// 0x8c0
			 0x0120f2dd, /// 0x8c4
			 0x2d44b50b, /// 0x8c8
			 0x97c1c021, /// 0x8cc
			 0x71ad87fc, /// 0x8d0
			 0xdcf7d23b, /// 0x8d4
			 0x04d286cd, /// 0x8d8
			 0x2b249bf6, /// 0x8dc
			 0x4a616a7f, /// 0x8e0
			 0x1372cefe, /// 0x8e4
			 0x18e6f56c, /// 0x8e8
			 0x330340b7, /// 0x8ec
			 0x5ef2b9a4, /// 0x8f0
			 0x437dde05, /// 0x8f4
			 0xbec2114c, /// 0x8f8
			 0x7329ef5f, /// 0x8fc
			 0xc82b8fcf, /// 0x900
			 0x375481f9, /// 0x904
			 0x5e891cb2, /// 0x908
			 0x7a0e0dbc, /// 0x90c
			 0xbdc680dd, /// 0x910
			 0x733f6652, /// 0x914
			 0x6787f638, /// 0x918
			 0xab58eccc, /// 0x91c
			 0xd42f7635, /// 0x920
			 0xb55c71a0, /// 0x924
			 0x97dae2b6, /// 0x928
			 0x929e3ab9, /// 0x92c
			 0x726387a4, /// 0x930
			 0x8e83b4ba, /// 0x934
			 0x0b3e9753, /// 0x938
			 0xda394c44, /// 0x93c
			 0xdb4f4de6, /// 0x940
			 0x0d02c7d9, /// 0x944
			 0x78f7fc66, /// 0x948
			 0x40648913, /// 0x94c
			 0x96813f7e, /// 0x950
			 0xfc8c3980, /// 0x954
			 0x88a32299, /// 0x958
			 0xa6ba58d9, /// 0x95c
			 0xdf9d6114, /// 0x960
			 0x9b9e2a97, /// 0x964
			 0xe77933cb, /// 0x968
			 0x065b4a81, /// 0x96c
			 0xa3d12af5, /// 0x970
			 0x87c29bcc, /// 0x974
			 0xdb018277, /// 0x978
			 0x04661df9, /// 0x97c
			 0x9d300303, /// 0x980
			 0x2b868e82, /// 0x984
			 0x5b007d04, /// 0x988
			 0xd2b7c84b, /// 0x98c
			 0xb5a81c81, /// 0x990
			 0x8eed5540, /// 0x994
			 0x0d5662ad, /// 0x998
			 0x45cd5ff9, /// 0x99c
			 0xd5296315, /// 0x9a0
			 0x38ed6991, /// 0x9a4
			 0x04bf10d0, /// 0x9a8
			 0x533ba755, /// 0x9ac
			 0x0e175a67, /// 0x9b0
			 0x8410485e, /// 0x9b4
			 0x98183808, /// 0x9b8
			 0x27ca864d, /// 0x9bc
			 0x291108d4, /// 0x9c0
			 0x0dd08339, /// 0x9c4
			 0x02221c95, /// 0x9c8
			 0xf85f3248, /// 0x9cc
			 0x7d9cef82, /// 0x9d0
			 0x8ffe2086, /// 0x9d4
			 0xe95486a3, /// 0x9d8
			 0x5315e14c, /// 0x9dc
			 0x801d80a2, /// 0x9e0
			 0xe62a9b71, /// 0x9e4
			 0x01282b40, /// 0x9e8
			 0x5fdb5401, /// 0x9ec
			 0x2e0a6526, /// 0x9f0
			 0x100224db, /// 0x9f4
			 0x1928960f, /// 0x9f8
			 0x66f8ef53, /// 0x9fc
			 0x44e99a4b, /// 0xa00
			 0x3d9b314a, /// 0xa04
			 0x40dadb50, /// 0xa08
			 0x12466b6d, /// 0xa0c
			 0xa72cddc2, /// 0xa10
			 0x0faab5eb, /// 0xa14
			 0x639979bf, /// 0xa18
			 0x0b5a988b, /// 0xa1c
			 0x21d269f9, /// 0xa20
			 0xcbd9533e, /// 0xa24
			 0x73d09ae3, /// 0xa28
			 0x5ae6b72b, /// 0xa2c
			 0xbbcc697b, /// 0xa30
			 0x27f032cd, /// 0xa34
			 0x19483c19, /// 0xa38
			 0xdccc77e9, /// 0xa3c
			 0x142f8e90, /// 0xa40
			 0x6db8f136, /// 0xa44
			 0x9a2a287e, /// 0xa48
			 0x55e0acb8, /// 0xa4c
			 0x36a6e06d, /// 0xa50
			 0x92720ab3, /// 0xa54
			 0xefa57d7a, /// 0xa58
			 0x6680e215, /// 0xa5c
			 0xfc874367, /// 0xa60
			 0xea9dd5b7, /// 0xa64
			 0xf4a938b9, /// 0xa68
			 0x9f4b41e3, /// 0xa6c
			 0xba25c6fb, /// 0xa70
			 0x70b480c8, /// 0xa74
			 0x60a2b18e, /// 0xa78
			 0x8e2f22eb, /// 0xa7c
			 0x71752e88, /// 0xa80
			 0xff9d804c, /// 0xa84
			 0x476ce97a, /// 0xa88
			 0xac0eef0f, /// 0xa8c
			 0x485844da, /// 0xa90
			 0x1e9de5c7, /// 0xa94
			 0x5d8d4189, /// 0xa98
			 0xe990527b, /// 0xa9c
			 0xd7d730b6, /// 0xaa0
			 0xbd0d970b, /// 0xaa4
			 0xf0d50e29, /// 0xaa8
			 0xf3d7b96d, /// 0xaac
			 0x1686e86b, /// 0xab0
			 0x01ba0072, /// 0xab4
			 0xaaca3e58, /// 0xab8
			 0x4dcdde69, /// 0xabc
			 0xc77157df, /// 0xac0
			 0x0ff2c861, /// 0xac4
			 0x80ac17f8, /// 0xac8
			 0x75837db3, /// 0xacc
			 0xf9e405e5, /// 0xad0
			 0xa9b98bc7, /// 0xad4
			 0x7c628518, /// 0xad8
			 0x583e75f3, /// 0xadc
			 0xa0529f33, /// 0xae0
			 0xa27373c9, /// 0xae4
			 0xd9623078, /// 0xae8
			 0xeeddff20, /// 0xaec
			 0xe913c64c, /// 0xaf0
			 0xc42faabc, /// 0xaf4
			 0x3cfbf375, /// 0xaf8
			 0xa924afa4, /// 0xafc
			 0xf56e360b, /// 0xb00
			 0x0c6db032, /// 0xb04
			 0x4d73d421, /// 0xb08
			 0x2c7a5dcb, /// 0xb0c
			 0x12963d6c, /// 0xb10
			 0x2f1ba00a, /// 0xb14
			 0x0b5c8755, /// 0xb18
			 0x699442e8, /// 0xb1c
			 0x44d1c8db, /// 0xb20
			 0x43b80bff, /// 0xb24
			 0x639c185c, /// 0xb28
			 0x045b2858, /// 0xb2c
			 0xd55eed8b, /// 0xb30
			 0x2449d78b, /// 0xb34
			 0x2e46143c, /// 0xb38
			 0xc2a493f9, /// 0xb3c
			 0x7948fc35, /// 0xb40
			 0x8737141e, /// 0xb44
			 0xde8fa4cc, /// 0xb48
			 0x537a2139, /// 0xb4c
			 0x42f97dee, /// 0xb50
			 0x562e05a0, /// 0xb54
			 0x3b2da071, /// 0xb58
			 0xa1c48c66, /// 0xb5c
			 0xb6bc4ec5, /// 0xb60
			 0xa5f8536f, /// 0xb64
			 0xb62403b5, /// 0xb68
			 0xfa91707e, /// 0xb6c
			 0xfe9af0cb, /// 0xb70
			 0x873a6e94, /// 0xb74
			 0xc4278a61, /// 0xb78
			 0x3ef5b525, /// 0xb7c
			 0xf1c422c8, /// 0xb80
			 0xfd337e83, /// 0xb84
			 0x217a043c, /// 0xb88
			 0xdd388e3c, /// 0xb8c
			 0x5a5bd167, /// 0xb90
			 0xc54bb2b2, /// 0xb94
			 0xcf3d6db0, /// 0xb98
			 0xa81354ea, /// 0xb9c
			 0xd140413d, /// 0xba0
			 0xcee1fc10, /// 0xba4
			 0xf9de3543, /// 0xba8
			 0x13041d40, /// 0xbac
			 0xb08f901c, /// 0xbb0
			 0x48d0179b, /// 0xbb4
			 0xd60ea473, /// 0xbb8
			 0xa0893bae, /// 0xbbc
			 0xbd4f31a5, /// 0xbc0
			 0x4e996707, /// 0xbc4
			 0x514b3a1f, /// 0xbc8
			 0x22b076be, /// 0xbcc
			 0xa1048f47, /// 0xbd0
			 0xde54a82c, /// 0xbd4
			 0xa72018c8, /// 0xbd8
			 0x738d103d, /// 0xbdc
			 0xd7858c8e, /// 0xbe0
			 0x8629cfc5, /// 0xbe4
			 0x58a5eeff, /// 0xbe8
			 0xe14dcf89, /// 0xbec
			 0x44184048, /// 0xbf0
			 0xf71e61b7, /// 0xbf4
			 0x2ad757da, /// 0xbf8
			 0x6d4fdd96, /// 0xbfc
			 0x7a14a0eb, /// 0xc00
			 0xc3856e6e, /// 0xc04
			 0xdb44105b, /// 0xc08
			 0x414c5f27, /// 0xc0c
			 0x0cad92c9, /// 0xc10
			 0x01f6a8a0, /// 0xc14
			 0xc05c7053, /// 0xc18
			 0x28119b48, /// 0xc1c
			 0xd6d7c57a, /// 0xc20
			 0x053317f4, /// 0xc24
			 0x82a3fcc5, /// 0xc28
			 0xd96f4f89, /// 0xc2c
			 0xb0d5e289, /// 0xc30
			 0x78a1c53c, /// 0xc34
			 0xd65ffb64, /// 0xc38
			 0xaab22a59, /// 0xc3c
			 0x59d34d80, /// 0xc40
			 0xa3867049, /// 0xc44
			 0xe5ff3aaa, /// 0xc48
			 0xedf82e08, /// 0xc4c
			 0xf9ac78f3, /// 0xc50
			 0x1c353998, /// 0xc54
			 0xacfaf081, /// 0xc58
			 0x31f6d74b, /// 0xc5c
			 0x78cb6b41, /// 0xc60
			 0x430cc997, /// 0xc64
			 0xfa2f4cf9, /// 0xc68
			 0x38467d89, /// 0xc6c
			 0x2f0268ff, /// 0xc70
			 0xcc3084ee, /// 0xc74
			 0x30f07f73, /// 0xc78
			 0xdfb9ee18, /// 0xc7c
			 0x2bb0512a, /// 0xc80
			 0x3bc314f4, /// 0xc84
			 0x254b6f01, /// 0xc88
			 0xff9bd5d2, /// 0xc8c
			 0xc75ff2de, /// 0xc90
			 0xcae33430, /// 0xc94
			 0xaee44c4e, /// 0xc98
			 0x40a0baf0, /// 0xc9c
			 0x67e89708, /// 0xca0
			 0xbca1a042, /// 0xca4
			 0x9903d012, /// 0xca8
			 0xb660df97, /// 0xcac
			 0x15597ee0, /// 0xcb0
			 0xa1f5f12f, /// 0xcb4
			 0x276b0719, /// 0xcb8
			 0xfdf3b295, /// 0xcbc
			 0x916db39c, /// 0xcc0
			 0x2677a631, /// 0xcc4
			 0x05543b60, /// 0xcc8
			 0x67207425, /// 0xccc
			 0x0e33ea07, /// 0xcd0
			 0x10e00f2e, /// 0xcd4
			 0x788d6a61, /// 0xcd8
			 0x97187e03, /// 0xcdc
			 0xbadb7d40, /// 0xce0
			 0x73eb4d46, /// 0xce4
			 0x8f9f2676, /// 0xce8
			 0x2faaede4, /// 0xcec
			 0x8b568948, /// 0xcf0
			 0x9de6ac7b, /// 0xcf4
			 0xe761f66a, /// 0xcf8
			 0x6cd09e80, /// 0xcfc
			 0x4acba3d8, /// 0xd00
			 0xbdc7ccf2, /// 0xd04
			 0x129220bb, /// 0xd08
			 0x0e3b3f3f, /// 0xd0c
			 0x0e013a77, /// 0xd10
			 0x2ca51ab3, /// 0xd14
			 0x19f404cd, /// 0xd18
			 0xec2e61ad, /// 0xd1c
			 0x7c2eb51d, /// 0xd20
			 0x0b357995, /// 0xd24
			 0x29dfe9e8, /// 0xd28
			 0x3c73f85b, /// 0xd2c
			 0x4938ffe4, /// 0xd30
			 0x461722ae, /// 0xd34
			 0x95f817a2, /// 0xd38
			 0xc5d0e126, /// 0xd3c
			 0x508abf69, /// 0xd40
			 0x1d0b7559, /// 0xd44
			 0x3cae9fb9, /// 0xd48
			 0x0a639ebe, /// 0xd4c
			 0xa7d0fca4, /// 0xd50
			 0x17878cb5, /// 0xd54
			 0x446a246a, /// 0xd58
			 0x4d8fe517, /// 0xd5c
			 0x0d1d12c4, /// 0xd60
			 0x3764fade, /// 0xd64
			 0x1faf9f67, /// 0xd68
			 0xe595deb3, /// 0xd6c
			 0x5be6793f, /// 0xd70
			 0xc7cadb5c, /// 0xd74
			 0xf971da36, /// 0xd78
			 0xe4f7a7fd, /// 0xd7c
			 0x08c08fa0, /// 0xd80
			 0x4bfcd404, /// 0xd84
			 0x63f8b4ca, /// 0xd88
			 0x82c2a367, /// 0xd8c
			 0xc8654929, /// 0xd90
			 0x94289655, /// 0xd94
			 0xa13134a3, /// 0xd98
			 0x9990251c, /// 0xd9c
			 0xf1dd947a, /// 0xda0
			 0x650e14ee, /// 0xda4
			 0x95f37b28, /// 0xda8
			 0xd7077654, /// 0xdac
			 0xdd89c6d0, /// 0xdb0
			 0x53896707, /// 0xdb4
			 0xefa446e9, /// 0xdb8
			 0x73f90c2b, /// 0xdbc
			 0x7b480245, /// 0xdc0
			 0xa6209157, /// 0xdc4
			 0xb77d52f3, /// 0xdc8
			 0x57d73dc6, /// 0xdcc
			 0xf021baed, /// 0xdd0
			 0x1ffa1b0c, /// 0xdd4
			 0x86ad2075, /// 0xdd8
			 0xf43b37d3, /// 0xddc
			 0x0e4d73cf, /// 0xde0
			 0x7cf4cc0c, /// 0xde4
			 0x7bd80ded, /// 0xde8
			 0xe9c74749, /// 0xdec
			 0x9d85d9c3, /// 0xdf0
			 0x69ff5a37, /// 0xdf4
			 0x5805f376, /// 0xdf8
			 0x99c8edd9, /// 0xdfc
			 0xbe738e04, /// 0xe00
			 0xa57638c8, /// 0xe04
			 0x587e75f6, /// 0xe08
			 0x3af49719, /// 0xe0c
			 0xff2eb98f, /// 0xe10
			 0xd7e7f6d7, /// 0xe14
			 0x51bb3d99, /// 0xe18
			 0x18177f95, /// 0xe1c
			 0x6df0fea6, /// 0xe20
			 0x13a7b74b, /// 0xe24
			 0x4e8f1415, /// 0xe28
			 0x12fafca2, /// 0xe2c
			 0xb7262111, /// 0xe30
			 0xb92fa238, /// 0xe34
			 0x8a3f82af, /// 0xe38
			 0x95b300ba, /// 0xe3c
			 0xe2624abd, /// 0xe40
			 0x7a5fcf54, /// 0xe44
			 0xaeec78a5, /// 0xe48
			 0xfcf49315, /// 0xe4c
			 0x201f640c, /// 0xe50
			 0x6cbc556e, /// 0xe54
			 0xa2bffbe9, /// 0xe58
			 0xcdb8355b, /// 0xe5c
			 0x3f0a6609, /// 0xe60
			 0xf272a7d0, /// 0xe64
			 0xc8a269c6, /// 0xe68
			 0x45bc3311, /// 0xe6c
			 0xa0976aaf, /// 0xe70
			 0x8a11f9d8, /// 0xe74
			 0x8572684e, /// 0xe78
			 0x54d1b151, /// 0xe7c
			 0x38256b0a, /// 0xe80
			 0x5fa828e9, /// 0xe84
			 0xe03afaa0, /// 0xe88
			 0xfd450e03, /// 0xe8c
			 0x81392e2d, /// 0xe90
			 0x588d1d04, /// 0xe94
			 0xd81874e8, /// 0xe98
			 0x597382d3, /// 0xe9c
			 0x07b40bd1, /// 0xea0
			 0x161f4343, /// 0xea4
			 0xdbe41036, /// 0xea8
			 0xbd920d20, /// 0xeac
			 0x25d7bc1c, /// 0xeb0
			 0x2fe1e86b, /// 0xeb4
			 0x326e2524, /// 0xeb8
			 0x89d24419, /// 0xebc
			 0x7b18d30c, /// 0xec0
			 0x55a81123, /// 0xec4
			 0x49e7dffc, /// 0xec8
			 0x45e82b54, /// 0xecc
			 0x65767c55, /// 0xed0
			 0xa29b04bf, /// 0xed4
			 0x07687872, /// 0xed8
			 0x3acb1a78, /// 0xedc
			 0x7aaa65ac, /// 0xee0
			 0x84e93699, /// 0xee4
			 0x343b7ece, /// 0xee8
			 0x3b4d08ab, /// 0xeec
			 0x217b81ee, /// 0xef0
			 0xc1838900, /// 0xef4
			 0xd8f7c80b, /// 0xef8
			 0x43453d40, /// 0xefc
			 0xe7117726, /// 0xf00
			 0x24ee08a9, /// 0xf04
			 0xdae325a4, /// 0xf08
			 0x19fdf58d, /// 0xf0c
			 0xeabad21e, /// 0xf10
			 0x9a92d69d, /// 0xf14
			 0x4e8cb5c0, /// 0xf18
			 0xa1add274, /// 0xf1c
			 0x3dda9e1f, /// 0xf20
			 0x74674789, /// 0xf24
			 0x0284bb28, /// 0xf28
			 0x102b337b, /// 0xf2c
			 0x3ca1d260, /// 0xf30
			 0x3da47880, /// 0xf34
			 0x53bee429, /// 0xf38
			 0xb8221079, /// 0xf3c
			 0x2a9618e3, /// 0xf40
			 0xb4311a43, /// 0xf44
			 0x8a293657, /// 0xf48
			 0x2a81826f, /// 0xf4c
			 0x71c5fdb9, /// 0xf50
			 0x3e5a6047, /// 0xf54
			 0x27fa1c0e, /// 0xf58
			 0x7ce1285d, /// 0xf5c
			 0xc6724fbc, /// 0xf60
			 0xa1cd17ef, /// 0xf64
			 0x46f9db97, /// 0xf68
			 0xaf153e7f, /// 0xf6c
			 0xa6cc0502, /// 0xf70
			 0x1a71e59a, /// 0xf74
			 0xbaa7f7ba, /// 0xf78
			 0x23f56e57, /// 0xf7c
			 0xdaf1ebe9, /// 0xf80
			 0xb99e0e9e, /// 0xf84
			 0x400c5b16, /// 0xf88
			 0xd14d93e6, /// 0xf8c
			 0x8e7c6517, /// 0xf90
			 0xdf057cd8, /// 0xf94
			 0xbce000b0, /// 0xf98
			 0x3f672f1f, /// 0xf9c
			 0xf5ee8e19, /// 0xfa0
			 0x1491b4d3, /// 0xfa4
			 0x0e984d5e, /// 0xfa8
			 0xd21c3ce8, /// 0xfac
			 0x9896ab97, /// 0xfb0
			 0x111e4e21, /// 0xfb4
			 0x61547237, /// 0xfb8
			 0x2e19024f, /// 0xfbc
			 0x804a2bbb, /// 0xfc0
			 0x64d18f7d, /// 0xfc4
			 0x67992548, /// 0xfc8
			 0x9f08f1db, /// 0xfcc
			 0xccffba46, /// 0xfd0
			 0xbd6446f5, /// 0xfd4
			 0x23f1dc13, /// 0xfd8
			 0xceb4f37e, /// 0xfdc
			 0x80aa7776, /// 0xfe0
			 0x672226b5, /// 0xfe4
			 0xf69f6b61, /// 0xfe8
			 0x722393ab, /// 0xfec
			 0xf1b1eb97, /// 0xff0
			 0xc644b478, /// 0xff4
			 0xdfda4cb7, /// 0xff8
			 0xe1d7eb77 /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x8eecd432, /// 0x0
			 0xfac0ab68, /// 0x4
			 0x175059b2, /// 0x8
			 0x19a707a6, /// 0xc
			 0x7ec1ad3a, /// 0x10
			 0xd5222e30, /// 0x14
			 0x9c6bfb60, /// 0x18
			 0xd8892259, /// 0x1c
			 0xf217bebb, /// 0x20
			 0x2f36ff7d, /// 0x24
			 0x5f086db8, /// 0x28
			 0x6b1f14e5, /// 0x2c
			 0xce760681, /// 0x30
			 0x26afce0a, /// 0x34
			 0x801f6bd9, /// 0x38
			 0xabfe2cf9, /// 0x3c
			 0x091b3e73, /// 0x40
			 0x077c6ef7, /// 0x44
			 0x00c908d6, /// 0x48
			 0x4c917202, /// 0x4c
			 0xa0e70aca, /// 0x50
			 0x1f22a43f, /// 0x54
			 0x09b0c6f8, /// 0x58
			 0x7f3f4383, /// 0x5c
			 0xcc7127a5, /// 0x60
			 0xc8aa6fb6, /// 0x64
			 0x3d7a8a58, /// 0x68
			 0xabd4ece6, /// 0x6c
			 0x56285e60, /// 0x70
			 0x971ca62d, /// 0x74
			 0x542533d7, /// 0x78
			 0xba0a3917, /// 0x7c
			 0x503f6ed6, /// 0x80
			 0x096ff2be, /// 0x84
			 0xed0b98f3, /// 0x88
			 0xec6cddb0, /// 0x8c
			 0x53fcb06e, /// 0x90
			 0xccc37ae6, /// 0x94
			 0xc2315532, /// 0x98
			 0x09b218d1, /// 0x9c
			 0x25599dbf, /// 0xa0
			 0x6f2b92b4, /// 0xa4
			 0xc213d766, /// 0xa8
			 0x70eb26ce, /// 0xac
			 0x1a4bb96b, /// 0xb0
			 0xdf04e936, /// 0xb4
			 0x14370d45, /// 0xb8
			 0x0b98049b, /// 0xbc
			 0x60944541, /// 0xc0
			 0xb5a40b2c, /// 0xc4
			 0x64828d23, /// 0xc8
			 0x64f63afe, /// 0xcc
			 0xcf2d4db5, /// 0xd0
			 0x60f46acd, /// 0xd4
			 0x1b544090, /// 0xd8
			 0x207cd2b8, /// 0xdc
			 0xf3a1fb8c, /// 0xe0
			 0xaa6bb7f9, /// 0xe4
			 0x0c124d46, /// 0xe8
			 0x9ebed43d, /// 0xec
			 0x6de040f7, /// 0xf0
			 0x2126c234, /// 0xf4
			 0x0a2655b1, /// 0xf8
			 0xf6c1cb11, /// 0xfc
			 0xec4cc66e, /// 0x100
			 0x715812a0, /// 0x104
			 0x428dc83f, /// 0x108
			 0x8c6815d8, /// 0x10c
			 0x98e17f95, /// 0x110
			 0x2ca2fdfe, /// 0x114
			 0x5f9919a0, /// 0x118
			 0x7abd5f51, /// 0x11c
			 0xf411beb1, /// 0x120
			 0x5c6a32fb, /// 0x124
			 0x837dcf3f, /// 0x128
			 0x7bd798f2, /// 0x12c
			 0x025a4c3b, /// 0x130
			 0x73859525, /// 0x134
			 0x6ad1bdd1, /// 0x138
			 0xe567fef1, /// 0x13c
			 0x4f7c44d4, /// 0x140
			 0xf60c5e80, /// 0x144
			 0x93d56f6c, /// 0x148
			 0xfb9aa513, /// 0x14c
			 0x831ee7bc, /// 0x150
			 0xd9e1855b, /// 0x154
			 0x733aaa14, /// 0x158
			 0x23b6a381, /// 0x15c
			 0x9330c921, /// 0x160
			 0xb91c4d74, /// 0x164
			 0xd1ce4309, /// 0x168
			 0xeb1a11ee, /// 0x16c
			 0xec002ee5, /// 0x170
			 0x872fbba2, /// 0x174
			 0x4f1cea3f, /// 0x178
			 0x8d443a41, /// 0x17c
			 0x36a13d17, /// 0x180
			 0x3265c199, /// 0x184
			 0x68698596, /// 0x188
			 0x77a30f45, /// 0x18c
			 0x4bd4ffb1, /// 0x190
			 0x61505f53, /// 0x194
			 0xcb772553, /// 0x198
			 0x9e53493e, /// 0x19c
			 0x089a1f60, /// 0x1a0
			 0xe3ed5c13, /// 0x1a4
			 0x5037c342, /// 0x1a8
			 0xbf32ac21, /// 0x1ac
			 0x7acc49b9, /// 0x1b0
			 0x6e227603, /// 0x1b4
			 0xfeec06e8, /// 0x1b8
			 0x5821844b, /// 0x1bc
			 0xfb0592ce, /// 0x1c0
			 0x93bc5eda, /// 0x1c4
			 0x5835392b, /// 0x1c8
			 0x3d65e513, /// 0x1cc
			 0xf31bcaa5, /// 0x1d0
			 0x346a785f, /// 0x1d4
			 0x29d414f7, /// 0x1d8
			 0xf0981ad1, /// 0x1dc
			 0x5222fd27, /// 0x1e0
			 0x38e7f30f, /// 0x1e4
			 0xc63fa4b5, /// 0x1e8
			 0x7bfb46ab, /// 0x1ec
			 0xa4dc8b57, /// 0x1f0
			 0xf92ded05, /// 0x1f4
			 0xae7f7018, /// 0x1f8
			 0x608a0d52, /// 0x1fc
			 0x7cfe02cd, /// 0x200
			 0x3a993081, /// 0x204
			 0x8872bfb6, /// 0x208
			 0x9c9565b9, /// 0x20c
			 0x9aa5c906, /// 0x210
			 0x31ab5eb1, /// 0x214
			 0x9d3fc155, /// 0x218
			 0x2bbeb294, /// 0x21c
			 0xd50f2973, /// 0x220
			 0xf0b2885c, /// 0x224
			 0x86791aa1, /// 0x228
			 0xa50f6874, /// 0x22c
			 0xf4e70246, /// 0x230
			 0xc3c89aad, /// 0x234
			 0x42ea8848, /// 0x238
			 0xbd2734ce, /// 0x23c
			 0x7f54501f, /// 0x240
			 0x2da5c3bd, /// 0x244
			 0x291dd78f, /// 0x248
			 0x84e7b335, /// 0x24c
			 0x6cedfc35, /// 0x250
			 0x14c50322, /// 0x254
			 0x9b1ade09, /// 0x258
			 0xfdc7cb1b, /// 0x25c
			 0x736c907b, /// 0x260
			 0x0066781c, /// 0x264
			 0x59d8d851, /// 0x268
			 0x2656c49a, /// 0x26c
			 0x233621ab, /// 0x270
			 0x46216b46, /// 0x274
			 0x6989acb3, /// 0x278
			 0xe96a0046, /// 0x27c
			 0x278ccadd, /// 0x280
			 0x9a9a8970, /// 0x284
			 0x6009bd81, /// 0x288
			 0xa9f2662f, /// 0x28c
			 0xd4081506, /// 0x290
			 0x399797f5, /// 0x294
			 0x0f0ec9f5, /// 0x298
			 0x53f329c0, /// 0x29c
			 0xfc96f774, /// 0x2a0
			 0xca5a5877, /// 0x2a4
			 0xf9b5f29c, /// 0x2a8
			 0x12e9f2d2, /// 0x2ac
			 0xb0d24a7e, /// 0x2b0
			 0x02ead179, /// 0x2b4
			 0xb277cde8, /// 0x2b8
			 0xb94931f9, /// 0x2bc
			 0xc422734f, /// 0x2c0
			 0x7dd6647b, /// 0x2c4
			 0xd40f107b, /// 0x2c8
			 0x08358be8, /// 0x2cc
			 0xc2d37f87, /// 0x2d0
			 0x35714557, /// 0x2d4
			 0xcc6c0d72, /// 0x2d8
			 0x551ce07c, /// 0x2dc
			 0xca51a197, /// 0x2e0
			 0x15ca8af9, /// 0x2e4
			 0xe3f70144, /// 0x2e8
			 0x74303de5, /// 0x2ec
			 0x74dcf4a4, /// 0x2f0
			 0x6e6be700, /// 0x2f4
			 0x323cf6d4, /// 0x2f8
			 0x958b2228, /// 0x2fc
			 0x7e69e8ad, /// 0x300
			 0xf665fc31, /// 0x304
			 0x9a338b5e, /// 0x308
			 0x1a591342, /// 0x30c
			 0x582b6b57, /// 0x310
			 0x53d9ed5a, /// 0x314
			 0x3e8493c7, /// 0x318
			 0x466ec064, /// 0x31c
			 0x4feb0ee6, /// 0x320
			 0x04ff15db, /// 0x324
			 0x1b7bf1a4, /// 0x328
			 0x123269e4, /// 0x32c
			 0x0a60e240, /// 0x330
			 0x16acc9ce, /// 0x334
			 0xbf7d83d2, /// 0x338
			 0xcdcfba17, /// 0x33c
			 0x4ee5b967, /// 0x340
			 0x8472dba7, /// 0x344
			 0x5ef3ec4c, /// 0x348
			 0xdc09966f, /// 0x34c
			 0x6ef30421, /// 0x350
			 0x219d5904, /// 0x354
			 0x5b8d4d24, /// 0x358
			 0xba204d66, /// 0x35c
			 0x0f08a6db, /// 0x360
			 0xdb7ef464, /// 0x364
			 0x46591022, /// 0x368
			 0x297c59c1, /// 0x36c
			 0x1d9904c1, /// 0x370
			 0x4f29ea1d, /// 0x374
			 0x9a7e6260, /// 0x378
			 0x3989d54a, /// 0x37c
			 0x234bc324, /// 0x380
			 0x7ddcebb3, /// 0x384
			 0x21ad04b8, /// 0x388
			 0xc08c3aa4, /// 0x38c
			 0xa3c14e55, /// 0x390
			 0x423ec424, /// 0x394
			 0xbd927078, /// 0x398
			 0x5b524eba, /// 0x39c
			 0x89d0f47b, /// 0x3a0
			 0x525eb85e, /// 0x3a4
			 0xbd9302b2, /// 0x3a8
			 0xe83caa7b, /// 0x3ac
			 0xede3b3fd, /// 0x3b0
			 0xc3c9deb6, /// 0x3b4
			 0xecdbcb2c, /// 0x3b8
			 0x05dcec27, /// 0x3bc
			 0x274ce952, /// 0x3c0
			 0x41653364, /// 0x3c4
			 0x4cfdbc49, /// 0x3c8
			 0x7b19ab14, /// 0x3cc
			 0x32f506ae, /// 0x3d0
			 0x8bea932b, /// 0x3d4
			 0xcf0f6505, /// 0x3d8
			 0x364170fe, /// 0x3dc
			 0x1a6a8f14, /// 0x3e0
			 0xca073e55, /// 0x3e4
			 0x8fe25524, /// 0x3e8
			 0x135f72db, /// 0x3ec
			 0x6dc93adc, /// 0x3f0
			 0xf53e957e, /// 0x3f4
			 0xd1ecd9ce, /// 0x3f8
			 0x06212e04, /// 0x3fc
			 0xc8174077, /// 0x400
			 0xaaebd48c, /// 0x404
			 0x69c81e0b, /// 0x408
			 0x983cee45, /// 0x40c
			 0xaaedb20f, /// 0x410
			 0x00d0d1da, /// 0x414
			 0xf2389c4b, /// 0x418
			 0x163d6139, /// 0x41c
			 0xbea09ab6, /// 0x420
			 0x3d8536ea, /// 0x424
			 0x1d896c61, /// 0x428
			 0x4a8be9b8, /// 0x42c
			 0x113c8ed2, /// 0x430
			 0xc3d16344, /// 0x434
			 0xc66e3f95, /// 0x438
			 0x31cc8391, /// 0x43c
			 0x5fc8e49f, /// 0x440
			 0xc6fd17aa, /// 0x444
			 0x8f599ec6, /// 0x448
			 0xda7d5bc6, /// 0x44c
			 0x99e2ed90, /// 0x450
			 0xa6fd2da2, /// 0x454
			 0x5b9f5f9e, /// 0x458
			 0xf3178f20, /// 0x45c
			 0xc31ca6a5, /// 0x460
			 0x3e85a6af, /// 0x464
			 0x97495285, /// 0x468
			 0x011baad3, /// 0x46c
			 0x31fe2b15, /// 0x470
			 0xa1188972, /// 0x474
			 0x61eb40ba, /// 0x478
			 0xc73f99c3, /// 0x47c
			 0xc9e4b2ab, /// 0x480
			 0xca1e8470, /// 0x484
			 0xc0aa8bc5, /// 0x488
			 0x405fa6e9, /// 0x48c
			 0xf6f5f8e5, /// 0x490
			 0xd9c125b2, /// 0x494
			 0x29d478b4, /// 0x498
			 0x5020859b, /// 0x49c
			 0x9949585c, /// 0x4a0
			 0xf624e1a8, /// 0x4a4
			 0xb70614c0, /// 0x4a8
			 0x5cc4b55b, /// 0x4ac
			 0xd8dcda17, /// 0x4b0
			 0x2431eed9, /// 0x4b4
			 0x25e5a3bd, /// 0x4b8
			 0x5cb7dc2f, /// 0x4bc
			 0xac6df3a3, /// 0x4c0
			 0x60460457, /// 0x4c4
			 0x297a851a, /// 0x4c8
			 0x11e7fcd7, /// 0x4cc
			 0xc9f6a8e4, /// 0x4d0
			 0x2f2a51bc, /// 0x4d4
			 0xa00de238, /// 0x4d8
			 0xf2b495a3, /// 0x4dc
			 0x5606a28e, /// 0x4e0
			 0xe994211d, /// 0x4e4
			 0x1ed061b4, /// 0x4e8
			 0x6c57aeec, /// 0x4ec
			 0xb74c6339, /// 0x4f0
			 0xe1ac0131, /// 0x4f4
			 0xf7a6d8b7, /// 0x4f8
			 0xd9eebe7d, /// 0x4fc
			 0xb50f4ecc, /// 0x500
			 0x51fd8c59, /// 0x504
			 0xaefe6608, /// 0x508
			 0x3eeb2e72, /// 0x50c
			 0xff9b1f0c, /// 0x510
			 0x65381ea0, /// 0x514
			 0x0e48af96, /// 0x518
			 0xab868159, /// 0x51c
			 0xd3be918d, /// 0x520
			 0x413596aa, /// 0x524
			 0x5a0b67ae, /// 0x528
			 0xd52627b7, /// 0x52c
			 0x55749ad6, /// 0x530
			 0xdde9a704, /// 0x534
			 0xd9cc86b6, /// 0x538
			 0x4e41c088, /// 0x53c
			 0xb0fe51e8, /// 0x540
			 0x088c8436, /// 0x544
			 0x995ba017, /// 0x548
			 0x577a371f, /// 0x54c
			 0x3723ccca, /// 0x550
			 0xdff8a6db, /// 0x554
			 0x615f6ca1, /// 0x558
			 0x19d11881, /// 0x55c
			 0x349f707b, /// 0x560
			 0x00fed57f, /// 0x564
			 0x37a67289, /// 0x568
			 0x93822054, /// 0x56c
			 0x820c9c0d, /// 0x570
			 0x9807f3ce, /// 0x574
			 0x4f725452, /// 0x578
			 0x523496a3, /// 0x57c
			 0x4b6528ba, /// 0x580
			 0x08f6e1ea, /// 0x584
			 0xb553325b, /// 0x588
			 0x9587c2a3, /// 0x58c
			 0x8d5a4810, /// 0x590
			 0x70bdcfd5, /// 0x594
			 0xe982e857, /// 0x598
			 0xd0096e6b, /// 0x59c
			 0x5301905d, /// 0x5a0
			 0x5f56c7b1, /// 0x5a4
			 0x52214f65, /// 0x5a8
			 0x0df57960, /// 0x5ac
			 0xa669a83a, /// 0x5b0
			 0xd08ff1e6, /// 0x5b4
			 0x37ee5be5, /// 0x5b8
			 0x9213b11b, /// 0x5bc
			 0x40617a5a, /// 0x5c0
			 0xa5987e59, /// 0x5c4
			 0x646bdbc6, /// 0x5c8
			 0x4f069c5b, /// 0x5cc
			 0x9cf7b0a6, /// 0x5d0
			 0xab33f66f, /// 0x5d4
			 0x2fcba7d6, /// 0x5d8
			 0xdfdea33b, /// 0x5dc
			 0x0f1dbc18, /// 0x5e0
			 0xbd1637c4, /// 0x5e4
			 0x80f421c6, /// 0x5e8
			 0x358d7ab3, /// 0x5ec
			 0x5ca5b9d6, /// 0x5f0
			 0xec0b3449, /// 0x5f4
			 0xfba3622d, /// 0x5f8
			 0x6a5770c4, /// 0x5fc
			 0x93776d0a, /// 0x600
			 0xfc894366, /// 0x604
			 0xd00d4feb, /// 0x608
			 0x0ecb1ebd, /// 0x60c
			 0x48544b11, /// 0x610
			 0x39035545, /// 0x614
			 0xfe0666d2, /// 0x618
			 0xa64f2572, /// 0x61c
			 0xd71fce9d, /// 0x620
			 0x9d439940, /// 0x624
			 0xdd35f1fb, /// 0x628
			 0x780b50bf, /// 0x62c
			 0x80453ca5, /// 0x630
			 0x21a5fa13, /// 0x634
			 0xb6ed3cdf, /// 0x638
			 0xd5c57e77, /// 0x63c
			 0xad110b68, /// 0x640
			 0xee21b79b, /// 0x644
			 0x67bb383e, /// 0x648
			 0x3c0048ab, /// 0x64c
			 0xc9826ba5, /// 0x650
			 0xa6349f0a, /// 0x654
			 0x668fa003, /// 0x658
			 0x957a88ed, /// 0x65c
			 0x40671270, /// 0x660
			 0x3892454d, /// 0x664
			 0xde260f02, /// 0x668
			 0xb43cd948, /// 0x66c
			 0xadbd865e, /// 0x670
			 0xabe55629, /// 0x674
			 0x276621de, /// 0x678
			 0xc84597a9, /// 0x67c
			 0x90443861, /// 0x680
			 0xfc240277, /// 0x684
			 0xd3bc734b, /// 0x688
			 0x231b4371, /// 0x68c
			 0xb3089130, /// 0x690
			 0x7c020196, /// 0x694
			 0x1ff134c4, /// 0x698
			 0xbf288041, /// 0x69c
			 0xaadd29ad, /// 0x6a0
			 0x049f7f7a, /// 0x6a4
			 0x51b91abb, /// 0x6a8
			 0x4d448291, /// 0x6ac
			 0x2e875524, /// 0x6b0
			 0x15334cd6, /// 0x6b4
			 0x909ea5a8, /// 0x6b8
			 0xcb7e934c, /// 0x6bc
			 0x69ffed5f, /// 0x6c0
			 0x6ceaa7d0, /// 0x6c4
			 0xc86289cb, /// 0x6c8
			 0x2f0a0310, /// 0x6cc
			 0x0f654de7, /// 0x6d0
			 0xb10ecc9b, /// 0x6d4
			 0x05e65630, /// 0x6d8
			 0xf1e60b52, /// 0x6dc
			 0xc10512e6, /// 0x6e0
			 0x7f7db192, /// 0x6e4
			 0x47515d87, /// 0x6e8
			 0x8d37eb0a, /// 0x6ec
			 0xfc6e539b, /// 0x6f0
			 0x4b6eb5c6, /// 0x6f4
			 0x5cd5869a, /// 0x6f8
			 0x232a8ea7, /// 0x6fc
			 0xf3157bbc, /// 0x700
			 0x69b02271, /// 0x704
			 0x714a9695, /// 0x708
			 0x03e24662, /// 0x70c
			 0x73447a13, /// 0x710
			 0x022ff631, /// 0x714
			 0x5bd133ff, /// 0x718
			 0x0ed9d28c, /// 0x71c
			 0x08e93b24, /// 0x720
			 0x107c06cf, /// 0x724
			 0x65116c37, /// 0x728
			 0x79920869, /// 0x72c
			 0xbdedc6eb, /// 0x730
			 0x5381d3c8, /// 0x734
			 0x3298697a, /// 0x738
			 0xe7525b8d, /// 0x73c
			 0x517de23d, /// 0x740
			 0xd8aa8ae8, /// 0x744
			 0x9705b49f, /// 0x748
			 0x8b69b5dd, /// 0x74c
			 0xc4ab88fc, /// 0x750
			 0xea8149a0, /// 0x754
			 0xcd6fac06, /// 0x758
			 0xa4c359da, /// 0x75c
			 0xf4df285d, /// 0x760
			 0x188beef9, /// 0x764
			 0x2ac96006, /// 0x768
			 0x99a8d182, /// 0x76c
			 0xc17ed78b, /// 0x770
			 0x53d6041e, /// 0x774
			 0x02a61555, /// 0x778
			 0x1d223a1d, /// 0x77c
			 0xeb81cc07, /// 0x780
			 0xb949fd50, /// 0x784
			 0x5c70351a, /// 0x788
			 0x1f5c242c, /// 0x78c
			 0xd70a1480, /// 0x790
			 0x1e6e2eed, /// 0x794
			 0x286757dd, /// 0x798
			 0xe692e4ce, /// 0x79c
			 0x3e9abb7c, /// 0x7a0
			 0x4e7020e6, /// 0x7a4
			 0xf27183ea, /// 0x7a8
			 0xa320830e, /// 0x7ac
			 0x817acf6e, /// 0x7b0
			 0xaf54c82b, /// 0x7b4
			 0x347347ad, /// 0x7b8
			 0x7bf23e5b, /// 0x7bc
			 0xb11ea02c, /// 0x7c0
			 0xeaede80a, /// 0x7c4
			 0x890e04f0, /// 0x7c8
			 0xd88cedb0, /// 0x7cc
			 0x1922b449, /// 0x7d0
			 0xf921d3f9, /// 0x7d4
			 0xdfeef456, /// 0x7d8
			 0xbb22209f, /// 0x7dc
			 0xa8136f86, /// 0x7e0
			 0x7c608bcc, /// 0x7e4
			 0x0e2d4b02, /// 0x7e8
			 0xb976b5ab, /// 0x7ec
			 0xbe8380b3, /// 0x7f0
			 0x6e01e0de, /// 0x7f4
			 0x00506614, /// 0x7f8
			 0x226d42a1, /// 0x7fc
			 0xc839641e, /// 0x800
			 0x9977ca81, /// 0x804
			 0x1bf2a6dc, /// 0x808
			 0x96d9e482, /// 0x80c
			 0xb9c75bd6, /// 0x810
			 0xdca22227, /// 0x814
			 0x6ef8a73e, /// 0x818
			 0xead25cf7, /// 0x81c
			 0x51109b43, /// 0x820
			 0x1f51afab, /// 0x824
			 0x94f20dda, /// 0x828
			 0x71940929, /// 0x82c
			 0x8fbcd7a6, /// 0x830
			 0x9c7c5a17, /// 0x834
			 0xc6ae0894, /// 0x838
			 0x9c93c926, /// 0x83c
			 0x1a9d9dc3, /// 0x840
			 0x3a26f418, /// 0x844
			 0xbd638fc1, /// 0x848
			 0x12062ae7, /// 0x84c
			 0x6ceff781, /// 0x850
			 0xc718dc1b, /// 0x854
			 0x77285554, /// 0x858
			 0xd5d58e16, /// 0x85c
			 0xc00243cd, /// 0x860
			 0x75cf73c9, /// 0x864
			 0x4ce93779, /// 0x868
			 0x3e616e63, /// 0x86c
			 0x2aea2cea, /// 0x870
			 0x2f2707b1, /// 0x874
			 0xdf1c6380, /// 0x878
			 0x49546c74, /// 0x87c
			 0x8c623d27, /// 0x880
			 0xee661b2f, /// 0x884
			 0x914b0728, /// 0x888
			 0x66c024af, /// 0x88c
			 0xc9a532d2, /// 0x890
			 0x44a1b6ab, /// 0x894
			 0x487ea353, /// 0x898
			 0x3ae18a0a, /// 0x89c
			 0x167ba6b3, /// 0x8a0
			 0xe7410f03, /// 0x8a4
			 0xa5e8f584, /// 0x8a8
			 0x7c36951a, /// 0x8ac
			 0xb0b53f51, /// 0x8b0
			 0xe2a610a7, /// 0x8b4
			 0xd4a0c9b8, /// 0x8b8
			 0x0c927f83, /// 0x8bc
			 0x7724d85e, /// 0x8c0
			 0x600a8393, /// 0x8c4
			 0xf1c7caf0, /// 0x8c8
			 0x309dccf9, /// 0x8cc
			 0x1ad75f78, /// 0x8d0
			 0x8693d7f6, /// 0x8d4
			 0x46337fdf, /// 0x8d8
			 0x762266af, /// 0x8dc
			 0x774f87f2, /// 0x8e0
			 0x8112054a, /// 0x8e4
			 0x5cc5a9be, /// 0x8e8
			 0x04512ea5, /// 0x8ec
			 0xd1f91884, /// 0x8f0
			 0xa8f48691, /// 0x8f4
			 0xa083e260, /// 0x8f8
			 0x0dc5903b, /// 0x8fc
			 0xaa2987ab, /// 0x900
			 0xa3a3c4f0, /// 0x904
			 0x2e3d39e8, /// 0x908
			 0x15f1facc, /// 0x90c
			 0xca0ab79a, /// 0x910
			 0x886bc9e8, /// 0x914
			 0xce6b2043, /// 0x918
			 0x6419925c, /// 0x91c
			 0x137291ae, /// 0x920
			 0xec98a57e, /// 0x924
			 0xb8d1cac5, /// 0x928
			 0xfdf760fa, /// 0x92c
			 0xda24cbb4, /// 0x930
			 0xc59185c0, /// 0x934
			 0xaf7e3a46, /// 0x938
			 0x5ee08549, /// 0x93c
			 0x125fdb9a, /// 0x940
			 0x9c6c000f, /// 0x944
			 0x72156e80, /// 0x948
			 0x16cec30b, /// 0x94c
			 0x2a1b790e, /// 0x950
			 0x4faa3fe3, /// 0x954
			 0x010f07ce, /// 0x958
			 0x458d19ab, /// 0x95c
			 0xa5cfb673, /// 0x960
			 0x32d4d1a2, /// 0x964
			 0xaad5e474, /// 0x968
			 0x3137707e, /// 0x96c
			 0x9d258430, /// 0x970
			 0xe1034de9, /// 0x974
			 0x99e04e80, /// 0x978
			 0x133f9112, /// 0x97c
			 0x1da32fc2, /// 0x980
			 0x2af554a3, /// 0x984
			 0x69208c3d, /// 0x988
			 0x1c6585e7, /// 0x98c
			 0xcb287c5f, /// 0x990
			 0x888ffb21, /// 0x994
			 0x0fab938b, /// 0x998
			 0x26f0b135, /// 0x99c
			 0x8695b1d6, /// 0x9a0
			 0x0fd52744, /// 0x9a4
			 0x55763cff, /// 0x9a8
			 0x15de123e, /// 0x9ac
			 0xcb5157d4, /// 0x9b0
			 0x1bc06fa8, /// 0x9b4
			 0xbb59222c, /// 0x9b8
			 0x47dfadf2, /// 0x9bc
			 0xc8249ba9, /// 0x9c0
			 0x79dee39d, /// 0x9c4
			 0x9c28cc4f, /// 0x9c8
			 0x9b696ab8, /// 0x9cc
			 0x6031aee3, /// 0x9d0
			 0xcdc238e9, /// 0x9d4
			 0x3b9298da, /// 0x9d8
			 0x6f3b1410, /// 0x9dc
			 0xfaf9a3a6, /// 0x9e0
			 0x0ba86a4e, /// 0x9e4
			 0x25ac7e81, /// 0x9e8
			 0x6da90c3e, /// 0x9ec
			 0x11a96150, /// 0x9f0
			 0x342732ed, /// 0x9f4
			 0xe75184a7, /// 0x9f8
			 0x2ca61849, /// 0x9fc
			 0x0a1fe769, /// 0xa00
			 0xd138fb41, /// 0xa04
			 0x7a9843a9, /// 0xa08
			 0xa176cd9f, /// 0xa0c
			 0xc16adbd3, /// 0xa10
			 0xadd3b98b, /// 0xa14
			 0x052ecfc7, /// 0xa18
			 0xabc58eae, /// 0xa1c
			 0xe4caf98b, /// 0xa20
			 0x7b43a91e, /// 0xa24
			 0xd144cc0b, /// 0xa28
			 0x427ef26a, /// 0xa2c
			 0xd1f9a1a0, /// 0xa30
			 0xc8fc5502, /// 0xa34
			 0xe3a8e096, /// 0xa38
			 0x1083d3ca, /// 0xa3c
			 0x692acd37, /// 0xa40
			 0x18ac64c5, /// 0xa44
			 0xa4d1ee4c, /// 0xa48
			 0x20e7cb3f, /// 0xa4c
			 0x697177b0, /// 0xa50
			 0x2a91aaec, /// 0xa54
			 0x5ef11fcc, /// 0xa58
			 0x64e8a8fd, /// 0xa5c
			 0x9b5adfa1, /// 0xa60
			 0xf290a328, /// 0xa64
			 0x51ee99e5, /// 0xa68
			 0xac56fda1, /// 0xa6c
			 0x0b4f746c, /// 0xa70
			 0xe55f44f1, /// 0xa74
			 0xa1f2a2b2, /// 0xa78
			 0x8d4ce2af, /// 0xa7c
			 0x0c49fb3d, /// 0xa80
			 0x5e45a95e, /// 0xa84
			 0xad332ef7, /// 0xa88
			 0xbec78d1e, /// 0xa8c
			 0xdc389b01, /// 0xa90
			 0x4a1516b8, /// 0xa94
			 0x9afc37f3, /// 0xa98
			 0xda0f3b22, /// 0xa9c
			 0x582b8660, /// 0xaa0
			 0xbbfa6ded, /// 0xaa4
			 0xa363429f, /// 0xaa8
			 0xd9844fd2, /// 0xaac
			 0x23f8f4e5, /// 0xab0
			 0xb50f8c11, /// 0xab4
			 0x0be17e36, /// 0xab8
			 0xa29ec687, /// 0xabc
			 0x9ff7bfc0, /// 0xac0
			 0x7708f910, /// 0xac4
			 0x14229fbc, /// 0xac8
			 0x55051135, /// 0xacc
			 0xb79c9754, /// 0xad0
			 0xecf27515, /// 0xad4
			 0xf03079ad, /// 0xad8
			 0xbb907913, /// 0xadc
			 0x34f22647, /// 0xae0
			 0x544c4559, /// 0xae4
			 0x61fc2e36, /// 0xae8
			 0xb03196ed, /// 0xaec
			 0x44d8ff9c, /// 0xaf0
			 0x60f39cdd, /// 0xaf4
			 0x5f36d6ad, /// 0xaf8
			 0xdd88cdf7, /// 0xafc
			 0x07070b71, /// 0xb00
			 0x49e91c65, /// 0xb04
			 0x8ec5fbf4, /// 0xb08
			 0x21962a8c, /// 0xb0c
			 0x6b8be5c3, /// 0xb10
			 0xd9f39129, /// 0xb14
			 0x84950a2d, /// 0xb18
			 0xb90ae209, /// 0xb1c
			 0x68222c54, /// 0xb20
			 0x5222dbc0, /// 0xb24
			 0xd39228bd, /// 0xb28
			 0xa0ceeafc, /// 0xb2c
			 0xf336224b, /// 0xb30
			 0xceca4b58, /// 0xb34
			 0xf161d485, /// 0xb38
			 0xf1d2a0c8, /// 0xb3c
			 0x0abb3ad7, /// 0xb40
			 0xe7edfaed, /// 0xb44
			 0x98faca37, /// 0xb48
			 0x0ce6842d, /// 0xb4c
			 0x3b419e21, /// 0xb50
			 0x1e607ee4, /// 0xb54
			 0xf1fd703d, /// 0xb58
			 0x1765bd94, /// 0xb5c
			 0xfb5cdaa6, /// 0xb60
			 0x8b5edcf2, /// 0xb64
			 0xdf20bc7b, /// 0xb68
			 0x3afdffcb, /// 0xb6c
			 0x7050c5d3, /// 0xb70
			 0xa0d30ba9, /// 0xb74
			 0x2e015b75, /// 0xb78
			 0x55d1c66d, /// 0xb7c
			 0x986a35ad, /// 0xb80
			 0xf294ac17, /// 0xb84
			 0x11f15f0a, /// 0xb88
			 0xa45ecfa6, /// 0xb8c
			 0xf6ba9d17, /// 0xb90
			 0x8d115a12, /// 0xb94
			 0x8f87a969, /// 0xb98
			 0x4821ee34, /// 0xb9c
			 0x1154b59e, /// 0xba0
			 0x00134095, /// 0xba4
			 0x1bd68188, /// 0xba8
			 0x42d57048, /// 0xbac
			 0xfeeb0a34, /// 0xbb0
			 0x623860f2, /// 0xbb4
			 0x7f50afb4, /// 0xbb8
			 0xa1cc9c66, /// 0xbbc
			 0x9e3cf7e6, /// 0xbc0
			 0x60fe46af, /// 0xbc4
			 0xa1552368, /// 0xbc8
			 0x7196dc5b, /// 0xbcc
			 0x9921b9f3, /// 0xbd0
			 0xdffa4cbd, /// 0xbd4
			 0xfff08e77, /// 0xbd8
			 0x21468978, /// 0xbdc
			 0x1be8248c, /// 0xbe0
			 0xd77637a4, /// 0xbe4
			 0x692a9e2c, /// 0xbe8
			 0x336bfcd2, /// 0xbec
			 0xa566da3f, /// 0xbf0
			 0xacc74910, /// 0xbf4
			 0xe5760ce8, /// 0xbf8
			 0x4c5b8870, /// 0xbfc
			 0xfe714faf, /// 0xc00
			 0xd142e4b5, /// 0xc04
			 0xc56922b8, /// 0xc08
			 0xe91492c6, /// 0xc0c
			 0x8262b151, /// 0xc10
			 0xfc53785d, /// 0xc14
			 0x42966659, /// 0xc18
			 0xddeceea0, /// 0xc1c
			 0x86883dab, /// 0xc20
			 0x09d77486, /// 0xc24
			 0xc79cf9da, /// 0xc28
			 0xf88a0d92, /// 0xc2c
			 0x3435c27c, /// 0xc30
			 0x763ffe7d, /// 0xc34
			 0x017302eb, /// 0xc38
			 0x595f0c37, /// 0xc3c
			 0x64cf078f, /// 0xc40
			 0x3cad25b5, /// 0xc44
			 0x700cf47a, /// 0xc48
			 0x81d4bf3f, /// 0xc4c
			 0x45ace5c6, /// 0xc50
			 0x4f3340d2, /// 0xc54
			 0x76033c12, /// 0xc58
			 0x330d538d, /// 0xc5c
			 0x042b47b8, /// 0xc60
			 0xfe1d269b, /// 0xc64
			 0x5978ab05, /// 0xc68
			 0xcd4743c4, /// 0xc6c
			 0x0c8cc985, /// 0xc70
			 0xf75db21c, /// 0xc74
			 0x11d03275, /// 0xc78
			 0xfad6d0de, /// 0xc7c
			 0xd9ff2166, /// 0xc80
			 0x8d0a816c, /// 0xc84
			 0x17ee883c, /// 0xc88
			 0x63310908, /// 0xc8c
			 0x28e200d3, /// 0xc90
			 0xea9263f1, /// 0xc94
			 0xe236f9de, /// 0xc98
			 0x08b473ad, /// 0xc9c
			 0xa6871ba6, /// 0xca0
			 0xce176f4a, /// 0xca4
			 0xc7a2b332, /// 0xca8
			 0x479bc5dc, /// 0xcac
			 0x68ac1f46, /// 0xcb0
			 0xc906ca03, /// 0xcb4
			 0xcc4190fc, /// 0xcb8
			 0x98ff92e0, /// 0xcbc
			 0x09a9b987, /// 0xcc0
			 0x3369c6eb, /// 0xcc4
			 0xd2fad7a1, /// 0xcc8
			 0x542e8df3, /// 0xccc
			 0x641ce876, /// 0xcd0
			 0x9175a4e3, /// 0xcd4
			 0x7e5247ed, /// 0xcd8
			 0x8159a009, /// 0xcdc
			 0x512cd099, /// 0xce0
			 0xea172db3, /// 0xce4
			 0x336eff63, /// 0xce8
			 0x7aea4eb8, /// 0xcec
			 0x1d52cfc1, /// 0xcf0
			 0xd6256660, /// 0xcf4
			 0x2d769553, /// 0xcf8
			 0x36ae1d91, /// 0xcfc
			 0xf6eb9a0a, /// 0xd00
			 0xd15b910a, /// 0xd04
			 0xd56abbec, /// 0xd08
			 0xe8f6c7b9, /// 0xd0c
			 0xb8b969f0, /// 0xd10
			 0x646d59a2, /// 0xd14
			 0x843dde63, /// 0xd18
			 0xda41659b, /// 0xd1c
			 0xceb1fb27, /// 0xd20
			 0xb3d06bbe, /// 0xd24
			 0x4e1f1409, /// 0xd28
			 0x964a97e7, /// 0xd2c
			 0x8a5b1fc5, /// 0xd30
			 0x59050778, /// 0xd34
			 0x51a18a60, /// 0xd38
			 0x16b81471, /// 0xd3c
			 0x1c4d2ee0, /// 0xd40
			 0xcc1aca2a, /// 0xd44
			 0x97781211, /// 0xd48
			 0xb057bd5b, /// 0xd4c
			 0x110bb94c, /// 0xd50
			 0xf370e06d, /// 0xd54
			 0x3c8538f8, /// 0xd58
			 0x6c90dfeb, /// 0xd5c
			 0xe09fc936, /// 0xd60
			 0xf30a666c, /// 0xd64
			 0x816bbe35, /// 0xd68
			 0x2c7f5d06, /// 0xd6c
			 0x9910ca1f, /// 0xd70
			 0x12a44067, /// 0xd74
			 0x08ce2f24, /// 0xd78
			 0x0c6dffeb, /// 0xd7c
			 0x154bf094, /// 0xd80
			 0x786528a6, /// 0xd84
			 0x67fe3fe3, /// 0xd88
			 0xd2152dd3, /// 0xd8c
			 0xf0021e36, /// 0xd90
			 0x38d9848d, /// 0xd94
			 0x6f5892ab, /// 0xd98
			 0x41355de2, /// 0xd9c
			 0x8bc79402, /// 0xda0
			 0x8b9dd3f4, /// 0xda4
			 0xda36a622, /// 0xda8
			 0x6433a283, /// 0xdac
			 0x222482b7, /// 0xdb0
			 0xd6146a10, /// 0xdb4
			 0x48d9a746, /// 0xdb8
			 0x77676ccf, /// 0xdbc
			 0xb5858c22, /// 0xdc0
			 0x7c068a7d, /// 0xdc4
			 0xf8432ffa, /// 0xdc8
			 0xf93ed1f7, /// 0xdcc
			 0x40822f2b, /// 0xdd0
			 0xc1591cf7, /// 0xdd4
			 0xfb0d8961, /// 0xdd8
			 0x641d2677, /// 0xddc
			 0xd3c0aa1b, /// 0xde0
			 0xd7a866a5, /// 0xde4
			 0x8e97b0d3, /// 0xde8
			 0xee2a5a52, /// 0xdec
			 0xad75b4d9, /// 0xdf0
			 0xd1212fec, /// 0xdf4
			 0x745eb814, /// 0xdf8
			 0x42498a6c, /// 0xdfc
			 0x0c7c0c16, /// 0xe00
			 0x28853c62, /// 0xe04
			 0xef9d5a7e, /// 0xe08
			 0x2940e979, /// 0xe0c
			 0x658ed84e, /// 0xe10
			 0x1331ecd9, /// 0xe14
			 0x09339cd4, /// 0xe18
			 0xdf2b4cb4, /// 0xe1c
			 0x174fe958, /// 0xe20
			 0x5be7e2fb, /// 0xe24
			 0xe2242f90, /// 0xe28
			 0x9398795c, /// 0xe2c
			 0x63608c7b, /// 0xe30
			 0x174573f0, /// 0xe34
			 0x9334e083, /// 0xe38
			 0x7bedfcca, /// 0xe3c
			 0x80412719, /// 0xe40
			 0xe042e2b6, /// 0xe44
			 0x85df902c, /// 0xe48
			 0x63231dcb, /// 0xe4c
			 0x5169c8eb, /// 0xe50
			 0xcee52efc, /// 0xe54
			 0xd1f5b56c, /// 0xe58
			 0x3be7df03, /// 0xe5c
			 0xee32e88a, /// 0xe60
			 0xbd57838a, /// 0xe64
			 0xe51003be, /// 0xe68
			 0x31772516, /// 0xe6c
			 0x40f402e6, /// 0xe70
			 0xf236ff46, /// 0xe74
			 0x65964f56, /// 0xe78
			 0x05b9c0dc, /// 0xe7c
			 0x28e0099a, /// 0xe80
			 0x5d7db963, /// 0xe84
			 0x28a5f21c, /// 0xe88
			 0x6df5906f, /// 0xe8c
			 0x977ac154, /// 0xe90
			 0x8277b837, /// 0xe94
			 0xb1811ba3, /// 0xe98
			 0x5bc1f8ce, /// 0xe9c
			 0xafa233fb, /// 0xea0
			 0x5391b0dc, /// 0xea4
			 0x6ab1f3b1, /// 0xea8
			 0xad4578e3, /// 0xeac
			 0x7f69f9ed, /// 0xeb0
			 0x90c73d6c, /// 0xeb4
			 0xf2243015, /// 0xeb8
			 0xad00e425, /// 0xebc
			 0xf88be401, /// 0xec0
			 0xee0ecd4d, /// 0xec4
			 0x976813d6, /// 0xec8
			 0xdc30096e, /// 0xecc
			 0x470b5958, /// 0xed0
			 0xeeca93b4, /// 0xed4
			 0x1ece4f4c, /// 0xed8
			 0x55d1cee6, /// 0xedc
			 0xa881e8f2, /// 0xee0
			 0x0210b928, /// 0xee4
			 0x7ab7fd18, /// 0xee8
			 0xc3a6092c, /// 0xeec
			 0x130002c6, /// 0xef0
			 0xafd9e746, /// 0xef4
			 0x8b6f2e33, /// 0xef8
			 0x95b1c482, /// 0xefc
			 0xe270ede2, /// 0xf00
			 0x2aca9194, /// 0xf04
			 0xa48efe26, /// 0xf08
			 0xdded79d9, /// 0xf0c
			 0x5d20f60a, /// 0xf10
			 0x0d6a1281, /// 0xf14
			 0x63d2d68d, /// 0xf18
			 0xb10ff85a, /// 0xf1c
			 0xd3789678, /// 0xf20
			 0x559222b9, /// 0xf24
			 0x64a47585, /// 0xf28
			 0x94ec1489, /// 0xf2c
			 0xddb446ee, /// 0xf30
			 0x38d1cd77, /// 0xf34
			 0x0b5aad37, /// 0xf38
			 0x2143ee95, /// 0xf3c
			 0xea218e9e, /// 0xf40
			 0x08ed18f2, /// 0xf44
			 0x4662aa88, /// 0xf48
			 0x9f0f3645, /// 0xf4c
			 0xe07dd53d, /// 0xf50
			 0x5d28c57a, /// 0xf54
			 0x3d034914, /// 0xf58
			 0x48266416, /// 0xf5c
			 0x0e2ffd88, /// 0xf60
			 0x553615dd, /// 0xf64
			 0x2b76a379, /// 0xf68
			 0x337b960d, /// 0xf6c
			 0xb187386f, /// 0xf70
			 0xcbeb85f8, /// 0xf74
			 0x83295378, /// 0xf78
			 0x414b1d9f, /// 0xf7c
			 0xb0982950, /// 0xf80
			 0xd629c28a, /// 0xf84
			 0x3b4dc414, /// 0xf88
			 0x6083d4fa, /// 0xf8c
			 0x1831ef85, /// 0xf90
			 0x70500d74, /// 0xf94
			 0x1d057592, /// 0xf98
			 0xf924e984, /// 0xf9c
			 0x4f1241e3, /// 0xfa0
			 0x9f26d8b5, /// 0xfa4
			 0x9c77f937, /// 0xfa8
			 0x00a3d94e, /// 0xfac
			 0xdf743f1d, /// 0xfb0
			 0x846f5785, /// 0xfb4
			 0x605310dc, /// 0xfb8
			 0x73ae84df, /// 0xfbc
			 0x09411abd, /// 0xfc0
			 0x37326118, /// 0xfc4
			 0x8b59e7b5, /// 0xfc8
			 0x7e0dc04b, /// 0xfcc
			 0x6539807c, /// 0xfd0
			 0xd63ee7e7, /// 0xfd4
			 0xa5b398e0, /// 0xfd8
			 0xf6e9d878, /// 0xfdc
			 0xfacdccfe, /// 0xfe0
			 0x415d6626, /// 0xfe4
			 0x5cd0fd83, /// 0xfe8
			 0x1f02d91e, /// 0xfec
			 0x67107331, /// 0xff0
			 0x13b9ad24, /// 0xff4
			 0xa20fec26, /// 0xff8
			 0xb33a79ac /// last
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
			 0x000005c8,
			 0x000005f4,
			 0x000005cc,
			 0x0000055c,
			 0x000006e0,
			 0x0000075c,
			 0x00000338,
			 0x000003f4,

			 /// vpu register f2
			 0x41f80000,
			 0x40a00000,
			 0x41c00000,
			 0x41000000,
			 0x41000000,
			 0x41600000,
			 0x40c00000,
			 0x41e00000,

			 /// vpu register f3
			 0x3f800000,
			 0x40a00000,
			 0x41a80000,
			 0x41100000,
			 0x41e80000,
			 0x41880000,
			 0x41300000,
			 0x41e80000,

			 /// vpu register f4
			 0x3f800000,
			 0x41f00000,
			 0x41e00000,
			 0x41d80000,
			 0x41f80000,
			 0x41b00000,
			 0x41d00000,
			 0x41e80000,

			 /// vpu register f5
			 0x41a80000,
			 0x41800000,
			 0x41a80000,
			 0x41f00000,
			 0x41980000,
			 0x40400000,
			 0x41f00000,
			 0x40e00000,

			 /// vpu register f6
			 0x41400000,
			 0x41700000,
			 0x41900000,
			 0x41000000,
			 0x41200000,
			 0x40e00000,
			 0x40e00000,
			 0x41880000,

			 /// vpu register f7
			 0x41100000,
			 0x42000000,
			 0x40a00000,
			 0x41400000,
			 0x41b80000,
			 0x40c00000,
			 0x41400000,
			 0x40000000,

			 /// vpu register f8
			 0x41f00000,
			 0x41300000,
			 0x3f800000,
			 0x41000000,
			 0x40c00000,
			 0x41a80000,
			 0x40000000,
			 0x41a80000,

			 /// vpu register f9
			 0x40800000,
			 0x41880000,
			 0x41a80000,
			 0x41b00000,
			 0x41700000,
			 0x41400000,
			 0x41200000,
			 0x41b80000,

			 /// vpu register f10
			 0x41000000,
			 0x41f00000,
			 0x41f00000,
			 0x41400000,
			 0x41d00000,
			 0x41b00000,
			 0x41d80000,
			 0x41b80000,

			 /// vpu register f11
			 0x40a00000,
			 0x41700000,
			 0x40c00000,
			 0x41700000,
			 0x41a80000,
			 0x41900000,
			 0x41f80000,
			 0x41b00000,

			 /// vpu register f12
			 0x41b80000,
			 0x42000000,
			 0x41d00000,
			 0x40c00000,
			 0x41d00000,
			 0x40000000,
			 0x41e80000,
			 0x41b00000,

			 /// vpu register f13
			 0x41d00000,
			 0x41b00000,
			 0x41f80000,
			 0x41b00000,
			 0x41880000,
			 0x41700000,
			 0x41400000,
			 0x41300000,

			 /// vpu register f14
			 0x40a00000,
			 0x41e80000,
			 0x41c00000,
			 0x41c00000,
			 0x41d00000,
			 0x41a00000,
			 0x41b00000,
			 0x41d80000,

			 /// vpu register f15
			 0x41c80000,
			 0x41d00000,
			 0x40a00000,
			 0x41600000,
			 0x41900000,
			 0x41f00000,
			 0x41400000,
			 0x41d80000,

			 /// vpu register f16
			 0x41a80000,
			 0x41200000,
			 0x40a00000,
			 0x40c00000,
			 0x41700000,
			 0x40a00000,
			 0x41e00000,
			 0x41a00000,

			 /// vpu register f17
			 0x41500000,
			 0x3f800000,
			 0x41e80000,
			 0x40400000,
			 0x41a80000,
			 0x42000000,
			 0x41600000,
			 0x41600000,

			 /// vpu register f18
			 0x41700000,
			 0x40400000,
			 0x41980000,
			 0x41b80000,
			 0x41e00000,
			 0x41e80000,
			 0x41f00000,
			 0x40a00000,

			 /// vpu register f19
			 0x41f80000,
			 0x41d00000,
			 0x41880000,
			 0x41f00000,
			 0x41f00000,
			 0x40a00000,
			 0x41c80000,
			 0x41a00000,

			 /// vpu register f20
			 0x41100000,
			 0x41800000,
			 0x40800000,
			 0x41800000,
			 0x42000000,
			 0x41880000,
			 0x41a00000,
			 0x41f80000,

			 /// vpu register f21
			 0x41b80000,
			 0x41d00000,
			 0x40800000,
			 0x41e80000,
			 0x41880000,
			 0x41e80000,
			 0x40e00000,
			 0x41100000,

			 /// vpu register f22
			 0x41100000,
			 0x40e00000,
			 0x41980000,
			 0x40000000,
			 0x3f800000,
			 0x42000000,
			 0x40c00000,
			 0x41700000,

			 /// vpu register f23
			 0x3f800000,
			 0x40800000,
			 0x40a00000,
			 0x41d00000,
			 0x40400000,
			 0x40400000,
			 0x41c80000,
			 0x40000000,

			 /// vpu register f24
			 0x40c00000,
			 0x41c00000,
			 0x41f80000,
			 0x41880000,
			 0x40400000,
			 0x40000000,
			 0x41300000,
			 0x41200000,

			 /// vpu register f25
			 0x41e80000,
			 0x41700000,
			 0x41b80000,
			 0x41200000,
			 0x3f800000,
			 0x41e80000,
			 0x41e00000,
			 0x41a00000,

			 /// vpu register f26
			 0x41880000,
			 0x41e80000,
			 0x41880000,
			 0x3f800000,
			 0x41600000,
			 0x41a00000,
			 0x41b00000,
			 0x41880000,

			 /// vpu register f27
			 0x41e00000,
			 0x41980000,
			 0x40400000,
			 0x41100000,
			 0x41e00000,
			 0x40c00000,
			 0x42000000,
			 0x41500000,

			 /// vpu register f28
			 0x40400000,
			 0x40c00000,
			 0x40a00000,
			 0x41300000,
			 0x41c00000,
			 0x41000000,
			 0x41a80000,
			 0x40400000,

			 /// vpu register f29
			 0x41800000,
			 0x40e00000,
			 0x40400000,
			 0x41e80000,
			 0x41100000,
			 0x41400000,
			 0x41a00000,
			 0x41f00000,

			 /// vpu register f30
			 0x41e00000,
			 0x41d80000,
			 0x41a00000,
			 0x41b00000,
			 0x41500000,
			 0x3f800000,
			 0x41900000,
			 0x41a00000,

			 /// vpu register f31
			 0x41a80000,
			 0x40c00000,
			 0x41000000,
			 0x40000000,
			 0x41c80000,
			 0x41c00000,
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
		"maskpopc x9, m0\n"                                   ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x5, m7\n"                                   ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x5, m5\n"                                   ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x3, m5\n"                                   ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x7, m1\n"                                   ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x4, m4\n"                                   ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x9, m0\n"                                   ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x7, m1\n"                                   ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x9, m3\n"                                   ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x7, m0\n"                                   ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x4, m2\n"                                   ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x5, m0\n"                                   ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x9, m6\n"                                   ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x7, m3\n"                                   ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x7, m2\n"                                   ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x3, m4\n"                                   ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x7, m5\n"                                   ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x5, m3\n"                                   ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x3, m2\n"                                   ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x4, m3\n"                                   ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x7, m7\n"                                   ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x4, m6\n"                                   ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x6, m0\n"                                   ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x7, m2\n"                                   ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x3, m6\n"                                   ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x7, m3\n"                                   ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x4, m6\n"                                   ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x4, m2\n"                                   ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x6, m2\n"                                   ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x6, m2\n"                                   ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x9, m5\n"                                   ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x5, m0\n"                                   ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x3, m2\n"                                   ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x4, m4\n"                                   ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x3, m3\n"                                   ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x9, m1\n"                                   ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x3, m0\n"                                   ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x7, m6\n"                                   ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x5, m2\n"                                   ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x6, m6\n"                                   ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x6, m5\n"                                   ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x9, m6\n"                                   ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x7, m3\n"                                   ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x6, m2\n"                                   ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x3, m0\n"                                   ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x5, m0\n"                                   ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x9, m2\n"                                   ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x9, m4\n"                                   ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x7, m1\n"                                   ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x7, m5\n"                                   ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x7, m6\n"                                   ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x7, m0\n"                                   ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x4, m3\n"                                   ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x3, m2\n"                                   ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x6, m2\n"                                   ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x9, m0\n"                                   ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x5, m0\n"                                   ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x7, m3\n"                                   ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x5, m7\n"                                   ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x3, m0\n"                                   ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x3, m7\n"                                   ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x6, m6\n"                                   ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x4, m5\n"                                   ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x6, m1\n"                                   ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x5, m5\n"                                   ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x6, m3\n"                                   ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x7, m5\n"                                   ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x5, m6\n"                                   ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x9, m3\n"                                   ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x3, m5\n"                                   ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x5, m6\n"                                   ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x5, m1\n"                                   ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x7, m5\n"                                   ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x7, m0\n"                                   ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x9, m6\n"                                   ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x5, m5\n"                                   ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x4, m0\n"                                   ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x9, m2\n"                                   ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x9, m3\n"                                   ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x5, m3\n"                                   ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x9, m0\n"                                   ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x7, m7\n"                                   ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x5, m1\n"                                   ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x9, m3\n"                                   ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x7, m0\n"                                   ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x5, m7\n"                                   ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x9, m0\n"                                   ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x7, m1\n"                                   ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x9, m6\n"                                   ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x5, m2\n"                                   ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x5, m1\n"                                   ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x3, m3\n"                                   ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x7, m6\n"                                   ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x7, m3\n"                                   ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x3, m1\n"                                   ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x9, m3\n"                                   ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x4, m1\n"                                   ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x9, m4\n"                                   ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskpopc x4, m3\n"                                   ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
