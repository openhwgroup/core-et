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
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00000
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00004
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00008
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0000c
			 0x7f800000,                                                  // inf                                         /// 00010
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00014
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00018
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0001c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00020
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00024
			 0x3f028f5c,                                                  // 0.51                                        /// 00028
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0002c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00030
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00034
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0003c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00040
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00044
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00048
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0004c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00050
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00054
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00058
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0005c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00060
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00064
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00068
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00070
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00074
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00078
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0007c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00080
			 0x00000000,                                                  // zero                                        /// 00084
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00088
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0008c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00090
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00094
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00098
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0009c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000a0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000a4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000ac
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000b0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000b4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000b8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0x0c700000,                                                  // Leading 1s:                                 /// 000c0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000c4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000c8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000cc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 000d0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000d4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000d8
			 0xcb000000,                                                  // -8388608.0                                  /// 000dc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000e0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000e4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000e8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 000ec
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000fc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00100
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00104
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00108
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0010c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00110
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00114
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00118
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0011c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00120
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00124
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00128
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0012c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00130
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00134
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00138
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0013c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00140
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00144
			 0x3f028f5c,                                                  // 0.51                                        /// 00148
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0014c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0015c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00160
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00164
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00168
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0016c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00170
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00174
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00178
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0017c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00180
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00184
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00188
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00190
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00194
			 0x0c780000,                                                  // Leading 1s:                                 /// 00198
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0019c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 001a4
			 0x0c700000,                                                  // Leading 1s:                                 /// 001a8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001ac
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001b0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 001b4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001b8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001c4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001cc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 001d0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0xff800000,                                                  // -inf                                        /// 001d8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001dc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 001e0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001e4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 001e8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001ec
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001f8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001fc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00200
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00204
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00208
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0020c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00210
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00214
			 0xcb000000,                                                  // -8388608.0                                  /// 00218
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0021c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00220
			 0xbf028f5c,                                                  // -0.51                                       /// 00224
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00228
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0022c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00230
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00234
			 0x0e000003,                                                  // Trailing 1s:                                /// 00238
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0023c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00240
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00244
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0024c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00250
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00254
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00258
			 0xcb000000,                                                  // -8388608.0                                  /// 0025c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00260
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00264
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00268
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0026c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00270
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00274
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0027c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00280
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00284
			 0xffc00001,                                                  // -signaling NaN                              /// 00288
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0028c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00290
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00294
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00298
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0029c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002a0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002a4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002a8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002ac
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002b4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 002b8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002bc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0x0c400000,                                                  // Leading 1s:                                 /// 002c4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002c8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002cc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 002d4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002dc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002e0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002e8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ec
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 002f0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 002f4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002f8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 002fc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00300
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00304
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00308
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0030c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00310
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00318
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00320
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00324
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00328
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0032c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00330
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00338
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0033c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00344
			 0x7fc00001,                                                  // signaling NaN                               /// 00348
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0034c
			 0xff800000,                                                  // -inf                                        /// 00350
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00354
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00358
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00360
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00364
			 0x80011111,                                                  // -9.7958E-41                                 /// 00368
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0036c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00370
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00374
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00378
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0037c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00384
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00388
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0038c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00390
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00394
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00398
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x00000000,                                                  // zero                                        /// 003a0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 003a4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003a8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003b0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003b4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003b8
			 0x0e000007,                                                  // Trailing 1s:                                /// 003bc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003c0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003c4
			 0x7fc00001,                                                  // signaling NaN                               /// 003c8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003cc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 003d4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003d8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003dc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003e0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003e8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 003ec
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 003f8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00404
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00408
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00410
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00414
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00418
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0041c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00420
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00424
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00428
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00430
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00434
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00438
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0043c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00440
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00444
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00448
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0044c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00450
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00454
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0045c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00460
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00464
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00468
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0046c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00470
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00474
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00478
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0047c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00480
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00484
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00488
			 0xff800000,                                                  // -inf                                        /// 0048c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00498
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0049c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 004a0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 004a4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004b0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004b4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004b8
			 0xffc00001,                                                  // -signaling NaN                              /// 004bc
			 0xff800000,                                                  // -inf                                        /// 004c0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004c4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 004c8
			 0x55555555,                                                  // 4 random values                             /// 004cc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 004d0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004d4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 004e0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004ec
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 004f0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 004f4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004f8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 004fc
			 0x00000000,                                                  // zero                                        /// 00500
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00504
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0050c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00510
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00514
			 0x4b000000,                                                  // 8388608.0                                   /// 00518
			 0x33333333,                                                  // 4 random values                             /// 0051c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00520
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00524
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00528
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0052c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00530
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00534
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00538
			 0x00000000,                                                  // zero                                        /// 0053c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00540
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00544
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00548
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0054c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00550
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00554
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00558
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0055c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00560
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00564
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00568
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0056c
			 0xbf028f5c,                                                  // -0.51                                       /// 00570
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00574
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00578
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0057c
			 0x33333333,                                                  // 4 random values                             /// 00580
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00584
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00588
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0x4b000000,                                                  // 8388608.0                                   /// 00590
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00594
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00598
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0059c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005a4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 005a8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005ac
			 0x0e001fff,                                                  // Trailing 1s:                                /// 005b0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 005b4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 005b8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 005bc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 005c0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005c4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005c8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005cc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 005d0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 005d4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 005d8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 005dc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005e4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 005e8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005f0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 005f4
			 0x0e000001,                                                  // Trailing 1s:                                /// 005f8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 005fc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00600
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00608
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0060c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00610
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00614
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00618
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0061c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00620
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00624
			 0x80011111,                                                  // -9.7958E-41                                 /// 00628
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0062c
			 0xffc00001,                                                  // -signaling NaN                              /// 00630
			 0x7fc00001,                                                  // signaling NaN                               /// 00634
			 0x7f800000,                                                  // inf                                         /// 00638
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0063c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00640
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00644
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0064c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00654
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00658
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0065c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00660
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00668
			 0x0c700000,                                                  // Leading 1s:                                 /// 0066c
			 0x80000000,                                                  // -zero                                       /// 00670
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00674
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00678
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0067c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00680
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00684
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00688
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0068c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00690
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00694
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00698
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0069c
			 0x4b000000,                                                  // 8388608.0                                   /// 006a0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 006a4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 006ac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 006b8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 006bc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 006c0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 006c4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006cc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006d8
			 0xbf028f5c,                                                  // -0.51                                       /// 006dc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 006e0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006ec
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006f0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006fc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00700
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00708
			 0x3f028f5c,                                                  // 0.51                                        /// 0070c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00710
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00714
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00718
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0071c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00720
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00724
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00728
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0072c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00734
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00738
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0073c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00740
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0xcb000000,                                                  // -8388608.0                                  /// 00748
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0074c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00750
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00754
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00758
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0075c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00760
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00764
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00768
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0076c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00770
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00774
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00778
			 0x33333333,                                                  // 4 random values                             /// 0077c
			 0xcb000000,                                                  // -8388608.0                                  /// 00780
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00788
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0xffc00001,                                                  // -signaling NaN                              /// 00790
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00794
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00798
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0079c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 007a0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 007a8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ac
			 0xffc00000,                                                  // -cquiet NaN                                 /// 007b0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0x7f800000,                                                  // inf                                         /// 007b8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007bc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007c0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007c4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007c8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007cc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 007d0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007d4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007d8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007dc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007e4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007f0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007f8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 007fc
			 0x3f028f5c,                                                  // 0.51                                        /// 00800
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00804
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00808
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0080c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00810
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00814
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00818
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0081c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00820
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00824
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00828
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0082c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00834
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00838
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0083c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00848
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0084c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00850
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00854
			 0x0e000001,                                                  // Trailing 1s:                                /// 00858
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0085c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00864
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00868
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0086c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00874
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00878
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00884
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00888
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0088c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00894
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00898
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0089c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008a0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x0e000001,                                                  // Trailing 1s:                                /// 008a8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008ac
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 008b4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008bc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008c0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008c4
			 0x3f028f5c,                                                  // 0.51                                        /// 008c8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008cc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 008d0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 008d4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008d8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008e0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008e8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 008ec
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 008f0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008f4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008f8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008fc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00900
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00904
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00908
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0090c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00914
			 0x0e000001,                                                  // Trailing 1s:                                /// 00918
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0091c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00924
			 0xff800000,                                                  // -inf                                        /// 00928
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0092c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00930
			 0x7f800000,                                                  // inf                                         /// 00934
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00938
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0093c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00940
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00944
			 0x4b000000,                                                  // 8388608.0                                   /// 00948
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0094c
			 0xff800000,                                                  // -inf                                        /// 00950
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00954
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00958
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0095c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00960
			 0x3f028f5c,                                                  // 0.51                                        /// 00964
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00968
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0096c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00970
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00974
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0097c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00980
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00984
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00990
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00994
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00998
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0099c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 009a0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 009a4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 009a8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 009ac
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009b8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009bc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 009c0
			 0xffc00001,                                                  // -signaling NaN                              /// 009c4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 009c8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009cc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009d0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009d4
			 0xff800000,                                                  // -inf                                        /// 009d8
			 0x00000000,                                                  // zero                                        /// 009dc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 009e0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009e4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 009e8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ec
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009f0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009f4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009f8
			 0x33333333,                                                  // 4 random values                             /// 009fc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a00
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a04
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a08
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a0c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a10
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a14
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a18
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a1c
			 0xcb000000,                                                  // -8388608.0                                  /// 00a20
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a24
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a28
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a2c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a30
			 0x7fc00001,                                                  // signaling NaN                               /// 00a34
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a38
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a3c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a44
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a48
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a4c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a50
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a58
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a60
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a64
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a68
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0x3f028f5c,                                                  // 0.51                                        /// 00a70
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a74
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a78
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a7c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a80
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a84
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a88
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a8c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a94
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a98
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a9c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00aa0
			 0x7f800000,                                                  // inf                                         /// 00aa4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00aa8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00aac
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ab4
			 0xffc00001,                                                  // -signaling NaN                              /// 00ab8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00abc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ac0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ac4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00acc
			 0x4b000000,                                                  // 8388608.0                                   /// 00ad0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ad8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00adc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ae0
			 0x3f028f5c,                                                  // 0.51                                        /// 00ae4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ae8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00aec
			 0x00011111,                                                  // 9.7958E-41                                  /// 00af0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00af4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00af8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00afc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b00
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b04
			 0x3f028f5c,                                                  // 0.51                                        /// 00b08
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b0c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b10
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b14
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b18
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b1c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b20
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b24
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b28
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b2c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b30
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b34
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b38
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b3c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b40
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b44
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b48
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b4c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b50
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b54
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b58
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b5c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b60
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b64
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b68
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b6c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b70
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b74
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b78
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b7c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b84
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b88
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b8c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b90
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b94
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b98
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b9c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ba0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x3f028f5c,                                                  // 0.51                                        /// 00ba8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00bac
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bb0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00bb8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00bc0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00bc8
			 0x00000000,                                                  // zero                                        /// 00bcc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00bd0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00bd4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00bd8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00bdc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00be0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00be4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00be8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00bec
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00bf0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00bf4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00bf8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c00
			 0x55555555,                                                  // 4 random values                             /// 00c04
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0xffc00001,                                                  // -signaling NaN                              /// 00c0c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c10
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c14
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c18
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c1c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c24
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c28
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c30
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c34
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c38
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c3c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c40
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c44
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c48
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c4c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c50
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c54
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c58
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c60
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c64
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c68
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c6c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c70
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c74
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c78
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c80
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c84
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c88
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c8c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c90
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c98
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c9c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ca0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ca8
			 0x7fc00001,                                                  // signaling NaN                               /// 00cac
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00cb0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00cb4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00cb8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00cbc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc0
			 0x33333333,                                                  // 4 random values                             /// 00cc4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ccc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00cd0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00cd4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00cd8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00cdc
			 0x00000000,                                                  // zero                                        /// 00ce0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ce8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00cec
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00cf0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00cf4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00cf8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00cfc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d00
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d04
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d08
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d0c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d10
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d1c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d20
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d24
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d28
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d2c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d30
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d34
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d38
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d3c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d40
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d44
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d48
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d4c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d50
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d54
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d58
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d5c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d64
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d70
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d74
			 0xffc00001,                                                  // -signaling NaN                              /// 00d78
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d7c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d80
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d84
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d8c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d90
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d94
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d98
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d9c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00da0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00da4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00dac
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00db0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00db8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00dbc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00dc0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00dc8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00dcc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00dd4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00dd8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ddc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00de0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00de4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00de8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00dec
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00df0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00df4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00df8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00dfc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e00
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e0c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e10
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e14
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e18
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e1c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e20
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e24
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e28
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e30
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e34
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e38
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e40
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e44
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e48
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e4c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e50
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e54
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e5c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e60
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e64
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e68
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e6c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e70
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e78
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e7c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e80
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e84
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e88
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e8c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e94
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e98
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e9c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ea4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00eb4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00eb8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ebc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ec0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ec4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ec8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ecc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ed0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ed4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ed8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00edc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ee0
			 0xcb000000,                                                  // -8388608.0                                  /// 00ee4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ee8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00eec
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ef0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ef4
			 0xbf028f5c,                                                  // -0.51                                       /// 00ef8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00efc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f00
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f04
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f08
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f0c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f10
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f14
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f18
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f1c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f20
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f24
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f28
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f2c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f30
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f34
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f3c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f40
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f44
			 0x33333333,                                                  // 4 random values                             /// 00f48
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f4c
			 0x80000000,                                                  // -zero                                       /// 00f50
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f58
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f5c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f60
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f64
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f68
			 0x33333333,                                                  // 4 random values                             /// 00f6c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f70
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f74
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f78
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f7c
			 0x00000000,                                                  // zero                                        /// 00f80
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f84
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f88
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f8c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f90
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f94
			 0xbf028f5c,                                                  // -0.51                                       /// 00f98
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f9c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x55555555,                                                  // 4 random values                             /// 00fa8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00fac
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00fb8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00fbc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00fc0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0xff800000,                                                  // -inf                                        /// 00fc8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00fcc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00fd0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00fd8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00fdc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fe4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ff0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ff4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ff8
			 0x33333333                                                  // 4 random values                             /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0xaeaf1f6e, /// 0x0
			 0xc7fa11d1, /// 0x4
			 0x771cdceb, /// 0x8
			 0xf282b6c5, /// 0xc
			 0xf3d436b6, /// 0x10
			 0x7a3ffc5b, /// 0x14
			 0x68342a97, /// 0x18
			 0x82418b4a, /// 0x1c
			 0x45effeda, /// 0x20
			 0x4f325ac3, /// 0x24
			 0x5f51ecca, /// 0x28
			 0xbb377637, /// 0x2c
			 0xac296b26, /// 0x30
			 0xc3762b7c, /// 0x34
			 0x3e3c3d74, /// 0x38
			 0x3ca6e2d9, /// 0x3c
			 0x222f352a, /// 0x40
			 0x015bc880, /// 0x44
			 0x761074a9, /// 0x48
			 0x9f899534, /// 0x4c
			 0x8fc3fc43, /// 0x50
			 0xdb5f9d7b, /// 0x54
			 0x4a34ea99, /// 0x58
			 0xab05450b, /// 0x5c
			 0x7a6c0b9a, /// 0x60
			 0x1b2d7e4d, /// 0x64
			 0x88b00c1f, /// 0x68
			 0xd664e735, /// 0x6c
			 0x1b5eb9a9, /// 0x70
			 0xaaefa8c1, /// 0x74
			 0xa66b3451, /// 0x78
			 0xba22de2f, /// 0x7c
			 0xb47071cb, /// 0x80
			 0xa4143cab, /// 0x84
			 0x386f6b68, /// 0x88
			 0x0dddb7af, /// 0x8c
			 0xe9faf33a, /// 0x90
			 0x3336864a, /// 0x94
			 0xb2862374, /// 0x98
			 0x87c817e5, /// 0x9c
			 0xe813ee0b, /// 0xa0
			 0x8ef95c12, /// 0xa4
			 0x224d831b, /// 0xa8
			 0xd899cefe, /// 0xac
			 0x28f094f9, /// 0xb0
			 0x28dbb2f8, /// 0xb4
			 0x30e2437b, /// 0xb8
			 0x847e30dc, /// 0xbc
			 0x2ec04df6, /// 0xc0
			 0x0b68f2a3, /// 0xc4
			 0x65f0adec, /// 0xc8
			 0x65ffcfbe, /// 0xcc
			 0x5847a466, /// 0xd0
			 0x5ebc2dd5, /// 0xd4
			 0x8e07e733, /// 0xd8
			 0x7449ed76, /// 0xdc
			 0x686f700c, /// 0xe0
			 0xa1bb2657, /// 0xe4
			 0xf32dd790, /// 0xe8
			 0x6d0fb544, /// 0xec
			 0xe00d3b8b, /// 0xf0
			 0x54ed64b1, /// 0xf4
			 0xc328067d, /// 0xf8
			 0xf3ea53aa, /// 0xfc
			 0x5020f42c, /// 0x100
			 0x5ab46235, /// 0x104
			 0x4e4e9576, /// 0x108
			 0x7ace043c, /// 0x10c
			 0xd0759351, /// 0x110
			 0xb6fab05a, /// 0x114
			 0x3a6d11f7, /// 0x118
			 0x59ffa9bd, /// 0x11c
			 0x246a4cd6, /// 0x120
			 0x435508f3, /// 0x124
			 0x669a9462, /// 0x128
			 0x8dab5514, /// 0x12c
			 0x2bdd8eb3, /// 0x130
			 0x137ad323, /// 0x134
			 0x1ed0e272, /// 0x138
			 0x568778c7, /// 0x13c
			 0x354503b0, /// 0x140
			 0xca211d26, /// 0x144
			 0x33cc1e2f, /// 0x148
			 0x622404de, /// 0x14c
			 0xa451bdfc, /// 0x150
			 0xa07ccc4b, /// 0x154
			 0xd909c6e3, /// 0x158
			 0xf3f4ece5, /// 0x15c
			 0xe906805c, /// 0x160
			 0x0e5924ef, /// 0x164
			 0xc87029f6, /// 0x168
			 0x85331ae1, /// 0x16c
			 0x07d1295a, /// 0x170
			 0xa64c3493, /// 0x174
			 0x6f040dd8, /// 0x178
			 0xce077eaa, /// 0x17c
			 0x9ea99924, /// 0x180
			 0x431f66b2, /// 0x184
			 0x24c3ecb2, /// 0x188
			 0xdb82cf62, /// 0x18c
			 0x0af89554, /// 0x190
			 0x8e24d61f, /// 0x194
			 0x421dca3e, /// 0x198
			 0x66d51f7e, /// 0x19c
			 0xa710ccab, /// 0x1a0
			 0x378e1872, /// 0x1a4
			 0xddc81388, /// 0x1a8
			 0xb71f5e9f, /// 0x1ac
			 0x96f85be0, /// 0x1b0
			 0xf8f542a5, /// 0x1b4
			 0x1daff046, /// 0x1b8
			 0x0a48c4cf, /// 0x1bc
			 0x344462f0, /// 0x1c0
			 0x71840a7c, /// 0x1c4
			 0x159cc990, /// 0x1c8
			 0x233cb70d, /// 0x1cc
			 0x63f87243, /// 0x1d0
			 0x3106da85, /// 0x1d4
			 0xabdcea81, /// 0x1d8
			 0x307923f2, /// 0x1dc
			 0x5c8c09e6, /// 0x1e0
			 0xc4f740ee, /// 0x1e4
			 0x9afd982b, /// 0x1e8
			 0x46890fa1, /// 0x1ec
			 0x4084cc5b, /// 0x1f0
			 0x456b6dea, /// 0x1f4
			 0xad1f52e0, /// 0x1f8
			 0x362c4a7d, /// 0x1fc
			 0xc64cdd54, /// 0x200
			 0x46452563, /// 0x204
			 0xf4a5ab06, /// 0x208
			 0xd5e3e47f, /// 0x20c
			 0x1f8d5680, /// 0x210
			 0xf4a19f17, /// 0x214
			 0x143f25b8, /// 0x218
			 0x897e285a, /// 0x21c
			 0xa4e09226, /// 0x220
			 0x38a1ba69, /// 0x224
			 0x3b9b9433, /// 0x228
			 0x0e5acc95, /// 0x22c
			 0x5fc17145, /// 0x230
			 0x8ca4b69c, /// 0x234
			 0x6cf38ca9, /// 0x238
			 0x3de55558, /// 0x23c
			 0xb5bb480e, /// 0x240
			 0xdc0bd43f, /// 0x244
			 0x2bc0c3b3, /// 0x248
			 0xbe83139d, /// 0x24c
			 0x27c42329, /// 0x250
			 0x85b8f7a0, /// 0x254
			 0xaac11178, /// 0x258
			 0xd4292655, /// 0x25c
			 0xbcef4b03, /// 0x260
			 0x24a2a5b4, /// 0x264
			 0x6b1e3ee7, /// 0x268
			 0x8f39294f, /// 0x26c
			 0xde0ed443, /// 0x270
			 0xeea9b226, /// 0x274
			 0xbeabe7ec, /// 0x278
			 0x5eba9a49, /// 0x27c
			 0x8f9de68c, /// 0x280
			 0xfde4167d, /// 0x284
			 0xb1c49dc5, /// 0x288
			 0xb4d4d783, /// 0x28c
			 0x22a20eb7, /// 0x290
			 0x8f28fb82, /// 0x294
			 0xedae17f8, /// 0x298
			 0x9f52efd2, /// 0x29c
			 0xa23c2abc, /// 0x2a0
			 0x0f07d128, /// 0x2a4
			 0x3bfb938a, /// 0x2a8
			 0x6ed6e27a, /// 0x2ac
			 0x377e3758, /// 0x2b0
			 0x03511d82, /// 0x2b4
			 0x5f9eb513, /// 0x2b8
			 0xb760bf05, /// 0x2bc
			 0x61dfd662, /// 0x2c0
			 0xf51ae5b3, /// 0x2c4
			 0xed8713c8, /// 0x2c8
			 0xe2575e14, /// 0x2cc
			 0x23e53546, /// 0x2d0
			 0x772b9b10, /// 0x2d4
			 0x841cfa2a, /// 0x2d8
			 0xeeecd239, /// 0x2dc
			 0x6d419a8c, /// 0x2e0
			 0xe6ca3e68, /// 0x2e4
			 0xbc12e8ad, /// 0x2e8
			 0xd8146897, /// 0x2ec
			 0x887b3cb0, /// 0x2f0
			 0x99b6ad4e, /// 0x2f4
			 0xb2787ac5, /// 0x2f8
			 0x11d3ae28, /// 0x2fc
			 0x47f3a655, /// 0x300
			 0x9633f776, /// 0x304
			 0x9cc89904, /// 0x308
			 0xb30f7c71, /// 0x30c
			 0xefda267d, /// 0x310
			 0x7549876f, /// 0x314
			 0x0701d0d0, /// 0x318
			 0x29a93f13, /// 0x31c
			 0xc15c5f09, /// 0x320
			 0xe6509bb6, /// 0x324
			 0xe11ece82, /// 0x328
			 0x987da574, /// 0x32c
			 0x32cf2b10, /// 0x330
			 0xe2307e5d, /// 0x334
			 0xcc84a058, /// 0x338
			 0xe444f7df, /// 0x33c
			 0x530ba2a5, /// 0x340
			 0x1f432a86, /// 0x344
			 0xeff0e720, /// 0x348
			 0xd8ecc540, /// 0x34c
			 0xcfd44d18, /// 0x350
			 0x74a657ba, /// 0x354
			 0x2007d4fb, /// 0x358
			 0x32ae1363, /// 0x35c
			 0x12842f92, /// 0x360
			 0xed61b030, /// 0x364
			 0x719cfab9, /// 0x368
			 0xd5bde3d2, /// 0x36c
			 0xc31e2a6c, /// 0x370
			 0x126764bd, /// 0x374
			 0x63f04ab1, /// 0x378
			 0x7b30e646, /// 0x37c
			 0x2445335e, /// 0x380
			 0xe68fda6a, /// 0x384
			 0x067d384d, /// 0x388
			 0x6f552dfe, /// 0x38c
			 0xd54368b4, /// 0x390
			 0xa7371888, /// 0x394
			 0xc25b83ab, /// 0x398
			 0x2663248b, /// 0x39c
			 0x7a22cec1, /// 0x3a0
			 0x3fb57f22, /// 0x3a4
			 0xeb1f18ff, /// 0x3a8
			 0xda222c02, /// 0x3ac
			 0x953c9c9b, /// 0x3b0
			 0xddf78347, /// 0x3b4
			 0x4abe3a8c, /// 0x3b8
			 0xacc4a0ae, /// 0x3bc
			 0x5d83e9ce, /// 0x3c0
			 0xd7ceb0bb, /// 0x3c4
			 0xba378ef6, /// 0x3c8
			 0x812e2479, /// 0x3cc
			 0x9ff0958f, /// 0x3d0
			 0x2266c6c7, /// 0x3d4
			 0x00356558, /// 0x3d8
			 0x1b78dc54, /// 0x3dc
			 0x033ca150, /// 0x3e0
			 0xd32ab6ae, /// 0x3e4
			 0xce7e6066, /// 0x3e8
			 0x04592a79, /// 0x3ec
			 0xf0eee5bd, /// 0x3f0
			 0xd761e211, /// 0x3f4
			 0x3d72e4a3, /// 0x3f8
			 0x021d2d38, /// 0x3fc
			 0x43be2257, /// 0x400
			 0xa32d8bda, /// 0x404
			 0xc3efd4ce, /// 0x408
			 0x3e62846e, /// 0x40c
			 0x8613ea74, /// 0x410
			 0xe35eb347, /// 0x414
			 0x7c67712a, /// 0x418
			 0xb7c9c3c0, /// 0x41c
			 0x3b611f2a, /// 0x420
			 0x1fb3434a, /// 0x424
			 0xeeb79a5a, /// 0x428
			 0x9270f18a, /// 0x42c
			 0x5370c186, /// 0x430
			 0xae3c449a, /// 0x434
			 0x77599bab, /// 0x438
			 0x0cebb0ce, /// 0x43c
			 0x95ddc021, /// 0x440
			 0x1d13b9fc, /// 0x444
			 0x4ced299b, /// 0x448
			 0xdd845bc0, /// 0x44c
			 0x103c711e, /// 0x450
			 0x5956941a, /// 0x454
			 0x79cffe1a, /// 0x458
			 0x6387a279, /// 0x45c
			 0x0ba6447f, /// 0x460
			 0xc6e7a808, /// 0x464
			 0x0d268357, /// 0x468
			 0x89c77e19, /// 0x46c
			 0x59d11ef9, /// 0x470
			 0xa19aa486, /// 0x474
			 0xf4007ae1, /// 0x478
			 0x08bdbdac, /// 0x47c
			 0x3bdadf02, /// 0x480
			 0xd3accfd9, /// 0x484
			 0xfddfed09, /// 0x488
			 0x0c6832f0, /// 0x48c
			 0xd8b9e4f3, /// 0x490
			 0x0f0d789d, /// 0x494
			 0xa4e7f20f, /// 0x498
			 0x00b484ca, /// 0x49c
			 0xafe4cb0f, /// 0x4a0
			 0xf9e86852, /// 0x4a4
			 0xf0f14e7f, /// 0x4a8
			 0xb3e0a024, /// 0x4ac
			 0x724acb36, /// 0x4b0
			 0xf31a04cc, /// 0x4b4
			 0x32922b1f, /// 0x4b8
			 0x131df8b7, /// 0x4bc
			 0x6c4f2dc3, /// 0x4c0
			 0x10c25fe4, /// 0x4c4
			 0xe75b830c, /// 0x4c8
			 0xc4df2f4f, /// 0x4cc
			 0x5f489c1c, /// 0x4d0
			 0xcaa211b2, /// 0x4d4
			 0x9866630c, /// 0x4d8
			 0xd29f20fb, /// 0x4dc
			 0xe9ea92be, /// 0x4e0
			 0xab7d0069, /// 0x4e4
			 0x82022a19, /// 0x4e8
			 0xf96b1491, /// 0x4ec
			 0x2dcbe71a, /// 0x4f0
			 0x7dc7fac2, /// 0x4f4
			 0xb4d99027, /// 0x4f8
			 0x11025af6, /// 0x4fc
			 0xf2ee1588, /// 0x500
			 0xe7537845, /// 0x504
			 0xaf1ec3f6, /// 0x508
			 0xac1b479e, /// 0x50c
			 0x7b9f26cf, /// 0x510
			 0xceecea3f, /// 0x514
			 0xcfcffbf4, /// 0x518
			 0x11f6997c, /// 0x51c
			 0xedc04617, /// 0x520
			 0x7911e52c, /// 0x524
			 0xa1431647, /// 0x528
			 0x31377177, /// 0x52c
			 0x51d0c538, /// 0x530
			 0x1e8126c1, /// 0x534
			 0x09f61216, /// 0x538
			 0xcf01f741, /// 0x53c
			 0xc5d66011, /// 0x540
			 0x0b4b1de8, /// 0x544
			 0xf8587565, /// 0x548
			 0x48831430, /// 0x54c
			 0x858ab648, /// 0x550
			 0xa528f32a, /// 0x554
			 0xc1f2c2a1, /// 0x558
			 0xf48d679a, /// 0x55c
			 0x0b9bfd76, /// 0x560
			 0xe3b72eb0, /// 0x564
			 0xe8880486, /// 0x568
			 0x5a00949e, /// 0x56c
			 0x0f1b7cb0, /// 0x570
			 0x9250589a, /// 0x574
			 0xbb2ff1ac, /// 0x578
			 0x4ccb269e, /// 0x57c
			 0x54ca4613, /// 0x580
			 0xa2357bae, /// 0x584
			 0x9e752924, /// 0x588
			 0x50c2dc87, /// 0x58c
			 0xf0920d37, /// 0x590
			 0x1a53fe08, /// 0x594
			 0xc5f63a00, /// 0x598
			 0x9aa0181b, /// 0x59c
			 0x19dd28a1, /// 0x5a0
			 0x1aad591e, /// 0x5a4
			 0x8ca7ab93, /// 0x5a8
			 0x658a24f6, /// 0x5ac
			 0x0cf8b64c, /// 0x5b0
			 0x62643959, /// 0x5b4
			 0x39bbdd09, /// 0x5b8
			 0xb44fa380, /// 0x5bc
			 0x22e2658f, /// 0x5c0
			 0xb9d264a6, /// 0x5c4
			 0x71741d4f, /// 0x5c8
			 0xb9405b61, /// 0x5cc
			 0xe1b268e7, /// 0x5d0
			 0xfe9ab101, /// 0x5d4
			 0x1d029efb, /// 0x5d8
			 0x3050e11a, /// 0x5dc
			 0x2f96100d, /// 0x5e0
			 0xa78799af, /// 0x5e4
			 0xa8be72e8, /// 0x5e8
			 0x105fe16f, /// 0x5ec
			 0x62f0210c, /// 0x5f0
			 0x10593056, /// 0x5f4
			 0xc6287df8, /// 0x5f8
			 0xa656912e, /// 0x5fc
			 0x0f613e82, /// 0x600
			 0x9a6bed82, /// 0x604
			 0xf2b16a7d, /// 0x608
			 0x6a692391, /// 0x60c
			 0x6326fe0e, /// 0x610
			 0xe8c3e491, /// 0x614
			 0x94939c12, /// 0x618
			 0xd12f9392, /// 0x61c
			 0x668c8fd1, /// 0x620
			 0x56092b5c, /// 0x624
			 0x868ccd9c, /// 0x628
			 0x6e89613b, /// 0x62c
			 0x3d3c3278, /// 0x630
			 0x1549679e, /// 0x634
			 0x5b9b1f5d, /// 0x638
			 0xcb820978, /// 0x63c
			 0x93c27af7, /// 0x640
			 0x08fda290, /// 0x644
			 0x4393d50b, /// 0x648
			 0xb749a4ed, /// 0x64c
			 0xfa2857c9, /// 0x650
			 0x36e94a9c, /// 0x654
			 0xc70fb636, /// 0x658
			 0xbbeba7c3, /// 0x65c
			 0x1b10dd38, /// 0x660
			 0x4e5916fe, /// 0x664
			 0xafa793bd, /// 0x668
			 0xceeeecb3, /// 0x66c
			 0xd4ecb0c5, /// 0x670
			 0xa663aa77, /// 0x674
			 0xa2f80e3c, /// 0x678
			 0xe4f03d9b, /// 0x67c
			 0xf557203f, /// 0x680
			 0x2051d716, /// 0x684
			 0x3d9f44b8, /// 0x688
			 0x53956eb7, /// 0x68c
			 0xa36658db, /// 0x690
			 0x605e1655, /// 0x694
			 0x0e17747e, /// 0x698
			 0xb5633535, /// 0x69c
			 0x08424cff, /// 0x6a0
			 0x544fa1a7, /// 0x6a4
			 0x12b991ec, /// 0x6a8
			 0x0f857c91, /// 0x6ac
			 0x4455a005, /// 0x6b0
			 0x69ee3551, /// 0x6b4
			 0xf0ef12db, /// 0x6b8
			 0x8126a5d2, /// 0x6bc
			 0x074b00c0, /// 0x6c0
			 0x167a4c6a, /// 0x6c4
			 0xd2d8c825, /// 0x6c8
			 0x4f21a255, /// 0x6cc
			 0x643aa916, /// 0x6d0
			 0x708efde3, /// 0x6d4
			 0x70db55a3, /// 0x6d8
			 0xa3eec909, /// 0x6dc
			 0xb2355d6d, /// 0x6e0
			 0x0d8689cb, /// 0x6e4
			 0xc4c0df2b, /// 0x6e8
			 0x6f017a59, /// 0x6ec
			 0x01a38935, /// 0x6f0
			 0x22921970, /// 0x6f4
			 0x1c2772aa, /// 0x6f8
			 0x6e7154e6, /// 0x6fc
			 0xec506dff, /// 0x700
			 0x59f8cdaa, /// 0x704
			 0x09d95c57, /// 0x708
			 0xb2d5c695, /// 0x70c
			 0x1c0ee550, /// 0x710
			 0x2d6bf436, /// 0x714
			 0x9d966080, /// 0x718
			 0xeb388c7b, /// 0x71c
			 0x6217e53d, /// 0x720
			 0x60d05e37, /// 0x724
			 0xb854833d, /// 0x728
			 0x20949f88, /// 0x72c
			 0xcec00c58, /// 0x730
			 0x3869135d, /// 0x734
			 0xef829da6, /// 0x738
			 0x5e16b7ee, /// 0x73c
			 0x2d1e40ad, /// 0x740
			 0x4c7f7c0c, /// 0x744
			 0x31365800, /// 0x748
			 0x29b487ff, /// 0x74c
			 0x56edaa92, /// 0x750
			 0x99f10f1f, /// 0x754
			 0x9723c6fc, /// 0x758
			 0x0351bac2, /// 0x75c
			 0xa9a6b6b4, /// 0x760
			 0x525132c1, /// 0x764
			 0x4b5cfc16, /// 0x768
			 0xcc226e75, /// 0x76c
			 0x3e70bc50, /// 0x770
			 0x8fcb7984, /// 0x774
			 0x04f6fafb, /// 0x778
			 0x8e92bba0, /// 0x77c
			 0xe1b2a032, /// 0x780
			 0xb9a8acea, /// 0x784
			 0xbab21cc4, /// 0x788
			 0x62986493, /// 0x78c
			 0x419639a6, /// 0x790
			 0x69cd83f4, /// 0x794
			 0x796488a8, /// 0x798
			 0x5033ca8f, /// 0x79c
			 0x2f0e476f, /// 0x7a0
			 0xc0dddf8c, /// 0x7a4
			 0x4e84566f, /// 0x7a8
			 0x400b997d, /// 0x7ac
			 0x290c02aa, /// 0x7b0
			 0x6fff6aee, /// 0x7b4
			 0xc54f4af8, /// 0x7b8
			 0x1a9f0124, /// 0x7bc
			 0x6109ad51, /// 0x7c0
			 0x6752d212, /// 0x7c4
			 0x8543be79, /// 0x7c8
			 0x91522f63, /// 0x7cc
			 0x52247394, /// 0x7d0
			 0x1599c44a, /// 0x7d4
			 0xc3723edd, /// 0x7d8
			 0xe353104d, /// 0x7dc
			 0xd4c78584, /// 0x7e0
			 0x73fce1f8, /// 0x7e4
			 0x82cb66a5, /// 0x7e8
			 0x99adc095, /// 0x7ec
			 0x41fbd90c, /// 0x7f0
			 0xeafe11de, /// 0x7f4
			 0xdcac0562, /// 0x7f8
			 0x7a2d3958, /// 0x7fc
			 0x0b2d6b7c, /// 0x800
			 0x5da1af5b, /// 0x804
			 0xda177f80, /// 0x808
			 0x4284a6bc, /// 0x80c
			 0x6aa7a45b, /// 0x810
			 0x10ebfdb6, /// 0x814
			 0xbdf4a3e5, /// 0x818
			 0x078b8626, /// 0x81c
			 0xb2bb1333, /// 0x820
			 0x8510a59e, /// 0x824
			 0x2664eca8, /// 0x828
			 0x899f191c, /// 0x82c
			 0xb105e46a, /// 0x830
			 0x6ea1e0d3, /// 0x834
			 0x23d5a3cd, /// 0x838
			 0x0fd983ea, /// 0x83c
			 0xff14bb90, /// 0x840
			 0xca84aef2, /// 0x844
			 0x506234b3, /// 0x848
			 0xfe33c74c, /// 0x84c
			 0x2cc719b6, /// 0x850
			 0x2777c292, /// 0x854
			 0x73e065a6, /// 0x858
			 0xb31e3be2, /// 0x85c
			 0xf98cc618, /// 0x860
			 0xcb2e6d50, /// 0x864
			 0xf22795df, /// 0x868
			 0x65860aa3, /// 0x86c
			 0x7d42298a, /// 0x870
			 0x4dcb2f8f, /// 0x874
			 0x5227fca9, /// 0x878
			 0x43a6ea5d, /// 0x87c
			 0x9090c443, /// 0x880
			 0xfbf1fd21, /// 0x884
			 0x473e7a45, /// 0x888
			 0x05f7f91f, /// 0x88c
			 0xbc2319a0, /// 0x890
			 0x4122971e, /// 0x894
			 0x0b37219f, /// 0x898
			 0xa10cd849, /// 0x89c
			 0x447c2509, /// 0x8a0
			 0xdfc2dda5, /// 0x8a4
			 0x5e76f8fe, /// 0x8a8
			 0x151c01a9, /// 0x8ac
			 0xdc787aeb, /// 0x8b0
			 0x25f1febe, /// 0x8b4
			 0x6bea5eac, /// 0x8b8
			 0x36d6fc45, /// 0x8bc
			 0xf6973d43, /// 0x8c0
			 0xfa726d20, /// 0x8c4
			 0xe79425b8, /// 0x8c8
			 0xf11ba44a, /// 0x8cc
			 0x6a8e3d6a, /// 0x8d0
			 0xee80d223, /// 0x8d4
			 0x21147ee8, /// 0x8d8
			 0x2d387519, /// 0x8dc
			 0x6c9f4c77, /// 0x8e0
			 0x4e2fe54e, /// 0x8e4
			 0xaea46cc2, /// 0x8e8
			 0xe1ad5856, /// 0x8ec
			 0x042630e5, /// 0x8f0
			 0xdce75f0e, /// 0x8f4
			 0x45047203, /// 0x8f8
			 0x96403f67, /// 0x8fc
			 0x1eadfc63, /// 0x900
			 0x245afb1c, /// 0x904
			 0x553f73ee, /// 0x908
			 0xeba16fef, /// 0x90c
			 0x56953f52, /// 0x910
			 0x923f3243, /// 0x914
			 0x588d1bf9, /// 0x918
			 0xe1c582ee, /// 0x91c
			 0xbc86dd25, /// 0x920
			 0x132478e2, /// 0x924
			 0xe7e45178, /// 0x928
			 0x728de364, /// 0x92c
			 0xae5e2c4b, /// 0x930
			 0xd3045145, /// 0x934
			 0x71b1d762, /// 0x938
			 0x8ce3ef81, /// 0x93c
			 0x7911387d, /// 0x940
			 0x61002f64, /// 0x944
			 0x848985ca, /// 0x948
			 0x3f602a19, /// 0x94c
			 0xdcf9d2cd, /// 0x950
			 0xccf2bf6d, /// 0x954
			 0x27d17b70, /// 0x958
			 0x8307aec8, /// 0x95c
			 0xb04b5636, /// 0x960
			 0x24bb39fa, /// 0x964
			 0xa2e52290, /// 0x968
			 0x22083532, /// 0x96c
			 0xf2a5aec7, /// 0x970
			 0x9e44437a, /// 0x974
			 0x024aa2a7, /// 0x978
			 0x52269f62, /// 0x97c
			 0xe3d0071f, /// 0x980
			 0x2cde2342, /// 0x984
			 0x02c95665, /// 0x988
			 0xf76ce043, /// 0x98c
			 0x38cc69d6, /// 0x990
			 0x7b5d491b, /// 0x994
			 0x424bdd17, /// 0x998
			 0x6e9b854a, /// 0x99c
			 0xe5f8b4d5, /// 0x9a0
			 0x14d6cfbf, /// 0x9a4
			 0xd59df1df, /// 0x9a8
			 0xb68b2859, /// 0x9ac
			 0x790e93e5, /// 0x9b0
			 0xd633ab7b, /// 0x9b4
			 0x4f19997b, /// 0x9b8
			 0xbe74eda9, /// 0x9bc
			 0xb846f6b1, /// 0x9c0
			 0xe5774886, /// 0x9c4
			 0xd1db20fc, /// 0x9c8
			 0xd8b2b2e9, /// 0x9cc
			 0xff5477d9, /// 0x9d0
			 0x7076f390, /// 0x9d4
			 0x315617f4, /// 0x9d8
			 0xb17e8eae, /// 0x9dc
			 0x6394253b, /// 0x9e0
			 0xce6e768b, /// 0x9e4
			 0x81b2a257, /// 0x9e8
			 0x2bcfec29, /// 0x9ec
			 0xd60205c9, /// 0x9f0
			 0x941c7ffa, /// 0x9f4
			 0x6cd098ad, /// 0x9f8
			 0x893fa619, /// 0x9fc
			 0xc4063ec7, /// 0xa00
			 0x914e1b64, /// 0xa04
			 0x31136035, /// 0xa08
			 0x7d827362, /// 0xa0c
			 0x61e37a16, /// 0xa10
			 0x2661c4c8, /// 0xa14
			 0xb9f710a8, /// 0xa18
			 0xf77b652b, /// 0xa1c
			 0x5dd7dcd9, /// 0xa20
			 0xcb266aa6, /// 0xa24
			 0x079aaa47, /// 0xa28
			 0xac86b368, /// 0xa2c
			 0x0919aff2, /// 0xa30
			 0x536c9a79, /// 0xa34
			 0x8bd80cc9, /// 0xa38
			 0x8f6d207f, /// 0xa3c
			 0xe3207f7d, /// 0xa40
			 0xba55ac56, /// 0xa44
			 0x6e05207b, /// 0xa48
			 0xb023a3f4, /// 0xa4c
			 0xdd0a8b7c, /// 0xa50
			 0x0af2cc59, /// 0xa54
			 0x8c48a471, /// 0xa58
			 0xdfa9dd2a, /// 0xa5c
			 0x8b1080ed, /// 0xa60
			 0x49ca9a33, /// 0xa64
			 0x1e0ac9fc, /// 0xa68
			 0xce8f6868, /// 0xa6c
			 0x0600f9c5, /// 0xa70
			 0x88662698, /// 0xa74
			 0xae4d2d82, /// 0xa78
			 0xdaced1d2, /// 0xa7c
			 0x45a3088e, /// 0xa80
			 0x66859173, /// 0xa84
			 0x73ae4eef, /// 0xa88
			 0x7b09ecd2, /// 0xa8c
			 0xbaa856a3, /// 0xa90
			 0x598bfad9, /// 0xa94
			 0xfc8c8fdc, /// 0xa98
			 0xc690c614, /// 0xa9c
			 0x0d270fef, /// 0xaa0
			 0x42f64fba, /// 0xaa4
			 0xa69e8d0c, /// 0xaa8
			 0x0ea92394, /// 0xaac
			 0x8e682b1b, /// 0xab0
			 0xb545a696, /// 0xab4
			 0x2f3aae67, /// 0xab8
			 0xdee14196, /// 0xabc
			 0xb76f0186, /// 0xac0
			 0x704ff49e, /// 0xac4
			 0xda598112, /// 0xac8
			 0xe18e3410, /// 0xacc
			 0xa2619c37, /// 0xad0
			 0xca0f8071, /// 0xad4
			 0xcc290a37, /// 0xad8
			 0x1c59d12c, /// 0xadc
			 0xf9015fc3, /// 0xae0
			 0x64d69dbc, /// 0xae4
			 0xfed4f20a, /// 0xae8
			 0x344bf2cc, /// 0xaec
			 0x6f687df3, /// 0xaf0
			 0x1a2be42c, /// 0xaf4
			 0xaa93b509, /// 0xaf8
			 0xec0f5061, /// 0xafc
			 0x630ff7be, /// 0xb00
			 0xc5a81b61, /// 0xb04
			 0x916d565c, /// 0xb08
			 0x752aaf5d, /// 0xb0c
			 0x9905828a, /// 0xb10
			 0x34691b98, /// 0xb14
			 0x5c1a8093, /// 0xb18
			 0x737b4ad6, /// 0xb1c
			 0x8236f3e6, /// 0xb20
			 0xe2e6f732, /// 0xb24
			 0x563c4211, /// 0xb28
			 0x38ac842f, /// 0xb2c
			 0xa5350b40, /// 0xb30
			 0xfdbdc3c3, /// 0xb34
			 0xb9f3a029, /// 0xb38
			 0x2d7e73c6, /// 0xb3c
			 0xffa42747, /// 0xb40
			 0x3e7dc84a, /// 0xb44
			 0xb047ed3e, /// 0xb48
			 0x2c13bba1, /// 0xb4c
			 0xa3861793, /// 0xb50
			 0xa938b389, /// 0xb54
			 0xc7c0e61a, /// 0xb58
			 0x30d66bb8, /// 0xb5c
			 0x56588c97, /// 0xb60
			 0xf97a5249, /// 0xb64
			 0xf121f847, /// 0xb68
			 0x3486deca, /// 0xb6c
			 0xf8561d59, /// 0xb70
			 0x47d48ddb, /// 0xb74
			 0x3a5374bf, /// 0xb78
			 0x9040d11d, /// 0xb7c
			 0xa7d5e905, /// 0xb80
			 0x27d41e94, /// 0xb84
			 0x43378a0e, /// 0xb88
			 0x9e7f41bb, /// 0xb8c
			 0x5851fc1e, /// 0xb90
			 0x7946a534, /// 0xb94
			 0xe70d0f1f, /// 0xb98
			 0x63f194c2, /// 0xb9c
			 0x3c53e48a, /// 0xba0
			 0x735fd9f6, /// 0xba4
			 0xcbae083b, /// 0xba8
			 0xfa1eb685, /// 0xbac
			 0x5ed86176, /// 0xbb0
			 0x898a676c, /// 0xbb4
			 0x64f14969, /// 0xbb8
			 0x611c7dec, /// 0xbbc
			 0x42fc6032, /// 0xbc0
			 0x46c6b54e, /// 0xbc4
			 0x03928756, /// 0xbc8
			 0xb0ab2510, /// 0xbcc
			 0x172890c4, /// 0xbd0
			 0x552d3e79, /// 0xbd4
			 0xeb3c4319, /// 0xbd8
			 0x33a52b55, /// 0xbdc
			 0xc2d6da3b, /// 0xbe0
			 0xca598379, /// 0xbe4
			 0x764416f3, /// 0xbe8
			 0x36600352, /// 0xbec
			 0x2a4fedfb, /// 0xbf0
			 0x5d46a3de, /// 0xbf4
			 0x3b33c806, /// 0xbf8
			 0xc3360554, /// 0xbfc
			 0x13e14f4d, /// 0xc00
			 0xff887428, /// 0xc04
			 0x7d065828, /// 0xc08
			 0x82b31ce5, /// 0xc0c
			 0x169038be, /// 0xc10
			 0x72437c0c, /// 0xc14
			 0x50ba4834, /// 0xc18
			 0x3981be99, /// 0xc1c
			 0x649d862e, /// 0xc20
			 0xc3f9f050, /// 0xc24
			 0x6f77a48c, /// 0xc28
			 0x7e188547, /// 0xc2c
			 0x352bb28b, /// 0xc30
			 0x683b65db, /// 0xc34
			 0x79d5669d, /// 0xc38
			 0x5afaee9a, /// 0xc3c
			 0x39780604, /// 0xc40
			 0x29293899, /// 0xc44
			 0xf403d2a7, /// 0xc48
			 0x137f7d40, /// 0xc4c
			 0x7563a088, /// 0xc50
			 0x69da4da1, /// 0xc54
			 0x327395d8, /// 0xc58
			 0xc40409a8, /// 0xc5c
			 0x99cfd02c, /// 0xc60
			 0x76e9bb72, /// 0xc64
			 0x784e5f29, /// 0xc68
			 0xe6028cb2, /// 0xc6c
			 0x076f0506, /// 0xc70
			 0x7d904c40, /// 0xc74
			 0x66c8f7de, /// 0xc78
			 0xe0a0c983, /// 0xc7c
			 0xf4f7a96c, /// 0xc80
			 0x3b242344, /// 0xc84
			 0x41f559d9, /// 0xc88
			 0x56be13cd, /// 0xc8c
			 0x377f5d11, /// 0xc90
			 0x06cdd0e5, /// 0xc94
			 0xb79549ce, /// 0xc98
			 0x8a997cfe, /// 0xc9c
			 0x0117f9e4, /// 0xca0
			 0x639e5bf1, /// 0xca4
			 0xf3175eec, /// 0xca8
			 0xbfd75174, /// 0xcac
			 0x4386083e, /// 0xcb0
			 0x59c8a03a, /// 0xcb4
			 0xaf8334df, /// 0xcb8
			 0xc74c739f, /// 0xcbc
			 0xe37d8878, /// 0xcc0
			 0xf4e17f22, /// 0xcc4
			 0x1f2cc986, /// 0xcc8
			 0xb75e0425, /// 0xccc
			 0x5cbaa0a2, /// 0xcd0
			 0x7d0b1bdc, /// 0xcd4
			 0x8d2a1b08, /// 0xcd8
			 0x5fb0ba64, /// 0xcdc
			 0x228c216d, /// 0xce0
			 0xef248247, /// 0xce4
			 0xde46589c, /// 0xce8
			 0xc3152480, /// 0xcec
			 0x7c35e540, /// 0xcf0
			 0xb9e42970, /// 0xcf4
			 0x0719b4c9, /// 0xcf8
			 0x12fbcee5, /// 0xcfc
			 0x1f99e175, /// 0xd00
			 0x8789b07f, /// 0xd04
			 0xd0fb81a5, /// 0xd08
			 0x3a7df030, /// 0xd0c
			 0xf85f644f, /// 0xd10
			 0x8009c778, /// 0xd14
			 0x174b6243, /// 0xd18
			 0xe25c7746, /// 0xd1c
			 0xc75bb47d, /// 0xd20
			 0x2da7e1d4, /// 0xd24
			 0x015b7f92, /// 0xd28
			 0xd499e87f, /// 0xd2c
			 0xd75a84ec, /// 0xd30
			 0x39718d75, /// 0xd34
			 0x74a49d8a, /// 0xd38
			 0x93ccc582, /// 0xd3c
			 0x34df6cb2, /// 0xd40
			 0x105b308c, /// 0xd44
			 0x17b01946, /// 0xd48
			 0xca1baab1, /// 0xd4c
			 0x1c8ca188, /// 0xd50
			 0x4525c2ba, /// 0xd54
			 0x17a9f0f5, /// 0xd58
			 0x47345107, /// 0xd5c
			 0x06aabb84, /// 0xd60
			 0xf8d33bcb, /// 0xd64
			 0x44d8e075, /// 0xd68
			 0xeea649f9, /// 0xd6c
			 0xb916d856, /// 0xd70
			 0x32a094c5, /// 0xd74
			 0x03704675, /// 0xd78
			 0x9810a373, /// 0xd7c
			 0x6753606e, /// 0xd80
			 0x131f339e, /// 0xd84
			 0xce8215e4, /// 0xd88
			 0xe3ea9b69, /// 0xd8c
			 0x810b940d, /// 0xd90
			 0xcbf185ea, /// 0xd94
			 0xdd9773b2, /// 0xd98
			 0x6b623e06, /// 0xd9c
			 0x8755c983, /// 0xda0
			 0x639bfdbe, /// 0xda4
			 0x9ade2f28, /// 0xda8
			 0x59cb5b52, /// 0xdac
			 0xb279d6a3, /// 0xdb0
			 0x867d1205, /// 0xdb4
			 0x7bd2a33e, /// 0xdb8
			 0x2a1e568a, /// 0xdbc
			 0xf1092a23, /// 0xdc0
			 0x04fc4af9, /// 0xdc4
			 0x6ac72711, /// 0xdc8
			 0x73ba666e, /// 0xdcc
			 0x4a296931, /// 0xdd0
			 0x7887f1ca, /// 0xdd4
			 0x8347d611, /// 0xdd8
			 0xb9eba8da, /// 0xddc
			 0x4e61ffcf, /// 0xde0
			 0x3636267b, /// 0xde4
			 0x2c41d596, /// 0xde8
			 0x246083fa, /// 0xdec
			 0xe852aa9b, /// 0xdf0
			 0x693da0d8, /// 0xdf4
			 0x290c75ef, /// 0xdf8
			 0x7c4bb4df, /// 0xdfc
			 0x904120e1, /// 0xe00
			 0x42214334, /// 0xe04
			 0x9dd14c44, /// 0xe08
			 0x41861cfb, /// 0xe0c
			 0x0e61cdcd, /// 0xe10
			 0x54df766d, /// 0xe14
			 0x8201f234, /// 0xe18
			 0x5b9c2751, /// 0xe1c
			 0xfc64f494, /// 0xe20
			 0xc076223d, /// 0xe24
			 0x39bded97, /// 0xe28
			 0xc159d5e2, /// 0xe2c
			 0xcb606b49, /// 0xe30
			 0xc9a95379, /// 0xe34
			 0x729f3c90, /// 0xe38
			 0x5dfdad28, /// 0xe3c
			 0x56b448e3, /// 0xe40
			 0xa63cabdc, /// 0xe44
			 0x495232ac, /// 0xe48
			 0x6eb6e957, /// 0xe4c
			 0xcf197eee, /// 0xe50
			 0xe35bc8cf, /// 0xe54
			 0x87458eb8, /// 0xe58
			 0xbcb94d83, /// 0xe5c
			 0x35cf0e2e, /// 0xe60
			 0x6e92bba6, /// 0xe64
			 0xe05bb9c4, /// 0xe68
			 0xd18285f4, /// 0xe6c
			 0xd9e48b01, /// 0xe70
			 0xa60bdd33, /// 0xe74
			 0xcb2bc26f, /// 0xe78
			 0xfdbbe061, /// 0xe7c
			 0x0261197f, /// 0xe80
			 0x5b117d5c, /// 0xe84
			 0x86a0e0e5, /// 0xe88
			 0x174205c3, /// 0xe8c
			 0xb58b6584, /// 0xe90
			 0xe5c58134, /// 0xe94
			 0x216b6c8b, /// 0xe98
			 0x5c30fd57, /// 0xe9c
			 0xd7941aa8, /// 0xea0
			 0xfc170d89, /// 0xea4
			 0x9965aaae, /// 0xea8
			 0x6513561c, /// 0xeac
			 0x40d01522, /// 0xeb0
			 0x63b6c761, /// 0xeb4
			 0xfbc3be85, /// 0xeb8
			 0xf4572ef0, /// 0xebc
			 0xa0ea2f9f, /// 0xec0
			 0xcd8a111f, /// 0xec4
			 0xbcbb0dd3, /// 0xec8
			 0x244b121d, /// 0xecc
			 0x6bc551e0, /// 0xed0
			 0xfe688bfc, /// 0xed4
			 0x319366d8, /// 0xed8
			 0xa6169469, /// 0xedc
			 0xf9ed5e55, /// 0xee0
			 0x7c2258b1, /// 0xee4
			 0x593c24b4, /// 0xee8
			 0xcabd7638, /// 0xeec
			 0xbaa2f91b, /// 0xef0
			 0x1aa23b89, /// 0xef4
			 0x90146141, /// 0xef8
			 0xe4d72a49, /// 0xefc
			 0x54960326, /// 0xf00
			 0x7cc81d37, /// 0xf04
			 0x2bf75f2b, /// 0xf08
			 0xdd2c9248, /// 0xf0c
			 0x0e76aafe, /// 0xf10
			 0xc36f2dc3, /// 0xf14
			 0x10a39c8f, /// 0xf18
			 0x07960679, /// 0xf1c
			 0xe4997834, /// 0xf20
			 0x8d8d1508, /// 0xf24
			 0x6fcfad94, /// 0xf28
			 0xc3ee18a6, /// 0xf2c
			 0xacbe0539, /// 0xf30
			 0xe4fde83a, /// 0xf34
			 0x29964e6e, /// 0xf38
			 0x5e3afc90, /// 0xf3c
			 0x4e382cbc, /// 0xf40
			 0xf08ef616, /// 0xf44
			 0x5d800349, /// 0xf48
			 0xa05f21c9, /// 0xf4c
			 0x96e9db07, /// 0xf50
			 0x14d0f47a, /// 0xf54
			 0xe90b4951, /// 0xf58
			 0x68d673d8, /// 0xf5c
			 0x775d0f19, /// 0xf60
			 0x1a0daadb, /// 0xf64
			 0x48485802, /// 0xf68
			 0xcde037f5, /// 0xf6c
			 0xb207bb95, /// 0xf70
			 0xe5e91fef, /// 0xf74
			 0xa7792b05, /// 0xf78
			 0x5c24cf41, /// 0xf7c
			 0x3efff458, /// 0xf80
			 0xfb10d436, /// 0xf84
			 0xe3f08576, /// 0xf88
			 0x3dbb9d07, /// 0xf8c
			 0x50d0f6ae, /// 0xf90
			 0x5eb2735a, /// 0xf94
			 0x8458e54b, /// 0xf98
			 0x923af8f4, /// 0xf9c
			 0xd8ebebb1, /// 0xfa0
			 0x44b07b2b, /// 0xfa4
			 0xdb132241, /// 0xfa8
			 0xcf32f92d, /// 0xfac
			 0x604b9b44, /// 0xfb0
			 0x9fa6d295, /// 0xfb4
			 0x6f21102f, /// 0xfb8
			 0x57748747, /// 0xfbc
			 0x78f7f3db, /// 0xfc0
			 0xaceeabb0, /// 0xfc4
			 0x0d6565d0, /// 0xfc8
			 0x588f5c57, /// 0xfcc
			 0xea00e01e, /// 0xfd0
			 0xa5735fe0, /// 0xfd4
			 0x37bea1fd, /// 0xfd8
			 0x58c6cd1a, /// 0xfdc
			 0x57b3ff6a, /// 0xfe0
			 0x001673ff, /// 0xfe4
			 0xd7246f79, /// 0xfe8
			 0x30e27336, /// 0xfec
			 0x3d1a1a18, /// 0xff0
			 0xa5a0853e, /// 0xff4
			 0x08c8766c, /// 0xff8
			 0xd313cccb /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00000
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00004
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00008
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0000c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00010
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0001c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00020
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00024
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0002c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00030
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00034
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00038
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0003c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00040
			 0x00000000,                                                  // zero                                        /// 00044
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00048
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0004c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00050
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00054
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00058
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0005c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00060
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00064
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00068
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00070
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00074
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00078
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0007c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0x7f800000,                                                  // inf                                         /// 00084
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00088
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0008c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00090
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00094
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00098
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0009c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000a0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 000a4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000a8
			 0xff800000,                                                  // -inf                                        /// 000ac
			 0x00011111,                                                  // 9.7958E-41                                  /// 000b0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000b4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000b8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000bc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0x0e000001,                                                  // Trailing 1s:                                /// 000c4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000c8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000cc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000d0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 000d4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 000d8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 000dc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 000e0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000ec
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 000f0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000f4
			 0x0c700000,                                                  // Leading 1s:                                 /// 000f8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00100
			 0x0e000003,                                                  // Trailing 1s:                                /// 00104
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0010c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00110
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00114
			 0x33333333,                                                  // 4 random values                             /// 00118
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0011c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00120
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00124
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00128
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0012c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00130
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00134
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00138
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0013c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00144
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00148
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0014c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0x0e000003,                                                  // Trailing 1s:                                /// 00154
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0015c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00160
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00164
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00168
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00174
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00178
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0017c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00180
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00184
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00188
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0018c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00190
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00198
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0019c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 001a4
			 0x80011111,                                                  // -9.7958E-41                                 /// 001a8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0x0c400000,                                                  // Leading 1s:                                 /// 001b0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001b8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001c0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 001c4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 001c8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001d0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 001d4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001dc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001e0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001e4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001e8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ec
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f0
			 0xcb000000,                                                  // -8388608.0                                  /// 001f4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 001fc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00200
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00204
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00208
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0020c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0xcb000000,                                                  // -8388608.0                                  /// 00214
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0021c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00220
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00224
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00228
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0022c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00230
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00234
			 0x0c600000,                                                  // Leading 1s:                                 /// 00238
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00240
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00244
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0024c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00250
			 0xff800000,                                                  // -inf                                        /// 00254
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00258
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0025c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00260
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00264
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0x4b000000,                                                  // 8388608.0                                   /// 0026c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00270
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00274
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00278
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0027c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00280
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00284
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00288
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0028c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00290
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00294
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00298
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0029c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 002a0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002a4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002a8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 002ac
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002b0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 002b4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 002bc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 002c0
			 0xffc00001,                                                  // -signaling NaN                              /// 002c4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002cc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002d0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 002d4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 002dc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002e0
			 0x00000000,                                                  // zero                                        /// 002e4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002e8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ec
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 002f4
			 0x0e000001,                                                  // Trailing 1s:                                /// 002f8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00304
			 0x0e000003,                                                  // Trailing 1s:                                /// 00308
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0030c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00310
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00314
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00318
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0031c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00320
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00324
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00328
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0032c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00334
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00338
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0033c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00340
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00344
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00348
			 0xcb000000,                                                  // -8388608.0                                  /// 0034c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00350
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00354
			 0x0c400000,                                                  // Leading 1s:                                 /// 00358
			 0x00000000,                                                  // zero                                        /// 0035c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00360
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00364
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00368
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0036c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00370
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00374
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00378
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00380
			 0x0c700000,                                                  // Leading 1s:                                 /// 00384
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00388
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0038c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00390
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00394
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0039c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003a0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003a4
			 0x0c400000,                                                  // Leading 1s:                                 /// 003a8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0x0c700000,                                                  // Leading 1s:                                 /// 003b4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 003b8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003bc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003c0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 003c8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003d0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 003d4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 003d8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003dc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003e4
			 0xbf028f5c,                                                  // -0.51                                       /// 003e8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ec
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 003f4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 003f8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0xbf028f5c,                                                  // -0.51                                       /// 00400
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00404
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00408
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0040c
			 0x80000000,                                                  // -zero                                       /// 00410
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00414
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00418
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0041c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00420
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00424
			 0x7f800000,                                                  // inf                                         /// 00428
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00430
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00434
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00438
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00440
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00444
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00448
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00450
			 0xff800000,                                                  // -inf                                        /// 00454
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00458
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0045c
			 0x3f028f5c,                                                  // 0.51                                        /// 00460
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00464
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0046c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00470
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00474
			 0xbf028f5c,                                                  // -0.51                                       /// 00478
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0047c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00480
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00484
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00488
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0048c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00498
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004a0
			 0x7f800000,                                                  // inf                                         /// 004a4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004a8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 004ac
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004b0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 004b4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004b8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 004bc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 004c0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 004c8
			 0x4b000000,                                                  // 8388608.0                                   /// 004cc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004d4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004d8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004dc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 004e0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 004e4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004f0
			 0xff800000,                                                  // -inf                                        /// 004f4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004f8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004fc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00500
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00504
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00508
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0050c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00510
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00514
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00518
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0051c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00520
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00524
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0052c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00530
			 0x55555555,                                                  // 4 random values                             /// 00534
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00538
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00540
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00548
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0054c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00550
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00554
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00558
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00560
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00564
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00568
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0056c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00570
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00574
			 0xff800000,                                                  // -inf                                        /// 00578
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0057c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00580
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00584
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00588
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0058c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00590
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00594
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00598
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0059c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005a0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 005a8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 005b0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005b8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005bc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0x33333333,                                                  // 4 random values                             /// 005c4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005c8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005cc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 005d8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005dc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005e0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 005e4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 005ec
			 0xAAAAAAAA,                                                  // 4 random values                             /// 005f0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f8
			 0x7fc00001,                                                  // signaling NaN                               /// 005fc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00600
			 0x0c400000,                                                  // Leading 1s:                                 /// 00604
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00608
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0060c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00610
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00614
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00618
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0061c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00620
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00624
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00628
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0062c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00630
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00634
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00638
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0063c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00640
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00644
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00648
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0064c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00650
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00654
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00658
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0065c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00660
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00664
			 0x7fc00001,                                                  // signaling NaN                               /// 00668
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0066c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00670
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00674
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00678
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00680
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00684
			 0x0e000001,                                                  // Trailing 1s:                                /// 00688
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0068c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00690
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00694
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00698
			 0x00000000,                                                  // zero                                        /// 0069c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 006a0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 006a4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x80011111,                                                  // -9.7958E-41                                 /// 006ac
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006b0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 006b4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 006b8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006bc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006c0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006c4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006c8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 006cc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 006d0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006d4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006d8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 006dc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006e0
			 0x7f800000,                                                  // inf                                         /// 006e4
			 0xffc00001,                                                  // -signaling NaN                              /// 006e8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 006ec
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 006f4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 006f8
			 0x00000000,                                                  // zero                                        /// 006fc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00700
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00708
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0070c
			 0x55555555,                                                  // 4 random values                             /// 00710
			 0x0e000007,                                                  // Trailing 1s:                                /// 00714
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00718
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0071c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00720
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00724
			 0x80000000,                                                  // -zero                                       /// 00728
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0072c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00738
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0073c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00740
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00744
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00748
			 0x0c780000,                                                  // Leading 1s:                                 /// 0074c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00750
			 0xcb000000,                                                  // -8388608.0                                  /// 00754
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00758
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0075c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00760
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00764
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00768
			 0xffc00001,                                                  // -signaling NaN                              /// 0076c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00770
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00774
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00778
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00780
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00784
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00788
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0078c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00790
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00794
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00798
			 0x33333333,                                                  // 4 random values                             /// 0079c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 007a4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 007a8
			 0x80000000,                                                  // -zero                                       /// 007ac
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007b0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 007b8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007bc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007c4
			 0x7f800000,                                                  // inf                                         /// 007c8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 007cc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 007d0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007d4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007d8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007dc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007e0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007e4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007e8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007ec
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 007f0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 007f4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007f8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007fc
			 0x80000000,                                                  // -zero                                       /// 00800
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00804
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00808
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0080c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00818
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0081c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00820
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00828
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0082c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00834
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00838
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00840
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00844
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00848
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00850
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00854
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00858
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0085c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00860
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00864
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00868
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0086c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00870
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00874
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00878
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0087c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00880
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00884
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00888
			 0x0c600000,                                                  // Leading 1s:                                 /// 0088c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00890
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00894
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00898
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0089c
			 0x0c780000,                                                  // Leading 1s:                                 /// 008a0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008a4
			 0x33333333,                                                  // 4 random values                             /// 008a8
			 0x0c700000,                                                  // Leading 1s:                                 /// 008ac
			 0xff800000,                                                  // -inf                                        /// 008b0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 008b4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008bc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008cc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008d4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008d8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008dc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008e4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 008e8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 008ec
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f0
			 0x3f028f5c,                                                  // 0.51                                        /// 008f4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 008f8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008fc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00900
			 0x0c600000,                                                  // Leading 1s:                                 /// 00904
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00908
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0090c
			 0x00000000,                                                  // zero                                        /// 00910
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00914
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00918
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00920
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00924
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00928
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0092c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00930
			 0x0e000001,                                                  // Trailing 1s:                                /// 00934
			 0xff800000,                                                  // -inf                                        /// 00938
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0093c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00940
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00944
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00948
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0094c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00950
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00954
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00958
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0095c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00964
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00968
			 0x0c600000,                                                  // Leading 1s:                                 /// 0096c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00970
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0x0e000001,                                                  // Trailing 1s:                                /// 00978
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0097c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00980
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00984
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00988
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0098c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00990
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00994
			 0xff800000,                                                  // -inf                                        /// 00998
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0099c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 009a4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009ac
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 009b4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009b8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009bc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009c0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 009c8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009d4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009dc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009e0
			 0x3f028f5c,                                                  // 0.51                                        /// 009e4
			 0x0e000001,                                                  // Trailing 1s:                                /// 009e8
			 0x00011111,                                                  // 9.7958E-41                                  /// 009ec
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009f4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 009f8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a00
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a04
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a08
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a0c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a10
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a14
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a18
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a1c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a20
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a24
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a28
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a2c
			 0x55555555,                                                  // 4 random values                             /// 00a30
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a34
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a38
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a3c
			 0xcb000000,                                                  // -8388608.0                                  /// 00a40
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a44
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a48
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a4c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a50
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a54
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a58
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a5c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a60
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a64
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a68
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a6c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a70
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a74
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a78
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a7c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a88
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a8c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a90
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a94
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a98
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a9c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00aa0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00aa4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ab0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ab4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ab8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00abc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ac4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ac8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ad8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00adc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ae0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ae4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00aec
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00af4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00afc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b00
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b04
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b08
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b0c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b10
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b14
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b18
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b20
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b24
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b28
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b2c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b30
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b34
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b38
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b3c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b40
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b44
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b48
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b4c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b50
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b54
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b58
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b5c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b64
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b68
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b6c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b70
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b74
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b78
			 0x3f028f5c,                                                  // 0.51                                        /// 00b7c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b80
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b88
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b8c
			 0x80000000,                                                  // -zero                                       /// 00b90
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b94
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b98
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b9c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ba0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ba4
			 0x7f800000,                                                  // inf                                         /// 00ba8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00bac
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00bb0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00bb4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00bb8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00bbc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00bc0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00bc4
			 0xbf028f5c,                                                  // -0.51                                       /// 00bc8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00bcc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00bd0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00bd4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00bd8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00bdc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00be8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00bec
			 0x4b000000,                                                  // 8388608.0                                   /// 00bf0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bf4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00bf8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00bfc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c00
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c08
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c0c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0xcb000000,                                                  // -8388608.0                                  /// 00c14
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c18
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c1c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c24
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c28
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c2c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c34
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c38
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c40
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c44
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c48
			 0x3f028f5c,                                                  // 0.51                                        /// 00c4c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c50
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c54
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c58
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c5c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c60
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c64
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c70
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c74
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c78
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c7c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c80
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c84
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c8c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c90
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c94
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c9c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ca4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ca8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00cac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00cb0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00cb4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00cbc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00cc0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00cc4
			 0x3f028f5c,                                                  // 0.51                                        /// 00cc8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ccc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00cd4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00cdc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ce0
			 0xff800000,                                                  // -inf                                        /// 00ce4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ce8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00cec
			 0x7fc00001,                                                  // signaling NaN                               /// 00cf0
			 0xbf028f5c,                                                  // -0.51                                       /// 00cf4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cf8
			 0x7fc00001,                                                  // signaling NaN                               /// 00cfc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d00
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d04
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d08
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d0c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d10
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d14
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d18
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d20
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d24
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d28
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d2c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d30
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d34
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d38
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d3c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d40
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d44
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d48
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d4c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d50
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d54
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d58
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d5c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d60
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d64
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d68
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d6c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d70
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d78
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d7c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d80
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d84
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d88
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d8c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d94
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d98
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d9c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00da0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00da4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00da8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00db0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00db4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00db8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00dbc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00dc0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00dc8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00dcc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00dd0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00dd8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ddc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00de0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00de4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00de8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dec
			 0x55555555,                                                  // 4 random values                             /// 00df0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00df4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00dfc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e00
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e04
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e08
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e0c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e10
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e14
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e18
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e1c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e20
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e24
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e28
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e2c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e30
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e34
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e38
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e3c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e40
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e44
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e48
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e4c
			 0x00000000,                                                  // zero                                        /// 00e50
			 0xffc00001,                                                  // -signaling NaN                              /// 00e54
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e58
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e5c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e64
			 0xbf028f5c,                                                  // -0.51                                       /// 00e68
			 0x33333333,                                                  // 4 random values                             /// 00e6c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e74
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e78
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e7c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e80
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e84
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e88
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e8c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e90
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e94
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e98
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e9c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ea0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ea4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ea8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00eac
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00eb0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00eb4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00eb8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ebc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ec0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ec4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ecc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ed0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ed4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ed8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ee0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ee4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ee8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00eec
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ef0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ef8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00efc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f00
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f04
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f0c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f10
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f18
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f1c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f20
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f28
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f2c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f30
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0x3f028f5c,                                                  // 0.51                                        /// 00f3c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f40
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f44
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f48
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f4c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f50
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f54
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f58
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f5c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f68
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f6c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f70
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f74
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f78
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f7c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f80
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f84
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f88
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f8c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f90
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f94
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f9c
			 0xcb000000,                                                  // -8388608.0                                  /// 00fa0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fa4
			 0xff800000,                                                  // -inf                                        /// 00fa8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fac
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00fb0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00fb4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00fb8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00fbc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00fc4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00fc8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fcc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00fd0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00fd4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00fd8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00fdc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00fe4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00fe8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fec
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ff0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ff4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff8
			 0x0c7ffffc                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00000
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0000c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00010
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00014
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0001c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00020
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00024
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00028
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0002c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00030
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00034
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00038
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0003c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00044
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00048
			 0xffc00001,                                                  // -signaling NaN                              /// 0004c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00050
			 0x0e000003,                                                  // Trailing 1s:                                /// 00054
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00058
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0005c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00060
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00064
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0006c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00070
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00074
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00078
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0007c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00084
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00088
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0008c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00090
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00094
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0009c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000a0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000a8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 000ac
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000b0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000b4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000b8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000bc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000c0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c8
			 0x7f800000,                                                  // inf                                         /// 000cc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000d0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000d4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000d8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000e0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000e4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 000e8
			 0x0e000003,                                                  // Trailing 1s:                                /// 000ec
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000f8
			 0x7f800000,                                                  // inf                                         /// 000fc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00100
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00104
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00108
			 0x7fc00001,                                                  // signaling NaN                               /// 0010c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00110
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00118
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0011c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00120
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00124
			 0xffc00001,                                                  // -signaling NaN                              /// 00128
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0012c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00130
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00134
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00138
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0013c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00140
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00144
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0014c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00150
			 0x0e000007,                                                  // Trailing 1s:                                /// 00154
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00158
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0015c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00160
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00164
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00168
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0016c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00170
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00174
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00178
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0017c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00180
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00184
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00188
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0018c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00190
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00194
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00198
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0019c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001a4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001a8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001ac
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 001b0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001b4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 001b8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 001bc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001c4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001c8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 001d0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001d8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001dc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001e0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ec
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001f0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001f4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001f8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001fc
			 0xff800000,                                                  // -inf                                        /// 00200
			 0x33333333,                                                  // 4 random values                             /// 00204
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00208
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0020c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00210
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00218
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0021c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00220
			 0x0e000003,                                                  // Trailing 1s:                                /// 00224
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00228
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0022c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00230
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00234
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00238
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x55555555,                                                  // 4 random values                             /// 00240
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00244
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00248
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0024c
			 0x80000000,                                                  // -zero                                       /// 00250
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00254
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00260
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00264
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00270
			 0xbf028f5c,                                                  // -0.51                                       /// 00274
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0027c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00280
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00288
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0028c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00290
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00294
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00298
			 0x3f028f5c,                                                  // 0.51                                        /// 0029c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 002a0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002a4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 002a8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 002ac
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002b0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 002b4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 002b8
			 0x80011111,                                                  // -9.7958E-41                                 /// 002bc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002c8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002cc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002d0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 002d4
			 0x0e000001,                                                  // Trailing 1s:                                /// 002d8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002dc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 002e0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 002e4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002e8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002ec
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 002f0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002f4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 002f8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002fc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00300
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00304
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00308
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00310
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00314
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00318
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0031c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00320
			 0x55555555,                                                  // 4 random values                             /// 00324
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00328
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0032c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00330
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00334
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00338
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00340
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00344
			 0x0c400000,                                                  // Leading 1s:                                 /// 00348
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0034c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00358
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0035c
			 0xff800000,                                                  // -inf                                        /// 00360
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00364
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00368
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0036c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00370
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00374
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0037c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00380
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00384
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00388
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0038c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00390
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00398
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0039c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003a0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003a4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003a8
			 0x33333333,                                                  // 4 random values                             /// 003ac
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003b0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003b4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003bc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003c0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003c8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003cc
			 0x4b000000,                                                  // 8388608.0                                   /// 003d0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 003d4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 003d8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 003dc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003e4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003e8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003ec
			 0x0e007fff,                                                  // Trailing 1s:                                /// 003f0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003f4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 003f8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003fc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00400
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00404
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00408
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0040c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00410
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00414
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00418
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0041c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00420
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00424
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00428
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00430
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00434
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00438
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x7f800000,                                                  // inf                                         /// 00444
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0044c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00450
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00454
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00458
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0045c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00460
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00464
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00468
			 0x0c400000,                                                  // Leading 1s:                                 /// 0046c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00470
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00474
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00478
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0047c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00480
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00484
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00488
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0048c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00490
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00494
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00498
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0049c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 004a0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 004a4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 004a8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 004b0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 004b4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004b8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004bc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 004c4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 004c8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 004d0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 004d4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004d8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 004dc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004e0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 004e4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 004e8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004ec
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004f4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004f8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004fc
			 0x00000000,                                                  // zero                                        /// 00500
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00504
			 0x0c600000,                                                  // Leading 1s:                                 /// 00508
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0050c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00510
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00518
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0051c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00520
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00524
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00528
			 0xbf028f5c,                                                  // -0.51                                       /// 0052c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00530
			 0xbf028f5c,                                                  // -0.51                                       /// 00534
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00538
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00540
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00544
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00548
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0054c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00550
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00558
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0055c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00560
			 0x7fc00001,                                                  // signaling NaN                               /// 00564
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00568
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0056c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00570
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00574
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0057c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00580
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00584
			 0x3f028f5c,                                                  // 0.51                                        /// 00588
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0058c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00590
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0x7f800000,                                                  // inf                                         /// 00598
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0059c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 005a0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005a4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005a8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005ac
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005b4
			 0x80011111,                                                  // -9.7958E-41                                 /// 005b8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005bc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 005c0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005cc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005d0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 005d4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 005d8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 005dc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0x80011111,                                                  // -9.7958E-41                                 /// 005f0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 005f4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 005f8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005fc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00600
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00608
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0060c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00610
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00614
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00618
			 0x0e000001,                                                  // Trailing 1s:                                /// 0061c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00620
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00624
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0062c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00630
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00634
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0063c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00640
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00644
			 0x0e000001,                                                  // Trailing 1s:                                /// 00648
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00650
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00654
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00658
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0065c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00660
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00664
			 0x00000000,                                                  // zero                                        /// 00668
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00670
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00674
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00678
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0067c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00680
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00688
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0068c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00690
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00694
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00698
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0069c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 006a0
			 0x80000000,                                                  // -zero                                       /// 006a4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006ac
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006b0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006bc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 006c0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006c8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 006cc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006d0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 006d4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006d8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006dc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 006e0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006e4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 006e8
			 0x55555555,                                                  // 4 random values                             /// 006ec
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006fc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00700
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00704
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00708
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0070c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00714
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00718
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0071c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00720
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00728
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0072c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00730
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00734
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00738
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0073c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00740
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00744
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00748
			 0x0c780000,                                                  // Leading 1s:                                 /// 0074c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00750
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00754
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00758
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0075c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00760
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00764
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00768
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0076c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00770
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00774
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00778
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0077c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0x0c400000,                                                  // Leading 1s:                                 /// 00788
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0078c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00790
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00794
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00798
			 0xcb000000,                                                  // -8388608.0                                  /// 0079c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007a0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 007a4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007a8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ac
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007b0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007b4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007b8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007bc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007c4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007c8
			 0x00000000,                                                  // zero                                        /// 007cc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 007d0
			 0x0c400000,                                                  // Leading 1s:                                 /// 007d4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007dc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007e0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 007e4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e8
			 0x80000000,                                                  // -zero                                       /// 007ec
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007f0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007f4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 007f8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 007fc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00800
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00804
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00808
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0080c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00810
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00814
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00818
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0081c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00820
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00824
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00828
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0082c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00830
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00834
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00838
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00844
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00848
			 0x33333333,                                                  // 4 random values                             /// 0084c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00850
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00854
			 0x00000000,                                                  // zero                                        /// 00858
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0085c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00864
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00868
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0086c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00870
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00874
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00878
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0087c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00880
			 0x0e000003,                                                  // Trailing 1s:                                /// 00884
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00888
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0088c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00890
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00894
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00898
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0089c
			 0x80000000,                                                  // -zero                                       /// 008a0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 008a4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 008a8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008ac
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008b0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008b8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008bc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008c0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008c4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008c8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008cc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 008d0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 008d4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008d8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008dc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e0
			 0x0e000007,                                                  // Trailing 1s:                                /// 008e4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 008e8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008ec
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008f0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 008f4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008f8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 008fc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00900
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00904
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00908
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0090c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00918
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0091c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00920
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00924
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00928
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0092c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00930
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00934
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00938
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0093c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00940
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00944
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00948
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0094c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00950
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00954
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00958
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0095c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00964
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00968
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0096c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00970
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00974
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00978
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0097c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00980
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00984
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00988
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00990
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00994
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00998
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0099c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009ac
			 0x0e00007f,                                                  // Trailing 1s:                                /// 009b0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009b4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b8
			 0x0e000001,                                                  // Trailing 1s:                                /// 009bc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009c0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009c4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009c8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009cc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009d0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009d8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 009dc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009e4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009e8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ec
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 009f8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009fc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a00
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a04
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a08
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a0c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a10
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a14
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a18
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a1c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a20
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a24
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a28
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a30
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a34
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a38
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a3c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a40
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a48
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a50
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a54
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a58
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a5c
			 0x33333333,                                                  // 4 random values                             /// 00a60
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a64
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a70
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a74
			 0x00000000,                                                  // zero                                        /// 00a78
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a7c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a84
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a88
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a8c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a90
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a94
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a98
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a9c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00aa0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00aa8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aac
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ab4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00abc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ac0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ac4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ac8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00acc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ad4
			 0x4b000000,                                                  // 8388608.0                                   /// 00ad8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00adc
			 0x4b000000,                                                  // 8388608.0                                   /// 00ae0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00aec
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00af0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00af4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00af8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b00
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b04
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b08
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b0c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b10
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b14
			 0x00000000,                                                  // zero                                        /// 00b18
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b20
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b24
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b28
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b2c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b30
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b34
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b3c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b40
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b44
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b48
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b4c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b50
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b58
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b5c
			 0x33333333,                                                  // 4 random values                             /// 00b60
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b64
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b68
			 0x4b000000,                                                  // 8388608.0                                   /// 00b6c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b74
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b78
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b80
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b88
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b8c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b90
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b94
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b98
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ba0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ba8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00bac
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00bb0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00bb8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00bbc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00bc0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00bc4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00bc8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00bcc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00bd8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00bdc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00be0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00be4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00be8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00bec
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00bf0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00bf4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bf8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bfc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c00
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c04
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c08
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c0c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c10
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c14
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c18
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c1c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c20
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c24
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c28
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c30
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c34
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c38
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c44
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c48
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c4c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c50
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c54
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c58
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c5c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c60
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c6c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c70
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c74
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c78
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c7c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c84
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c88
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c8c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c90
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c98
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c9c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ca0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ca4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cac
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00cb0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00cb4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00cb8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00cbc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00cc4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00cc8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00cd0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00cd4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00cdc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ce0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ce4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ce8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cec
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00cf0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00cf8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00cfc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d00
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d04
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d08
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d0c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d10
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d18
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d20
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d28
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d2c
			 0xffc00001,                                                  // -signaling NaN                              /// 00d30
			 0xff800000,                                                  // -inf                                        /// 00d34
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d38
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d44
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d48
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d4c
			 0x7f800000,                                                  // inf                                         /// 00d50
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d54
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d58
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d5c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d60
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d64
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d68
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d70
			 0x55555555,                                                  // 4 random values                             /// 00d74
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d78
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d7c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d80
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d88
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d8c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d90
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d94
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d98
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d9c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00da0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00da4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00da8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00dac
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00db8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00dbc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00dc0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00dc4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00dc8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dcc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00dd8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ddc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00de0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00de4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00dec
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00df0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00df4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00df8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00dfc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e04
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e08
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e0c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e10
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e14
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e18
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e1c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e20
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e24
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e28
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e30
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e34
			 0xbf028f5c,                                                  // -0.51                                       /// 00e38
			 0xbf028f5c,                                                  // -0.51                                       /// 00e3c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e40
			 0x00000000,                                                  // zero                                        /// 00e44
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e48
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e4c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e50
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e54
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e58
			 0x80000000,                                                  // -zero                                       /// 00e5c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e60
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e64
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e68
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e6c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e70
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e74
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e7c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e80
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e84
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e88
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e8c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e90
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e98
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e9c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea0
			 0x33333333,                                                  // 4 random values                             /// 00ea4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00eac
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00eb0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00eb4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00eb8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ebc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ec0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ec4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ec8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ed0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ed4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ed8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00edc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ee4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00eec
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ef0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ef4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ef8
			 0x7f800000,                                                  // inf                                         /// 00efc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f00
			 0xffc00001,                                                  // -signaling NaN                              /// 00f04
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f08
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f0c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f10
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f14
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f1c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f24
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f28
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f2c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f30
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f34
			 0x33333333,                                                  // 4 random values                             /// 00f38
			 0x7f800000,                                                  // inf                                         /// 00f3c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f40
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f44
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f48
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f4c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f50
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f54
			 0x80000000,                                                  // -zero                                       /// 00f58
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f5c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f60
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f64
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f68
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f6c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f70
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f74
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f78
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f7c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f80
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f84
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f8c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f90
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f94
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f98
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f9c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00fa0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00fa8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00fac
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00fb4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00fbc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fc0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00fc4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fc8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00fcc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00fd0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00fd4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fd8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fe0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00fec
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ff0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ff4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ff8
			 0x80002000                                                  // SP - 1 bit alone set in mantissa -ve        /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x5d5d45fb, /// 0x0
			 0xa3d594d4, /// 0x4
			 0x46f89bb8, /// 0x8
			 0x22ae48dc, /// 0xc
			 0xd0e0c6c4, /// 0x10
			 0x8eee3e7a, /// 0x14
			 0x9f65e516, /// 0x18
			 0x6a797528, /// 0x1c
			 0xfc89fa4d, /// 0x20
			 0xf68ac415, /// 0x24
			 0x924c732a, /// 0x28
			 0xf6680c2e, /// 0x2c
			 0x9402476a, /// 0x30
			 0x6a46a50b, /// 0x34
			 0xcd53c541, /// 0x38
			 0x6d2d3526, /// 0x3c
			 0x7dadd7d0, /// 0x40
			 0x11c23d1a, /// 0x44
			 0xc39af92f, /// 0x48
			 0xc40b005a, /// 0x4c
			 0x09085da1, /// 0x50
			 0xcfac84e1, /// 0x54
			 0xc4bb813c, /// 0x58
			 0x2731e0ea, /// 0x5c
			 0x32d2a83c, /// 0x60
			 0xa988b065, /// 0x64
			 0xdb65b2c6, /// 0x68
			 0xc0c4088d, /// 0x6c
			 0x6504b2ff, /// 0x70
			 0xd5fa68b2, /// 0x74
			 0x7af3d815, /// 0x78
			 0x786d696a, /// 0x7c
			 0x6b35450d, /// 0x80
			 0x468885cb, /// 0x84
			 0x7bb1e458, /// 0x88
			 0x0aeabc82, /// 0x8c
			 0x420e8533, /// 0x90
			 0xb10d9710, /// 0x94
			 0x4a7986b0, /// 0x98
			 0xca361ec7, /// 0x9c
			 0xd87bd1ce, /// 0xa0
			 0x9af7c1a5, /// 0xa4
			 0xd32bd7bc, /// 0xa8
			 0x1e08ae27, /// 0xac
			 0x50813b38, /// 0xb0
			 0xc8a913a9, /// 0xb4
			 0xab03297f, /// 0xb8
			 0xd20474e6, /// 0xbc
			 0xfdf7d63d, /// 0xc0
			 0xbb97c921, /// 0xc4
			 0x5547594e, /// 0xc8
			 0x26c417d2, /// 0xcc
			 0x76de6310, /// 0xd0
			 0x50a4bfa1, /// 0xd4
			 0x93df883d, /// 0xd8
			 0xbf8f2f39, /// 0xdc
			 0x47405e76, /// 0xe0
			 0xce9191e0, /// 0xe4
			 0x91f720b6, /// 0xe8
			 0x87f05e89, /// 0xec
			 0x379c81ec, /// 0xf0
			 0xd74250a0, /// 0xf4
			 0x6aa3c3f8, /// 0xf8
			 0xefd02661, /// 0xfc
			 0xa4cb21d7, /// 0x100
			 0x43d7e307, /// 0x104
			 0x747d2d18, /// 0x108
			 0xd2d11d09, /// 0x10c
			 0x06d621ad, /// 0x110
			 0xff14aee5, /// 0x114
			 0xd62bc263, /// 0x118
			 0xb73d86e5, /// 0x11c
			 0xcd706ea3, /// 0x120
			 0x01c98c55, /// 0x124
			 0x0365bf83, /// 0x128
			 0x38ceb3dd, /// 0x12c
			 0x1384b1e8, /// 0x130
			 0xebde5a8a, /// 0x134
			 0x4d61ba20, /// 0x138
			 0xdfd9b290, /// 0x13c
			 0x9eb344e2, /// 0x140
			 0x20b40fd5, /// 0x144
			 0xc4973ea2, /// 0x148
			 0x9622e1d5, /// 0x14c
			 0xa361e767, /// 0x150
			 0x76052358, /// 0x154
			 0x991245f6, /// 0x158
			 0x6e4afde9, /// 0x15c
			 0xc5ba326f, /// 0x160
			 0x9d93c5d7, /// 0x164
			 0xca40cf1e, /// 0x168
			 0xe4f34153, /// 0x16c
			 0xcca4c1d4, /// 0x170
			 0xfd31b986, /// 0x174
			 0x45164245, /// 0x178
			 0x7b9aecde, /// 0x17c
			 0x95379ac7, /// 0x180
			 0x0c47badc, /// 0x184
			 0x3d9df116, /// 0x188
			 0x94b79cf1, /// 0x18c
			 0xedffcc46, /// 0x190
			 0xc8201db3, /// 0x194
			 0x22c2b6c2, /// 0x198
			 0x060a388e, /// 0x19c
			 0x8b04c7a1, /// 0x1a0
			 0x0ef502f3, /// 0x1a4
			 0xb22d66a3, /// 0x1a8
			 0x9e1413bf, /// 0x1ac
			 0x0dfb80f8, /// 0x1b0
			 0xa15a7dcd, /// 0x1b4
			 0x7e54e701, /// 0x1b8
			 0x72b94424, /// 0x1bc
			 0xa0da5f80, /// 0x1c0
			 0xbf43dc9a, /// 0x1c4
			 0xa53cd1d8, /// 0x1c8
			 0xf3abc47e, /// 0x1cc
			 0x020dd3a0, /// 0x1d0
			 0xa724d020, /// 0x1d4
			 0x306365a4, /// 0x1d8
			 0xc7ee8dfd, /// 0x1dc
			 0xba751d69, /// 0x1e0
			 0xc614871a, /// 0x1e4
			 0x23836001, /// 0x1e8
			 0x09213329, /// 0x1ec
			 0xa68e0fae, /// 0x1f0
			 0xacfa2e50, /// 0x1f4
			 0x70958697, /// 0x1f8
			 0x0a81c602, /// 0x1fc
			 0x468251a5, /// 0x200
			 0x65b2d03b, /// 0x204
			 0x91fb3016, /// 0x208
			 0x8c79f7d2, /// 0x20c
			 0x4451cb40, /// 0x210
			 0xe5873e72, /// 0x214
			 0x9c091e3d, /// 0x218
			 0xcad7043a, /// 0x21c
			 0x6c178048, /// 0x220
			 0x73f7ec36, /// 0x224
			 0xa1fa8dcc, /// 0x228
			 0xe33b279d, /// 0x22c
			 0x5834f65b, /// 0x230
			 0x970fdfaf, /// 0x234
			 0x41fd0075, /// 0x238
			 0x7e8b92d9, /// 0x23c
			 0xb35cae1a, /// 0x240
			 0x09f965c7, /// 0x244
			 0x20a177a0, /// 0x248
			 0x398f3741, /// 0x24c
			 0x4d96b914, /// 0x250
			 0xf28c36e6, /// 0x254
			 0x862a470f, /// 0x258
			 0x029256e1, /// 0x25c
			 0x2393681d, /// 0x260
			 0x649a5d81, /// 0x264
			 0x50247ff4, /// 0x268
			 0xeaa2a917, /// 0x26c
			 0x6688bec4, /// 0x270
			 0xa4df6789, /// 0x274
			 0x4261d53b, /// 0x278
			 0x691f0d5d, /// 0x27c
			 0x193e5d4c, /// 0x280
			 0x847aef37, /// 0x284
			 0x6ba2c176, /// 0x288
			 0x8677a7c9, /// 0x28c
			 0xbb48e707, /// 0x290
			 0x9ad038d5, /// 0x294
			 0x26d82743, /// 0x298
			 0xfd920a88, /// 0x29c
			 0xc8ed0125, /// 0x2a0
			 0xc2523340, /// 0x2a4
			 0xb3bc4c20, /// 0x2a8
			 0xe8411c36, /// 0x2ac
			 0x597d2c39, /// 0x2b0
			 0xa76655eb, /// 0x2b4
			 0x0b88577e, /// 0x2b8
			 0x1266349c, /// 0x2bc
			 0x77fb4713, /// 0x2c0
			 0xb4002eb3, /// 0x2c4
			 0x451889bf, /// 0x2c8
			 0x29f5524a, /// 0x2cc
			 0xe9beeb69, /// 0x2d0
			 0x3b0034be, /// 0x2d4
			 0x01c8ca86, /// 0x2d8
			 0x299fb642, /// 0x2dc
			 0xaa7d58c0, /// 0x2e0
			 0x74dd95cf, /// 0x2e4
			 0x6d58f39e, /// 0x2e8
			 0x83a86d49, /// 0x2ec
			 0x6a68a638, /// 0x2f0
			 0x496b271b, /// 0x2f4
			 0xadbfed2e, /// 0x2f8
			 0x5a9e20e0, /// 0x2fc
			 0x76e497f4, /// 0x300
			 0x2d23585a, /// 0x304
			 0xc6bcd89a, /// 0x308
			 0xf659fb42, /// 0x30c
			 0xca7340ff, /// 0x310
			 0x1588b02e, /// 0x314
			 0xf8f04bbd, /// 0x318
			 0x77217694, /// 0x31c
			 0x0a62a1c9, /// 0x320
			 0x5643ef0c, /// 0x324
			 0x5092bdf5, /// 0x328
			 0xca7b3a28, /// 0x32c
			 0x5eb90a7b, /// 0x330
			 0xe982a1d0, /// 0x334
			 0x19403d0d, /// 0x338
			 0x5dcc0902, /// 0x33c
			 0x9246160e, /// 0x340
			 0xbbdc5117, /// 0x344
			 0xa80ffa9d, /// 0x348
			 0x37ca2f9d, /// 0x34c
			 0xea79fe61, /// 0x350
			 0x4c8938e6, /// 0x354
			 0xbfb756ba, /// 0x358
			 0x59b8ac5d, /// 0x35c
			 0x36e40dc7, /// 0x360
			 0xb96f6b85, /// 0x364
			 0x997ff37f, /// 0x368
			 0xeaf1c205, /// 0x36c
			 0xa8dc72c6, /// 0x370
			 0x14bd4679, /// 0x374
			 0xd901608a, /// 0x378
			 0x3dd44f8b, /// 0x37c
			 0xa94a196e, /// 0x380
			 0x6011abac, /// 0x384
			 0xdb679be8, /// 0x388
			 0x9b90b09a, /// 0x38c
			 0xa378f6a9, /// 0x390
			 0x481fc299, /// 0x394
			 0xbdc9b38c, /// 0x398
			 0xc7313c64, /// 0x39c
			 0x76ad9a76, /// 0x3a0
			 0x4ef66a81, /// 0x3a4
			 0x39573ebf, /// 0x3a8
			 0x28f5baf8, /// 0x3ac
			 0x3296312b, /// 0x3b0
			 0x8dd3600c, /// 0x3b4
			 0x068c3c40, /// 0x3b8
			 0x36d122ba, /// 0x3bc
			 0x29db131a, /// 0x3c0
			 0x3d18b4c1, /// 0x3c4
			 0x2184840d, /// 0x3c8
			 0x9e546734, /// 0x3cc
			 0x926021e3, /// 0x3d0
			 0xa9d1dff0, /// 0x3d4
			 0xee2c3b7c, /// 0x3d8
			 0x9a875572, /// 0x3dc
			 0x633b4d41, /// 0x3e0
			 0x542262f2, /// 0x3e4
			 0xd0ef266e, /// 0x3e8
			 0x4ee4916a, /// 0x3ec
			 0xc6918d7c, /// 0x3f0
			 0xe61719ca, /// 0x3f4
			 0xfd6fa351, /// 0x3f8
			 0xee2e5f6d, /// 0x3fc
			 0x9480df94, /// 0x400
			 0xd81e8410, /// 0x404
			 0xc4e9301d, /// 0x408
			 0x0aedec7a, /// 0x40c
			 0xd90ff9e1, /// 0x410
			 0x57efcf7f, /// 0x414
			 0xcc5d6715, /// 0x418
			 0x9f4b11dd, /// 0x41c
			 0x32e00a47, /// 0x420
			 0xb1037583, /// 0x424
			 0x59da27e0, /// 0x428
			 0x6f1684f4, /// 0x42c
			 0x4f60ad01, /// 0x430
			 0x54c8843e, /// 0x434
			 0x40f022d6, /// 0x438
			 0x8f1c1662, /// 0x43c
			 0x8a3cd914, /// 0x440
			 0xdeec9d7f, /// 0x444
			 0x4e7dbf31, /// 0x448
			 0x52b90133, /// 0x44c
			 0x286ef8da, /// 0x450
			 0xb4f504dd, /// 0x454
			 0x469690c2, /// 0x458
			 0xc63d845f, /// 0x45c
			 0x7bc9fec8, /// 0x460
			 0xf345117f, /// 0x464
			 0x42c7937f, /// 0x468
			 0xf2dfc77d, /// 0x46c
			 0x8a07d43b, /// 0x470
			 0x082fccc2, /// 0x474
			 0x5e604eb1, /// 0x478
			 0xde4ccf67, /// 0x47c
			 0x2b9e10ac, /// 0x480
			 0x681fc1a4, /// 0x484
			 0xee480df3, /// 0x488
			 0xbab73beb, /// 0x48c
			 0x60c5b91b, /// 0x490
			 0xf5fe0826, /// 0x494
			 0x55ad0684, /// 0x498
			 0x4f16c643, /// 0x49c
			 0x738a1353, /// 0x4a0
			 0xbffdc4c5, /// 0x4a4
			 0x789466c5, /// 0x4a8
			 0x8bd03419, /// 0x4ac
			 0x2e1dfd25, /// 0x4b0
			 0xd6a3bd78, /// 0x4b4
			 0x05faf3c9, /// 0x4b8
			 0x20359b74, /// 0x4bc
			 0x84b0e6fe, /// 0x4c0
			 0xd3f92391, /// 0x4c4
			 0x7c306d67, /// 0x4c8
			 0x49a40195, /// 0x4cc
			 0xaec3e063, /// 0x4d0
			 0x07e527c6, /// 0x4d4
			 0xcdf8dd76, /// 0x4d8
			 0xf013391b, /// 0x4dc
			 0x6eb832c9, /// 0x4e0
			 0x93fc92aa, /// 0x4e4
			 0x348a6062, /// 0x4e8
			 0x9ded671e, /// 0x4ec
			 0xc42b0e2f, /// 0x4f0
			 0xda61827f, /// 0x4f4
			 0x5201401d, /// 0x4f8
			 0x06878741, /// 0x4fc
			 0x07e1af98, /// 0x500
			 0x37086bc1, /// 0x504
			 0xae0938c5, /// 0x508
			 0xc285721a, /// 0x50c
			 0x55377022, /// 0x510
			 0xe0baa232, /// 0x514
			 0xb4cfc133, /// 0x518
			 0x5d6dd4e6, /// 0x51c
			 0xeb5c9a34, /// 0x520
			 0xb5a28603, /// 0x524
			 0x2dcbb9dd, /// 0x528
			 0x43e2d20b, /// 0x52c
			 0x45b332aa, /// 0x530
			 0xb5937d5d, /// 0x534
			 0xddf03be6, /// 0x538
			 0xc5bb18c9, /// 0x53c
			 0x20aa4961, /// 0x540
			 0x0132e1e7, /// 0x544
			 0x47922678, /// 0x548
			 0x5a02d340, /// 0x54c
			 0x5a45fa69, /// 0x550
			 0x522f82eb, /// 0x554
			 0x7d0ff774, /// 0x558
			 0xc1c34df6, /// 0x55c
			 0x5e549226, /// 0x560
			 0xa80f7113, /// 0x564
			 0x7afee3f4, /// 0x568
			 0xae125ef7, /// 0x56c
			 0xd6062776, /// 0x570
			 0xc9b05e4f, /// 0x574
			 0xbf281518, /// 0x578
			 0x40a48b8b, /// 0x57c
			 0xa3518335, /// 0x580
			 0xdf8f7b82, /// 0x584
			 0x9691609c, /// 0x588
			 0xf49c8f68, /// 0x58c
			 0x2d7e52c6, /// 0x590
			 0x118ce984, /// 0x594
			 0x322991fc, /// 0x598
			 0xdaabe42c, /// 0x59c
			 0x8196e36e, /// 0x5a0
			 0x78f07f09, /// 0x5a4
			 0xd33832df, /// 0x5a8
			 0xa4b7ff01, /// 0x5ac
			 0x0fc7707d, /// 0x5b0
			 0x35d75c52, /// 0x5b4
			 0x380d3203, /// 0x5b8
			 0x0d512bc1, /// 0x5bc
			 0x0c8f7489, /// 0x5c0
			 0x7694ba38, /// 0x5c4
			 0x5a212e2c, /// 0x5c8
			 0xefa2307f, /// 0x5cc
			 0xf8b9a236, /// 0x5d0
			 0x86cc406a, /// 0x5d4
			 0x768d9ad8, /// 0x5d8
			 0xc39ba132, /// 0x5dc
			 0xdbe4bb0b, /// 0x5e0
			 0x6c18dfc1, /// 0x5e4
			 0xf5856af4, /// 0x5e8
			 0x9dacd13a, /// 0x5ec
			 0x083e9072, /// 0x5f0
			 0x2c930914, /// 0x5f4
			 0xfeae4977, /// 0x5f8
			 0x6fe9af3c, /// 0x5fc
			 0x16ccd862, /// 0x600
			 0xa7631964, /// 0x604
			 0xfc1becbe, /// 0x608
			 0x2add7c1b, /// 0x60c
			 0x5c25eb77, /// 0x610
			 0x05abec04, /// 0x614
			 0x8751873f, /// 0x618
			 0x2174b603, /// 0x61c
			 0x5457262e, /// 0x620
			 0x19071247, /// 0x624
			 0x7839dcb5, /// 0x628
			 0x63aa33a2, /// 0x62c
			 0xfbca4358, /// 0x630
			 0x6f9b191e, /// 0x634
			 0xfda2f18a, /// 0x638
			 0xd72ed62e, /// 0x63c
			 0x01b48bc5, /// 0x640
			 0x460ccdac, /// 0x644
			 0xdf56b9ef, /// 0x648
			 0x8556c88a, /// 0x64c
			 0x06015dc5, /// 0x650
			 0x70e384cf, /// 0x654
			 0x2198d480, /// 0x658
			 0x0a9026ee, /// 0x65c
			 0x979226b6, /// 0x660
			 0x1ef8c8ae, /// 0x664
			 0x75a564da, /// 0x668
			 0xad64bba0, /// 0x66c
			 0xf1b1b770, /// 0x670
			 0x6241f3b2, /// 0x674
			 0x4c1e0e80, /// 0x678
			 0xa79b3716, /// 0x67c
			 0x0aaa7af2, /// 0x680
			 0xa04252e3, /// 0x684
			 0x8e8ca6bf, /// 0x688
			 0x9a413dfa, /// 0x68c
			 0x09b5ed22, /// 0x690
			 0x63fe24f0, /// 0x694
			 0x94a716ab, /// 0x698
			 0x69534403, /// 0x69c
			 0x00606a6f, /// 0x6a0
			 0xda65a8e7, /// 0x6a4
			 0x06cfbf1e, /// 0x6a8
			 0x3cb2b5aa, /// 0x6ac
			 0xd9a31303, /// 0x6b0
			 0xc7c9bc98, /// 0x6b4
			 0x2ed8b36c, /// 0x6b8
			 0x834eb884, /// 0x6bc
			 0x74ea2e38, /// 0x6c0
			 0xccefb094, /// 0x6c4
			 0x1f1e4ca7, /// 0x6c8
			 0xcabd0f51, /// 0x6cc
			 0xd53d5177, /// 0x6d0
			 0xefe4fd2c, /// 0x6d4
			 0x0a32c72f, /// 0x6d8
			 0x04ddebed, /// 0x6dc
			 0x43021684, /// 0x6e0
			 0x3b86619a, /// 0x6e4
			 0x5ce4134c, /// 0x6e8
			 0xb7ea8e66, /// 0x6ec
			 0x6fdabe93, /// 0x6f0
			 0x8d47ca04, /// 0x6f4
			 0x72991ad0, /// 0x6f8
			 0x7610a438, /// 0x6fc
			 0x132cf6d0, /// 0x700
			 0x121db813, /// 0x704
			 0x29192342, /// 0x708
			 0xa90a3994, /// 0x70c
			 0x80af6bc1, /// 0x710
			 0xf25805c7, /// 0x714
			 0x00b8e309, /// 0x718
			 0x3261b6ee, /// 0x71c
			 0xd2b3da1b, /// 0x720
			 0xc8e746eb, /// 0x724
			 0x70823471, /// 0x728
			 0x3263ef7e, /// 0x72c
			 0xd8b1ea19, /// 0x730
			 0x6132948a, /// 0x734
			 0xb7c1a602, /// 0x738
			 0xc7d9608a, /// 0x73c
			 0x03e603bd, /// 0x740
			 0x68e97564, /// 0x744
			 0x7577ef16, /// 0x748
			 0xf77aea9f, /// 0x74c
			 0xc38ef81f, /// 0x750
			 0x721921ef, /// 0x754
			 0x045ec573, /// 0x758
			 0x5a65dd00, /// 0x75c
			 0x87791690, /// 0x760
			 0x9f5378f1, /// 0x764
			 0xd8e22050, /// 0x768
			 0x24d90c03, /// 0x76c
			 0x1d8843f5, /// 0x770
			 0xec5968cb, /// 0x774
			 0x2d9ba990, /// 0x778
			 0x62f38f78, /// 0x77c
			 0x95f1a822, /// 0x780
			 0xd118ab45, /// 0x784
			 0xa1a53f28, /// 0x788
			 0x8019a5fe, /// 0x78c
			 0x4ac443b9, /// 0x790
			 0xfb101cf6, /// 0x794
			 0x34189285, /// 0x798
			 0xf21ce958, /// 0x79c
			 0x33538a63, /// 0x7a0
			 0x64e9ec77, /// 0x7a4
			 0xafcb83c8, /// 0x7a8
			 0xc0b055fa, /// 0x7ac
			 0x69f17e38, /// 0x7b0
			 0x9f1bd403, /// 0x7b4
			 0x920d5417, /// 0x7b8
			 0xfe6c7e17, /// 0x7bc
			 0xb1562383, /// 0x7c0
			 0x163ee658, /// 0x7c4
			 0x7cfd765c, /// 0x7c8
			 0x7394564b, /// 0x7cc
			 0xe907399b, /// 0x7d0
			 0xa43c1b2f, /// 0x7d4
			 0x4bea7ce0, /// 0x7d8
			 0xed30d801, /// 0x7dc
			 0x1a6898a6, /// 0x7e0
			 0xde975fba, /// 0x7e4
			 0x92725210, /// 0x7e8
			 0xae0f114d, /// 0x7ec
			 0x6b94043f, /// 0x7f0
			 0xb9e8a941, /// 0x7f4
			 0x5d78489a, /// 0x7f8
			 0xb8a6d988, /// 0x7fc
			 0x527377d5, /// 0x800
			 0x0af6c612, /// 0x804
			 0x1668da92, /// 0x808
			 0x9fc7dcec, /// 0x80c
			 0x4fea6284, /// 0x810
			 0x1ebf4bd1, /// 0x814
			 0xb474b597, /// 0x818
			 0xecd2cd32, /// 0x81c
			 0x023145c8, /// 0x820
			 0x44c11493, /// 0x824
			 0x552f991e, /// 0x828
			 0xdaa00bd8, /// 0x82c
			 0xb1a972f7, /// 0x830
			 0x4369d952, /// 0x834
			 0x18eaf136, /// 0x838
			 0x42ab2e40, /// 0x83c
			 0x21ad4d80, /// 0x840
			 0xde777b3e, /// 0x844
			 0xa476e14d, /// 0x848
			 0x471972c7, /// 0x84c
			 0xafe2ddad, /// 0x850
			 0xe9430cea, /// 0x854
			 0x91b0bc1d, /// 0x858
			 0x5ab51397, /// 0x85c
			 0x15967f69, /// 0x860
			 0x58c3816d, /// 0x864
			 0xe32b7880, /// 0x868
			 0xb7a021cc, /// 0x86c
			 0xc6b138f5, /// 0x870
			 0xacae493a, /// 0x874
			 0xf4fb179c, /// 0x878
			 0x0c500684, /// 0x87c
			 0xe6bb5d15, /// 0x880
			 0xbd3d0141, /// 0x884
			 0x55311f28, /// 0x888
			 0xaf6caad4, /// 0x88c
			 0xabb085a4, /// 0x890
			 0xa2a2e587, /// 0x894
			 0xba74972b, /// 0x898
			 0x5027784a, /// 0x89c
			 0x3dd97dcc, /// 0x8a0
			 0xe0aeca29, /// 0x8a4
			 0x834b3073, /// 0x8a8
			 0xe3fc531b, /// 0x8ac
			 0x8ec8050e, /// 0x8b0
			 0x4b210c07, /// 0x8b4
			 0x9008094d, /// 0x8b8
			 0x489bc4ce, /// 0x8bc
			 0xe1e36ac2, /// 0x8c0
			 0x75dbe586, /// 0x8c4
			 0x3df4eb08, /// 0x8c8
			 0xea09f029, /// 0x8cc
			 0x0e3fd81e, /// 0x8d0
			 0x91437ea1, /// 0x8d4
			 0x9f8b7c46, /// 0x8d8
			 0xe3cd1c08, /// 0x8dc
			 0xe263ae24, /// 0x8e0
			 0x3d9c5355, /// 0x8e4
			 0x06f33810, /// 0x8e8
			 0x84e675c0, /// 0x8ec
			 0xe0d757d0, /// 0x8f0
			 0xbd81d798, /// 0x8f4
			 0xe6a42ed5, /// 0x8f8
			 0x45e44f4b, /// 0x8fc
			 0xbb9a6b73, /// 0x900
			 0x649b85ce, /// 0x904
			 0x0e4a34af, /// 0x908
			 0x215fad35, /// 0x90c
			 0xbad7777a, /// 0x910
			 0xd3ec48ac, /// 0x914
			 0x9b61b904, /// 0x918
			 0x77a99ce6, /// 0x91c
			 0xe38e2723, /// 0x920
			 0xfb24d2da, /// 0x924
			 0xd9dfb79d, /// 0x928
			 0xc1d1ddb6, /// 0x92c
			 0xa63bc0db, /// 0x930
			 0xadb9c6b4, /// 0x934
			 0xf9b95c98, /// 0x938
			 0x45809722, /// 0x93c
			 0xc678fe19, /// 0x940
			 0x70227f19, /// 0x944
			 0xfde525a2, /// 0x948
			 0x9014fbb2, /// 0x94c
			 0x27a6b7ff, /// 0x950
			 0x4a36cf29, /// 0x954
			 0xe31e699b, /// 0x958
			 0x796ac5b8, /// 0x95c
			 0x8888329f, /// 0x960
			 0x82e0f858, /// 0x964
			 0x1080500b, /// 0x968
			 0xd9415abb, /// 0x96c
			 0x10f3fe91, /// 0x970
			 0xfc8bea16, /// 0x974
			 0x783c68b1, /// 0x978
			 0x68d63fa7, /// 0x97c
			 0x3fdd7fcc, /// 0x980
			 0x8990cddd, /// 0x984
			 0x10769468, /// 0x988
			 0x8b168d25, /// 0x98c
			 0x2b3b4033, /// 0x990
			 0xc68d9299, /// 0x994
			 0xe60264ab, /// 0x998
			 0xd238fb8c, /// 0x99c
			 0x99313ba3, /// 0x9a0
			 0xfe7caba9, /// 0x9a4
			 0xe7efc671, /// 0x9a8
			 0x53b61396, /// 0x9ac
			 0xf92dc05f, /// 0x9b0
			 0xdea8d142, /// 0x9b4
			 0xcb1fe9a1, /// 0x9b8
			 0x26106395, /// 0x9bc
			 0xf272d971, /// 0x9c0
			 0x9dbc60b7, /// 0x9c4
			 0x7af3be94, /// 0x9c8
			 0xa910d12c, /// 0x9cc
			 0x50cd7c96, /// 0x9d0
			 0x1910df3b, /// 0x9d4
			 0x63a84734, /// 0x9d8
			 0xcf867048, /// 0x9dc
			 0x8e100ec8, /// 0x9e0
			 0xeb9f087f, /// 0x9e4
			 0x25aa6700, /// 0x9e8
			 0xb28df525, /// 0x9ec
			 0x9fc88493, /// 0x9f0
			 0x63d6aee9, /// 0x9f4
			 0xc72704e8, /// 0x9f8
			 0xddec9a5c, /// 0x9fc
			 0xaa58a263, /// 0xa00
			 0x857df266, /// 0xa04
			 0x45c78a57, /// 0xa08
			 0xe4bb8f9e, /// 0xa0c
			 0xdb0083e1, /// 0xa10
			 0xe39db25e, /// 0xa14
			 0x04e796d0, /// 0xa18
			 0x03ba44c4, /// 0xa1c
			 0xfd1205bd, /// 0xa20
			 0x6996472a, /// 0xa24
			 0x54d4e9c1, /// 0xa28
			 0xfe73c271, /// 0xa2c
			 0x38753d47, /// 0xa30
			 0x0d66eadf, /// 0xa34
			 0xc1ce8ebf, /// 0xa38
			 0x6b05ca1e, /// 0xa3c
			 0xb1dcbe2c, /// 0xa40
			 0x17a731fc, /// 0xa44
			 0xe5470172, /// 0xa48
			 0xb44aeecf, /// 0xa4c
			 0xbd318c12, /// 0xa50
			 0x34bcc757, /// 0xa54
			 0xe0dd1056, /// 0xa58
			 0x508fbe95, /// 0xa5c
			 0x38016019, /// 0xa60
			 0x3eb60b99, /// 0xa64
			 0xa94f2d1e, /// 0xa68
			 0x5428d485, /// 0xa6c
			 0x109a1a4f, /// 0xa70
			 0x985afae6, /// 0xa74
			 0x68c3908a, /// 0xa78
			 0xd5eecc34, /// 0xa7c
			 0x2a76a68b, /// 0xa80
			 0x9a7a77ef, /// 0xa84
			 0x2be22879, /// 0xa88
			 0x3cfdadb9, /// 0xa8c
			 0x0d3dca3e, /// 0xa90
			 0xb6754df4, /// 0xa94
			 0xe4192113, /// 0xa98
			 0x346a0a43, /// 0xa9c
			 0x334a01a5, /// 0xaa0
			 0xf3faf871, /// 0xaa4
			 0x4877a14a, /// 0xaa8
			 0x3138f52b, /// 0xaac
			 0xc8cf7c93, /// 0xab0
			 0x93913d9b, /// 0xab4
			 0xc40179ee, /// 0xab8
			 0xce32e78e, /// 0xabc
			 0x0b6d421f, /// 0xac0
			 0x8a628b84, /// 0xac4
			 0x46eb4ca7, /// 0xac8
			 0xb6562932, /// 0xacc
			 0x166dc5da, /// 0xad0
			 0x324ea504, /// 0xad4
			 0xcaf53d85, /// 0xad8
			 0x89e92937, /// 0xadc
			 0xb9539080, /// 0xae0
			 0x03caae2c, /// 0xae4
			 0x17aa3d06, /// 0xae8
			 0x915d59b2, /// 0xaec
			 0x42d1a8bd, /// 0xaf0
			 0x639102bb, /// 0xaf4
			 0x6b766112, /// 0xaf8
			 0xe0adaeb7, /// 0xafc
			 0xe2d92d0c, /// 0xb00
			 0x35c1a0db, /// 0xb04
			 0xbdd5182a, /// 0xb08
			 0x75520087, /// 0xb0c
			 0x23cb5626, /// 0xb10
			 0x29ea1252, /// 0xb14
			 0x75bd6f1a, /// 0xb18
			 0x34e3611d, /// 0xb1c
			 0x01effd7f, /// 0xb20
			 0xc54533e8, /// 0xb24
			 0xebd39106, /// 0xb28
			 0x901d2d70, /// 0xb2c
			 0x95cf93b0, /// 0xb30
			 0x78c5bd6e, /// 0xb34
			 0x42c620fa, /// 0xb38
			 0x12917c47, /// 0xb3c
			 0x26a69148, /// 0xb40
			 0xb11faba2, /// 0xb44
			 0xabefd998, /// 0xb48
			 0xe73c51e6, /// 0xb4c
			 0xd5640ba9, /// 0xb50
			 0x18d098bf, /// 0xb54
			 0x6318ba6f, /// 0xb58
			 0x20d6a211, /// 0xb5c
			 0x99e89c8f, /// 0xb60
			 0xfb1f8692, /// 0xb64
			 0x75fc9070, /// 0xb68
			 0x5ae8e45a, /// 0xb6c
			 0xf20872bf, /// 0xb70
			 0x46dec831, /// 0xb74
			 0x978c4c66, /// 0xb78
			 0x604f76db, /// 0xb7c
			 0xf2508b33, /// 0xb80
			 0x68a09d62, /// 0xb84
			 0x25a10b25, /// 0xb88
			 0xb34f5240, /// 0xb8c
			 0x58afa448, /// 0xb90
			 0x369a3867, /// 0xb94
			 0x77a40720, /// 0xb98
			 0xe22927b6, /// 0xb9c
			 0xf46c7435, /// 0xba0
			 0xbb509546, /// 0xba4
			 0x661db814, /// 0xba8
			 0x449f50ef, /// 0xbac
			 0x83fd862c, /// 0xbb0
			 0x71533f6b, /// 0xbb4
			 0xf2d234e6, /// 0xbb8
			 0xb74b8b0d, /// 0xbbc
			 0xf8fdd9b5, /// 0xbc0
			 0xa873243e, /// 0xbc4
			 0xe8117912, /// 0xbc8
			 0x884b17b3, /// 0xbcc
			 0x6292fe99, /// 0xbd0
			 0x7a5fd670, /// 0xbd4
			 0xd3d629bf, /// 0xbd8
			 0x4185fe7d, /// 0xbdc
			 0xf78480b7, /// 0xbe0
			 0x6c3d48eb, /// 0xbe4
			 0x7a6946ec, /// 0xbe8
			 0x65b8ad21, /// 0xbec
			 0xc5f7c8ef, /// 0xbf0
			 0xb5502639, /// 0xbf4
			 0x45f795a7, /// 0xbf8
			 0x04691d8c, /// 0xbfc
			 0x2f6f7dfa, /// 0xc00
			 0x9021b954, /// 0xc04
			 0xd174fd76, /// 0xc08
			 0x7a12e0b9, /// 0xc0c
			 0xcae37a3c, /// 0xc10
			 0xbaa4045b, /// 0xc14
			 0x6417117c, /// 0xc18
			 0xb74eba72, /// 0xc1c
			 0xe774d5a0, /// 0xc20
			 0x6d148421, /// 0xc24
			 0x559d005e, /// 0xc28
			 0x577af7cc, /// 0xc2c
			 0xab442cb7, /// 0xc30
			 0x987bfd09, /// 0xc34
			 0x81ae449b, /// 0xc38
			 0x72554bbd, /// 0xc3c
			 0x4dcdd0e3, /// 0xc40
			 0x2179be1b, /// 0xc44
			 0xac22f222, /// 0xc48
			 0xc7f0ed69, /// 0xc4c
			 0xa9e48973, /// 0xc50
			 0x88cdbfe5, /// 0xc54
			 0xc14506ba, /// 0xc58
			 0x9b4cdeb4, /// 0xc5c
			 0xe84cdcca, /// 0xc60
			 0xfd10a232, /// 0xc64
			 0xe7eadc02, /// 0xc68
			 0x8ed8376b, /// 0xc6c
			 0x67300993, /// 0xc70
			 0x137d5296, /// 0xc74
			 0xff46c780, /// 0xc78
			 0xbef6f5cd, /// 0xc7c
			 0x71f13050, /// 0xc80
			 0x34938e56, /// 0xc84
			 0x19f6564b, /// 0xc88
			 0xe9c688a2, /// 0xc8c
			 0xbffba718, /// 0xc90
			 0x70eb15be, /// 0xc94
			 0x48df85c7, /// 0xc98
			 0xfe6a0720, /// 0xc9c
			 0x842ed3c8, /// 0xca0
			 0x351d83ff, /// 0xca4
			 0x01243b52, /// 0xca8
			 0x77585b18, /// 0xcac
			 0xa7875769, /// 0xcb0
			 0x1f9b4d01, /// 0xcb4
			 0x5da74c38, /// 0xcb8
			 0xc9393cd3, /// 0xcbc
			 0xd9c4a1a1, /// 0xcc0
			 0xc8ce3d89, /// 0xcc4
			 0x80f5556e, /// 0xcc8
			 0x25fcb9a5, /// 0xccc
			 0xe42745bf, /// 0xcd0
			 0x19faa529, /// 0xcd4
			 0xfa6623bf, /// 0xcd8
			 0xbe97f1d7, /// 0xcdc
			 0xfaa7f9b1, /// 0xce0
			 0xb76c0ec6, /// 0xce4
			 0xfc430370, /// 0xce8
			 0x5a19c392, /// 0xcec
			 0xb5e5c9dd, /// 0xcf0
			 0x24ef73d3, /// 0xcf4
			 0xaf0742f1, /// 0xcf8
			 0x3962fd86, /// 0xcfc
			 0x18bfc38a, /// 0xd00
			 0x545f83ed, /// 0xd04
			 0xcf308647, /// 0xd08
			 0xff505a05, /// 0xd0c
			 0xb3f61ae7, /// 0xd10
			 0x46d059ad, /// 0xd14
			 0xe90f6c22, /// 0xd18
			 0xb7d4b0c7, /// 0xd1c
			 0xbf6d6c4d, /// 0xd20
			 0xd703bc35, /// 0xd24
			 0x276c6d32, /// 0xd28
			 0x39196c09, /// 0xd2c
			 0xd3a5d1be, /// 0xd30
			 0x85443f5e, /// 0xd34
			 0x44eaa512, /// 0xd38
			 0xcea22b2a, /// 0xd3c
			 0x39819cf6, /// 0xd40
			 0xf86ef8a9, /// 0xd44
			 0x745be787, /// 0xd48
			 0x45e6c172, /// 0xd4c
			 0x44ab1cea, /// 0xd50
			 0x76791ff6, /// 0xd54
			 0x530d9c8b, /// 0xd58
			 0x3f5a1c7e, /// 0xd5c
			 0x435bf342, /// 0xd60
			 0xb4be3d0b, /// 0xd64
			 0xc5014a28, /// 0xd68
			 0xc958c0b6, /// 0xd6c
			 0xc6809f14, /// 0xd70
			 0x175c5f33, /// 0xd74
			 0x3a134234, /// 0xd78
			 0x03b4a1cb, /// 0xd7c
			 0x88516d25, /// 0xd80
			 0x11b86d3e, /// 0xd84
			 0xb6254f07, /// 0xd88
			 0xcd8138c4, /// 0xd8c
			 0x91fca495, /// 0xd90
			 0x5e92b652, /// 0xd94
			 0xb371b8c4, /// 0xd98
			 0x5767dcf9, /// 0xd9c
			 0x7b665c48, /// 0xda0
			 0xd150403e, /// 0xda4
			 0xf72632fc, /// 0xda8
			 0xc3690394, /// 0xdac
			 0x0564ac7b, /// 0xdb0
			 0x2ae544be, /// 0xdb4
			 0xe7260919, /// 0xdb8
			 0x1d66a5a9, /// 0xdbc
			 0x37384c69, /// 0xdc0
			 0xb6d66d22, /// 0xdc4
			 0x67f1f204, /// 0xdc8
			 0x059f9964, /// 0xdcc
			 0x4fb1891e, /// 0xdd0
			 0x706900a4, /// 0xdd4
			 0xb6f2ebb1, /// 0xdd8
			 0xcb47f66d, /// 0xddc
			 0x9cd415d7, /// 0xde0
			 0xca5ecb77, /// 0xde4
			 0xdd397cdb, /// 0xde8
			 0xd4eb0b3b, /// 0xdec
			 0xa8c5cf85, /// 0xdf0
			 0xb2181652, /// 0xdf4
			 0x4159c55a, /// 0xdf8
			 0x9341883a, /// 0xdfc
			 0xa5c43a03, /// 0xe00
			 0xa5f47427, /// 0xe04
			 0x6952bfae, /// 0xe08
			 0xbcaaa3d0, /// 0xe0c
			 0x51c4d2dc, /// 0xe10
			 0x04c661be, /// 0xe14
			 0xb25f84fd, /// 0xe18
			 0x101741c3, /// 0xe1c
			 0x4282083c, /// 0xe20
			 0x738802a2, /// 0xe24
			 0xa53ce1c9, /// 0xe28
			 0x860ff871, /// 0xe2c
			 0x7a3101f4, /// 0xe30
			 0x708ca66b, /// 0xe34
			 0x38909de0, /// 0xe38
			 0xc7f0dbd3, /// 0xe3c
			 0x37cdf12d, /// 0xe40
			 0xd680435f, /// 0xe44
			 0x2da97073, /// 0xe48
			 0xe0cd7fde, /// 0xe4c
			 0x84a24593, /// 0xe50
			 0xccac0817, /// 0xe54
			 0xaeee0fbd, /// 0xe58
			 0x76125c8d, /// 0xe5c
			 0x62eb40ca, /// 0xe60
			 0x90f8b364, /// 0xe64
			 0x99652e46, /// 0xe68
			 0x6421ab30, /// 0xe6c
			 0x962891be, /// 0xe70
			 0x5c497970, /// 0xe74
			 0x4ae9768b, /// 0xe78
			 0xe46cbe68, /// 0xe7c
			 0x4f9c33d9, /// 0xe80
			 0xe02cd83d, /// 0xe84
			 0xd52931d1, /// 0xe88
			 0x7e9e6713, /// 0xe8c
			 0x2507b51c, /// 0xe90
			 0x0ceb7c5f, /// 0xe94
			 0x9c25e1ff, /// 0xe98
			 0xa1006856, /// 0xe9c
			 0x98ee07db, /// 0xea0
			 0xe8f4b0cc, /// 0xea4
			 0xc818f1f5, /// 0xea8
			 0x05a530b2, /// 0xeac
			 0x4e45ca37, /// 0xeb0
			 0x7e2152c5, /// 0xeb4
			 0xaa99f89c, /// 0xeb8
			 0x86869c8a, /// 0xebc
			 0xbf154850, /// 0xec0
			 0x73230263, /// 0xec4
			 0x48d4c77b, /// 0xec8
			 0xad713586, /// 0xecc
			 0xf41d4f30, /// 0xed0
			 0x67cac4ac, /// 0xed4
			 0x0c5aef75, /// 0xed8
			 0xe8cb2d8f, /// 0xedc
			 0xd9485789, /// 0xee0
			 0xe2f02a17, /// 0xee4
			 0xf256f3e5, /// 0xee8
			 0xa5007659, /// 0xeec
			 0x968f8d5c, /// 0xef0
			 0x604e7f0d, /// 0xef4
			 0xe2beeec3, /// 0xef8
			 0xc944e38b, /// 0xefc
			 0xe9e75046, /// 0xf00
			 0xb5cee0ec, /// 0xf04
			 0xf3f89d29, /// 0xf08
			 0x6dd443bb, /// 0xf0c
			 0x3a1872b2, /// 0xf10
			 0xba222ce9, /// 0xf14
			 0x7b63706b, /// 0xf18
			 0x85d0d92a, /// 0xf1c
			 0x837f4c77, /// 0xf20
			 0xd8644aac, /// 0xf24
			 0x73cdb0e8, /// 0xf28
			 0x2ae61d1b, /// 0xf2c
			 0x6db7aacd, /// 0xf30
			 0x3700c3e3, /// 0xf34
			 0xd2b31b8a, /// 0xf38
			 0xb873292d, /// 0xf3c
			 0x95b43059, /// 0xf40
			 0x650bd882, /// 0xf44
			 0x54f0c720, /// 0xf48
			 0x369db131, /// 0xf4c
			 0x70adf0b0, /// 0xf50
			 0x5b77636a, /// 0xf54
			 0x0ce1541a, /// 0xf58
			 0xc69712fb, /// 0xf5c
			 0xd5f854db, /// 0xf60
			 0xbc6a4fa6, /// 0xf64
			 0x9c2cd3cb, /// 0xf68
			 0xf3f1b10a, /// 0xf6c
			 0x845df653, /// 0xf70
			 0x48803f28, /// 0xf74
			 0xb79a96b7, /// 0xf78
			 0x4263898d, /// 0xf7c
			 0xf48f1c25, /// 0xf80
			 0xe995c297, /// 0xf84
			 0x91fa5a4e, /// 0xf88
			 0x40b554bd, /// 0xf8c
			 0x47be4792, /// 0xf90
			 0xe01f10a6, /// 0xf94
			 0x7aaa809f, /// 0xf98
			 0xd52d33fa, /// 0xf9c
			 0x61e56d4f, /// 0xfa0
			 0xd9247a7a, /// 0xfa4
			 0xd01e0ed7, /// 0xfa8
			 0x7c2bf2b2, /// 0xfac
			 0x584724c1, /// 0xfb0
			 0xff872c71, /// 0xfb4
			 0x826f6f89, /// 0xfb8
			 0x1a441e04, /// 0xfbc
			 0x0b25f45a, /// 0xfc0
			 0x470b8163, /// 0xfc4
			 0xeab21e19, /// 0xfc8
			 0x15d68e9e, /// 0xfcc
			 0xab0a6f62, /// 0xfd0
			 0x93f789e4, /// 0xfd4
			 0x812fbf01, /// 0xfd8
			 0x7beca2b4, /// 0xfdc
			 0xb5215629, /// 0xfe0
			 0x90c954db, /// 0xfe4
			 0x689cb701, /// 0xfe8
			 0xd47c1864, /// 0xfec
			 0x3348bb53, /// 0xff0
			 0x28ba4877, /// 0xff4
			 0x4ff05695, /// 0xff8
			 0x5bf8a5bd /// last
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
			 0x00000030,
			 0x00000398,
			 0x0000019c,
			 0x000001fc,
			 0x000001e0,
			 0x000002b0,
			 0x000001c0,
			 0x00000314,

			 /// vpu register f2
			 0x41900000,
			 0x41e80000,
			 0x40e00000,
			 0x41c80000,
			 0x41a00000,
			 0x41f00000,
			 0x41700000,
			 0x41f00000,

			 /// vpu register f3
			 0x41500000,
			 0x41c00000,
			 0x41300000,
			 0x41900000,
			 0x41800000,
			 0x41400000,
			 0x40e00000,
			 0x40c00000,

			 /// vpu register f4
			 0x41d80000,
			 0x41600000,
			 0x41b80000,
			 0x41100000,
			 0x40800000,
			 0x41400000,
			 0x41200000,
			 0x41500000,

			 /// vpu register f5
			 0x41e80000,
			 0x41700000,
			 0x41f80000,
			 0x40e00000,
			 0x41700000,
			 0x41e80000,
			 0x41800000,
			 0x40a00000,

			 /// vpu register f6
			 0x40c00000,
			 0x40e00000,
			 0x41e00000,
			 0x40c00000,
			 0x40000000,
			 0x41b80000,
			 0x41980000,
			 0x41f80000,

			 /// vpu register f7
			 0x41b00000,
			 0x41e00000,
			 0x41600000,
			 0x41f00000,
			 0x40000000,
			 0x41400000,
			 0x41300000,
			 0x41b80000,

			 /// vpu register f8
			 0x41900000,
			 0x41700000,
			 0x41400000,
			 0x41c80000,
			 0x40000000,
			 0x40000000,
			 0x41880000,
			 0x41400000,

			 /// vpu register f9
			 0x41a00000,
			 0x41500000,
			 0x3f800000,
			 0x41e80000,
			 0x40400000,
			 0x41700000,
			 0x41d80000,
			 0x41200000,

			 /// vpu register f10
			 0x41100000,
			 0x41f00000,
			 0x41d80000,
			 0x41d00000,
			 0x40800000,
			 0x41300000,
			 0x41c00000,
			 0x41e00000,

			 /// vpu register f11
			 0x41d80000,
			 0x41e80000,
			 0x40800000,
			 0x41200000,
			 0x41900000,
			 0x41800000,
			 0x40c00000,
			 0x3f800000,

			 /// vpu register f12
			 0x40a00000,
			 0x41200000,
			 0x40e00000,
			 0x41880000,
			 0x41c00000,
			 0x40e00000,
			 0x40000000,
			 0x41e80000,

			 /// vpu register f13
			 0x41f80000,
			 0x42000000,
			 0x41b00000,
			 0x41f00000,
			 0x40c00000,
			 0x41c80000,
			 0x41900000,
			 0x41a00000,

			 /// vpu register f14
			 0x41200000,
			 0x41880000,
			 0x41d80000,
			 0x41900000,
			 0x40a00000,
			 0x41300000,
			 0x41100000,
			 0x41100000,

			 /// vpu register f15
			 0x41900000,
			 0x41000000,
			 0x41800000,
			 0x41d80000,
			 0x41100000,
			 0x41c80000,
			 0x41300000,
			 0x41980000,

			 /// vpu register f16
			 0x41880000,
			 0x41f00000,
			 0x41d80000,
			 0x41d00000,
			 0x40e00000,
			 0x41e00000,
			 0x41e80000,
			 0x41980000,

			 /// vpu register f17
			 0x41d00000,
			 0x40000000,
			 0x40c00000,
			 0x40c00000,
			 0x41300000,
			 0x41b00000,
			 0x41600000,
			 0x41880000,

			 /// vpu register f18
			 0x41300000,
			 0x41900000,
			 0x40e00000,
			 0x41000000,
			 0x41600000,
			 0x41d00000,
			 0x40400000,
			 0x42000000,

			 /// vpu register f19
			 0x41700000,
			 0x41a00000,
			 0x41000000,
			 0x41200000,
			 0x41900000,
			 0x41700000,
			 0x41500000,
			 0x41f00000,

			 /// vpu register f20
			 0x40e00000,
			 0x41c80000,
			 0x41d00000,
			 0x41300000,
			 0x41d80000,
			 0x41d80000,
			 0x41100000,
			 0x41e00000,

			 /// vpu register f21
			 0x41980000,
			 0x41d00000,
			 0x41300000,
			 0x3f800000,
			 0x41200000,
			 0x41900000,
			 0x41900000,
			 0x41e80000,

			 /// vpu register f22
			 0x41c00000,
			 0x40e00000,
			 0x41800000,
			 0x41980000,
			 0x41200000,
			 0x41f80000,
			 0x41600000,
			 0x41c80000,

			 /// vpu register f23
			 0x41d80000,
			 0x41700000,
			 0x40000000,
			 0x40000000,
			 0x41000000,
			 0x41000000,
			 0x40a00000,
			 0x40a00000,

			 /// vpu register f24
			 0x41880000,
			 0x40400000,
			 0x40a00000,
			 0x41300000,
			 0x41100000,
			 0x41100000,
			 0x41100000,
			 0x41a80000,

			 /// vpu register f25
			 0x41600000,
			 0x41e00000,
			 0x41c80000,
			 0x3f800000,
			 0x41700000,
			 0x40e00000,
			 0x41c80000,
			 0x40400000,

			 /// vpu register f26
			 0x41100000,
			 0x41a80000,
			 0x41e00000,
			 0x40400000,
			 0x41a80000,
			 0x41d00000,
			 0x41600000,
			 0x41b00000,

			 /// vpu register f27
			 0x3f800000,
			 0x41f00000,
			 0x41e00000,
			 0x41f80000,
			 0x41d00000,
			 0x42000000,
			 0x42000000,
			 0x41f00000,

			 /// vpu register f28
			 0x41500000,
			 0x41d00000,
			 0x41900000,
			 0x40a00000,
			 0x41100000,
			 0x41f00000,
			 0x41d80000,
			 0x40e00000,

			 /// vpu register f29
			 0x41b00000,
			 0x41d80000,
			 0x41800000,
			 0x41980000,
			 0x40e00000,
			 0x41d80000,
			 0x42000000,
			 0x41000000,

			 /// vpu register f30
			 0x41800000,
			 0x40c00000,
			 0x41200000,
			 0x41a00000,
			 0x40c00000,
			 0x41200000,
			 0x41500000,
			 0x41900000,

			 /// vpu register f31
			 0x41900000,
			 0x41200000,
			 0x40400000,
			 0x41500000,
			 0x41e00000,
			 0x41c80000,
			 0x41100000,
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
		"nop\n"
		"nop\n"
		"fmul.ps f8, f7, f0, rup\n"                           ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f9, f14, f30, dyn\n"                         ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f4, f26, f12, rdn\n"                         ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f28, f10, f21, rup\n"                        ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f25, f13, f21, dyn\n"                        ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f22, f25, f30, rdn\n"                        ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f18, f25, f17, rne\n"                        ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f4, f6, f25, rtz\n"                          ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f9, f15, f25, rmm\n"                         ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f25, f3, f24, rup\n"                         ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f18, f25, f20, rdn\n"                        ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f22, f23, f7, dyn\n"                         ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f16, f29, f12, rup\n"                        ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f20, f29, f2, rmm\n"                         ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f17, f6, f21, rdn\n"                         ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f30, f27, f17, rtz\n"                        ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f2, f1, f23, rne\n"                          ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f19, f15, f11, rne\n"                        ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f18, f12, f25, rdn\n"                        ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f3, f18, f11, rdn\n"                         ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f2, f4, f16, rtz\n"                          ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f23, f6, f12, rup\n"                         ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f3, f16, f24, rdn\n"                         ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f18, f0, f8, dyn\n"                          ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f2, f12, f23, rmm\n"                         ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f8, f1, f11, rtz\n"                          ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f29, f12, f5, rup\n"                         ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f30, f2, f7, rdn\n"                          ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f13, f7, f14, rup\n"                         ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f5, f7, f4, rup\n"                           ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f14, f18, f23, rup\n"                        ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f24, f20, f6, rmm\n"                         ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f19, f15, f15, rne\n"                        ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f21, f24, f24, dyn\n"                        ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f24, f1, f20, rmm\n"                         ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f28, f28, f30, dyn\n"                        ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f3, f30, f13, rmm\n"                         ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f24, f13, f24, rup\n"                        ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f26, f25, f10, rtz\n"                        ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f6, f28, f13, rdn\n"                         ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f4, f17, f24, rdn\n"                         ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f27, f16, f23, rup\n"                        ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f16, f5, f24, rmm\n"                         ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f23, f7, f30, rup\n"                         ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f28, f19, f6, rdn\n"                         ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f30, f10, f22, rmm\n"                        ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f4, f27, f17, rne\n"                         ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f6, f2, f22, rdn\n"                          ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f11, f10, f22, rmm\n"                        ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f18, f25, f15, rdn\n"                        ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f18, f27, f11, dyn\n"                        ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f11, f20, f21, rup\n"                        ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f14, f8, f2, dyn\n"                          ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f12, f28, f13, rne\n"                        ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f27, f5, f1, rtz\n"                          ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f29, f1, f11, rmm\n"                         ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f20, f2, f24, dyn\n"                         ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f20, f18, f22, rne\n"                        ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f5, f17, f15, dyn\n"                         ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f30, f16, f3, rup\n"                         ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f30, f6, f27, rne\n"                         ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f30, f22, f9, rtz\n"                         ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f25, f21, f24, rne\n"                        ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f17, f17, f30, rtz\n"                        ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f10, f8, f27, rdn\n"                         ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f25, f11, f12, rtz\n"                        ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f20, f22, f13, rdn\n"                        ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f9, f18, f0, rmm\n"                          ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f9, f21, f12, rmm\n"                         ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f5, f26, f1, rtz\n"                          ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f18, f12, f21, rdn\n"                        ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f14, f27, f20, dyn\n"                        ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f6, f9, f19, rmm\n"                          ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f11, f27, f15, rne\n"                        ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f15, f29, f1, rne\n"                         ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f2, f15, f13, rne\n"                         ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f14, f3, f12, rmm\n"                         ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f17, f9, f13, rdn\n"                         ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f18, f6, f9, rne\n"                          ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f6, f13, f23, rup\n"                         ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f9, f28, f23, rne\n"                         ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f10, f0, f22, rne\n"                         ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f18, f9, f7, rne\n"                          ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f25, f6, f16, rdn\n"                         ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f25, f22, f27, rdn\n"                        ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f8, f21, f28, rne\n"                         ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f6, f1, f3, dyn\n"                           ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f11, f13, f1, rne\n"                         ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f18, f1, f17, dyn\n"                         ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f11, f13, f13, rdn\n"                        ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f17, f20, f5, rtz\n"                         ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f19, f17, f22, rmm\n"                        ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f4, f0, f13, rtz\n"                          ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f3, f3, f23, rmm\n"                          ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f16, f4, f10, rup\n"                         ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f18, f12, f1, dyn\n"                         ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f14, f1, f29, dyn\n"                         ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f3, f29, f23, rne\n"                         ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.ps f22, f12, f11, rtz\n"                        ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
