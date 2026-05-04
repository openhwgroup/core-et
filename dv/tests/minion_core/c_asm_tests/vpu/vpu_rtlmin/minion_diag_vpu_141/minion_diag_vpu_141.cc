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
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00000
			 0xffc00001,                                                  // -signaling NaN                              /// 00004
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00008
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0000c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00010
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00014
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0001c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00020
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00024
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00028
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0002c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00030
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00038
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0003c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00040
			 0x33333333,                                                  // 4 random values                             /// 00044
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0004c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00050
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00054
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00058
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0005c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00060
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00064
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0006c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00070
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00074
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00078
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00080
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0008c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00090
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00094
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00098
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0009c
			 0x33333333,                                                  // 4 random values                             /// 000a0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 000a4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a8
			 0x80000000,                                                  // -zero                                       /// 000ac
			 0x80011111,                                                  // -9.7958E-41                                 /// 000b0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 000b4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 000bc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000c0
			 0x55555555,                                                  // 4 random values                             /// 000c4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000cc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000d0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 000d8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 000e0
			 0x3f028f5c,                                                  // 0.51                                        /// 000e4
			 0x7f800000,                                                  // inf                                         /// 000e8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ec
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000f4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00100
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00104
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00108
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0010c
			 0x80000000,                                                  // -zero                                       /// 00110
			 0x00011111,                                                  // 9.7958E-41                                  /// 00114
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00118
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0011c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00120
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00124
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00128
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00130
			 0x0e000003,                                                  // Trailing 1s:                                /// 00134
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0013c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00144
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00148
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0014c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00150
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0015c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00160
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00164
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00168
			 0x0e000007,                                                  // Trailing 1s:                                /// 0016c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00174
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00178
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00180
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00184
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0x7fc00001,                                                  // signaling NaN                               /// 0018c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00190
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00194
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00198
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0019c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 001a0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001a4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 001a8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001b0
			 0xcb000000,                                                  // -8388608.0                                  /// 001b4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 001b8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001bc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001c0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 001c8
			 0xffc00001,                                                  // -signaling NaN                              /// 001cc
			 0x0c700000,                                                  // Leading 1s:                                 /// 001d0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001dc
			 0x80000000,                                                  // -zero                                       /// 001e0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001e4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 001e8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001ec
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 001f8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001fc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00204
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00208
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0020c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00210
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00214
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00218
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0021c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00220
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00224
			 0xcb000000,                                                  // -8388608.0                                  /// 00228
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0022c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00230
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00234
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00238
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0023c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00240
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00244
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00248
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0024c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0x00000000,                                                  // zero                                        /// 00254
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0025c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00260
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00264
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00270
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00274
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00278
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0027c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00280
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00284
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00288
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0028c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00290
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00294
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00298
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0029c
			 0x0c400000,                                                  // Leading 1s:                                 /// 002a0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 002a4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 002a8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002ac
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002b0
			 0x33333333,                                                  // 4 random values                             /// 002b4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002bc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0xff800000,                                                  // -inf                                        /// 002c4
			 0x4b000000,                                                  // 8388608.0                                   /// 002c8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002cc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 002d0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 002d4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 002d8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 002dc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002e0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002e4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002e8
			 0x0e000003,                                                  // Trailing 1s:                                /// 002ec
			 0x00000000,                                                  // zero                                        /// 002f0
			 0x55555555,                                                  // 4 random values                             /// 002f4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00300
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00304
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00308
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0030c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00310
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00318
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0031c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00320
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00324
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00328
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0032c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00334
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00338
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0033c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00340
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00344
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00348
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0034c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00350
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00354
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00358
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0035c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00360
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00364
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00368
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0036c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00370
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00374
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00378
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0037c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0038c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00390
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0039c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 003a8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0x00000000,                                                  // zero                                        /// 003b0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003b4
			 0xbf028f5c,                                                  // -0.51                                       /// 003b8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 003bc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003c0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003c4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 003c8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 003cc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 003d0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003d4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003d8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003dc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 003e0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003e8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003ec
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 003f0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003fc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00400
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00404
			 0x00011111,                                                  // 9.7958E-41                                  /// 00408
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0040c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00410
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00414
			 0x55555555,                                                  // 4 random values                             /// 00418
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0041c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00420
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00424
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00428
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0042c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00430
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00434
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00438
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0043c
			 0x7f800000,                                                  // inf                                         /// 00440
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00444
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00448
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0044c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00450
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00454
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00458
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0045c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00464
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00470
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00474
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0047c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00480
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00484
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00488
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0048c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00494
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0x00000000,                                                  // zero                                        /// 0049c
			 0x80000000,                                                  // -zero                                       /// 004a0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 004a4
			 0x0c780000,                                                  // Leading 1s:                                 /// 004a8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004b4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 004b8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004bc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004c4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c8
			 0x00000000,                                                  // zero                                        /// 004cc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 004d0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 004d4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004d8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004dc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004e4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004e8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 004ec
			 0x0c780000,                                                  // Leading 1s:                                 /// 004f0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 004f8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 004fc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00500
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00504
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00508
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00510
			 0x0e000001,                                                  // Trailing 1s:                                /// 00514
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00518
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00520
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00524
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00528
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00530
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00534
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00538
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0053c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00540
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00548
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0054c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00550
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00554
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00558
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0055c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00560
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00564
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00568
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0x7f800000,                                                  // inf                                         /// 00570
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00574
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0057c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0x0e000007,                                                  // Trailing 1s:                                /// 00584
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00588
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00590
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00594
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00598
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0059c
			 0x0e000001,                                                  // Trailing 1s:                                /// 005a0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005a8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005b4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b8
			 0x33333333,                                                  // 4 random values                             /// 005bc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 005c8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005cc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 005d0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 005d8
			 0x4b000000,                                                  // 8388608.0                                   /// 005dc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005e0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 005e4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 005e8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005ec
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 005f0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005f4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 005f8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005fc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00600
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00604
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00608
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0060c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00610
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00614
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00618
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0061c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00620
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00624
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00628
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00634
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00638
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0063c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00640
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00644
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00648
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0064c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00650
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00654
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00658
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00660
			 0x80011111,                                                  // -9.7958E-41                                 /// 00664
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00668
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0066c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00670
			 0x7f800000,                                                  // inf                                         /// 00674
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00680
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00684
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00688
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0068c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00690
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00694
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00698
			 0xff800000,                                                  // -inf                                        /// 0069c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 006a4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006a8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 006ac
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 006b0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 006b8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006bc
			 0x00011111,                                                  // 9.7958E-41                                  /// 006c0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 006c4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006cc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 006d4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006d8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 006dc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 006e0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006e8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ec
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006f0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006f8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006fc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00700
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00704
			 0x0c600000,                                                  // Leading 1s:                                 /// 00708
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0070c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00710
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00714
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00718
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0071c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00724
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00728
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0072c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00730
			 0xffc00001,                                                  // -signaling NaN                              /// 00734
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00738
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0073c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00740
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00744
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00748
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0074c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00750
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00754
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00758
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0075c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00764
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00768
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0076c
			 0x80000000,                                                  // -zero                                       /// 00770
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00774
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00778
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00780
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00784
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00788
			 0x7fc00001,                                                  // signaling NaN                               /// 0078c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00790
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00794
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00798
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0079c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007a0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007a4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007a8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007ac
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 007b0
			 0xffc00001,                                                  // -signaling NaN                              /// 007b4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007b8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007bc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007c0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 007c8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007cc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 007d0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d4
			 0x0c600000,                                                  // Leading 1s:                                 /// 007d8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 007dc
			 0x7f800000,                                                  // inf                                         /// 007e0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 007e8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007ec
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007f0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 007f4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007f8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007fc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00800
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00804
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00808
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0080c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00810
			 0xffc00001,                                                  // -signaling NaN                              /// 00814
			 0xff800000,                                                  // -inf                                        /// 00818
			 0x0e000003,                                                  // Trailing 1s:                                /// 0081c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00820
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00824
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00828
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0082c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00830
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00834
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00844
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00848
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00850
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00854
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00858
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0085c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00864
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00868
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0086c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00870
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00874
			 0xcb000000,                                                  // -8388608.0                                  /// 00878
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0087c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00880
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00884
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00888
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0x7fc00001,                                                  // signaling NaN                               /// 00894
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00898
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0089c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008a4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 008a8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ac
			 0x00000000, // 0                                             // SP +ve numbers                              /// 008b0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 008b4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 008bc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 008c0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008c4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008cc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 008d0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008d4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 008d8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 008dc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008e0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 008e4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 008e8
			 0x55555555,                                                  // 4 random values                             /// 008ec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008f4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008f8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 008fc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00900
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00908
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0090c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00910
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00918
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0091c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00924
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00928
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0092c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00930
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00934
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00938
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0093c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00940
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00944
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00948
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0094c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00950
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00954
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00958
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0095c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0x80011111,                                                  // -9.7958E-41                                 /// 00964
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00968
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0096c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00970
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00974
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00978
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0097c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00980
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00984
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00988
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0098c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00990
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00994
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00998
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0099c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 009a0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009a4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 009a8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0x0c600000,                                                  // Leading 1s:                                 /// 009b0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009b8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009bc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009c4
			 0x33333333,                                                  // 4 random values                             /// 009c8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 009cc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009d0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 009d4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009d8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 009dc
			 0x00000000,                                                  // zero                                        /// 009e0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009e4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 009ec
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009f0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009f4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a00
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a08
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a0c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a10
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a14
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a18
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a1c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a20
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a24
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a2c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a34
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a38
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a3c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a40
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a44
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a48
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a4c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a50
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a58
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a5c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a60
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a64
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a68
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a6c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a70
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a74
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a78
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a7c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a84
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a88
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a8c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a90
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a94
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a98
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a9c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00aa0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00aa4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa8
			 0xbf028f5c,                                                  // -0.51                                       /// 00aac
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ab0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ab4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ab8
			 0xff800000,                                                  // -inf                                        /// 00abc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ac4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ac8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00acc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ad0
			 0xffc00001,                                                  // -signaling NaN                              /// 00ad4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ad8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00adc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ae4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00aec
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00afc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b04
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b08
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b0c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b10
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b14
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b18
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b20
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b24
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b28
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0xff800000,                                                  // -inf                                        /// 00b30
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b34
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b38
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b3c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b40
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b44
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b4c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b50
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b54
			 0xbf028f5c,                                                  // -0.51                                       /// 00b58
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b5c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b60
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b64
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b68
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b6c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b70
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b74
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b78
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b7c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b80
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b84
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b88
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b90
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b94
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b9c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ba4
			 0x3f028f5c,                                                  // 0.51                                        /// 00ba8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00bac
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00bb0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00bb4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00bb8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00bc0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00bc4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00bc8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bcc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00bd4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00bd8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bdc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00be4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00be8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00bec
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bf0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00bf4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00bf8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bfc
			 0xff800000,                                                  // -inf                                        /// 00c00
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c04
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c08
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c0c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c10
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c14
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c18
			 0x00000000,                                                  // zero                                        /// 00c1c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c20
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c24
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c28
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c2c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c30
			 0x55555555,                                                  // 4 random values                             /// 00c34
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c38
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c3c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c44
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c48
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c50
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c54
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c58
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c5c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c64
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c6c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c70
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c74
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c78
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c7c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c80
			 0xffc00001,                                                  // -signaling NaN                              /// 00c84
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c8c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c90
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c94
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c98
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c9c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ca8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00cac
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00cb4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00cb8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00cbc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00cc0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00cc8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ccc
			 0xff800000,                                                  // -inf                                        /// 00cd0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00cd4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ce0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ce4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ce8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00cec
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00cf0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00cf4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cf8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0xbf028f5c,                                                  // -0.51                                       /// 00d00
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d04
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d08
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d0c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d10
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d18
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d20
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d24
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d28
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d2c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d30
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d34
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d38
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d3c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d40
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d44
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d48
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d4c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d50
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d58
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d5c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d60
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d64
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d68
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d6c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d70
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d7c
			 0xff800000,                                                  // -inf                                        /// 00d80
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d88
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d8c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d90
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d94
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d98
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d9c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00da0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00da4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00da8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00dac
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00db0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00db4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00db8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00dc0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00dc4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00dc8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00dcc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00dd0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00dd4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00dd8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ddc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00de0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00de4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00de8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00dec
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00dfc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e00
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e04
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e08
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e0c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e10
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e14
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e18
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e1c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e20
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e28
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e2c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e30
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e34
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e38
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e3c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e40
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e44
			 0x4b000000,                                                  // 8388608.0                                   /// 00e48
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e4c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e50
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e54
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e58
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e5c
			 0xffc00001,                                                  // -signaling NaN                              /// 00e60
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e64
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e68
			 0x7fc00001,                                                  // signaling NaN                               /// 00e6c
			 0x7f800000,                                                  // inf                                         /// 00e70
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e78
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e7c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e84
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e88
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e8c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e94
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e98
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e9c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ea0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea4
			 0x55555555,                                                  // 4 random values                             /// 00ea8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00eb0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00eb8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ebc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ec0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ec4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ec8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ecc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ed4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ed8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00edc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ee4
			 0x7fc00001,                                                  // signaling NaN                               /// 00ee8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00eec
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ef0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ef4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00efc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f00
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f04
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f08
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f10
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f14
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f18
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f1c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f24
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f28
			 0x7f800000,                                                  // inf                                         /// 00f2c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f30
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0xcb000000,                                                  // -8388608.0                                  /// 00f38
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f3c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f40
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f48
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f4c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f54
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f58
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f5c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f60
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f64
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f68
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f6c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f74
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f78
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f7c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f80
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f84
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f88
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f8c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f90
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f94
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f98
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f9c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00fa0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fa8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fac
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00fb0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fb4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00fb8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00fbc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00fc0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00fcc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00fd0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00fd4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00fd8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00fdc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00fe0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00fe4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fe8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fec
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ff4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ff8
			 0x00020000                                                  // SP - 1 bit alone set in mantissa +ve        /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00000
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00004
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00008
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0000c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00010
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00014
			 0x55555555,                                                  // 4 random values                             /// 00018
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0001c
			 0x7fc00001,                                                  // signaling NaN                               /// 00020
			 0x00000000,                                                  // zero                                        /// 00024
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00028
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0002c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00030
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00034
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00038
			 0x0e000001,                                                  // Trailing 1s:                                /// 0003c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00040
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00044
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00048
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0004c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00054
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00058
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0005c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00060
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00064
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00068
			 0xcb000000,                                                  // -8388608.0                                  /// 0006c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00070
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00074
			 0x0e000007,                                                  // Trailing 1s:                                /// 00078
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0007c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00080
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00084
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00088
			 0x0c400000,                                                  // Leading 1s:                                 /// 0008c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00090
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00094
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0009c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 000a0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 000a4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000a8
			 0x00011111,                                                  // 9.7958E-41                                  /// 000ac
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 000b8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 000c0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 000c4
			 0x0c780000,                                                  // Leading 1s:                                 /// 000c8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000cc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000d0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 000d4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000d8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000e0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000e4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000e8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 000f4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000f8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000fc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00104
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00108
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0010c
			 0xbf028f5c,                                                  // -0.51                                       /// 00110
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00114
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0011c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00120
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00124
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00128
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0012c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00130
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00134
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00138
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0013c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00140
			 0x3f028f5c,                                                  // 0.51                                        /// 00144
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00148
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0014c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00150
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00154
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00158
			 0xcb000000,                                                  // -8388608.0                                  /// 0015c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00160
			 0x3f028f5c,                                                  // 0.51                                        /// 00164
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00168
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0016c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00174
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0017c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00184
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00188
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0xcb000000,                                                  // -8388608.0                                  /// 00190
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00194
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00198
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0019c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a0
			 0x3f028f5c,                                                  // 0.51                                        /// 001a4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001a8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 001ac
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001b0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 001b4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 001b8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001bc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001c0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 001c8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001cc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 001d0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001d4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 001d8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 001dc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 001e0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001e4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 001ec
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 001f0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001fc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00200
			 0xbf028f5c,                                                  // -0.51                                       /// 00204
			 0x0e000003,                                                  // Trailing 1s:                                /// 00208
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0020c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00210
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00214
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00218
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0021c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00220
			 0x0c700000,                                                  // Leading 1s:                                 /// 00224
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00228
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0022c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00230
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00234
			 0x0c780000,                                                  // Leading 1s:                                 /// 00238
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0023c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00240
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00244
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00248
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0024c
			 0x7fc00001,                                                  // signaling NaN                               /// 00250
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00254
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00258
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00260
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00264
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00268
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0026c
			 0x3f028f5c,                                                  // 0.51                                        /// 00270
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00274
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00278
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00280
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00284
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00288
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x00000000,                                                  // zero                                        /// 00294
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00298
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0029c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 002a0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 002a8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002ac
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 002b0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 002b4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002bc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002c0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002c4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002c8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 002cc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 002d0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002d4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002d8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002dc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002e0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002e4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 002ec
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002f0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002f4
			 0x0e000007,                                                  // Trailing 1s:                                /// 002f8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 002fc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00300
			 0x55555555,                                                  // 4 random values                             /// 00304
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00308
			 0x0c400000,                                                  // Leading 1s:                                 /// 0030c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00310
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00318
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0031c
			 0xcb000000,                                                  // -8388608.0                                  /// 00320
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00324
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00328
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00334
			 0x7f800000,                                                  // inf                                         /// 00338
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0033c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00344
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00348
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0034c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00350
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00354
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00358
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0035c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00360
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00364
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00368
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0036c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00370
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0037c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00380
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00384
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00388
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0038c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00390
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00394
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00398
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003a0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 003a4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003a8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003b4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b8
			 0x0c600000,                                                  // Leading 1s:                                 /// 003bc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003c0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003c4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003cc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003d0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 003d8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 003dc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003e4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 003e8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ec
			 0x0e000007,                                                  // Trailing 1s:                                /// 003f0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003f4
			 0x80000000,                                                  // -zero                                       /// 003f8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00400
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00404
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00408
			 0xcb000000,                                                  // -8388608.0                                  /// 0040c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00414
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00418
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0041c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00420
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00424
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00428
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0042c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00430
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00434
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00438
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0043c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00440
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00444
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00448
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0044c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00450
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00454
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00458
			 0x80000000,                                                  // -zero                                       /// 0045c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00460
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00464
			 0xffc00001,                                                  // -signaling NaN                              /// 00468
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0046c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00470
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00474
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00478
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0047c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00480
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00484
			 0x00011111,                                                  // 9.7958E-41                                  /// 00488
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0048c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00490
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00494
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00498
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0049c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 004a0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 004a8
			 0x7f800000,                                                  // inf                                         /// 004ac
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004b0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 004b4
			 0x4b000000,                                                  // 8388608.0                                   /// 004b8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 004bc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004c0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004c8
			 0xffc00001,                                                  // -signaling NaN                              /// 004cc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 004d0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004d4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004dc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004e0
			 0x3f028f5c,                                                  // 0.51                                        /// 004e4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004e8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ec
			 0x0e000003,                                                  // Trailing 1s:                                /// 004f0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004f4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004f8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004fc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00504
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00508
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0050c
			 0xffc00001,                                                  // -signaling NaN                              /// 00510
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00518
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0051c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00520
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00524
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00528
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0052c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00530
			 0x0c700000,                                                  // Leading 1s:                                 /// 00534
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00538
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0053c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00540
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00544
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00548
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0054c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00550
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00554
			 0xcb000000,                                                  // -8388608.0                                  /// 00558
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0055c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00560
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00564
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00568
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0056c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00570
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00574
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00578
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0057c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00580
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00584
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00588
			 0x00011111,                                                  // 9.7958E-41                                  /// 0058c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00590
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00594
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00598
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0059c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 005a0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 005a4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005a8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 005ac
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005b0
			 0x7f800000,                                                  // inf                                         /// 005b4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005b8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005bc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005c4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0xbf028f5c,                                                  // -0.51                                       /// 005cc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005d0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005d4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 005d8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005dc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005e0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005e4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 005ec
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 005f0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 005f4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005f8
			 0x3f028f5c,                                                  // 0.51                                        /// 005fc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00600
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00604
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00608
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0060c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00610
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00614
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00618
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0061c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00620
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00624
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00628
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0062c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00630
			 0xff800000,                                                  // -inf                                        /// 00634
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0063c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00640
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00644
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00648
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0064c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00650
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00654
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00658
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0065c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00660
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00664
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00668
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0066c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00674
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00678
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0067c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00680
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00684
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00688
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0068c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00690
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00694
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00698
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0069c
			 0x4b000000,                                                  // 8388608.0                                   /// 006a0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 006a4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006a8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006b0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006b4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 006b8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 006bc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006c0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006c4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 006c8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006cc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006d8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 006dc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006e0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006e4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006e8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ec
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 006f0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006f4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006fc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00704
			 0xbf028f5c,                                                  // -0.51                                       /// 00708
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0070c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00710
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0071c
			 0x00000000,                                                  // zero                                        /// 00720
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00728
			 0x4b000000,                                                  // 8388608.0                                   /// 0072c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00730
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00734
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0073c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00740
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00744
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00748
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0074c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00750
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00754
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00758
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0075c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00760
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00764
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00768
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0076c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00770
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00774
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00778
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00788
			 0x0c780000,                                                  // Leading 1s:                                 /// 0078c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0x0e000007,                                                  // Trailing 1s:                                /// 00794
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00798
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0079c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 007a8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ac
			 0x0c700000,                                                  // Leading 1s:                                 /// 007b0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007b4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 007b8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007bc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 007c0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007c4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007c8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 007cc
			 0x80011111,                                                  // -9.7958E-41                                 /// 007d0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007d4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007d8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007dc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 007e4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007e8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007f0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007f4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007fc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00800
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00804
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00808
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0080c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00810
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00814
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00818
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0081c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00820
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00828
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0082c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00830
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00834
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00838
			 0xcb000000,                                                  // -8388608.0                                  /// 0083c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00840
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00844
			 0x7f800000,                                                  // inf                                         /// 00848
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00850
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00854
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00858
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0085c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00864
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00868
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0086c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00870
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00874
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00878
			 0x00011111,                                                  // 9.7958E-41                                  /// 0087c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00884
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0088c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00890
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00894
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00898
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008a4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008ac
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008b0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 008b4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008bc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 008c8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008cc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 008d0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008d4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008d8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008e0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008e8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008f4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 008f8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 008fc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00900
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00904
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00908
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0090c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00910
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00914
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0x0e000003,                                                  // Trailing 1s:                                /// 0091c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00920
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00924
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00928
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0092c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00930
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00934
			 0x33333333,                                                  // 4 random values                             /// 00938
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0093c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00940
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00944
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00948
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0094c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00954
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00958
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0095c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00960
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00964
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00968
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00970
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00974
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00978
			 0x3f028f5c,                                                  // 0.51                                        /// 0097c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00980
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00988
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0098c
			 0x55555555,                                                  // 4 random values                             /// 00990
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00994
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00998
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0099c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009a0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009a8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 009ac
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009b0
			 0x3f028f5c,                                                  // 0.51                                        /// 009b4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 009b8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 009bc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009c4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009c8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009cc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 009d0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009d4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 009d8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 009dc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009e0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009e8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009ec
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009f0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009fc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a00
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a04
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a08
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a0c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a10
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a14
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a18
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a1c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a20
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a24
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0xff800000,                                                  // -inf                                        /// 00a2c
			 0x7f800000,                                                  // inf                                         /// 00a30
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a34
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a38
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a3c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a40
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a44
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a48
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a4c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a50
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a54
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a58
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a5c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a60
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a64
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a68
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a6c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a70
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a74
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a78
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a7c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a80
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x33333333,                                                  // 4 random values                             /// 00a88
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a90
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a94
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a98
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a9c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00aa0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00aac
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ab0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ab8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00abc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ac0
			 0xff800000,                                                  // -inf                                        /// 00ac4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ac8
			 0x7f800000,                                                  // inf                                         /// 00acc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ad0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ad4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00adc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ae0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ae4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ae8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00aec
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00af0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00af4
			 0xbf028f5c,                                                  // -0.51                                       /// 00af8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00afc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b00
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b04
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b08
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b0c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b10
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b14
			 0x00000000,                                                  // zero                                        /// 00b18
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b1c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b20
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b24
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b28
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b2c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b30
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b34
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b38
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b3c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b40
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b44
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b48
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b4c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b50
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b54
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b58
			 0x80000000,                                                  // -zero                                       /// 00b5c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b60
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b64
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b68
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b6c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b70
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b74
			 0x00000000,                                                  // zero                                        /// 00b78
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b7c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b80
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b84
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b8c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b94
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b9c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ba8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00bac
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00bb4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00bb8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bbc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00bc8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bcc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00bd0
			 0xcb000000,                                                  // -8388608.0                                  /// 00bd4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00bd8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00bdc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00be0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00be8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bec
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00bf0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00bf8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00bfc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c00
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c04
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c08
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c10
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c14
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c18
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c1c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c20
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c24
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c28
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c2c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c30
			 0x7f800000,                                                  // inf                                         /// 00c34
			 0x7fc00001,                                                  // signaling NaN                               /// 00c38
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c3c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c48
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c4c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c50
			 0x3f028f5c,                                                  // 0.51                                        /// 00c54
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c58
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c5c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c60
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c64
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c6c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c70
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c74
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c78
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c7c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c80
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c84
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c88
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c8c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c90
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c94
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c98
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c9c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ca0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ca4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cac
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00cb0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cb8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cbc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cc0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00cc4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00cc8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ccc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cd0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00cd4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cdc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ce0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ce4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ce8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00cec
			 0x80000000,                                                  // -zero                                       /// 00cf0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00cf4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00cfc
			 0xbf028f5c,                                                  // -0.51                                       /// 00d00
			 0xff800000,                                                  // -inf                                        /// 00d04
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d08
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d0c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d10
			 0x7fc00001,                                                  // signaling NaN                               /// 00d14
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d18
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d1c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d20
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0xbf028f5c,                                                  // -0.51                                       /// 00d28
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d2c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d30
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d34
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d38
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d40
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d44
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d48
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d4c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d54
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d58
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d5c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d60
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d64
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d68
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d70
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0xffc00001,                                                  // -signaling NaN                              /// 00d78
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d7c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d80
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d84
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d88
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d8c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d90
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d94
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d98
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d9c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00da0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00da8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00dac
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00db8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00dbc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00dc8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00dcc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00dd0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ddc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00de0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00de4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00de8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00dec
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00df0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00df4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00df8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00dfc
			 0x7fc00001,                                                  // signaling NaN                               /// 00e00
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e04
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e08
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e0c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e10
			 0xff800000,                                                  // -inf                                        /// 00e14
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e18
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e1c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e28
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e2c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e30
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e34
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e38
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e40
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e44
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e48
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e4c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e50
			 0x33333333,                                                  // 4 random values                             /// 00e54
			 0x33333333,                                                  // 4 random values                             /// 00e58
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e5c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e60
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e64
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e68
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e6c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e70
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e74
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e78
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e7c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e80
			 0xcb000000,                                                  // -8388608.0                                  /// 00e84
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e88
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e8c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e90
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e94
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e98
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e9c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ea0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ea4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00eac
			 0x0e000001,                                                  // Trailing 1s:                                /// 00eb0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00eb8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ebc
			 0xffc00001,                                                  // -signaling NaN                              /// 00ec0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ec4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ec8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ecc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ed0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ed4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ed8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00edc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ee0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00eec
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef0
			 0x7fc00001,                                                  // signaling NaN                               /// 00ef4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00efc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f00
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f08
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f10
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f14
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f1c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f20
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f2c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f30
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f34
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f38
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f44
			 0x7fc00001,                                                  // signaling NaN                               /// 00f48
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f4c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f50
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f54
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f58
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f5c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f60
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f64
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f68
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f6c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f70
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f78
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f7c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f80
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f84
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f88
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f8c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f90
			 0x33333333,                                                  // 4 random values                             /// 00f94
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f98
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00fa0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fa8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00fac
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00fb0
			 0xcb000000,                                                  // -8388608.0                                  /// 00fb4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fb8
			 0x7fc00001,                                                  // signaling NaN                               /// 00fbc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00fc0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00fc4
			 0xff800000,                                                  // -inf                                        /// 00fc8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fcc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00fd0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00fd4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fd8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00fdc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00fe0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fe4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fec
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ff0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ff4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ff8
			 0x80000001                                                  // -1.4E-45 (-denorm)                          /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00004
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00008
			 0x4b000000,                                                  // 8388608.0                                   /// 0000c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00010
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00018
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0001c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00020
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00024
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00028
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0002c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00030
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00034
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00038
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0003c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00040
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00044
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00048
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0004c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00050
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00054
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00058
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0005c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00060
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00068
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0006c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00070
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00078
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0007c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00080
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00088
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0008c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00090
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0009c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000a0
			 0xffc00001,                                                  // -signaling NaN                              /// 000a4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000a8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000ac
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0xff800000,                                                  // -inf                                        /// 000b4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 000b8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000bc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000c4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 000c8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 000cc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 000d0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000d4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 000d8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 000e0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000e8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000ec
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000f0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f4
			 0x0c780000,                                                  // Leading 1s:                                 /// 000f8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000fc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00100
			 0xcb000000,                                                  // -8388608.0                                  /// 00104
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0010c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00110
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00114
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00118
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0011c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00120
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00124
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00128
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0012c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00130
			 0xffc00001,                                                  // -signaling NaN                              /// 00134
			 0x00011111,                                                  // 9.7958E-41                                  /// 00138
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0013c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00144
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00148
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0014c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00150
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00154
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00158
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00160
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00164
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00168
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0016c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00170
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00174
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00178
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0017c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00180
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00184
			 0x7f800000,                                                  // inf                                         /// 00188
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00190
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00194
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00198
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0019c
			 0x0e000003,                                                  // Trailing 1s:                                /// 001a0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0x3f028f5c,                                                  // 0.51                                        /// 001a8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0x0c700000,                                                  // Leading 1s:                                 /// 001b0
			 0x0c700000,                                                  // Leading 1s:                                 /// 001b4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001b8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 001bc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001c0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001c4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 001c8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 001cc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 001d0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001d4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 001d8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001dc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001e0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001e4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001ec
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001f0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 001f4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 001f8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001fc
			 0x00000000,                                                  // zero                                        /// 00200
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00204
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0020c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00210
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00214
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00218
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0021c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00220
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00224
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00228
			 0x80011111,                                                  // -9.7958E-41                                 /// 0022c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00230
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00234
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00238
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0023c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00240
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00244
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00248
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00250
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00254
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00258
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00260
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00264
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00268
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0026c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00270
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00274
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00278
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0027c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00280
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00284
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00288
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0028c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00290
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00294
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00298
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0029c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002a0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 002a4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002a8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 002b0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002b4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002b8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002bc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002c0
			 0x00011111,                                                  // 9.7958E-41                                  /// 002c4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 002c8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 002cc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002d4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002d8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002dc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002e0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002e4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002e8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002ec
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002f0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002f8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002fc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00300
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00304
			 0x0c600000,                                                  // Leading 1s:                                 /// 00308
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0030c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00310
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00314
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00318
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0031c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00320
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00324
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00328
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00330
			 0x55555555,                                                  // 4 random values                             /// 00334
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00340
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00344
			 0xff800000,                                                  // -inf                                        /// 00348
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00350
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00354
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00358
			 0xcb000000,                                                  // -8388608.0                                  /// 0035c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00360
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00368
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00370
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00374
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00378
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0037c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00380
			 0xffc00001,                                                  // -signaling NaN                              /// 00384
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00388
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0038c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00390
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00394
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00398
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0039c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003a0
			 0x0e000001,                                                  // Trailing 1s:                                /// 003a4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 003ac
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003b0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003b4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003b8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 003bc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 003c0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 003c4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003c8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003cc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 003d0
			 0x00000000,                                                  // zero                                        /// 003d4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003d8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003dc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e4
			 0x0c700000,                                                  // Leading 1s:                                 /// 003e8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003ec
			 0x00011111,                                                  // 9.7958E-41                                  /// 003f0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 003fc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00400
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00404
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00408
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00410
			 0xbf028f5c,                                                  // -0.51                                       /// 00414
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00418
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0041c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00420
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00424
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00428
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0042c
			 0xbf028f5c,                                                  // -0.51                                       /// 00430
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0043c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00440
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00444
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0044c
			 0xbf028f5c,                                                  // -0.51                                       /// 00450
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00454
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00458
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0045c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00460
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00464
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00468
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0046c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00470
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00474
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0047c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00480
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00484
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00488
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0048c
			 0xcb000000,                                                  // -8388608.0                                  /// 00490
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00494
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00498
			 0x4b000000,                                                  // 8388608.0                                   /// 0049c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004a4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004a8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004ac
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 004b4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 004b8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 004bc
			 0xff800000,                                                  // -inf                                        /// 004c0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 004c4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 004c8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 004d0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 004d4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004d8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 004dc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 004e0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 004e4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 004e8
			 0x0e000007,                                                  // Trailing 1s:                                /// 004ec
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 004f4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 004fc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00500
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00504
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00508
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0050c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00510
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00514
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00518
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0051c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00520
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00524
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00528
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0052c
			 0x4b000000,                                                  // 8388608.0                                   /// 00530
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00538
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0053c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00540
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00548
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0054c
			 0x80000000,                                                  // -zero                                       /// 00550
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00554
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00558
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0055c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00560
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00564
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00568
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0056c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00570
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00574
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00578
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0057c
			 0x7f800000,                                                  // inf                                         /// 00580
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00584
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00588
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0058c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00590
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00594
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00598
			 0x0c780000,                                                  // Leading 1s:                                 /// 0059c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 005a8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 005b8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005bc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 005c0
			 0xffc00001,                                                  // -signaling NaN                              /// 005c4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005cc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005d0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 005d8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 005e0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005e4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005e8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005ec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f0
			 0x0e000007,                                                  // Trailing 1s:                                /// 005f4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005fc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00608
			 0xff800000,                                                  // -inf                                        /// 0060c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00610
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00614
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00618
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0061c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00620
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00624
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00628
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0062c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00630
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00638
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0063c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00644
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00648
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0064c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00650
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00654
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00658
			 0x0c780000,                                                  // Leading 1s:                                 /// 0065c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00660
			 0x7f800000,                                                  // inf                                         /// 00664
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00668
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0066c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00670
			 0x33333333,                                                  // 4 random values                             /// 00674
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0067c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00680
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00684
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0x0e000003,                                                  // Trailing 1s:                                /// 0068c
			 0x33333333,                                                  // 4 random values                             /// 00690
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00694
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00698
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 006a0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 006a4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0x0e000001,                                                  // Trailing 1s:                                /// 006b0
			 0x3f028f5c,                                                  // 0.51                                        /// 006b4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b8
			 0x0c780000,                                                  // Leading 1s:                                 /// 006bc
			 0x55555555,                                                  // 4 random values                             /// 006c0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006c4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006c8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 006cc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006d8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006dc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006e0
			 0x4b000000,                                                  // 8388608.0                                   /// 006e4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 006e8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006ec
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006f0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006f4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 006f8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006fc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00704
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00708
			 0xcb000000,                                                  // -8388608.0                                  /// 0070c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00710
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00714
			 0x55555555,                                                  // 4 random values                             /// 00718
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0071c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00720
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00728
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0072c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00734
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00738
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0073c
			 0x7fc00001,                                                  // signaling NaN                               /// 00740
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0x0c780000,                                                  // Leading 1s:                                 /// 00748
			 0x55555555,                                                  // 4 random values                             /// 0074c
			 0xff800000,                                                  // -inf                                        /// 00750
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00758
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0075c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00760
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00764
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00768
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0076c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00770
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00774
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00778
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0077c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00780
			 0x00000000,                                                  // zero                                        /// 00784
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00788
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0078c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00790
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00794
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00798
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0079c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007a0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 007a4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 007a8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007ac
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007b0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007b4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007b8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007cc
			 0x00000000,                                                  // zero                                        /// 007d0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 007d8
			 0xcb000000,                                                  // -8388608.0                                  /// 007dc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007e4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 007e8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ec
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007f4
			 0x80000000,                                                  // -zero                                       /// 007f8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 007fc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00800
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00804
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00808
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0080c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00810
			 0x3f028f5c,                                                  // 0.51                                        /// 00814
			 0x80000000,                                                  // -zero                                       /// 00818
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0081c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00820
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00824
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00828
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0082c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00834
			 0x00011111,                                                  // 9.7958E-41                                  /// 00838
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x00000000,                                                  // zero                                        /// 00840
			 0x0c780000,                                                  // Leading 1s:                                 /// 00844
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00848
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0084c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00850
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00858
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0085c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00864
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0086c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00870
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00874
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00878
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0087c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00880
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00884
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00888
			 0x80000000,                                                  // -zero                                       /// 0088c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00890
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00894
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00898
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0089c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008a0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008a4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 008a8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008ac
			 0x0e000001,                                                  // Trailing 1s:                                /// 008b0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008b4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008b8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 008bc
			 0x7fc00001,                                                  // signaling NaN                               /// 008c0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008c4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008c8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 008cc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008d0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008d4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008d8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008dc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 008e4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008e8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008ec
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 008f4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008f8
			 0x80011111,                                                  // -9.7958E-41                                 /// 008fc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00900
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00904
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00908
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0xff800000,                                                  // -inf                                        /// 00918
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0091c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00920
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00924
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00928
			 0x7f800000,                                                  // inf                                         /// 0092c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00934
			 0xffc00001,                                                  // -signaling NaN                              /// 00938
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0093c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00940
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00944
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00948
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0094c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00954
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00958
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0095c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00960
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00964
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00968
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0096c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00970
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00974
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00978
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0097c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00980
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00984
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00988
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0098c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00990
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00994
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00998
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0099c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009a0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 009a8
			 0x55555555,                                                  // 4 random values                             /// 009ac
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 009b4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009b8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009bc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 009c0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009c4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009c8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009cc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 009d0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009d8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009dc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 009e0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 009e4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 009ec
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009f0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009f8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009fc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a00
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a04
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a08
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a0c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a10
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a18
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a1c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a20
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a24
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a28
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a2c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a34
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a38
			 0x7fc00001,                                                  // signaling NaN                               /// 00a3c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a40
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a44
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a48
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a4c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a50
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a54
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a58
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a5c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a60
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a6c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a70
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a74
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a78
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a7c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a80
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a84
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a88
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a94
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a98
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a9c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00aa0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00aa8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00aac
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ab4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00abc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ac4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ac8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00acc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ad0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ad4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ad8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00adc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ae0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ae4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ae8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00aec
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00af0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00afc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b00
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b04
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b08
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b0c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b10
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b14
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b18
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b1c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b20
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b24
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b28
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b30
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b34
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b38
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b40
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b44
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b48
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b4c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b50
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b54
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b58
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b5c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b60
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b64
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b68
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b6c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b70
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b74
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b78
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b7c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b80
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b88
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x3f028f5c,                                                  // 0.51                                        /// 00b90
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b94
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b98
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b9c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ba0
			 0x7fc00001,                                                  // signaling NaN                               /// 00ba4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ba8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00bac
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00bb0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00bb4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00bb8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00bc0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00bc4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00bc8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bcc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00bd0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00bd4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00bd8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bdc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00be4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00be8
			 0x7fc00001,                                                  // signaling NaN                               /// 00bec
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00bf0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00bf4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00bf8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c00
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c04
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c08
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c10
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c18
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c1c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c20
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c24
			 0x80000000,                                                  // -zero                                       /// 00c28
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c2c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c30
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c34
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c38
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c3c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c40
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c44
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c48
			 0x80000000,                                                  // -zero                                       /// 00c4c
			 0xcb000000,                                                  // -8388608.0                                  /// 00c50
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c54
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c58
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c5c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c60
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c64
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c68
			 0x7fc00001,                                                  // signaling NaN                               /// 00c6c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c70
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c74
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c78
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c7c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c80
			 0x55555555,                                                  // 4 random values                             /// 00c84
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c88
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c8c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c90
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c94
			 0x80000000,                                                  // -zero                                       /// 00c98
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c9c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ca0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ca4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cac
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cb0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00cb4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00cb8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00cbc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cc0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cc4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ccc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cd0
			 0x3f028f5c,                                                  // 0.51                                        /// 00cd4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00cd8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00cdc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00cec
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00cf0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00cf4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cf8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00cfc
			 0x7f800000,                                                  // inf                                         /// 00d00
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d04
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d08
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d0c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d14
			 0x55555555,                                                  // 4 random values                             /// 00d18
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d1c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d24
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d28
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d2c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d30
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d34
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d38
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0xff800000,                                                  // -inf                                        /// 00d40
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d44
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d48
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d4c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d50
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d54
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d58
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d5c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x7f800000,                                                  // inf                                         /// 00d64
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d68
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d70
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d74
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d78
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d7c
			 0xffc00001,                                                  // -signaling NaN                              /// 00d80
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d84
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d94
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d98
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d9c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00da0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00da4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00dac
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00db0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00db4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00dbc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00dc0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00dc4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00dc8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00dcc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00dd0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00dd4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00dd8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ddc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00de0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00de4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00dec
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00dfc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e00
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e04
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e08
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e0c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e10
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e14
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e18
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e1c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e20
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e24
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e28
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e2c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e30
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e34
			 0xcb000000,                                                  // -8388608.0                                  /// 00e38
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e40
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e44
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e48
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e4c
			 0x80000000,                                                  // -zero                                       /// 00e50
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e58
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e5c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e60
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e64
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e68
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e6c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e70
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e78
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e7c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e80
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e84
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e88
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e8c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e90
			 0xcb000000,                                                  // -8388608.0                                  /// 00e94
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e98
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e9c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ea0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ea4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00eac
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00eb0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00eb4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ebc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ec0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ec4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ec8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ecc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ed4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ed8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00edc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ee0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ee8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00eec
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ef8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00efc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f00
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f04
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f08
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f0c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f10
			 0xffc00001,                                                  // -signaling NaN                              /// 00f14
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f18
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f20
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f2c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f30
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f34
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f3c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f40
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f44
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f48
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f4c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f50
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f54
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f58
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f5c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f60
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f68
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f6c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f74
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f78
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f7c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f80
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f84
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f8c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f90
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f94
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f98
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00fa4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fac
			 0x0c780000,                                                  // Leading 1s:                                 /// 00fb0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00fb4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fb8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fbc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fc4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00fc8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00fcc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00fd4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00fd8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fdc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00fe0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00fe4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fe8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00fec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ff4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ff8
			 0x0e00000f                                                  // Trailing 1s:                                /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00000
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00004
			 0x0c600000,                                                  // Leading 1s:                                 /// 00008
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0000c
			 0x7f800000,                                                  // inf                                         /// 00010
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00014
			 0x80011111,                                                  // -9.7958E-41                                 /// 00018
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0001c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00020
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00024
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x80000000,                                                  // -zero                                       /// 0002c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00030
			 0xff800000,                                                  // -inf                                        /// 00034
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00038
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0003c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00040
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00044
			 0x7fc00001,                                                  // signaling NaN                               /// 00048
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0004c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00050
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00054
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00058
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0005c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00060
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00064
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00068
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0006c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00070
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00074
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00078
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00080
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00084
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00088
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00090
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00094
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0009c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000a0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000a4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ac
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 000b0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000b4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x0c400000,                                                  // Leading 1s:                                 /// 000bc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 000c4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000c8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 000cc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000d0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 000d4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 000d8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000e0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000e4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 000e8
			 0x80011111,                                                  // -9.7958E-41                                 /// 000ec
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000f0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000f8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 000fc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00100
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00104
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00108
			 0x7fc00001,                                                  // signaling NaN                               /// 0010c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00110
			 0x80000000,                                                  // -zero                                       /// 00114
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00118
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0011c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00120
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00124
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0xffc00001,                                                  // -signaling NaN                              /// 0012c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00130
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00134
			 0x00011111,                                                  // 9.7958E-41                                  /// 00138
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0013c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00140
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00148
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0014c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00150
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00158
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0015c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00160
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00164
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00168
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0xff800000,                                                  // -inf                                        /// 00170
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00174
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00178
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0017c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00180
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00184
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0x0c400000,                                                  // Leading 1s:                                 /// 0018c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00190
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00194
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00198
			 0x3f028f5c,                                                  // 0.51                                        /// 0019c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001a0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 001a4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001a8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 001ac
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001b4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001bc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001c0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001c4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 001c8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001d0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001d4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 001d8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 001dc
			 0x0c780000,                                                  // Leading 1s:                                 /// 001e0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001e4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001e8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001ec
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 001f4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001f8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001fc
			 0x55555555,                                                  // 4 random values                             /// 00200
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00204
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00208
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00210
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00214
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00218
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0021c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00220
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00224
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00228
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0022c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00230
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00234
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00238
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0023c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00240
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00244
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00248
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0024c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00254
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00258
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00260
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00264
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00268
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0026c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00270
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00274
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0027c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00280
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00284
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00288
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00290
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00294
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00298
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0029c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002a4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002a8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002ac
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 002b0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002b4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 002b8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002bc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002c8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002cc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002d0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 002d4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002d8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002dc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 002e0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002e4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002e8
			 0x7f800000,                                                  // inf                                         /// 002ec
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002f0
			 0xff800000,                                                  // -inf                                        /// 002f4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002f8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00300
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00304
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00308
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0030c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00310
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00314
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00318
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0031c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00320
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00324
			 0xff800000,                                                  // -inf                                        /// 00328
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00330
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00334
			 0xbf028f5c,                                                  // -0.51                                       /// 00338
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0033c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00340
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00344
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0034c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00350
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0x00011111,                                                  // 9.7958E-41                                  /// 00358
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0035c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00360
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00364
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00368
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0036c
			 0x55555555,                                                  // 4 random values                             /// 00370
			 0x0e000001,                                                  // Trailing 1s:                                /// 00374
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0037c
			 0x55555555,                                                  // 4 random values                             /// 00380
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00384
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0038c
			 0x4b000000,                                                  // 8388608.0                                   /// 00390
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00394
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00398
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0039c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003a0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003a4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003a8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003ac
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 003b4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003b8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003c0
			 0x0c400000,                                                  // Leading 1s:                                 /// 003c4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003c8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 003cc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 003d4
			 0x0c700000,                                                  // Leading 1s:                                 /// 003d8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003dc
			 0xcb000000,                                                  // -8388608.0                                  /// 003e0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 003e4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 003ec
			 0x0c780000,                                                  // Leading 1s:                                 /// 003f0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 003f4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003f8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00400
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00404
			 0xff800000,                                                  // -inf                                        /// 00408
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0040c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00414
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00418
			 0x4b000000,                                                  // 8388608.0                                   /// 0041c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00420
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x55555555,                                                  // 4 random values                             /// 00428
			 0x0c400000,                                                  // Leading 1s:                                 /// 0042c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00430
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00434
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00438
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0043c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00440
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00448
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00450
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00454
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0045c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00460
			 0x00011111,                                                  // 9.7958E-41                                  /// 00464
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00468
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0046c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00470
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00474
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00484
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00488
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0048c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00490
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00494
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00498
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 004a0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004a4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 004a8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 004ac
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004b0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 004b4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004b8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004bc
			 0x80000000,                                                  // -zero                                       /// 004c0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004c4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 004c8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 004cc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 004d0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 004d4
			 0x00000000,                                                  // zero                                        /// 004d8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004dc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004e0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004e8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004ec
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 004f4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 004f8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 004fc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00500
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00504
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00508
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0050c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00510
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00520
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00524
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00530
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00534
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00538
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0053c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00540
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00544
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00548
			 0xcb000000,                                                  // -8388608.0                                  /// 0054c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00550
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00554
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00558
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0055c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00560
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00564
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00568
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00570
			 0x0c780000,                                                  // Leading 1s:                                 /// 00574
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0057c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00584
			 0x7fc00001,                                                  // signaling NaN                               /// 00588
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00590
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00598
			 0x4b000000,                                                  // 8388608.0                                   /// 0059c
			 0x0e000001,                                                  // Trailing 1s:                                /// 005a0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 005a4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005a8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 005ac
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b0
			 0x4b000000,                                                  // 8388608.0                                   /// 005b4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005b8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005bc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005c0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 005c4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005cc
			 0x33333333,                                                  // 4 random values                             /// 005d0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 005d4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005d8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005dc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005e4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005ec
			 0x00000000,                                                  // zero                                        /// 005f0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005f4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0xcb000000,                                                  // -8388608.0                                  /// 005fc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00608
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0060c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00610
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00614
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00618
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0061c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00620
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00624
			 0x55555555,                                                  // 4 random values                             /// 00628
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0062c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00630
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00634
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00638
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0063c
			 0xff800000,                                                  // -inf                                        /// 00640
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00644
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00648
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0064c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00650
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00654
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00658
			 0x0c700000,                                                  // Leading 1s:                                 /// 0065c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00660
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00664
			 0x0e000003,                                                  // Trailing 1s:                                /// 00668
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0066c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00670
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00678
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0067c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00680
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00684
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00688
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00690
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00694
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00698
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0069c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 006a0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006a4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 006a8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 006ac
			 0x0e00000f,                                                  // Trailing 1s:                                /// 006b0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 006b4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006b8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006bc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006c0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006c4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006c8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 006cc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006d0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006d4
			 0x80011111,                                                  // -9.7958E-41                                 /// 006d8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 006dc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006e4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006e8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ec
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006f8
			 0x00011111,                                                  // 9.7958E-41                                  /// 006fc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00700
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00704
			 0xbf028f5c,                                                  // -0.51                                       /// 00708
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0070c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00714
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00718
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0071c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00720
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00724
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00728
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0072c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00730
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00734
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0073c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00748
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0074c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00750
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00754
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00758
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0075c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00760
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00764
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00768
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0076c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00770
			 0x0e000001,                                                  // Trailing 1s:                                /// 00774
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00778
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0077c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00780
			 0x7f800000,                                                  // inf                                         /// 00784
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0078c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00794
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00798
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0079c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 007a4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007a8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007ac
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 007b0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007b4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 007b8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x00000000,                                                  // zero                                        /// 007c0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 007c4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 007c8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007cc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007d0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007d8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 007dc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007e4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007e8
			 0x0c600000,                                                  // Leading 1s:                                 /// 007ec
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 007f0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 007f4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 007f8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00800
			 0x00000000,                                                  // zero                                        /// 00804
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00808
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0080c
			 0x7f800000,                                                  // inf                                         /// 00810
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00814
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00818
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0081c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00820
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00824
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00828
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0082c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00834
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00838
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0083c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00840
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00844
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00848
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00850
			 0x7f800000,                                                  // inf                                         /// 00854
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0085c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00860
			 0x80000000,                                                  // -zero                                       /// 00864
			 0xffc00001,                                                  // -signaling NaN                              /// 00868
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00870
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00874
			 0x7f800000,                                                  // inf                                         /// 00878
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0087c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00880
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00884
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0088c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00890
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00894
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00898
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0089c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008a4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008a8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 008ac
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 008b4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 008b8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008c0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008c4
			 0x00011111,                                                  // 9.7958E-41                                  /// 008c8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008cc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008d4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008d8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008e0
			 0x0e000007,                                                  // Trailing 1s:                                /// 008e4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008e8
			 0x7f800000,                                                  // inf                                         /// 008ec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008f0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008f4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008fc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00900
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00904
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00908
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00910
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00914
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00918
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0091c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00924
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00928
			 0x4b000000,                                                  // 8388608.0                                   /// 0092c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00930
			 0x0c600000,                                                  // Leading 1s:                                 /// 00934
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00938
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0093c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00940
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00944
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00948
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0094c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00950
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00954
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00958
			 0x00000000,                                                  // zero                                        /// 0095c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00960
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00964
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00968
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0096c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00970
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00974
			 0xbf028f5c,                                                  // -0.51                                       /// 00978
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0097c
			 0x4b000000,                                                  // 8388608.0                                   /// 00980
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00984
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00988
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0098c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00990
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00994
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00998
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0099c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009a0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009a8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009ac
			 0x80000000,                                                  // -zero                                       /// 009b0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009b4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b8
			 0x00000000,                                                  // zero                                        /// 009bc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009c4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009c8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009cc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009d0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009d4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009d8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009dc
			 0xcb000000,                                                  // -8388608.0                                  /// 009e0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009e4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 009e8
			 0x80011111,                                                  // -9.7958E-41                                 /// 009ec
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009f0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009f4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009f8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009fc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a00
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a04
			 0x00000000,                                                  // zero                                        /// 00a08
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a0c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a10
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a18
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a1c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a24
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a28
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a2c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a30
			 0xcb000000,                                                  // -8388608.0                                  /// 00a34
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a38
			 0x4b000000,                                                  // 8388608.0                                   /// 00a3c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a40
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a44
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a48
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a4c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a50
			 0xff800000,                                                  // -inf                                        /// 00a54
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a5c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a60
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a64
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a68
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a6c
			 0x7fc00001,                                                  // signaling NaN                               /// 00a70
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a74
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a78
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a80
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a84
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a88
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a8c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a90
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a94
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a98
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a9c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00aa0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00aa8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00aac
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ab0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ab8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00abc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ac0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ac4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ac8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ad0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ad8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00adc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ae0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ae4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ae8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00aec
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00af4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00af8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00afc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b00
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b04
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b08
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b0c
			 0xff800000,                                                  // -inf                                        /// 00b10
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b14
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b18
			 0x80000000,                                                  // -zero                                       /// 00b1c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b20
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b24
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b28
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b2c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b30
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b34
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b38
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b3c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b40
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b44
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b48
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b4c
			 0x55555555,                                                  // 4 random values                             /// 00b50
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b54
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b58
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b5c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b60
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b64
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b68
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b6c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b70
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b74
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b7c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b84
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b88
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b8c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b90
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b94
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b9c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ba0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ba4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ba8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bac
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00bb0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00bb4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00bb8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00bbc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00bc0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00bc4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00bc8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00bcc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00bd4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00bd8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bdc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00be4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00bec
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00bf0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00bf4
			 0x4b000000,                                                  // 8388608.0                                   /// 00bf8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00bfc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c00
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c04
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c08
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c0c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c10
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c14
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c18
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c1c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c20
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c24
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c28
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c2c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c30
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c34
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c38
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x4b000000,                                                  // 8388608.0                                   /// 00c40
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c44
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c48
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c4c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c50
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c54
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c58
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c5c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c60
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c64
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c68
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c6c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c70
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c74
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c78
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c7c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c80
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c84
			 0x7fc00001,                                                  // signaling NaN                               /// 00c88
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c8c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c90
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c94
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c98
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c9c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ca0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ca8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00cac
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00cb0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00cb4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00cb8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cbc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cc0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00cc4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cc8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ccc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00cd0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00cd8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ce4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00cec
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00cf0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00cf4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00cf8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00cfc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d00
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d04
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d08
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d0c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d10
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d14
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d18
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d20
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d24
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d2c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d30
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d34
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d38
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d3c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d40
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d44
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d48
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d4c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d50
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d54
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d58
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d5c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d60
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d64
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d68
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d6c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d70
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d74
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d78
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d7c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d80
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d84
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d88
			 0x4b000000,                                                  // 8388608.0                                   /// 00d8c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d90
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d94
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d98
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d9c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00da4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00da8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00dac
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00db0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00db4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00dbc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00dc0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00dc4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00dcc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00dd4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00dd8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ddc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00de0
			 0x7f800000,                                                  // inf                                         /// 00de4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00de8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00dec
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00df0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00df4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00df8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00dfc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e00
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e04
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e0c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e10
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e14
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e18
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e24
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e30
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e34
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e38
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e40
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e44
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e48
			 0x3f028f5c,                                                  // 0.51                                        /// 00e4c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e50
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e58
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e5c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e60
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e64
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e68
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e6c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e70
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e74
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e7c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e80
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e84
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e88
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e8c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e90
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e94
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e98
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e9c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ea4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00eb4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00eb8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ebc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ec4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ec8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ecc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ed0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ed4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ed8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ee0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00eec
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ef8
			 0x33333333,                                                  // 4 random values                             /// 00efc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f00
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f08
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f0c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f10
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f14
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f18
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f1c
			 0x7f800000,                                                  // inf                                         /// 00f20
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f2c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f30
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f34
			 0xff800000,                                                  // -inf                                        /// 00f38
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f3c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f40
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f44
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f48
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f4c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f50
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f54
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f58
			 0x00000000,                                                  // zero                                        /// 00f5c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f64
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f68
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f6c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f70
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f74
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f78
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f7c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f80
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f84
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f88
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f8c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f90
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f94
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f98
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00fa0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fa4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00fa8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00fac
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00fb0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fb4
			 0xffc00001,                                                  // -signaling NaN                              /// 00fb8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00fc0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00fc4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00fc8
			 0xcb000000,                                                  // -8388608.0                                  /// 00fcc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00fd0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fd4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00fd8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fdc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00fe0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00fe4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00fec
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ff0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ff8
			 0x0e00ffff                                                  // Trailing 1s:                                /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x5f8747f1, /// 0x0
			 0x6d5af9b2, /// 0x4
			 0xd4d924b0, /// 0x8
			 0xb483a5e4, /// 0xc
			 0x6a120ec1, /// 0x10
			 0xbc1baa33, /// 0x14
			 0xdadde6e2, /// 0x18
			 0x21f4f5cd, /// 0x1c
			 0x6072c7a5, /// 0x20
			 0xed6c0d8d, /// 0x24
			 0xa3a95b60, /// 0x28
			 0x949e1b10, /// 0x2c
			 0xd8a8e6ae, /// 0x30
			 0x34056724, /// 0x34
			 0xc87cd9ef, /// 0x38
			 0x5daf782c, /// 0x3c
			 0x1e189edf, /// 0x40
			 0x576180fe, /// 0x44
			 0x2f6d42e8, /// 0x48
			 0x692ed8c5, /// 0x4c
			 0x5dd2e27f, /// 0x50
			 0xb9f176e6, /// 0x54
			 0x23dbeac7, /// 0x58
			 0xc90cb037, /// 0x5c
			 0x24c0c485, /// 0x60
			 0xceb40e75, /// 0x64
			 0x2708573c, /// 0x68
			 0x950bb16f, /// 0x6c
			 0x870baece, /// 0x70
			 0xe3cbf7d9, /// 0x74
			 0x8e611831, /// 0x78
			 0x8e4cd6f1, /// 0x7c
			 0xc11821c6, /// 0x80
			 0x344653ee, /// 0x84
			 0x07fa7c90, /// 0x88
			 0x93e31ae2, /// 0x8c
			 0xab362ccf, /// 0x90
			 0x8df00c6b, /// 0x94
			 0x29d78b53, /// 0x98
			 0xb8b707e0, /// 0x9c
			 0x02237381, /// 0xa0
			 0xf61ff0a5, /// 0xa4
			 0x92ef0fa4, /// 0xa8
			 0x7c65b783, /// 0xac
			 0x78945484, /// 0xb0
			 0xc745e430, /// 0xb4
			 0x415ee0aa, /// 0xb8
			 0xc7349a25, /// 0xbc
			 0x3c11ddab, /// 0xc0
			 0x83da5575, /// 0xc4
			 0x964eaff9, /// 0xc8
			 0xc829dae5, /// 0xcc
			 0xe6ce61c5, /// 0xd0
			 0x719f7336, /// 0xd4
			 0xc7f0f878, /// 0xd8
			 0x7673148f, /// 0xdc
			 0xfd323bd5, /// 0xe0
			 0x55a2cdbf, /// 0xe4
			 0xb7f092d3, /// 0xe8
			 0xe7b06fa3, /// 0xec
			 0x51fb225c, /// 0xf0
			 0x9def5d93, /// 0xf4
			 0xc849a5a4, /// 0xf8
			 0x63894318, /// 0xfc
			 0x51ca3bc4, /// 0x100
			 0xb3c2b07c, /// 0x104
			 0x99a65ee1, /// 0x108
			 0x51b00207, /// 0x10c
			 0xbefa5678, /// 0x110
			 0x0bd9a1b4, /// 0x114
			 0x1731e1fc, /// 0x118
			 0xe81c5ffe, /// 0x11c
			 0x80b61a67, /// 0x120
			 0x06a1660a, /// 0x124
			 0xe9cc1de5, /// 0x128
			 0xbc01390e, /// 0x12c
			 0x86820014, /// 0x130
			 0x88cb6849, /// 0x134
			 0x4c6486f9, /// 0x138
			 0x6f001f28, /// 0x13c
			 0xbfb9c91e, /// 0x140
			 0x6f0cc7b4, /// 0x144
			 0x30222a7e, /// 0x148
			 0x01f2feab, /// 0x14c
			 0x3540bdc1, /// 0x150
			 0x07442d11, /// 0x154
			 0x19191f27, /// 0x158
			 0x4d7b0b71, /// 0x15c
			 0x13a07ca6, /// 0x160
			 0xe24bdf2b, /// 0x164
			 0x9356bcab, /// 0x168
			 0x661e27cb, /// 0x16c
			 0x901420ae, /// 0x170
			 0xa1626792, /// 0x174
			 0xae5622b3, /// 0x178
			 0xc80c55f7, /// 0x17c
			 0xa704eede, /// 0x180
			 0xa6729aab, /// 0x184
			 0x41099684, /// 0x188
			 0x81a0338c, /// 0x18c
			 0xc5cebfc3, /// 0x190
			 0x676db8b3, /// 0x194
			 0xa553f71d, /// 0x198
			 0xaadc5c37, /// 0x19c
			 0xbb8157f8, /// 0x1a0
			 0xb36a1cee, /// 0x1a4
			 0x6e183fa2, /// 0x1a8
			 0x3954f699, /// 0x1ac
			 0x3926cbc6, /// 0x1b0
			 0x4ea996e2, /// 0x1b4
			 0x6f117cc5, /// 0x1b8
			 0x12e01b45, /// 0x1bc
			 0x97e6f1a0, /// 0x1c0
			 0x85ffddcc, /// 0x1c4
			 0xd208af8b, /// 0x1c8
			 0xaae7d62b, /// 0x1cc
			 0x3696038d, /// 0x1d0
			 0x47016fba, /// 0x1d4
			 0x047445ec, /// 0x1d8
			 0xda30d628, /// 0x1dc
			 0x85ad90d2, /// 0x1e0
			 0xa65fcf18, /// 0x1e4
			 0xd1e6b4d2, /// 0x1e8
			 0x73f0a4d7, /// 0x1ec
			 0x868e70e5, /// 0x1f0
			 0x00ffba96, /// 0x1f4
			 0x43d3587d, /// 0x1f8
			 0x2d24ab22, /// 0x1fc
			 0xa041f68e, /// 0x200
			 0x5cbdf2ce, /// 0x204
			 0x5419f5ad, /// 0x208
			 0xbbb037fe, /// 0x20c
			 0x258cd6c5, /// 0x210
			 0x7ef74d7c, /// 0x214
			 0x329fd6d7, /// 0x218
			 0x0a90cb3a, /// 0x21c
			 0xbc7bdebe, /// 0x220
			 0xa6715c56, /// 0x224
			 0x04d7f8a7, /// 0x228
			 0x06dc64e9, /// 0x22c
			 0xd79f82e6, /// 0x230
			 0x9cce7bac, /// 0x234
			 0x5a0bf4c9, /// 0x238
			 0xf5dc5d9d, /// 0x23c
			 0x0e3b2443, /// 0x240
			 0x9ba5b7e5, /// 0x244
			 0x1b375741, /// 0x248
			 0xcab239e4, /// 0x24c
			 0xae10675e, /// 0x250
			 0xd4407ff8, /// 0x254
			 0xc2ae04ee, /// 0x258
			 0xa08c5aa0, /// 0x25c
			 0xc0036b3e, /// 0x260
			 0x4a586258, /// 0x264
			 0x086fa574, /// 0x268
			 0xd2f57aa1, /// 0x26c
			 0x160770d8, /// 0x270
			 0xaf03d293, /// 0x274
			 0xc6bddaa6, /// 0x278
			 0xe90df1c4, /// 0x27c
			 0xb751b016, /// 0x280
			 0xf20e2ef8, /// 0x284
			 0x2b65d11d, /// 0x288
			 0x8b024cfb, /// 0x28c
			 0x7f202348, /// 0x290
			 0x0fd537d1, /// 0x294
			 0xbdd064b2, /// 0x298
			 0xe434faaf, /// 0x29c
			 0x5928f5bf, /// 0x2a0
			 0x28f3e592, /// 0x2a4
			 0xa52e6114, /// 0x2a8
			 0x37ed9930, /// 0x2ac
			 0xb012ed9d, /// 0x2b0
			 0x3fc07879, /// 0x2b4
			 0x9658b660, /// 0x2b8
			 0x76b6b417, /// 0x2bc
			 0x70a12991, /// 0x2c0
			 0x087985c5, /// 0x2c4
			 0x5a89f2ca, /// 0x2c8
			 0x35da6cf6, /// 0x2cc
			 0x63dee132, /// 0x2d0
			 0xdd1bc4a2, /// 0x2d4
			 0x53af01c7, /// 0x2d8
			 0x0d0accfe, /// 0x2dc
			 0x57d5b2f5, /// 0x2e0
			 0x93d6d00b, /// 0x2e4
			 0x927b9839, /// 0x2e8
			 0xf6af3a32, /// 0x2ec
			 0x28279bd7, /// 0x2f0
			 0x7c80e1de, /// 0x2f4
			 0x5db32231, /// 0x2f8
			 0x426d8bb9, /// 0x2fc
			 0x8413f741, /// 0x300
			 0x0d1b1a75, /// 0x304
			 0x4dcb6640, /// 0x308
			 0x2b660a1a, /// 0x30c
			 0x908bbc83, /// 0x310
			 0xd22a6c3b, /// 0x314
			 0xd08a6889, /// 0x318
			 0x33785b58, /// 0x31c
			 0xd89bdaed, /// 0x320
			 0x851d1b2b, /// 0x324
			 0x1ff96fd5, /// 0x328
			 0xe0fd751e, /// 0x32c
			 0x0883ebb8, /// 0x330
			 0xe4eadfeb, /// 0x334
			 0xe44a41f7, /// 0x338
			 0x84fb7c93, /// 0x33c
			 0x5e7480ea, /// 0x340
			 0xceb51d64, /// 0x344
			 0x76bc2c2d, /// 0x348
			 0xf9a0b6db, /// 0x34c
			 0x50c6ea15, /// 0x350
			 0x333abae9, /// 0x354
			 0x5750809d, /// 0x358
			 0xd4a818c5, /// 0x35c
			 0xf855c151, /// 0x360
			 0x275b5a30, /// 0x364
			 0x292205bb, /// 0x368
			 0xe288c143, /// 0x36c
			 0x9c2457ff, /// 0x370
			 0x3f0f7155, /// 0x374
			 0x72b60fdf, /// 0x378
			 0xf11ab4a2, /// 0x37c
			 0xf516392a, /// 0x380
			 0x4cfdd31b, /// 0x384
			 0x54c29421, /// 0x388
			 0xf9542e61, /// 0x38c
			 0x57990257, /// 0x390
			 0x142d2ec5, /// 0x394
			 0x69543c03, /// 0x398
			 0x39c3876f, /// 0x39c
			 0xa2d7b558, /// 0x3a0
			 0x89e7e22b, /// 0x3a4
			 0xb03bcf94, /// 0x3a8
			 0xc24c8346, /// 0x3ac
			 0x5ada009a, /// 0x3b0
			 0x860950f9, /// 0x3b4
			 0x91a85f27, /// 0x3b8
			 0x2a15e00f, /// 0x3bc
			 0xafef248c, /// 0x3c0
			 0xf51b6a11, /// 0x3c4
			 0x564d6be1, /// 0x3c8
			 0x4c38dd35, /// 0x3cc
			 0xb634edaa, /// 0x3d0
			 0xabd13db5, /// 0x3d4
			 0x4a188646, /// 0x3d8
			 0xc648906c, /// 0x3dc
			 0x11f9c96a, /// 0x3e0
			 0x04fc19db, /// 0x3e4
			 0x69551eda, /// 0x3e8
			 0xa4b778ff, /// 0x3ec
			 0xb604d1a6, /// 0x3f0
			 0x1a9a6e7a, /// 0x3f4
			 0xb4e811dc, /// 0x3f8
			 0x0df57519, /// 0x3fc
			 0x6ca2655d, /// 0x400
			 0x1bf13666, /// 0x404
			 0xcf3b2f3a, /// 0x408
			 0x985a33ca, /// 0x40c
			 0xb5c2ee25, /// 0x410
			 0xeb40ead1, /// 0x414
			 0x2724cbdf, /// 0x418
			 0xeb4421bd, /// 0x41c
			 0x34737e3a, /// 0x420
			 0xbad725e9, /// 0x424
			 0xb978efc2, /// 0x428
			 0xed99cbcf, /// 0x42c
			 0xf14a6078, /// 0x430
			 0xe6448c55, /// 0x434
			 0x36e6ba22, /// 0x438
			 0x496601cf, /// 0x43c
			 0xcc93d205, /// 0x440
			 0x7091ad40, /// 0x444
			 0x09f9b8a4, /// 0x448
			 0x6a591dab, /// 0x44c
			 0x2e24908e, /// 0x450
			 0x11dc0afb, /// 0x454
			 0x9231bbd6, /// 0x458
			 0xf6f94ef4, /// 0x45c
			 0xdd570c2c, /// 0x460
			 0x7adea290, /// 0x464
			 0x93b7ebd1, /// 0x468
			 0x28a553ab, /// 0x46c
			 0xf1ed6fde, /// 0x470
			 0x6880f756, /// 0x474
			 0xe94bd5ef, /// 0x478
			 0x241dfaae, /// 0x47c
			 0x333468d3, /// 0x480
			 0x679ce5a3, /// 0x484
			 0xc637f419, /// 0x488
			 0x2ca085cf, /// 0x48c
			 0x24be59ab, /// 0x490
			 0x01841ea1, /// 0x494
			 0xeda4339d, /// 0x498
			 0xe1b4e6d0, /// 0x49c
			 0xd8793ee3, /// 0x4a0
			 0xa40009bb, /// 0x4a4
			 0x4a1171a2, /// 0x4a8
			 0x296f44bb, /// 0x4ac
			 0xb586730f, /// 0x4b0
			 0x55658297, /// 0x4b4
			 0xf5e82f98, /// 0x4b8
			 0x2d531402, /// 0x4bc
			 0x9fea6edb, /// 0x4c0
			 0xf22f19ca, /// 0x4c4
			 0x23d46220, /// 0x4c8
			 0xe66af952, /// 0x4cc
			 0xdf7bbcac, /// 0x4d0
			 0x36fdb5db, /// 0x4d4
			 0x327f33fb, /// 0x4d8
			 0x67e0685b, /// 0x4dc
			 0x163d978a, /// 0x4e0
			 0x36981674, /// 0x4e4
			 0x382ef2c5, /// 0x4e8
			 0x3f9035d6, /// 0x4ec
			 0xb6fd319e, /// 0x4f0
			 0x5b825db8, /// 0x4f4
			 0x2c3c039e, /// 0x4f8
			 0x26bbd84b, /// 0x4fc
			 0xb4448766, /// 0x500
			 0x835a418e, /// 0x504
			 0xd71572a1, /// 0x508
			 0xae5183c9, /// 0x50c
			 0xb1b167c9, /// 0x510
			 0x9c280ccc, /// 0x514
			 0x30ad47be, /// 0x518
			 0x64586653, /// 0x51c
			 0xd5f5d612, /// 0x520
			 0x62ae72df, /// 0x524
			 0x700ade8a, /// 0x528
			 0x8da8e94e, /// 0x52c
			 0xddbd12df, /// 0x530
			 0xe06229e4, /// 0x534
			 0xd6e8f51a, /// 0x538
			 0xa42b8207, /// 0x53c
			 0x6edf5eb7, /// 0x540
			 0x4fb41b2c, /// 0x544
			 0x5e80a530, /// 0x548
			 0xe23922a5, /// 0x54c
			 0x443efd70, /// 0x550
			 0x124a9da1, /// 0x554
			 0x15f6971a, /// 0x558
			 0xff8bfaa9, /// 0x55c
			 0x9afd2c1f, /// 0x560
			 0xc5f53527, /// 0x564
			 0x34cd8f56, /// 0x568
			 0xe727dace, /// 0x56c
			 0x2be5d753, /// 0x570
			 0x4e0f6920, /// 0x574
			 0xf4d58529, /// 0x578
			 0xaf6ecdc0, /// 0x57c
			 0x34e3acc4, /// 0x580
			 0xee1b5170, /// 0x584
			 0x82571e8f, /// 0x588
			 0x03663317, /// 0x58c
			 0xeb2edf01, /// 0x590
			 0xb475b2eb, /// 0x594
			 0xf50e2ad7, /// 0x598
			 0xbd9c583c, /// 0x59c
			 0xd68b83ef, /// 0x5a0
			 0xcd5d3bd5, /// 0x5a4
			 0x58c7f47e, /// 0x5a8
			 0xb99a0267, /// 0x5ac
			 0x4e131555, /// 0x5b0
			 0x6353880d, /// 0x5b4
			 0xd01ee7ca, /// 0x5b8
			 0x4221f321, /// 0x5bc
			 0xa8e2a5fe, /// 0x5c0
			 0x72276e83, /// 0x5c4
			 0x2c1f849e, /// 0x5c8
			 0x01b0a574, /// 0x5cc
			 0xc464002e, /// 0x5d0
			 0x48c8ecc7, /// 0x5d4
			 0x24fb5720, /// 0x5d8
			 0x294f98ac, /// 0x5dc
			 0x8daf8dc9, /// 0x5e0
			 0x1e709bd4, /// 0x5e4
			 0x6eb9798f, /// 0x5e8
			 0x1bcd9ab4, /// 0x5ec
			 0x0014264b, /// 0x5f0
			 0xaea585b0, /// 0x5f4
			 0xa09426ff, /// 0x5f8
			 0xaeca5f4a, /// 0x5fc
			 0x49bcdcad, /// 0x600
			 0xc47483c5, /// 0x604
			 0xfcc810e3, /// 0x608
			 0xf761d72a, /// 0x60c
			 0xb03019c6, /// 0x610
			 0xa103132c, /// 0x614
			 0xc1ecf59a, /// 0x618
			 0x49349b93, /// 0x61c
			 0x7f81e068, /// 0x620
			 0x882b88f7, /// 0x624
			 0x73c1a06a, /// 0x628
			 0x41c0a55b, /// 0x62c
			 0x1e3ef14e, /// 0x630
			 0x785c5b3b, /// 0x634
			 0xf55aa863, /// 0x638
			 0x0bfd2acc, /// 0x63c
			 0xce16e862, /// 0x640
			 0x7858f265, /// 0x644
			 0x7ce1dc53, /// 0x648
			 0x1a122749, /// 0x64c
			 0x8f7c0b3d, /// 0x650
			 0xeb01a4bb, /// 0x654
			 0x619d9312, /// 0x658
			 0x408683f0, /// 0x65c
			 0x169316f3, /// 0x660
			 0xe3b6175e, /// 0x664
			 0x1c72908c, /// 0x668
			 0x3f10b776, /// 0x66c
			 0xd8e31bfc, /// 0x670
			 0x823ec284, /// 0x674
			 0x6af11632, /// 0x678
			 0x5cbc3c7e, /// 0x67c
			 0xebe6f6ca, /// 0x680
			 0x6309ef2d, /// 0x684
			 0x8ed322ed, /// 0x688
			 0xb7aa6ef8, /// 0x68c
			 0xa8cca5c8, /// 0x690
			 0xf7106951, /// 0x694
			 0xb6e412a6, /// 0x698
			 0x02525957, /// 0x69c
			 0x1743b66b, /// 0x6a0
			 0x9b68bb61, /// 0x6a4
			 0x7d35bb0f, /// 0x6a8
			 0x6e403b79, /// 0x6ac
			 0xfb0a4e68, /// 0x6b0
			 0xc949104f, /// 0x6b4
			 0x9068b844, /// 0x6b8
			 0x8f44e092, /// 0x6bc
			 0xf9f87040, /// 0x6c0
			 0xb1398b8a, /// 0x6c4
			 0xff385b2e, /// 0x6c8
			 0x2b1c113c, /// 0x6cc
			 0xae3c2a48, /// 0x6d0
			 0xdbc420fd, /// 0x6d4
			 0xd23aec7d, /// 0x6d8
			 0x786f7593, /// 0x6dc
			 0x3537af51, /// 0x6e0
			 0x19b361fe, /// 0x6e4
			 0x62138401, /// 0x6e8
			 0xbb316fcf, /// 0x6ec
			 0x8561ddb8, /// 0x6f0
			 0xe1ca2459, /// 0x6f4
			 0x3b426b5b, /// 0x6f8
			 0xa60797bf, /// 0x6fc
			 0x230c9988, /// 0x700
			 0x3c4ee358, /// 0x704
			 0xff6cbcdb, /// 0x708
			 0x06d84c18, /// 0x70c
			 0xe8293265, /// 0x710
			 0x84c628ee, /// 0x714
			 0x9a6127e3, /// 0x718
			 0x3ad0d01c, /// 0x71c
			 0xc0b899b0, /// 0x720
			 0xebeb8f5e, /// 0x724
			 0xc48b0dc7, /// 0x728
			 0x85a21438, /// 0x72c
			 0x084442db, /// 0x730
			 0x4d498c6a, /// 0x734
			 0xf1a80a67, /// 0x738
			 0xff41c661, /// 0x73c
			 0xb7867836, /// 0x740
			 0xa1b9c9ec, /// 0x744
			 0x38f0465c, /// 0x748
			 0xc7138d11, /// 0x74c
			 0x180c929f, /// 0x750
			 0x03c3fe97, /// 0x754
			 0x7a7b5cb6, /// 0x758
			 0x5f52c6e5, /// 0x75c
			 0x76689c8b, /// 0x760
			 0xcd7c9797, /// 0x764
			 0x47d0b304, /// 0x768
			 0x568a263f, /// 0x76c
			 0x3724ba4f, /// 0x770
			 0xa0c22164, /// 0x774
			 0x6e7988ba, /// 0x778
			 0x107cb0ca, /// 0x77c
			 0x040ccff9, /// 0x780
			 0x1c6d962c, /// 0x784
			 0xecc3a74e, /// 0x788
			 0x500b3935, /// 0x78c
			 0xe2dd91cc, /// 0x790
			 0x01821828, /// 0x794
			 0x5ef69818, /// 0x798
			 0xfa1a5339, /// 0x79c
			 0xac43dc2f, /// 0x7a0
			 0x9d00ae00, /// 0x7a4
			 0xd9444961, /// 0x7a8
			 0x2904a0d1, /// 0x7ac
			 0xf3f6d4af, /// 0x7b0
			 0x03bac0bb, /// 0x7b4
			 0x0b062866, /// 0x7b8
			 0xc738e6e6, /// 0x7bc
			 0xb49800bf, /// 0x7c0
			 0xd6b1330e, /// 0x7c4
			 0xc01f9f25, /// 0x7c8
			 0x65751b36, /// 0x7cc
			 0xed6c12e7, /// 0x7d0
			 0x9cbf5844, /// 0x7d4
			 0x9a5c815f, /// 0x7d8
			 0xdb1012e4, /// 0x7dc
			 0x23cda801, /// 0x7e0
			 0xae03c34e, /// 0x7e4
			 0x73d84cfe, /// 0x7e8
			 0x2013c65f, /// 0x7ec
			 0xc89ca1ba, /// 0x7f0
			 0x709ad3bc, /// 0x7f4
			 0x4c3eaca5, /// 0x7f8
			 0x92ceef7f, /// 0x7fc
			 0xb46c49fd, /// 0x800
			 0x3b34a07b, /// 0x804
			 0x0dccaef9, /// 0x808
			 0x62321bc8, /// 0x80c
			 0xd96e7028, /// 0x810
			 0xef336db8, /// 0x814
			 0x565ce389, /// 0x818
			 0x267e5eea, /// 0x81c
			 0xd67bf25e, /// 0x820
			 0x2b50603f, /// 0x824
			 0xc4f86410, /// 0x828
			 0x08efd50e, /// 0x82c
			 0xd8103611, /// 0x830
			 0x58656ecc, /// 0x834
			 0x2bdf7a15, /// 0x838
			 0x52d099dd, /// 0x83c
			 0x44dbbb45, /// 0x840
			 0x6e9c9d08, /// 0x844
			 0x8b92e201, /// 0x848
			 0xcce5a62f, /// 0x84c
			 0x860dc0ed, /// 0x850
			 0xda5074b9, /// 0x854
			 0x6a84b85b, /// 0x858
			 0xb53b293e, /// 0x85c
			 0x042f489b, /// 0x860
			 0x96c3dcab, /// 0x864
			 0x873fd9a5, /// 0x868
			 0x7c8ca00a, /// 0x86c
			 0x4b08245e, /// 0x870
			 0xbdc70488, /// 0x874
			 0x894db4bb, /// 0x878
			 0x6a1fa1e9, /// 0x87c
			 0x7a943ba6, /// 0x880
			 0xfa84f3f3, /// 0x884
			 0xbac718c9, /// 0x888
			 0x51509bbb, /// 0x88c
			 0x87116d7d, /// 0x890
			 0xf8c47cd7, /// 0x894
			 0xbad61352, /// 0x898
			 0xd10ff20a, /// 0x89c
			 0x4979e61f, /// 0x8a0
			 0xbe1345f8, /// 0x8a4
			 0xe01ee952, /// 0x8a8
			 0xfc8f543b, /// 0x8ac
			 0xcda1d5dc, /// 0x8b0
			 0x206d6280, /// 0x8b4
			 0x380a1c5b, /// 0x8b8
			 0x58559970, /// 0x8bc
			 0xa3d2a18e, /// 0x8c0
			 0x55f8b64a, /// 0x8c4
			 0xc21e4985, /// 0x8c8
			 0x33f184d1, /// 0x8cc
			 0xd73f9813, /// 0x8d0
			 0x14cf1c45, /// 0x8d4
			 0x28a3c9c9, /// 0x8d8
			 0x64f18e64, /// 0x8dc
			 0xcb4f562d, /// 0x8e0
			 0x4c8f0452, /// 0x8e4
			 0xbd412d5d, /// 0x8e8
			 0x6049a114, /// 0x8ec
			 0xdf5f4ebc, /// 0x8f0
			 0x7ab3408c, /// 0x8f4
			 0x555ffdf3, /// 0x8f8
			 0xfb514506, /// 0x8fc
			 0xca976d5e, /// 0x900
			 0x58fcdff3, /// 0x904
			 0x8ee40528, /// 0x908
			 0x8c2487ad, /// 0x90c
			 0xc51af330, /// 0x910
			 0x51e68f85, /// 0x914
			 0x231fa2d5, /// 0x918
			 0xd66436d1, /// 0x91c
			 0xb560bd22, /// 0x920
			 0xcd3344e3, /// 0x924
			 0xd0c20d5c, /// 0x928
			 0xaebeea39, /// 0x92c
			 0x14f62830, /// 0x930
			 0x0eb31eff, /// 0x934
			 0xad3fcaa1, /// 0x938
			 0x58e136ff, /// 0x93c
			 0x393850fc, /// 0x940
			 0x1914f679, /// 0x944
			 0xf440b756, /// 0x948
			 0xeabb59ae, /// 0x94c
			 0x5851d3a5, /// 0x950
			 0xff307655, /// 0x954
			 0x2cf24713, /// 0x958
			 0x6b5e79a4, /// 0x95c
			 0x956c6aef, /// 0x960
			 0x886b43a4, /// 0x964
			 0xde3834b1, /// 0x968
			 0x283ca8d9, /// 0x96c
			 0xb8b65da8, /// 0x970
			 0x681a3d9e, /// 0x974
			 0xf47bb2c4, /// 0x978
			 0x48c51bf3, /// 0x97c
			 0x5afb77a7, /// 0x980
			 0x21385667, /// 0x984
			 0xa7c6e746, /// 0x988
			 0xacb5c209, /// 0x98c
			 0x5d0b915c, /// 0x990
			 0x2cb5d183, /// 0x994
			 0x78df7762, /// 0x998
			 0x49f5adc1, /// 0x99c
			 0x4912effe, /// 0x9a0
			 0xe06ad2c0, /// 0x9a4
			 0x91f479e8, /// 0x9a8
			 0xbf52adbe, /// 0x9ac
			 0xad1be174, /// 0x9b0
			 0x6ead26e0, /// 0x9b4
			 0x4f9dae2b, /// 0x9b8
			 0x55b7d289, /// 0x9bc
			 0x676b4675, /// 0x9c0
			 0x83be2730, /// 0x9c4
			 0xed1fbc56, /// 0x9c8
			 0xcc1b544f, /// 0x9cc
			 0x0b715cdb, /// 0x9d0
			 0x37bbd180, /// 0x9d4
			 0xf9ef1cb9, /// 0x9d8
			 0xd095217d, /// 0x9dc
			 0x62c6b3f8, /// 0x9e0
			 0x8f2f4898, /// 0x9e4
			 0xa434c298, /// 0x9e8
			 0xce23db95, /// 0x9ec
			 0x987e5364, /// 0x9f0
			 0x7a75b61e, /// 0x9f4
			 0x9ac9c1fc, /// 0x9f8
			 0x142ae707, /// 0x9fc
			 0x3e14bd93, /// 0xa00
			 0xe60e41a2, /// 0xa04
			 0x19085c84, /// 0xa08
			 0xed864d95, /// 0xa0c
			 0x4dd15d51, /// 0xa10
			 0x507ca379, /// 0xa14
			 0x717371a9, /// 0xa18
			 0xea9c5111, /// 0xa1c
			 0x5c57fda7, /// 0xa20
			 0x56e2f766, /// 0xa24
			 0x10f66d3d, /// 0xa28
			 0xb632d8ca, /// 0xa2c
			 0x2ebde86a, /// 0xa30
			 0xd23308f5, /// 0xa34
			 0x86750c31, /// 0xa38
			 0xcbe386e1, /// 0xa3c
			 0x5de32043, /// 0xa40
			 0x90727d3e, /// 0xa44
			 0x9a41b567, /// 0xa48
			 0x01ea8a57, /// 0xa4c
			 0x7bec642b, /// 0xa50
			 0xf84bf3b3, /// 0xa54
			 0x39322316, /// 0xa58
			 0xa98d695c, /// 0xa5c
			 0x1789966a, /// 0xa60
			 0x71aeae33, /// 0xa64
			 0x524c28d9, /// 0xa68
			 0x39153e09, /// 0xa6c
			 0xdc1fc8d4, /// 0xa70
			 0xf4090199, /// 0xa74
			 0x8054780d, /// 0xa78
			 0x1076480e, /// 0xa7c
			 0x2b547258, /// 0xa80
			 0x863f7def, /// 0xa84
			 0x140a9eaf, /// 0xa88
			 0x101ecafe, /// 0xa8c
			 0x0a39af46, /// 0xa90
			 0xb102efa8, /// 0xa94
			 0xf750e9bd, /// 0xa98
			 0x717020ab, /// 0xa9c
			 0x830c1b16, /// 0xaa0
			 0x8cffb5e3, /// 0xaa4
			 0x6f8695e0, /// 0xaa8
			 0xe01b3929, /// 0xaac
			 0x5bea6574, /// 0xab0
			 0xc1e64fb7, /// 0xab4
			 0x264ede7f, /// 0xab8
			 0x5ed84295, /// 0xabc
			 0x4f8e62aa, /// 0xac0
			 0x6db6bfb8, /// 0xac4
			 0x5ebed977, /// 0xac8
			 0x6195bfa8, /// 0xacc
			 0x625c1d89, /// 0xad0
			 0xd0fe2bbd, /// 0xad4
			 0x644004d5, /// 0xad8
			 0x5163c61b, /// 0xadc
			 0x85064e36, /// 0xae0
			 0x3c31af13, /// 0xae4
			 0xdf3f8f81, /// 0xae8
			 0xf495fcdf, /// 0xaec
			 0x7ba67146, /// 0xaf0
			 0xdeae99e4, /// 0xaf4
			 0x75d4c90a, /// 0xaf8
			 0xe989e40b, /// 0xafc
			 0xc2545b7e, /// 0xb00
			 0x6f1e2615, /// 0xb04
			 0x3845482b, /// 0xb08
			 0xa0056135, /// 0xb0c
			 0x808babdd, /// 0xb10
			 0xd224bcd3, /// 0xb14
			 0x10ca045e, /// 0xb18
			 0x04ebea5e, /// 0xb1c
			 0xe28d6451, /// 0xb20
			 0xc35bc2b3, /// 0xb24
			 0x1dcea8a9, /// 0xb28
			 0xcb77e271, /// 0xb2c
			 0xedb21a86, /// 0xb30
			 0x1dee5bc6, /// 0xb34
			 0x343ad8bc, /// 0xb38
			 0xbd46329a, /// 0xb3c
			 0x91cadea4, /// 0xb40
			 0x2b382c20, /// 0xb44
			 0x9a6d41ff, /// 0xb48
			 0x1cf6d768, /// 0xb4c
			 0x59e79a0a, /// 0xb50
			 0xeb87725e, /// 0xb54
			 0xd0591e98, /// 0xb58
			 0x5514b72f, /// 0xb5c
			 0xb74da905, /// 0xb60
			 0xafad7794, /// 0xb64
			 0x57c2bb30, /// 0xb68
			 0xf368aa16, /// 0xb6c
			 0x7baaa9e7, /// 0xb70
			 0x60589abd, /// 0xb74
			 0x468de6eb, /// 0xb78
			 0x8880e22c, /// 0xb7c
			 0xcae8bd0b, /// 0xb80
			 0x1b6495e9, /// 0xb84
			 0xef96ba99, /// 0xb88
			 0x4276c5e0, /// 0xb8c
			 0x964a05b8, /// 0xb90
			 0x80360ff2, /// 0xb94
			 0xe95046cf, /// 0xb98
			 0xf9cfb1e4, /// 0xb9c
			 0xfaba7953, /// 0xba0
			 0x08eb0770, /// 0xba4
			 0x2351474f, /// 0xba8
			 0x27e8ecd7, /// 0xbac
			 0xceb34b3d, /// 0xbb0
			 0xf775aa1b, /// 0xbb4
			 0xa789b14e, /// 0xbb8
			 0x53267e24, /// 0xbbc
			 0x7b055f58, /// 0xbc0
			 0x48d80021, /// 0xbc4
			 0x9e41b3bb, /// 0xbc8
			 0xd2acd5e1, /// 0xbcc
			 0x18cb7343, /// 0xbd0
			 0x22b705a8, /// 0xbd4
			 0x67c6bdf3, /// 0xbd8
			 0xfe7655ea, /// 0xbdc
			 0xfb2c8794, /// 0xbe0
			 0xee4b7520, /// 0xbe4
			 0x26f1b735, /// 0xbe8
			 0x33f19293, /// 0xbec
			 0x6ce7e09c, /// 0xbf0
			 0x10cd78d5, /// 0xbf4
			 0xf3aa6e70, /// 0xbf8
			 0xecb270c9, /// 0xbfc
			 0x33a81206, /// 0xc00
			 0xb403a56a, /// 0xc04
			 0x71415faa, /// 0xc08
			 0x752231ff, /// 0xc0c
			 0x241873d0, /// 0xc10
			 0x25ebb1e9, /// 0xc14
			 0x084f4346, /// 0xc18
			 0x6b3d93ad, /// 0xc1c
			 0xf172eaf2, /// 0xc20
			 0xab68bd76, /// 0xc24
			 0x16f38763, /// 0xc28
			 0x37826757, /// 0xc2c
			 0xb40a3950, /// 0xc30
			 0x9ca76ec0, /// 0xc34
			 0xeeb0cb35, /// 0xc38
			 0x3b89402a, /// 0xc3c
			 0x6e4f87f0, /// 0xc40
			 0x36452b78, /// 0xc44
			 0x69a0c631, /// 0xc48
			 0x40fc4e1e, /// 0xc4c
			 0xfc92e735, /// 0xc50
			 0x7a57a4a5, /// 0xc54
			 0x0f72922b, /// 0xc58
			 0x0c0dfd7b, /// 0xc5c
			 0x5db815f4, /// 0xc60
			 0x014142f0, /// 0xc64
			 0x3f974efa, /// 0xc68
			 0x98860e6d, /// 0xc6c
			 0xfef361a4, /// 0xc70
			 0x0f2b6457, /// 0xc74
			 0x85e3452e, /// 0xc78
			 0xf518dd3e, /// 0xc7c
			 0x5734fbb4, /// 0xc80
			 0xf1b00237, /// 0xc84
			 0x8f6c39af, /// 0xc88
			 0xf5ea8c1f, /// 0xc8c
			 0x396d7afc, /// 0xc90
			 0x78b7d737, /// 0xc94
			 0x09a2313b, /// 0xc98
			 0xfece2e52, /// 0xc9c
			 0xec62d233, /// 0xca0
			 0x59881da0, /// 0xca4
			 0x2a0dd26e, /// 0xca8
			 0xc499e79b, /// 0xcac
			 0xc5cfdcff, /// 0xcb0
			 0x6a144978, /// 0xcb4
			 0x2dc194fa, /// 0xcb8
			 0x7fdb7a56, /// 0xcbc
			 0x4a3457d3, /// 0xcc0
			 0x6e64995b, /// 0xcc4
			 0x5396debc, /// 0xcc8
			 0x814df747, /// 0xccc
			 0x2f872583, /// 0xcd0
			 0x40d3d9c3, /// 0xcd4
			 0x55802ab4, /// 0xcd8
			 0x18e35b02, /// 0xcdc
			 0x85a03829, /// 0xce0
			 0x2bcdc618, /// 0xce4
			 0x774fa05b, /// 0xce8
			 0xb3a0ff84, /// 0xcec
			 0x3439c8a4, /// 0xcf0
			 0x74caf1b9, /// 0xcf4
			 0x85993c6e, /// 0xcf8
			 0x54b0bf5b, /// 0xcfc
			 0x3e53c4ad, /// 0xd00
			 0x69236991, /// 0xd04
			 0xf1d8f4b0, /// 0xd08
			 0x37929688, /// 0xd0c
			 0xa8fa86d1, /// 0xd10
			 0xe76324d5, /// 0xd14
			 0x8375f3e4, /// 0xd18
			 0x2963c4d2, /// 0xd1c
			 0xfe452b76, /// 0xd20
			 0xf4513ba8, /// 0xd24
			 0x8756a06d, /// 0xd28
			 0xc12a3e66, /// 0xd2c
			 0x9a2a91c9, /// 0xd30
			 0x0fd5d318, /// 0xd34
			 0x1fc4db9e, /// 0xd38
			 0xac2e6402, /// 0xd3c
			 0x9ee55e38, /// 0xd40
			 0x5670ab24, /// 0xd44
			 0xe233c6e9, /// 0xd48
			 0x6a930224, /// 0xd4c
			 0x572fbb97, /// 0xd50
			 0xc56e90b2, /// 0xd54
			 0xa23710cd, /// 0xd58
			 0xbeb7f3db, /// 0xd5c
			 0x742012c7, /// 0xd60
			 0x633309fe, /// 0xd64
			 0x25c5a5ba, /// 0xd68
			 0x2667994d, /// 0xd6c
			 0x9c87852a, /// 0xd70
			 0x5227dcf6, /// 0xd74
			 0xbfc5b1ad, /// 0xd78
			 0xdec8b36b, /// 0xd7c
			 0x59f2c230, /// 0xd80
			 0x7116f309, /// 0xd84
			 0x94c0e343, /// 0xd88
			 0xf1355fe0, /// 0xd8c
			 0x7aae0ed6, /// 0xd90
			 0xd89be4a9, /// 0xd94
			 0x8d3b586e, /// 0xd98
			 0x39d3e5c3, /// 0xd9c
			 0x017620b5, /// 0xda0
			 0x1a3d280c, /// 0xda4
			 0x998ab030, /// 0xda8
			 0x6da7007d, /// 0xdac
			 0x5cf3f8ec, /// 0xdb0
			 0xc2690cc4, /// 0xdb4
			 0x7cf41913, /// 0xdb8
			 0x55e0a5a9, /// 0xdbc
			 0xc4a9f157, /// 0xdc0
			 0xda767ddd, /// 0xdc4
			 0xf873bf61, /// 0xdc8
			 0xbd7ed987, /// 0xdcc
			 0xbd79cd52, /// 0xdd0
			 0x892c2443, /// 0xdd4
			 0x9863f89b, /// 0xdd8
			 0x7a7cbcff, /// 0xddc
			 0xa8459113, /// 0xde0
			 0x234a44f6, /// 0xde4
			 0xc811136b, /// 0xde8
			 0xc83c49c8, /// 0xdec
			 0x88958198, /// 0xdf0
			 0x51ce21e4, /// 0xdf4
			 0x10ffc516, /// 0xdf8
			 0x1a008ffc, /// 0xdfc
			 0x8b7a546d, /// 0xe00
			 0x1e632e85, /// 0xe04
			 0x05dd5045, /// 0xe08
			 0xcf809b1f, /// 0xe0c
			 0xa6643d22, /// 0xe10
			 0x64dff54a, /// 0xe14
			 0x04eff438, /// 0xe18
			 0x60c6aa15, /// 0xe1c
			 0x010077c1, /// 0xe20
			 0x17121afe, /// 0xe24
			 0x0e60f2e2, /// 0xe28
			 0x21179637, /// 0xe2c
			 0x7278cb5f, /// 0xe30
			 0xc8bb677c, /// 0xe34
			 0x55c5fe2f, /// 0xe38
			 0x4ed87311, /// 0xe3c
			 0xd553d303, /// 0xe40
			 0x2a3d9015, /// 0xe44
			 0x4e22f26d, /// 0xe48
			 0x1e9719bc, /// 0xe4c
			 0x8193c688, /// 0xe50
			 0xb923ac23, /// 0xe54
			 0x1757bdcd, /// 0xe58
			 0xf2ee9aab, /// 0xe5c
			 0x2805ccb5, /// 0xe60
			 0x53fd9d14, /// 0xe64
			 0xf7af00f3, /// 0xe68
			 0xa15b39c1, /// 0xe6c
			 0xaa1160ae, /// 0xe70
			 0xbc42c592, /// 0xe74
			 0x9c5c45cc, /// 0xe78
			 0xb6dc34d9, /// 0xe7c
			 0x37fae938, /// 0xe80
			 0x12ce48d1, /// 0xe84
			 0x5a6b8d3b, /// 0xe88
			 0x2973b563, /// 0xe8c
			 0x3a7314a2, /// 0xe90
			 0xced0a014, /// 0xe94
			 0x022d62f0, /// 0xe98
			 0xce88640d, /// 0xe9c
			 0x80f809e9, /// 0xea0
			 0x2751e792, /// 0xea4
			 0xe02049c4, /// 0xea8
			 0xcad6356f, /// 0xeac
			 0x949ce319, /// 0xeb0
			 0x28915bdf, /// 0xeb4
			 0xf8a783ed, /// 0xeb8
			 0xe44be6d2, /// 0xebc
			 0x8abcd4b9, /// 0xec0
			 0x7705a0b6, /// 0xec4
			 0x6f311285, /// 0xec8
			 0xb97c1b14, /// 0xecc
			 0x8d3cc811, /// 0xed0
			 0x6a9633ed, /// 0xed4
			 0x25a3b4ca, /// 0xed8
			 0x1dce89b4, /// 0xedc
			 0x72e02824, /// 0xee0
			 0x99018161, /// 0xee4
			 0xb60d04aa, /// 0xee8
			 0x6a3835cd, /// 0xeec
			 0x9821c19b, /// 0xef0
			 0x61e9f95f, /// 0xef4
			 0x12b358e6, /// 0xef8
			 0x5f64909f, /// 0xefc
			 0xfde26260, /// 0xf00
			 0x8561d40c, /// 0xf04
			 0x9443a2c4, /// 0xf08
			 0xf20bbbba, /// 0xf0c
			 0x2344bf0e, /// 0xf10
			 0x34884243, /// 0xf14
			 0xf7443d8c, /// 0xf18
			 0xcbc28034, /// 0xf1c
			 0x9ccb5646, /// 0xf20
			 0xeb01ca7e, /// 0xf24
			 0x1e3b0b52, /// 0xf28
			 0x66ee0840, /// 0xf2c
			 0x7dee8fae, /// 0xf30
			 0xc2cf8b0b, /// 0xf34
			 0xc484757c, /// 0xf38
			 0x164d5a88, /// 0xf3c
			 0x18ac9a39, /// 0xf40
			 0xdd4aead6, /// 0xf44
			 0xdc92cc98, /// 0xf48
			 0xe531913b, /// 0xf4c
			 0x24483c6d, /// 0xf50
			 0x69fdfc4b, /// 0xf54
			 0x8a510014, /// 0xf58
			 0x70fb2753, /// 0xf5c
			 0xbdd5e1ca, /// 0xf60
			 0x4ddb82b1, /// 0xf64
			 0x3ec420aa, /// 0xf68
			 0x6e2ce6c1, /// 0xf6c
			 0xc784b305, /// 0xf70
			 0xcc72ad9c, /// 0xf74
			 0xd44d2002, /// 0xf78
			 0x5600f4b2, /// 0xf7c
			 0x56db9672, /// 0xf80
			 0xbcb3c28a, /// 0xf84
			 0x5b84279f, /// 0xf88
			 0xc4de9b94, /// 0xf8c
			 0x2af3738c, /// 0xf90
			 0x3fbf89ce, /// 0xf94
			 0xa970e3cd, /// 0xf98
			 0x247e82f0, /// 0xf9c
			 0x93951f62, /// 0xfa0
			 0x007ba134, /// 0xfa4
			 0xf40c6087, /// 0xfa8
			 0xf5eb3833, /// 0xfac
			 0xc360819b, /// 0xfb0
			 0x2c4a1cbb, /// 0xfb4
			 0xf982c8d4, /// 0xfb8
			 0x6fb820f5, /// 0xfbc
			 0x97a4f4ef, /// 0xfc0
			 0x6f897bb3, /// 0xfc4
			 0xe05b56c0, /// 0xfc8
			 0xc6499ef3, /// 0xfcc
			 0x0a148a1c, /// 0xfd0
			 0xc0812de1, /// 0xfd4
			 0xadf66f0f, /// 0xfd8
			 0x35c31711, /// 0xfdc
			 0xa55ac409, /// 0xfe0
			 0x4e8c6a6d, /// 0xfe4
			 0xf54adf1b, /// 0xfe8
			 0x7d4fabee, /// 0xfec
			 0xc5bad02f, /// 0xff0
			 0xdc535161, /// 0xff4
			 0x4f89baac, /// 0xff8
			 0x7ddc412a /// last
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
			 0x000006a4,
			 0x0000006c,
			 0x00000128,
			 0x00000334,
			 0x000004c0,
			 0x00000294,
			 0x0000060c,
			 0x000006ec,

			 /// vpu register f2
			 0x41880000,
			 0x41400000,
			 0x40a00000,
			 0x41d00000,
			 0x41e00000,
			 0x41b00000,
			 0x41a80000,
			 0x41b00000,

			 /// vpu register f3
			 0x40000000,
			 0x41600000,
			 0x41a80000,
			 0x41500000,
			 0x40400000,
			 0x41e00000,
			 0x3f800000,
			 0x41f00000,

			 /// vpu register f4
			 0x41b00000,
			 0x41600000,
			 0x41100000,
			 0x40c00000,
			 0x41c00000,
			 0x41400000,
			 0x40800000,
			 0x40800000,

			 /// vpu register f5
			 0x40400000,
			 0x3f800000,
			 0x41200000,
			 0x41100000,
			 0x40c00000,
			 0x41b00000,
			 0x40800000,
			 0x41b00000,

			 /// vpu register f6
			 0x41000000,
			 0x41d00000,
			 0x41600000,
			 0x41d80000,
			 0x41c00000,
			 0x42000000,
			 0x41300000,
			 0x40a00000,

			 /// vpu register f7
			 0x41c00000,
			 0x3f800000,
			 0x41a00000,
			 0x41b00000,
			 0x40e00000,
			 0x41200000,
			 0x41f80000,
			 0x41e80000,

			 /// vpu register f8
			 0x41d80000,
			 0x41e80000,
			 0x41d80000,
			 0x42000000,
			 0x41a80000,
			 0x41100000,
			 0x40e00000,
			 0x41700000,

			 /// vpu register f9
			 0x41000000,
			 0x41c80000,
			 0x40e00000,
			 0x40800000,
			 0x41e00000,
			 0x41c00000,
			 0x41e00000,
			 0x40000000,

			 /// vpu register f10
			 0x41c80000,
			 0x41880000,
			 0x41900000,
			 0x41d80000,
			 0x40a00000,
			 0x40800000,
			 0x41500000,
			 0x40000000,

			 /// vpu register f11
			 0x41d00000,
			 0x40c00000,
			 0x41f00000,
			 0x40000000,
			 0x41980000,
			 0x41b80000,
			 0x41300000,
			 0x41700000,

			 /// vpu register f12
			 0x41980000,
			 0x41e00000,
			 0x40a00000,
			 0x40400000,
			 0x40e00000,
			 0x41e00000,
			 0x41900000,
			 0x41300000,

			 /// vpu register f13
			 0x41f00000,
			 0x40a00000,
			 0x41100000,
			 0x41c00000,
			 0x41c00000,
			 0x41900000,
			 0x41400000,
			 0x3f800000,

			 /// vpu register f14
			 0x41a80000,
			 0x41600000,
			 0x41700000,
			 0x41100000,
			 0x40c00000,
			 0x41e80000,
			 0x41a00000,
			 0x41980000,

			 /// vpu register f15
			 0x40c00000,
			 0x41400000,
			 0x40400000,
			 0x41d00000,
			 0x40e00000,
			 0x41a80000,
			 0x40000000,
			 0x41b00000,

			 /// vpu register f16
			 0x41880000,
			 0x41900000,
			 0x41e80000,
			 0x41d80000,
			 0x40a00000,
			 0x40e00000,
			 0x41b00000,
			 0x41e80000,

			 /// vpu register f17
			 0x40000000,
			 0x41900000,
			 0x41e80000,
			 0x41c00000,
			 0x3f800000,
			 0x41b80000,
			 0x41100000,
			 0x41e00000,

			 /// vpu register f18
			 0x41a80000,
			 0x41400000,
			 0x40e00000,
			 0x41e00000,
			 0x40800000,
			 0x41700000,
			 0x41c80000,
			 0x41c00000,

			 /// vpu register f19
			 0x41b80000,
			 0x41500000,
			 0x41d00000,
			 0x41400000,
			 0x41800000,
			 0x40800000,
			 0x41400000,
			 0x41c00000,

			 /// vpu register f20
			 0x40e00000,
			 0x41e80000,
			 0x3f800000,
			 0x3f800000,
			 0x41300000,
			 0x41900000,
			 0x41c80000,
			 0x41200000,

			 /// vpu register f21
			 0x41400000,
			 0x41500000,
			 0x41d80000,
			 0x41800000,
			 0x40e00000,
			 0x40800000,
			 0x41880000,
			 0x41200000,

			 /// vpu register f22
			 0x3f800000,
			 0x41880000,
			 0x40c00000,
			 0x41600000,
			 0x41880000,
			 0x41e80000,
			 0x41f80000,
			 0x41c80000,

			 /// vpu register f23
			 0x40c00000,
			 0x41a80000,
			 0x41300000,
			 0x41100000,
			 0x41900000,
			 0x41f80000,
			 0x41500000,
			 0x41500000,

			 /// vpu register f24
			 0x41e80000,
			 0x41900000,
			 0x41e00000,
			 0x41880000,
			 0x41f80000,
			 0x41000000,
			 0x41300000,
			 0x41d00000,

			 /// vpu register f25
			 0x41c80000,
			 0x41d80000,
			 0x41f80000,
			 0x40000000,
			 0x41980000,
			 0x41b80000,
			 0x41100000,
			 0x40c00000,

			 /// vpu register f26
			 0x41c80000,
			 0x40e00000,
			 0x40e00000,
			 0x41c80000,
			 0x42000000,
			 0x41b80000,
			 0x41f80000,
			 0x40a00000,

			 /// vpu register f27
			 0x41700000,
			 0x41980000,
			 0x41980000,
			 0x41d80000,
			 0x41700000,
			 0x41400000,
			 0x40000000,
			 0x41300000,

			 /// vpu register f28
			 0x41980000,
			 0x41a00000,
			 0x41100000,
			 0x41100000,
			 0x40c00000,
			 0x42000000,
			 0x40e00000,
			 0x41c80000,

			 /// vpu register f29
			 0x40e00000,
			 0x41980000,
			 0x3f800000,
			 0x41700000,
			 0x40a00000,
			 0x41880000,
			 0x41980000,
			 0x41c00000,

			 /// vpu register f30
			 0x41980000,
			 0x41e80000,
			 0x41e00000,
			 0x41400000,
			 0x41100000,
			 0x41200000,
			 0x41000000,
			 0x41100000,

			 /// vpu register f31
			 0x41e00000,
			 0x41a00000,
			 0x41d80000,
			 0x41a00000,
			 0x41b80000,
			 0x41800000,
			 0x41200000,
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
		"fmaxu.pi f19, f9, f17\n"                             ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f30, f22, f20\n"                            ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f14, f22, f30\n"                            ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f14, f7, f2\n"                              ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f29, f12, f22\n"                            ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f9, f8, f19\n"                              ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f28, f11, f17\n"                            ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f2, f13, f0\n"                              ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f30, f1, f15\n"                             ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f8, f7, f26\n"                              ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f21, f8, f23\n"                             ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f27, f17, f8\n"                             ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f16, f5, f14\n"                             ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f16, f18, f21\n"                            ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f23, f15, f28\n"                            ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f2, f6, f1\n"                               ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f15, f7, f7\n"                              ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f26, f8, f9\n"                              ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f27, f7, f24\n"                             ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f7, f19, f27\n"                             ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f12, f3, f2\n"                              ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f23, f4, f22\n"                             ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f24, f28, f16\n"                            ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f21, f14, f24\n"                            ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f7, f9, f16\n"                              ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f4, f17, f16\n"                             ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f14, f15, f18\n"                            ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f4, f23, f2\n"                              ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f7, f16, f25\n"                             ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f10, f13, f0\n"                             ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f11, f12, f18\n"                            ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f18, f4, f29\n"                             ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f3, f0, f6\n"                               ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f22, f11, f19\n"                            ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f25, f21, f14\n"                            ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f16, f11, f19\n"                            ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f27, f18, f26\n"                            ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f10, f29, f6\n"                             ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f18, f28, f14\n"                            ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f4, f13, f9\n"                              ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f21, f17, f15\n"                            ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f4, f10, f23\n"                             ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f17, f5, f3\n"                              ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f11, f11, f0\n"                             ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f4, f2, f17\n"                              ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f5, f10, f16\n"                             ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f18, f2, f30\n"                             ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f23, f28, f26\n"                            ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f2, f12, f29\n"                             ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f20, f9, f2\n"                              ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f26, f22, f26\n"                            ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f10, f30, f27\n"                            ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f7, f14, f29\n"                             ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f24, f9, f28\n"                             ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f30, f14, f29\n"                            ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f7, f15, f16\n"                             ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f3, f3, f19\n"                              ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f24, f5, f0\n"                              ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f3, f25, f9\n"                              ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f3, f26, f8\n"                              ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f14, f11, f23\n"                            ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f9, f16, f15\n"                             ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f5, f11, f11\n"                             ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f12, f25, f15\n"                            ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f26, f15, f9\n"                             ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f9, f12, f20\n"                             ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f9, f6, f13\n"                              ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f7, f12, f25\n"                             ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f5, f19, f0\n"                              ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f13, f9, f22\n"                             ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f8, f22, f26\n"                             ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f24, f28, f14\n"                            ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f16, f16, f2\n"                             ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f9, f17, f0\n"                              ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f26, f30, f1\n"                             ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f14, f19, f29\n"                            ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f19, f1, f10\n"                             ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f15, f13, f7\n"                             ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f25, f10, f4\n"                             ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f27, f24, f16\n"                            ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f2, f20, f0\n"                              ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f2, f30, f20\n"                             ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f11, f7, f14\n"                             ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f25, f13, f12\n"                            ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f27, f25, f26\n"                            ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f26, f16, f20\n"                            ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f29, f13, f22\n"                            ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f12, f3, f11\n"                             ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f13, f19, f3\n"                             ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f22, f9, f19\n"                             ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f5, f0, f16\n"                              ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f3, f16, f18\n"                             ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f2, f26, f3\n"                              ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f22, f17, f20\n"                            ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f29, f12, f21\n"                            ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f23, f26, f14\n"                            ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f26, f15, f15\n"                            ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f23, f18, f15\n"                            ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmaxu.pi f28, f27, f0\n"                             ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
