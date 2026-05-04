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
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00000
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00004
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00008
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0000c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00010
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00014
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00018
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0001c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00020
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00024
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00028
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0002c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00030
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00034
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00038
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0003c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00040
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00044
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0004c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00050
			 0x0e000007,                                                  // Trailing 1s:                                /// 00054
			 0xcb000000,                                                  // -8388608.0                                  /// 00058
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0005c
			 0x80000000,                                                  // -zero                                       /// 00060
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00064
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00068
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0006c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00070
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00078
			 0x00000000,                                                  // zero                                        /// 0007c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00080
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00084
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x0e000001,                                                  // Trailing 1s:                                /// 0008c
			 0x00000000,                                                  // zero                                        /// 00090
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00098
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0009c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000a0
			 0xffc00001,                                                  // -signaling NaN                              /// 000a4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a8
			 0x7f800000,                                                  // inf                                         /// 000ac
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000b0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b4
			 0x0c700000,                                                  // Leading 1s:                                 /// 000b8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000bc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000c0
			 0x80011111,                                                  // -9.7958E-41                                 /// 000c4
			 0x0e000003,                                                  // Trailing 1s:                                /// 000c8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 000cc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 000d4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000dc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000e0
			 0x55555555,                                                  // 4 random values                             /// 000e4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000e8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x4b000000,                                                  // 8388608.0                                   /// 000f0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 000f4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000f8
			 0x0c780000,                                                  // Leading 1s:                                 /// 000fc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00100
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00104
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00108
			 0xff800000,                                                  // -inf                                        /// 0010c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00114
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0011c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00120
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00124
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00128
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0012c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00130
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00134
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00138
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0013c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00140
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00144
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00148
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0014c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00150
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00158
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0015c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00160
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00164
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00168
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00170
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00174
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00178
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0017c
			 0x00000000,                                                  // zero                                        /// 00180
			 0x0e000003,                                                  // Trailing 1s:                                /// 00184
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0018c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00190
			 0x7fc00001,                                                  // signaling NaN                               /// 00194
			 0x0c700000,                                                  // Leading 1s:                                 /// 00198
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0019c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001a0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ac
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001b0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001b4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001bc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 001c0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001c8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 001cc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001d0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001d4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001d8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 001dc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 001e8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001ec
			 0x7f800000,                                                  // inf                                         /// 001f0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001f4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001f8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001fc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00200
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00204
			 0xcb000000,                                                  // -8388608.0                                  /// 00208
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0020c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00210
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00214
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00220
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00224
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00228
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0022c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00230
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00234
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00238
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0023c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00240
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x00011111,                                                  // 9.7958E-41                                  /// 00248
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x3f028f5c,                                                  // 0.51                                        /// 00250
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00254
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00258
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0025c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00260
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00264
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00268
			 0x3f028f5c,                                                  // 0.51                                        /// 0026c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00270
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00274
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00278
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0027c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00280
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00284
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00288
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00290
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00294
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00298
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0029c
			 0x80011111,                                                  // -9.7958E-41                                 /// 002a0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002a4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002a8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002ac
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002b4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002b8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002bc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002c0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 002c4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 002cc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 002d0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002d4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002d8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002dc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 002f0
			 0x0e000003,                                                  // Trailing 1s:                                /// 002f4
			 0x33333333,                                                  // 4 random values                             /// 002f8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00304
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00308
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00310
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00314
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00318
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00320
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00324
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00328
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0032c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00330
			 0x0c600000,                                                  // Leading 1s:                                 /// 00334
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00338
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0033c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00344
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00348
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0034c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00350
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00354
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00358
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0035c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00360
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0xffc00001,                                                  // -signaling NaN                              /// 00368
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0036c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00370
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00374
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00378
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0037c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00380
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00384
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00388
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0038c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00390
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00398
			 0x80000000,                                                  // -zero                                       /// 0039c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 003a0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003a4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 003a8
			 0x0c400000,                                                  // Leading 1s:                                 /// 003ac
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003b0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003b4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 003b8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 003bc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 003c0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003c4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003c8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003cc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 003d4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003d8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003dc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003e0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e8
			 0x0e000003,                                                  // Trailing 1s:                                /// 003ec
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 003f0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003f4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00400
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00404
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00408
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0040c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00410
			 0x7fc00001,                                                  // signaling NaN                               /// 00414
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00418
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0041c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00420
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00424
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00428
			 0x4b000000,                                                  // 8388608.0                                   /// 0042c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00430
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00438
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0043c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00440
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00444
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00448
			 0x80011111,                                                  // -9.7958E-41                                 /// 0044c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00450
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00454
			 0xbf028f5c,                                                  // -0.51                                       /// 00458
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0045c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00460
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00464
			 0xff800000,                                                  // -inf                                        /// 00468
			 0xbf028f5c,                                                  // -0.51                                       /// 0046c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00470
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00474
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00478
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0047c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00480
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00484
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00488
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0048c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00490
			 0x33333333,                                                  // 4 random values                             /// 00494
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00498
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 004a0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b0
			 0x33333333,                                                  // 4 random values                             /// 004b4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004bc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004c0
			 0x00000000,                                                  // zero                                        /// 004c4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004c8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004cc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 004d0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004d4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 004dc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 004e0
			 0xbf028f5c,                                                  // -0.51                                       /// 004e4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004e8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004ec
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 004f0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 004f4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004f8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004fc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00500
			 0x0c780000,                                                  // Leading 1s:                                 /// 00504
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00508
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0050c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00510
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00518
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0051c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00520
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00524
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00528
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0052c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00534
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00538
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0053c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00540
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x80011111,                                                  // -9.7958E-41                                 /// 00548
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0054c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00550
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00554
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00558
			 0xbf028f5c,                                                  // -0.51                                       /// 0055c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00560
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00564
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00568
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0056c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00570
			 0x55555555,                                                  // 4 random values                             /// 00574
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00578
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0057c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00580
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00584
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00588
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0058c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00590
			 0x00000000,                                                  // zero                                        /// 00594
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00598
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0059c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 005a0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 005a8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005ac
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 005b0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005b4
			 0x7fc00001,                                                  // signaling NaN                               /// 005b8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005bc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005c0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 005c4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 005c8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005cc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 005d0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005d4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 005d8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005dc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005e4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 005e8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 005ec
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 005f0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005f4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 005f8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005fc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00600
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00604
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00608
			 0xffc00001,                                                  // -signaling NaN                              /// 0060c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00610
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00614
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0061c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00620
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00624
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00628
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00630
			 0x33333333,                                                  // 4 random values                             /// 00634
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0063c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00640
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00644
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00648
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0064c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00650
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00654
			 0xffc00001,                                                  // -signaling NaN                              /// 00658
			 0xcb000000,                                                  // -8388608.0                                  /// 0065c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00660
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00664
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00668
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0066c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00670
			 0x0c400000,                                                  // Leading 1s:                                 /// 00674
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0067c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00684
			 0x0c700000,                                                  // Leading 1s:                                 /// 00688
			 0xffc00001,                                                  // -signaling NaN                              /// 0068c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00690
			 0x3f028f5c,                                                  // 0.51                                        /// 00694
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00698
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0069c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 006a4
			 0x0c700000,                                                  // Leading 1s:                                 /// 006a8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006b8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006bc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006c0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 006c4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 006c8
			 0x4b000000,                                                  // 8388608.0                                   /// 006cc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 006d0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d4
			 0x0c400000,                                                  // Leading 1s:                                 /// 006d8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006dc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 006e0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006e4
			 0x0c780000,                                                  // Leading 1s:                                 /// 006e8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ec
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 006f0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 006f4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006f8
			 0x33333333,                                                  // 4 random values                             /// 006fc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00704
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00708
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0070c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00710
			 0x00000000,                                                  // zero                                        /// 00714
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0071c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00728
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0072c
			 0xbf028f5c,                                                  // -0.51                                       /// 00730
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00734
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00738
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0073c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00740
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00744
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00748
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0074c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00750
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00754
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00758
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0075c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00760
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00764
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00768
			 0x80011111,                                                  // -9.7958E-41                                 /// 0076c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00774
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00778
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00780
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00784
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00788
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00794
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0079c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007a0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007a4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007a8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ac
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 007b0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007b4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007b8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007bc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007c0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007c4
			 0x0e000001,                                                  // Trailing 1s:                                /// 007c8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 007cc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007d0
			 0x80011111,                                                  // -9.7958E-41                                 /// 007d4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 007d8
			 0x0e000007,                                                  // Trailing 1s:                                /// 007dc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 007e0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 007e4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007e8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 007ec
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007f0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007f4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007f8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 007fc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00804
			 0x0e000001,                                                  // Trailing 1s:                                /// 00808
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0080c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00810
			 0x33333333,                                                  // 4 random values                             /// 00814
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00818
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0081c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00820
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0xffc00001,                                                  // -signaling NaN                              /// 00828
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00830
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00834
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00838
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0083c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00840
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00844
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00848
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0084c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00850
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0x0e000007,                                                  // Trailing 1s:                                /// 00858
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0085c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00864
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00868
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0086c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00870
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00874
			 0x0c400000,                                                  // Leading 1s:                                 /// 00878
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00880
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00884
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00888
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0088c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00890
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00894
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00898
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0089c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008a0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0xcb000000,                                                  // -8388608.0                                  /// 008a8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ac
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008b0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008b4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008b8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008bc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008c0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008c4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008c8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008cc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008d0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008d4
			 0x0c780000,                                                  // Leading 1s:                                 /// 008d8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008e0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008e8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008f0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008f4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 008f8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008fc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00900
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00904
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00908
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0090c
			 0xbf028f5c,                                                  // -0.51                                       /// 00910
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00914
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00918
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0091c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00920
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00924
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00928
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0092c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00930
			 0x0c780000,                                                  // Leading 1s:                                 /// 00934
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00938
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0093c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00940
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00944
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00948
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0094c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00950
			 0x00011111,                                                  // 9.7958E-41                                  /// 00954
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00958
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0095c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00960
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00964
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00968
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0096c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00970
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00974
			 0xcb000000,                                                  // -8388608.0                                  /// 00978
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0097c
			 0x80000000,                                                  // -zero                                       /// 00980
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00984
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00988
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00990
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00994
			 0x0c780000,                                                  // Leading 1s:                                 /// 00998
			 0x0c400000,                                                  // Leading 1s:                                 /// 0099c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009a0
			 0xffc00001,                                                  // -signaling NaN                              /// 009a4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009a8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 009b0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009b4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009b8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009bc
			 0x0e000001,                                                  // Trailing 1s:                                /// 009c0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009c4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009c8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009cc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009d0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009d4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009d8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009dc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 009e4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009e8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009ec
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 009f0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009f4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 009fc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a00
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a04
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a08
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a0c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a10
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a18
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a1c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a20
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a24
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a30
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a34
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a38
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a3c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a40
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a44
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a48
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a50
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a54
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a58
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a5c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a60
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a64
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a6c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a70
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a78
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a84
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a88
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a8c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a90
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a94
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a98
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a9c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00aa0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00aa4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00aa8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00aac
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ab0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ab8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00abc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ac0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ac4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ac8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00acc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ad0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ad4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00adc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ae0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ae8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00aec
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00af4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00af8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b04
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b08
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b0c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b10
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b14
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b1c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b20
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b28
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b30
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b34
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b38
			 0x7fc00001,                                                  // signaling NaN                               /// 00b3c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b40
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b48
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b4c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b50
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b54
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b58
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b5c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b64
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b68
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b70
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b74
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b78
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b80
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b84
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b88
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b8c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b90
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b94
			 0x3f028f5c,                                                  // 0.51                                        /// 00b98
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b9c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ba4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ba8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0xff800000,                                                  // -inf                                        /// 00bb0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00bb4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00bbc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00bc0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00bc4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00bc8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00bcc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00bd8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00bdc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00be0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00be8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00bec
			 0x7f800000,                                                  // inf                                         /// 00bf0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00bf4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00bf8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00bfc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c00
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x33333333,                                                  // 4 random values                             /// 00c0c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c10
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c14
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c18
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c1c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c24
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c28
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c2c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c30
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c34
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c38
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c3c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c40
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c48
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c50
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c54
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c58
			 0x7f800000,                                                  // inf                                         /// 00c5c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c68
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c6c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c70
			 0x4b000000,                                                  // 8388608.0                                   /// 00c74
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c78
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c7c
			 0x3f028f5c,                                                  // 0.51                                        /// 00c80
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c84
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c88
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c8c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c90
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c94
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c98
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c9c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ca0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ca4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ca8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00cac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00cb4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00cbc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00cc0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00cc8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ccc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00cd0
			 0x7fc00001,                                                  // signaling NaN                               /// 00cd4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00cd8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cdc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ce0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ce4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00cec
			 0x0c600000,                                                  // Leading 1s:                                 /// 00cf0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00cf4
			 0x7f800000,                                                  // inf                                         /// 00cf8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00cfc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d00
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d04
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d08
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d0c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d10
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d14
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d18
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d20
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d24
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d28
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d2c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d30
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d34
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d38
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d40
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d48
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d4c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d50
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d54
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d58
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d5c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d64
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d70
			 0xffc00001,                                                  // -signaling NaN                              /// 00d74
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d78
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d7c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d80
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d84
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d8c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d90
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d94
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d98
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d9c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00da0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00da4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00da8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00db0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00db4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00db8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00dbc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00dc0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00dc4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00dc8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dcc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00dd8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ddc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00de0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00de8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00df0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00df4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00df8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00dfc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e00
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e04
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e08
			 0x55555555,                                                  // 4 random values                             /// 00e0c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e10
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e14
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e18
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e1c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e20
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e24
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e2c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e30
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e34
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e38
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e3c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e40
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e44
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e48
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e4c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e50
			 0xff800000,                                                  // -inf                                        /// 00e54
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e58
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e5c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e60
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e64
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e68
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e6c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e70
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e74
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e7c
			 0x55555555,                                                  // 4 random values                             /// 00e80
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e84
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e88
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e8c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e90
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e94
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e98
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e9c
			 0x4b000000,                                                  // 8388608.0                                   /// 00ea0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea4
			 0x33333333,                                                  // 4 random values                             /// 00ea8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00eb4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ebc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ec4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ecc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ed8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00edc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ee0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ee4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ee8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ef0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ef4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00efc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f00
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f0c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f10
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f14
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f18
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f1c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f20
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f24
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f28
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f2c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f30
			 0x3f028f5c,                                                  // 0.51                                        /// 00f34
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f38
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f40
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f44
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f4c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f50
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f54
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f58
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f5c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f60
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f68
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f6c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f70
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f74
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f78
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f7c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f80
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f84
			 0x7f800000,                                                  // inf                                         /// 00f88
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f90
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f94
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f98
			 0x4b000000,                                                  // 8388608.0                                   /// 00f9c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00fa0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00fa4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00fa8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00fac
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00fb0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00fb4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00fb8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fbc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00fc0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00fc8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fcc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00fd4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00fd8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00fdc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fe0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00fe4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00fec
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ff0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ff4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ff8
			 0x80020000                                                  // SP - 1 bit alone set in mantissa -ve        /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x55ccaac6, /// 0x0
			 0x04796e16, /// 0x4
			 0xe929c40d, /// 0x8
			 0x44c01c42, /// 0xc
			 0xc230a6df, /// 0x10
			 0x347edfab, /// 0x14
			 0x25e325d6, /// 0x18
			 0x2ebb1de5, /// 0x1c
			 0xb07096bf, /// 0x20
			 0x9c14ca40, /// 0x24
			 0x37a18b7b, /// 0x28
			 0x6bc1e2b8, /// 0x2c
			 0xd92edebd, /// 0x30
			 0x7b1acfb7, /// 0x34
			 0x17e333ef, /// 0x38
			 0x416362f7, /// 0x3c
			 0x1c3d5c2a, /// 0x40
			 0x78c434e0, /// 0x44
			 0xb585383e, /// 0x48
			 0x2dfab55c, /// 0x4c
			 0x5e462549, /// 0x50
			 0xb888a0ec, /// 0x54
			 0x4ccf654f, /// 0x58
			 0x5a889424, /// 0x5c
			 0x422cc24f, /// 0x60
			 0x4f103655, /// 0x64
			 0x508befc9, /// 0x68
			 0x8a1fafad, /// 0x6c
			 0x59d46ba4, /// 0x70
			 0x3eff768b, /// 0x74
			 0xb8f060dc, /// 0x78
			 0xbf80fa6a, /// 0x7c
			 0x2b93f96b, /// 0x80
			 0xf8153abf, /// 0x84
			 0x1ff5c3e5, /// 0x88
			 0xaac759cb, /// 0x8c
			 0xcc9527f5, /// 0x90
			 0x6d484b2a, /// 0x94
			 0x0fefbecb, /// 0x98
			 0x0856a24f, /// 0x9c
			 0x544016d7, /// 0xa0
			 0xb78a55f1, /// 0xa4
			 0x947b80ee, /// 0xa8
			 0xc0b0653e, /// 0xac
			 0x8df52f62, /// 0xb0
			 0x7bbc6427, /// 0xb4
			 0xd0c12580, /// 0xb8
			 0x5dbf0ae4, /// 0xbc
			 0x5738f09c, /// 0xc0
			 0x76ee98c3, /// 0xc4
			 0xdcd89852, /// 0xc8
			 0x41a4ad68, /// 0xcc
			 0x3f031956, /// 0xd0
			 0x7c57d6df, /// 0xd4
			 0xb9e129d2, /// 0xd8
			 0x3f551cbc, /// 0xdc
			 0xf511ab96, /// 0xe0
			 0xc6140efa, /// 0xe4
			 0x21067a3f, /// 0xe8
			 0x663d6d17, /// 0xec
			 0xe3a07f97, /// 0xf0
			 0x972e771a, /// 0xf4
			 0x2a37fb18, /// 0xf8
			 0x12fb0ec6, /// 0xfc
			 0xd473db2e, /// 0x100
			 0xe78b2f98, /// 0x104
			 0x03264b5e, /// 0x108
			 0x417d8d51, /// 0x10c
			 0x84da746b, /// 0x110
			 0x0a6e80c3, /// 0x114
			 0x16aa5e4f, /// 0x118
			 0xbdcc07d5, /// 0x11c
			 0x08e02d08, /// 0x120
			 0x60042ee1, /// 0x124
			 0x962dc834, /// 0x128
			 0x89e51cc3, /// 0x12c
			 0x3853aa30, /// 0x130
			 0xb387ae55, /// 0x134
			 0x830defb7, /// 0x138
			 0xa5dc137d, /// 0x13c
			 0x4f21faad, /// 0x140
			 0xeefca7b8, /// 0x144
			 0xf1d1c5f1, /// 0x148
			 0xbee7bdfb, /// 0x14c
			 0x9a4146e0, /// 0x150
			 0xa4c22ffe, /// 0x154
			 0xbd807e19, /// 0x158
			 0xda94d831, /// 0x15c
			 0xe943a2e0, /// 0x160
			 0xac81caa1, /// 0x164
			 0xacaee3b1, /// 0x168
			 0x41a3fb5a, /// 0x16c
			 0x7bd7a3c8, /// 0x170
			 0x1ad715eb, /// 0x174
			 0x59ff1c45, /// 0x178
			 0xc0e37530, /// 0x17c
			 0x3e7c448c, /// 0x180
			 0x408ac5a6, /// 0x184
			 0x01b44209, /// 0x188
			 0x93bf99f4, /// 0x18c
			 0x9d54b074, /// 0x190
			 0x715f1a4b, /// 0x194
			 0x128575d2, /// 0x198
			 0x90099f9b, /// 0x19c
			 0x75e09e37, /// 0x1a0
			 0xd24ac74b, /// 0x1a4
			 0x459a5838, /// 0x1a8
			 0x05ec6eee, /// 0x1ac
			 0x2ea8cf21, /// 0x1b0
			 0x67398fb6, /// 0x1b4
			 0xcdeaa3cd, /// 0x1b8
			 0x1a9e1c84, /// 0x1bc
			 0x3c999cd2, /// 0x1c0
			 0x05e77efb, /// 0x1c4
			 0x10cbb276, /// 0x1c8
			 0x404c058a, /// 0x1cc
			 0xda72420c, /// 0x1d0
			 0x3688f4a2, /// 0x1d4
			 0xba42dec9, /// 0x1d8
			 0x6a97100d, /// 0x1dc
			 0xdebcff4a, /// 0x1e0
			 0x1d2dd081, /// 0x1e4
			 0xb7e9882f, /// 0x1e8
			 0x43fce695, /// 0x1ec
			 0xcefdc8d5, /// 0x1f0
			 0xb9918398, /// 0x1f4
			 0x81c16ea2, /// 0x1f8
			 0x3b52ad9a, /// 0x1fc
			 0xe0eab0aa, /// 0x200
			 0x6b1eb713, /// 0x204
			 0x3453fe6f, /// 0x208
			 0x5f50286d, /// 0x20c
			 0xa1004384, /// 0x210
			 0x99eede38, /// 0x214
			 0x4fb5e8f7, /// 0x218
			 0x92f37c28, /// 0x21c
			 0xbba4af05, /// 0x220
			 0xbf39c78e, /// 0x224
			 0x465e4b5b, /// 0x228
			 0x80bad06a, /// 0x22c
			 0xba62368c, /// 0x230
			 0x95805f26, /// 0x234
			 0x8ce1dca8, /// 0x238
			 0xf96ba4fa, /// 0x23c
			 0x83235c93, /// 0x240
			 0xd3573c8c, /// 0x244
			 0x3b3b724b, /// 0x248
			 0xad1bffc4, /// 0x24c
			 0x07bba961, /// 0x250
			 0xe63adcd0, /// 0x254
			 0x80d44ddb, /// 0x258
			 0x7ed36fff, /// 0x25c
			 0x8dfa02a3, /// 0x260
			 0x4c1d1772, /// 0x264
			 0xde074722, /// 0x268
			 0x0fd89cc8, /// 0x26c
			 0xa152c924, /// 0x270
			 0xd284ae29, /// 0x274
			 0x940607ef, /// 0x278
			 0x0622c6dd, /// 0x27c
			 0xd7a49efc, /// 0x280
			 0xe859ef6f, /// 0x284
			 0xb0191929, /// 0x288
			 0x21e80dd2, /// 0x28c
			 0x4e90c8f6, /// 0x290
			 0x2ddda444, /// 0x294
			 0x8f8c3a11, /// 0x298
			 0xbc77e6a2, /// 0x29c
			 0xd2f67cc9, /// 0x2a0
			 0xb941bedc, /// 0x2a4
			 0xa8f15d4f, /// 0x2a8
			 0x5d11e780, /// 0x2ac
			 0xc9bd8df4, /// 0x2b0
			 0x4dd7a956, /// 0x2b4
			 0xa244627a, /// 0x2b8
			 0x5f4692c3, /// 0x2bc
			 0xa2eb72ee, /// 0x2c0
			 0x586e0eb9, /// 0x2c4
			 0xb83435d9, /// 0x2c8
			 0xcbfc7601, /// 0x2cc
			 0xc06a3401, /// 0x2d0
			 0x32f7175f, /// 0x2d4
			 0x8399529b, /// 0x2d8
			 0x3ebb3d92, /// 0x2dc
			 0x99eef100, /// 0x2e0
			 0x1e76c1bb, /// 0x2e4
			 0x8582383d, /// 0x2e8
			 0x1e99f7eb, /// 0x2ec
			 0x7ce14c08, /// 0x2f0
			 0x620ee925, /// 0x2f4
			 0x6ee609bd, /// 0x2f8
			 0x9460a4e2, /// 0x2fc
			 0xd07df5c8, /// 0x300
			 0x4e95e345, /// 0x304
			 0x4a7097ae, /// 0x308
			 0x1624ab6e, /// 0x30c
			 0x505ff140, /// 0x310
			 0xdb3aebbe, /// 0x314
			 0x7c8bf450, /// 0x318
			 0xaf69bf23, /// 0x31c
			 0xcbeb91d1, /// 0x320
			 0x1762863c, /// 0x324
			 0x95f59203, /// 0x328
			 0xf9b20f03, /// 0x32c
			 0xd3c618d8, /// 0x330
			 0x79da529f, /// 0x334
			 0xba9ab7fe, /// 0x338
			 0xc1d21a01, /// 0x33c
			 0xc1b6ede0, /// 0x340
			 0x4256f5ae, /// 0x344
			 0x9fbf5a6e, /// 0x348
			 0x9787927e, /// 0x34c
			 0x89c107eb, /// 0x350
			 0xe1f4d981, /// 0x354
			 0x4962a7c2, /// 0x358
			 0x2db31ea8, /// 0x35c
			 0xf67f8185, /// 0x360
			 0x580b0046, /// 0x364
			 0x4cabe490, /// 0x368
			 0xa8cba62a, /// 0x36c
			 0xf2aed7c9, /// 0x370
			 0xe2d29594, /// 0x374
			 0xbd39a372, /// 0x378
			 0x91467f74, /// 0x37c
			 0xf8ac8f16, /// 0x380
			 0x187a2725, /// 0x384
			 0x2cdec866, /// 0x388
			 0xf4487391, /// 0x38c
			 0x58662311, /// 0x390
			 0x8a5ac56c, /// 0x394
			 0xca1b4843, /// 0x398
			 0x124abacd, /// 0x39c
			 0x66a181d4, /// 0x3a0
			 0x783e6ac4, /// 0x3a4
			 0xc0a076e4, /// 0x3a8
			 0xc52a74b9, /// 0x3ac
			 0x68545e35, /// 0x3b0
			 0x327fbdb5, /// 0x3b4
			 0xc3b60010, /// 0x3b8
			 0xd2cb5cbf, /// 0x3bc
			 0x19e49a7d, /// 0x3c0
			 0xf2933214, /// 0x3c4
			 0x061fb198, /// 0x3c8
			 0x9da686e8, /// 0x3cc
			 0x9df7d2cd, /// 0x3d0
			 0xcca34857, /// 0x3d4
			 0x26e48e91, /// 0x3d8
			 0xb0dd13be, /// 0x3dc
			 0x50d68f3c, /// 0x3e0
			 0xed64ea3b, /// 0x3e4
			 0xdfe089fc, /// 0x3e8
			 0x58c4ae56, /// 0x3ec
			 0xa5c72143, /// 0x3f0
			 0x256b6654, /// 0x3f4
			 0x85890cd7, /// 0x3f8
			 0xf68171a4, /// 0x3fc
			 0x851b6ce2, /// 0x400
			 0x12c03473, /// 0x404
			 0x2fa3191f, /// 0x408
			 0x20cfbaa6, /// 0x40c
			 0x31b82f68, /// 0x410
			 0x15231b72, /// 0x414
			 0xad7f44e0, /// 0x418
			 0xdca3f571, /// 0x41c
			 0xed3594c6, /// 0x420
			 0xb8b3aa13, /// 0x424
			 0xa2753e0b, /// 0x428
			 0x8235210c, /// 0x42c
			 0x7e5c4677, /// 0x430
			 0x3b2d9eb5, /// 0x434
			 0xf8411dfe, /// 0x438
			 0xb569db65, /// 0x43c
			 0x8ee18ddf, /// 0x440
			 0xa9e7c68f, /// 0x444
			 0x91b5fbe9, /// 0x448
			 0xf5b75d8e, /// 0x44c
			 0x62b48898, /// 0x450
			 0x853374c3, /// 0x454
			 0xe190d552, /// 0x458
			 0x638199ea, /// 0x45c
			 0xb15b217d, /// 0x460
			 0xfb2e0032, /// 0x464
			 0xca8cf561, /// 0x468
			 0xd026c758, /// 0x46c
			 0x843c8217, /// 0x470
			 0xedaac999, /// 0x474
			 0x39e478bb, /// 0x478
			 0x5a62cad2, /// 0x47c
			 0x3af6c3a1, /// 0x480
			 0x1d99f24f, /// 0x484
			 0xfb5ae0e9, /// 0x488
			 0xf0244391, /// 0x48c
			 0x3d01602c, /// 0x490
			 0xb899c121, /// 0x494
			 0xb26998b1, /// 0x498
			 0xd1ee2505, /// 0x49c
			 0x930e0956, /// 0x4a0
			 0xd698ce2f, /// 0x4a4
			 0xc22cc83f, /// 0x4a8
			 0xb9a11ac3, /// 0x4ac
			 0x8452b3dc, /// 0x4b0
			 0xdc950fc6, /// 0x4b4
			 0x2f7925b0, /// 0x4b8
			 0x86cbc61c, /// 0x4bc
			 0x01242c5e, /// 0x4c0
			 0x957689ee, /// 0x4c4
			 0x224f4696, /// 0x4c8
			 0xed3069d2, /// 0x4cc
			 0xd18f7708, /// 0x4d0
			 0xe4131d4c, /// 0x4d4
			 0x121c5c8c, /// 0x4d8
			 0x3319b3c7, /// 0x4dc
			 0x69a47c18, /// 0x4e0
			 0xb27f164f, /// 0x4e4
			 0x607205b5, /// 0x4e8
			 0xf1ce9fb5, /// 0x4ec
			 0x60badbe0, /// 0x4f0
			 0x003c0519, /// 0x4f4
			 0x0ec87ed8, /// 0x4f8
			 0x211e71f4, /// 0x4fc
			 0x6e8c2f07, /// 0x500
			 0xfcf91985, /// 0x504
			 0xcadbd292, /// 0x508
			 0x644dfd44, /// 0x50c
			 0x89b72640, /// 0x510
			 0xfa20824d, /// 0x514
			 0x015737ea, /// 0x518
			 0x07ddb2a8, /// 0x51c
			 0xd3ddb123, /// 0x520
			 0xcafb42d2, /// 0x524
			 0xcc99fe85, /// 0x528
			 0x6514296d, /// 0x52c
			 0x55caffac, /// 0x530
			 0xce3c9119, /// 0x534
			 0xec791057, /// 0x538
			 0xd28f5d5a, /// 0x53c
			 0xa6175996, /// 0x540
			 0x11a512eb, /// 0x544
			 0x4270e7ec, /// 0x548
			 0x8f4df37e, /// 0x54c
			 0xa06d269f, /// 0x550
			 0xeceb8e18, /// 0x554
			 0xf5effb40, /// 0x558
			 0x4fcefea4, /// 0x55c
			 0x2d879786, /// 0x560
			 0x8c0bcae7, /// 0x564
			 0x39f1d657, /// 0x568
			 0x8d3fd8ea, /// 0x56c
			 0xbd5a2b1f, /// 0x570
			 0xaa8f2600, /// 0x574
			 0x264df456, /// 0x578
			 0xf2548625, /// 0x57c
			 0x3d17c7fe, /// 0x580
			 0x1d3a2f81, /// 0x584
			 0xfe43c283, /// 0x588
			 0xdcc05527, /// 0x58c
			 0x253be387, /// 0x590
			 0xede2783e, /// 0x594
			 0x070b0c1b, /// 0x598
			 0xd2977665, /// 0x59c
			 0x012b7ac0, /// 0x5a0
			 0x28d5f586, /// 0x5a4
			 0x2b9d3a4f, /// 0x5a8
			 0x6e08db14, /// 0x5ac
			 0xb1b79288, /// 0x5b0
			 0xcded2219, /// 0x5b4
			 0x75a5bbf9, /// 0x5b8
			 0x0f4bc003, /// 0x5bc
			 0xe19bf75f, /// 0x5c0
			 0xfb70b48c, /// 0x5c4
			 0xb73261a3, /// 0x5c8
			 0x75e812a4, /// 0x5cc
			 0xe8cef185, /// 0x5d0
			 0x1c0e0842, /// 0x5d4
			 0x3bf3af02, /// 0x5d8
			 0xd5278c35, /// 0x5dc
			 0x96e904e0, /// 0x5e0
			 0x80c4b91f, /// 0x5e4
			 0xb25b4f6d, /// 0x5e8
			 0x8ce08dec, /// 0x5ec
			 0x8a7574f7, /// 0x5f0
			 0xa368fe7b, /// 0x5f4
			 0xad20d082, /// 0x5f8
			 0xc2462140, /// 0x5fc
			 0xe0cd00ba, /// 0x600
			 0x302c470d, /// 0x604
			 0xc44f3f79, /// 0x608
			 0xefe521b8, /// 0x60c
			 0x04ce8098, /// 0x610
			 0x8f4e97fc, /// 0x614
			 0x8ffd90f4, /// 0x618
			 0x7fa54769, /// 0x61c
			 0xe07a48f8, /// 0x620
			 0x010fa816, /// 0x624
			 0xc4586f9d, /// 0x628
			 0xf366eb11, /// 0x62c
			 0x605493f3, /// 0x630
			 0x96282e9c, /// 0x634
			 0xd7302a40, /// 0x638
			 0x5eac3108, /// 0x63c
			 0xda9a9939, /// 0x640
			 0x691610f4, /// 0x644
			 0x3780c63c, /// 0x648
			 0x8511f12a, /// 0x64c
			 0xc4946633, /// 0x650
			 0x40d0a381, /// 0x654
			 0x80a8b1fa, /// 0x658
			 0x74a8dd90, /// 0x65c
			 0x4f9afb8a, /// 0x660
			 0xd914ef10, /// 0x664
			 0x469e7242, /// 0x668
			 0xc9f40821, /// 0x66c
			 0xf8e7726a, /// 0x670
			 0x2e576b78, /// 0x674
			 0xe5bfb0b7, /// 0x678
			 0x7dc778f4, /// 0x67c
			 0xecea139e, /// 0x680
			 0x34d45091, /// 0x684
			 0xab0d0068, /// 0x688
			 0x43da1d87, /// 0x68c
			 0x019393ca, /// 0x690
			 0x0594fee1, /// 0x694
			 0xb8fc9cfa, /// 0x698
			 0x4c5fe004, /// 0x69c
			 0xb809809a, /// 0x6a0
			 0xb3794e88, /// 0x6a4
			 0x092eb2d1, /// 0x6a8
			 0x198682e5, /// 0x6ac
			 0xe83f86a2, /// 0x6b0
			 0xefebe20b, /// 0x6b4
			 0x28e03d36, /// 0x6b8
			 0x07e4459f, /// 0x6bc
			 0xffa67295, /// 0x6c0
			 0xcf4ccaba, /// 0x6c4
			 0x63483976, /// 0x6c8
			 0x79eed741, /// 0x6cc
			 0xbdd1a1ad, /// 0x6d0
			 0x17a97a56, /// 0x6d4
			 0x98703ccd, /// 0x6d8
			 0x6be9c357, /// 0x6dc
			 0xd81db735, /// 0x6e0
			 0xaee5f397, /// 0x6e4
			 0xf61f15c1, /// 0x6e8
			 0xaab17833, /// 0x6ec
			 0x18fba8d8, /// 0x6f0
			 0x8c2b9f90, /// 0x6f4
			 0x988b4979, /// 0x6f8
			 0xbdc64080, /// 0x6fc
			 0x30321e71, /// 0x700
			 0x21e6bc6a, /// 0x704
			 0x321f7827, /// 0x708
			 0x3ae42f4a, /// 0x70c
			 0x9daab5eb, /// 0x710
			 0xebaffcca, /// 0x714
			 0xdb57458c, /// 0x718
			 0x0a5fcbb8, /// 0x71c
			 0xaafed488, /// 0x720
			 0x27f3b17d, /// 0x724
			 0x2703651a, /// 0x728
			 0x8b25a085, /// 0x72c
			 0x8cf06107, /// 0x730
			 0x2d553860, /// 0x734
			 0xc6084d7f, /// 0x738
			 0x05ca3499, /// 0x73c
			 0x3573f544, /// 0x740
			 0xde726ca7, /// 0x744
			 0xd207de62, /// 0x748
			 0xb67f9bff, /// 0x74c
			 0x3a509852, /// 0x750
			 0x30322734, /// 0x754
			 0x4388ee3c, /// 0x758
			 0x919f9f53, /// 0x75c
			 0xff255b66, /// 0x760
			 0x5c783532, /// 0x764
			 0x00b54298, /// 0x768
			 0xdc0e2b53, /// 0x76c
			 0x152dadb9, /// 0x770
			 0x02203d3f, /// 0x774
			 0x271b90e8, /// 0x778
			 0x2f917ffc, /// 0x77c
			 0xe01a4a9e, /// 0x780
			 0xc2969832, /// 0x784
			 0xed0dac04, /// 0x788
			 0xae9c5f0c, /// 0x78c
			 0x27499200, /// 0x790
			 0x49297ff9, /// 0x794
			 0x4d0ad72e, /// 0x798
			 0x8f575f1f, /// 0x79c
			 0x58719d3e, /// 0x7a0
			 0x0d20a261, /// 0x7a4
			 0xe8846c1f, /// 0x7a8
			 0x3aa1c4be, /// 0x7ac
			 0x62554f39, /// 0x7b0
			 0xa94bb6a2, /// 0x7b4
			 0x917bc766, /// 0x7b8
			 0x66405cd4, /// 0x7bc
			 0x61bec3ac, /// 0x7c0
			 0x4a44150a, /// 0x7c4
			 0xa5a61b26, /// 0x7c8
			 0xf3a7c27d, /// 0x7cc
			 0x5e877aff, /// 0x7d0
			 0x851b8987, /// 0x7d4
			 0x113b1890, /// 0x7d8
			 0xae1f6686, /// 0x7dc
			 0x94584e78, /// 0x7e0
			 0xd767ee9b, /// 0x7e4
			 0x828ffbec, /// 0x7e8
			 0xcf499ac6, /// 0x7ec
			 0x5888fd03, /// 0x7f0
			 0x83715323, /// 0x7f4
			 0x2599222c, /// 0x7f8
			 0xffffa3a3, /// 0x7fc
			 0x2c10a6d2, /// 0x800
			 0xb7934140, /// 0x804
			 0x4075a21f, /// 0x808
			 0xbcb4b382, /// 0x80c
			 0x4babd253, /// 0x810
			 0x04277427, /// 0x814
			 0x86427974, /// 0x818
			 0x95e7c42f, /// 0x81c
			 0xd6f8384e, /// 0x820
			 0x6244ac2f, /// 0x824
			 0xc363371c, /// 0x828
			 0x48114c29, /// 0x82c
			 0x49378c08, /// 0x830
			 0xed03d410, /// 0x834
			 0x374ce41c, /// 0x838
			 0x258ce88f, /// 0x83c
			 0x3df25ea5, /// 0x840
			 0x56dad105, /// 0x844
			 0x70dbc148, /// 0x848
			 0x36bdf00f, /// 0x84c
			 0x833a129e, /// 0x850
			 0xc7ca574b, /// 0x854
			 0xf42442e4, /// 0x858
			 0x65121c7b, /// 0x85c
			 0xf078a600, /// 0x860
			 0x40d27a19, /// 0x864
			 0x5fe70902, /// 0x868
			 0xdb234fa5, /// 0x86c
			 0x19f50322, /// 0x870
			 0xc1da2d68, /// 0x874
			 0xf0e3e5ce, /// 0x878
			 0x6c67da34, /// 0x87c
			 0x0898a22a, /// 0x880
			 0xaf14b272, /// 0x884
			 0xcf5fc304, /// 0x888
			 0xcade8f7f, /// 0x88c
			 0x44e662d6, /// 0x890
			 0x5f3766f6, /// 0x894
			 0xafe0937a, /// 0x898
			 0x828ee0ac, /// 0x89c
			 0x40075d4c, /// 0x8a0
			 0x300fe9ee, /// 0x8a4
			 0xa2a1a9c0, /// 0x8a8
			 0xf2d77ec0, /// 0x8ac
			 0x753be403, /// 0x8b0
			 0x0ea1c02e, /// 0x8b4
			 0x6c2798d0, /// 0x8b8
			 0xa250174d, /// 0x8bc
			 0x08dd9d07, /// 0x8c0
			 0x442e92cf, /// 0x8c4
			 0xddda8a06, /// 0x8c8
			 0x70b0408d, /// 0x8cc
			 0x5224c06d, /// 0x8d0
			 0x5ef44347, /// 0x8d4
			 0x65c6bb85, /// 0x8d8
			 0xcbee6dd0, /// 0x8dc
			 0x35691022, /// 0x8e0
			 0xba1cc497, /// 0x8e4
			 0xed9f0bb2, /// 0x8e8
			 0x0ea384e7, /// 0x8ec
			 0x46c6fe4a, /// 0x8f0
			 0xdac7cc8c, /// 0x8f4
			 0xa023aef0, /// 0x8f8
			 0xdf37eb0c, /// 0x8fc
			 0x22c6c886, /// 0x900
			 0x6a917841, /// 0x904
			 0x461e3abd, /// 0x908
			 0xb8058051, /// 0x90c
			 0xeafe2412, /// 0x910
			 0x6e820d6e, /// 0x914
			 0xdd4ea9f4, /// 0x918
			 0x3f1c4fce, /// 0x91c
			 0x076addf3, /// 0x920
			 0x31990007, /// 0x924
			 0x90bd8d96, /// 0x928
			 0xe46201b1, /// 0x92c
			 0x200afef4, /// 0x930
			 0xdd712607, /// 0x934
			 0x43a3f3ad, /// 0x938
			 0x31945720, /// 0x93c
			 0x97963779, /// 0x940
			 0x023de7b0, /// 0x944
			 0x43f9b09c, /// 0x948
			 0x3c9657e3, /// 0x94c
			 0x83184344, /// 0x950
			 0xbe713fa3, /// 0x954
			 0x3e8e2c47, /// 0x958
			 0x0272e465, /// 0x95c
			 0xebeb8258, /// 0x960
			 0xce82373c, /// 0x964
			 0x4c42384a, /// 0x968
			 0x07879bc0, /// 0x96c
			 0x8029e6f9, /// 0x970
			 0x23e229b9, /// 0x974
			 0xd3a23d8b, /// 0x978
			 0xae3b9f1d, /// 0x97c
			 0xb249bf6c, /// 0x980
			 0x0b101a58, /// 0x984
			 0x65451c78, /// 0x988
			 0xe65dd727, /// 0x98c
			 0x722017b9, /// 0x990
			 0xae99113c, /// 0x994
			 0x6a4ff7a0, /// 0x998
			 0x880d87a7, /// 0x99c
			 0x0e8b2434, /// 0x9a0
			 0x36d79ecd, /// 0x9a4
			 0x5a09c9b5, /// 0x9a8
			 0xb6103b50, /// 0x9ac
			 0x21e95aeb, /// 0x9b0
			 0x465ba777, /// 0x9b4
			 0x0712b39d, /// 0x9b8
			 0x0649b579, /// 0x9bc
			 0x51c78985, /// 0x9c0
			 0x5ae3f9e3, /// 0x9c4
			 0x5b3e6344, /// 0x9c8
			 0x1ec78485, /// 0x9cc
			 0xf8ac5ee1, /// 0x9d0
			 0x98dfe05e, /// 0x9d4
			 0x10867e3e, /// 0x9d8
			 0xd533b62e, /// 0x9dc
			 0xafa053b3, /// 0x9e0
			 0x42842013, /// 0x9e4
			 0x619fcf41, /// 0x9e8
			 0x2b9eeb38, /// 0x9ec
			 0xe6c4a9fe, /// 0x9f0
			 0xbebe176d, /// 0x9f4
			 0xb58f4736, /// 0x9f8
			 0x2232473f, /// 0x9fc
			 0x8df897b7, /// 0xa00
			 0x2cde0122, /// 0xa04
			 0xa4cf9877, /// 0xa08
			 0xadcf592e, /// 0xa0c
			 0x9a2affa2, /// 0xa10
			 0x63e8935e, /// 0xa14
			 0xd10fa8eb, /// 0xa18
			 0xc800e678, /// 0xa1c
			 0x3e2e22ba, /// 0xa20
			 0xca1e0cdb, /// 0xa24
			 0x3b1f614f, /// 0xa28
			 0x042d7a82, /// 0xa2c
			 0x2f7345a8, /// 0xa30
			 0xc06c26aa, /// 0xa34
			 0x0e0a7230, /// 0xa38
			 0x093730fd, /// 0xa3c
			 0x3c778b5b, /// 0xa40
			 0xa7ba1309, /// 0xa44
			 0xf0877dc3, /// 0xa48
			 0x25861e00, /// 0xa4c
			 0xa2c8f3f8, /// 0xa50
			 0x38a7a231, /// 0xa54
			 0xc37156e7, /// 0xa58
			 0x058a923e, /// 0xa5c
			 0x70664791, /// 0xa60
			 0xd99d1d10, /// 0xa64
			 0x221a2ef9, /// 0xa68
			 0xdcdc0a8e, /// 0xa6c
			 0x386f0616, /// 0xa70
			 0xbe500fe5, /// 0xa74
			 0x0b37c447, /// 0xa78
			 0x60ae21c0, /// 0xa7c
			 0xd669de58, /// 0xa80
			 0x5b11cfec, /// 0xa84
			 0x3c8ea7ba, /// 0xa88
			 0x6c1636e0, /// 0xa8c
			 0x21c5cd25, /// 0xa90
			 0x1f2eeeba, /// 0xa94
			 0x0027dce5, /// 0xa98
			 0x769480e2, /// 0xa9c
			 0x56f9793a, /// 0xaa0
			 0x6ebc65fb, /// 0xaa4
			 0x2432e009, /// 0xaa8
			 0xf14c7208, /// 0xaac
			 0x9db64786, /// 0xab0
			 0x75c35ee9, /// 0xab4
			 0x92770e1b, /// 0xab8
			 0x4ba6d6da, /// 0xabc
			 0x51672e2e, /// 0xac0
			 0x53a8d32f, /// 0xac4
			 0x854f52fb, /// 0xac8
			 0xb20fb4da, /// 0xacc
			 0x8568cdde, /// 0xad0
			 0xd86858dc, /// 0xad4
			 0xac57c649, /// 0xad8
			 0x0189a704, /// 0xadc
			 0xcd67f375, /// 0xae0
			 0xbc03c562, /// 0xae4
			 0xb657f97f, /// 0xae8
			 0xbaaef121, /// 0xaec
			 0x4e48b28a, /// 0xaf0
			 0xb816585b, /// 0xaf4
			 0x68db5e43, /// 0xaf8
			 0x325ddf2c, /// 0xafc
			 0x704144ea, /// 0xb00
			 0x382c843f, /// 0xb04
			 0xb87db943, /// 0xb08
			 0xc56b2cf5, /// 0xb0c
			 0x6e147f0f, /// 0xb10
			 0xcff85f3a, /// 0xb14
			 0xc4dd3562, /// 0xb18
			 0xd8abc603, /// 0xb1c
			 0x33845f86, /// 0xb20
			 0x1b0c5aa5, /// 0xb24
			 0xe172ec01, /// 0xb28
			 0xe3bcefe2, /// 0xb2c
			 0x02e2bfcf, /// 0xb30
			 0x25b17d31, /// 0xb34
			 0x46ccf648, /// 0xb38
			 0x2b5d04ed, /// 0xb3c
			 0xda40893b, /// 0xb40
			 0x6c64c066, /// 0xb44
			 0x5ee11fe2, /// 0xb48
			 0x83a7ad60, /// 0xb4c
			 0x40febb1d, /// 0xb50
			 0x1537dd00, /// 0xb54
			 0xb2d73821, /// 0xb58
			 0xf67c5cbe, /// 0xb5c
			 0xbecdb748, /// 0xb60
			 0xe652f8ba, /// 0xb64
			 0xfaffa25b, /// 0xb68
			 0xc7f3dc6f, /// 0xb6c
			 0xaf25e834, /// 0xb70
			 0xdc595e92, /// 0xb74
			 0x18e6628b, /// 0xb78
			 0x87ff9e11, /// 0xb7c
			 0x6c97a9cb, /// 0xb80
			 0x4b2fe665, /// 0xb84
			 0x1df45969, /// 0xb88
			 0x62824c72, /// 0xb8c
			 0x24ca76b1, /// 0xb90
			 0x13c27b44, /// 0xb94
			 0xf9c95eab, /// 0xb98
			 0xd0234123, /// 0xb9c
			 0x637762af, /// 0xba0
			 0x4df92314, /// 0xba4
			 0xf5dd0355, /// 0xba8
			 0x43c3c125, /// 0xbac
			 0xfd75388a, /// 0xbb0
			 0x6714fcdd, /// 0xbb4
			 0x0e1008ed, /// 0xbb8
			 0x74e054d6, /// 0xbbc
			 0x6b0fa389, /// 0xbc0
			 0xc10d93be, /// 0xbc4
			 0xbc2e7d41, /// 0xbc8
			 0x2f48082f, /// 0xbcc
			 0xad6cb81b, /// 0xbd0
			 0x647ac44f, /// 0xbd4
			 0xe29b983f, /// 0xbd8
			 0x79bb9907, /// 0xbdc
			 0x1c06b9d5, /// 0xbe0
			 0x60e86e0e, /// 0xbe4
			 0xf3b7ee52, /// 0xbe8
			 0x93d8562b, /// 0xbec
			 0x96b0939b, /// 0xbf0
			 0xdb97032c, /// 0xbf4
			 0x856d0217, /// 0xbf8
			 0xf7470b04, /// 0xbfc
			 0x78b7a586, /// 0xc00
			 0x93199ca0, /// 0xc04
			 0xa7247ae5, /// 0xc08
			 0xc87df60c, /// 0xc0c
			 0x3889533d, /// 0xc10
			 0x0553c11e, /// 0xc14
			 0x222bd660, /// 0xc18
			 0x0bedfb1c, /// 0xc1c
			 0xe9e075e0, /// 0xc20
			 0x0a1327d9, /// 0xc24
			 0x5fde7e90, /// 0xc28
			 0xa6188390, /// 0xc2c
			 0x493284fb, /// 0xc30
			 0x35af444a, /// 0xc34
			 0x4ee6a124, /// 0xc38
			 0xd70843ff, /// 0xc3c
			 0xe8dbb362, /// 0xc40
			 0xf09c57b6, /// 0xc44
			 0x09beae1f, /// 0xc48
			 0xedce719d, /// 0xc4c
			 0x88408bf2, /// 0xc50
			 0x377f751a, /// 0xc54
			 0x939a5cc2, /// 0xc58
			 0x335d7e59, /// 0xc5c
			 0xd093b575, /// 0xc60
			 0xcb4904ba, /// 0xc64
			 0x2b181dee, /// 0xc68
			 0x09812906, /// 0xc6c
			 0x91e0831d, /// 0xc70
			 0x3646e474, /// 0xc74
			 0xe757f28e, /// 0xc78
			 0x4e95599b, /// 0xc7c
			 0x70ec8091, /// 0xc80
			 0x7562e68e, /// 0xc84
			 0xa1c47762, /// 0xc88
			 0x6c1ff4bb, /// 0xc8c
			 0xac0d7742, /// 0xc90
			 0x69a32280, /// 0xc94
			 0xb2662506, /// 0xc98
			 0x4060a189, /// 0xc9c
			 0xfafe9e9d, /// 0xca0
			 0x77b19b22, /// 0xca4
			 0x6291dc7b, /// 0xca8
			 0x55132cec, /// 0xcac
			 0xe117ce0c, /// 0xcb0
			 0x688cf366, /// 0xcb4
			 0x92659ddb, /// 0xcb8
			 0x6bef23c7, /// 0xcbc
			 0xdde9cd4d, /// 0xcc0
			 0x0bd843d6, /// 0xcc4
			 0x81befa22, /// 0xcc8
			 0x316d7c94, /// 0xccc
			 0x595dd3e5, /// 0xcd0
			 0x4b8ef62b, /// 0xcd4
			 0x11205649, /// 0xcd8
			 0x1ebd97bf, /// 0xcdc
			 0x49c330db, /// 0xce0
			 0x8ecc98de, /// 0xce4
			 0x00f3288f, /// 0xce8
			 0x734c2a5c, /// 0xcec
			 0x3c13c4b2, /// 0xcf0
			 0x46aca0ff, /// 0xcf4
			 0xe5f969a9, /// 0xcf8
			 0x70a1243f, /// 0xcfc
			 0x3aebca57, /// 0xd00
			 0xe39eac2b, /// 0xd04
			 0x9dafee25, /// 0xd08
			 0x65350f87, /// 0xd0c
			 0x2f0ce1a6, /// 0xd10
			 0xd02edf8e, /// 0xd14
			 0xe368b8d4, /// 0xd18
			 0x0b23f4b1, /// 0xd1c
			 0x72bb086a, /// 0xd20
			 0x1fbbd41a, /// 0xd24
			 0x9669ff57, /// 0xd28
			 0x487ee513, /// 0xd2c
			 0xae93fd01, /// 0xd30
			 0x2f986d37, /// 0xd34
			 0x5e7f8fd1, /// 0xd38
			 0xead1e23d, /// 0xd3c
			 0x67d94523, /// 0xd40
			 0x2a8b535b, /// 0xd44
			 0x7ca726d0, /// 0xd48
			 0xd8458aa4, /// 0xd4c
			 0x4c4bfd38, /// 0xd50
			 0x501cbd3f, /// 0xd54
			 0xd15e93a8, /// 0xd58
			 0x8a565ed4, /// 0xd5c
			 0x231993de, /// 0xd60
			 0x70b800c5, /// 0xd64
			 0x46760526, /// 0xd68
			 0xf7857c33, /// 0xd6c
			 0x0a161b34, /// 0xd70
			 0x756875fd, /// 0xd74
			 0x0de29efa, /// 0xd78
			 0xc0ad10c4, /// 0xd7c
			 0x3fe7d5ed, /// 0xd80
			 0xae3be844, /// 0xd84
			 0x1e428d43, /// 0xd88
			 0xa531f995, /// 0xd8c
			 0xebeeea01, /// 0xd90
			 0xaa8b7e73, /// 0xd94
			 0x5d352772, /// 0xd98
			 0x745be834, /// 0xd9c
			 0xa8a9f166, /// 0xda0
			 0x89073f3d, /// 0xda4
			 0x30ab633a, /// 0xda8
			 0x4a8ea179, /// 0xdac
			 0x31a04799, /// 0xdb0
			 0xb25972f5, /// 0xdb4
			 0x15d6d6f7, /// 0xdb8
			 0xc0a103ce, /// 0xdbc
			 0xc7e92e79, /// 0xdc0
			 0xd5459b7d, /// 0xdc4
			 0x3c06fd33, /// 0xdc8
			 0xd5d58048, /// 0xdcc
			 0xf36891f1, /// 0xdd0
			 0x62b9abc9, /// 0xdd4
			 0xfae5923d, /// 0xdd8
			 0xafd4d02f, /// 0xddc
			 0x844db2a5, /// 0xde0
			 0xaea2eb89, /// 0xde4
			 0x28fface1, /// 0xde8
			 0x7abc7564, /// 0xdec
			 0xb76c71d8, /// 0xdf0
			 0x6438a984, /// 0xdf4
			 0x0453834d, /// 0xdf8
			 0xfadc4aa0, /// 0xdfc
			 0xf6eff1a6, /// 0xe00
			 0x839dd321, /// 0xe04
			 0x3d5f3b44, /// 0xe08
			 0x5844882f, /// 0xe0c
			 0x6753120c, /// 0xe10
			 0xd5f2888b, /// 0xe14
			 0x0bfa4e17, /// 0xe18
			 0x9f0c5bb1, /// 0xe1c
			 0xe00a56ba, /// 0xe20
			 0x0b76e2bc, /// 0xe24
			 0xa7cf16db, /// 0xe28
			 0xab1fff38, /// 0xe2c
			 0x13349b03, /// 0xe30
			 0xcf1b30e7, /// 0xe34
			 0xb37ec0af, /// 0xe38
			 0xca40f0dd, /// 0xe3c
			 0x6fed753b, /// 0xe40
			 0x83a218fb, /// 0xe44
			 0xa8294125, /// 0xe48
			 0xf14370ff, /// 0xe4c
			 0x6d333216, /// 0xe50
			 0x4f0cd2d0, /// 0xe54
			 0x9295fff0, /// 0xe58
			 0x846d379e, /// 0xe5c
			 0xed7e6bcc, /// 0xe60
			 0x65cf9a6b, /// 0xe64
			 0xe3020578, /// 0xe68
			 0xc2bcb9bc, /// 0xe6c
			 0xe32c3fa3, /// 0xe70
			 0xc7ae0c5e, /// 0xe74
			 0x893eec46, /// 0xe78
			 0xec38d4d6, /// 0xe7c
			 0x12936002, /// 0xe80
			 0x3c103c68, /// 0xe84
			 0xd4ae98b0, /// 0xe88
			 0x984cdb43, /// 0xe8c
			 0xc419e57b, /// 0xe90
			 0xe9e47ca5, /// 0xe94
			 0x0c7c7d18, /// 0xe98
			 0x71d73543, /// 0xe9c
			 0x77e87f7a, /// 0xea0
			 0x842980ef, /// 0xea4
			 0xa9fb2da9, /// 0xea8
			 0x64efb895, /// 0xeac
			 0x03916467, /// 0xeb0
			 0x8ba76411, /// 0xeb4
			 0x26fcad1f, /// 0xeb8
			 0xac51efda, /// 0xebc
			 0x3dc7d2e3, /// 0xec0
			 0x95e4ec7c, /// 0xec4
			 0xf2cd6789, /// 0xec8
			 0x2fab850f, /// 0xecc
			 0xd779e7c9, /// 0xed0
			 0xfe47ad14, /// 0xed4
			 0x5206fdc6, /// 0xed8
			 0x47ceed83, /// 0xedc
			 0x8c77d929, /// 0xee0
			 0xc183debc, /// 0xee4
			 0xf65f2d59, /// 0xee8
			 0xc5147d4d, /// 0xeec
			 0x88dc45da, /// 0xef0
			 0x886c816a, /// 0xef4
			 0x7607f706, /// 0xef8
			 0x952f9c92, /// 0xefc
			 0xc5d5066c, /// 0xf00
			 0x33b8e9fc, /// 0xf04
			 0x9e3e208a, /// 0xf08
			 0xaff232c7, /// 0xf0c
			 0x02231bfc, /// 0xf10
			 0x164f9bef, /// 0xf14
			 0x5b177d9e, /// 0xf18
			 0xe22b1866, /// 0xf1c
			 0x88daee95, /// 0xf20
			 0xccc202b8, /// 0xf24
			 0x479614f3, /// 0xf28
			 0xf6194bf8, /// 0xf2c
			 0x1a39d416, /// 0xf30
			 0x172357ac, /// 0xf34
			 0xf9809435, /// 0xf38
			 0xf2719c3e, /// 0xf3c
			 0x9b8a83a1, /// 0xf40
			 0xaf6125ea, /// 0xf44
			 0xcdf10b01, /// 0xf48
			 0x37a6b4b5, /// 0xf4c
			 0xd5a27d1e, /// 0xf50
			 0x70e96043, /// 0xf54
			 0x60dfd97d, /// 0xf58
			 0xfa4dcc1c, /// 0xf5c
			 0x1d4abea0, /// 0xf60
			 0x475f5e84, /// 0xf64
			 0x20b15e00, /// 0xf68
			 0xa6b6e99c, /// 0xf6c
			 0x5bf42251, /// 0xf70
			 0x089ee9bd, /// 0xf74
			 0xa129bfb9, /// 0xf78
			 0xa22aded0, /// 0xf7c
			 0x3f02b9b5, /// 0xf80
			 0x5239eab1, /// 0xf84
			 0x17e71b25, /// 0xf88
			 0x0afd96d4, /// 0xf8c
			 0x08e79e8e, /// 0xf90
			 0x16e850be, /// 0xf94
			 0xf64c844d, /// 0xf98
			 0xa6d356a1, /// 0xf9c
			 0x888db638, /// 0xfa0
			 0xf7e6d891, /// 0xfa4
			 0xbc809287, /// 0xfa8
			 0x711825f4, /// 0xfac
			 0x5c2dc610, /// 0xfb0
			 0xa147a123, /// 0xfb4
			 0x0be76f03, /// 0xfb8
			 0x45d7ec74, /// 0xfbc
			 0xb7be41eb, /// 0xfc0
			 0x918c357d, /// 0xfc4
			 0x814505c6, /// 0xfc8
			 0x4115cb59, /// 0xfcc
			 0x2f7b1c01, /// 0xfd0
			 0x54c949d7, /// 0xfd4
			 0x3eba39ed, /// 0xfd8
			 0xb4107584, /// 0xfdc
			 0xd155ba62, /// 0xfe0
			 0x7154ba7f, /// 0xfe4
			 0x50938419, /// 0xfe8
			 0xdc4cf37f, /// 0xfec
			 0xc0842a17, /// 0xff0
			 0xf8ffebc9, /// 0xff4
			 0x7a5e313e, /// 0xff8
			 0x30f28f43 /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00004
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0000c
			 0x80000000,                                                  // -zero                                       /// 00010
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00018
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0001c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00020
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00024
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00028
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0002c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00030
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00034
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00038
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0003c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00044
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0x7f800000,                                                  // inf                                         /// 0004c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00050
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00054
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00058
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0005c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00060
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00064
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00068
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0006c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00070
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00074
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00078
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00084
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00088
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0008c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00090
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00098
			 0x33333333,                                                  // 4 random values                             /// 0009c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000a4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000a8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 000ac
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000b4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 000c4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000c8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 000cc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000d0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 000d4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 000d8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 000dc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 000e4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 000e8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 000ec
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000f4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 000f8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000fc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00100
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00104
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0010c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00110
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00114
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00118
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0011c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00120
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00124
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0012c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00130
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00134
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00138
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0013c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00140
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00144
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00148
			 0x33333333,                                                  // 4 random values                             /// 0014c
			 0x7fc00001,                                                  // signaling NaN                               /// 00150
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00154
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00158
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0015c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00160
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00164
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00168
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0016c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00174
			 0x80000000,                                                  // -zero                                       /// 00178
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0017c
			 0xcb000000,                                                  // -8388608.0                                  /// 00180
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00184
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00188
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0018c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00190
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00194
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00198
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0019c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001a0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 001a4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 001a8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 001ac
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001b0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 001b8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001bc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 001c0
			 0x80011111,                                                  // -9.7958E-41                                 /// 001c4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001c8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 001cc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001d4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001d8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001dc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 001e4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001e8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001ec
			 0x0e000003,                                                  // Trailing 1s:                                /// 001f0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001fc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00200
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00204
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00208
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0020c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00210
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0021c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00220
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00224
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00228
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0022c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00230
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00234
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00238
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0023c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00240
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00244
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00248
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0024c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00254
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00258
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00260
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00264
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0026c
			 0xcb000000,                                                  // -8388608.0                                  /// 00270
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00274
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00278
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0027c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00280
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00284
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00288
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0028c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00294
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0029c
			 0x3f028f5c,                                                  // 0.51                                        /// 002a0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ac
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 002b0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002b8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002bc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002c0
			 0xff800000,                                                  // -inf                                        /// 002c4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002c8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002cc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002d0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 002d4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002dc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002e0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 002e4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002e8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 002ec
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f0
			 0x4b000000,                                                  // 8388608.0                                   /// 002f4
			 0x0c780000,                                                  // Leading 1s:                                 /// 002f8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 002fc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00300
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00304
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00308
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0030c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00314
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00318
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0031c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00320
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00324
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00328
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0032c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00330
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00334
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00338
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0033c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00340
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00344
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00348
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0034c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00350
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00354
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00358
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0035c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00360
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00364
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00368
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0036c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00370
			 0x00011111,                                                  // 9.7958E-41                                  /// 00374
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00378
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0037c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00380
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00384
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0038c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00390
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00394
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00398
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0039c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003a0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0xcb000000,                                                  // -8388608.0                                  /// 003a8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003ac
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003b0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003b4
			 0x7f800000,                                                  // inf                                         /// 003b8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003bc
			 0x0c700000,                                                  // Leading 1s:                                 /// 003c0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003c4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003c8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 003d8
			 0x7fc00001,                                                  // signaling NaN                               /// 003dc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 003e8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003ec
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003f0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003f4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003fc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00400
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00404
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00408
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0040c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00410
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00414
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00418
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0041c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00420
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00424
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00428
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00430
			 0x4b000000,                                                  // 8388608.0                                   /// 00434
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00438
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0043c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00444
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00448
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0044c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x33333333,                                                  // 4 random values                             /// 00454
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00458
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0045c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0x33333333,                                                  // 4 random values                             /// 00468
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0046c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00470
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00474
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00478
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0047c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00480
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00484
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00488
			 0x55555555,                                                  // 4 random values                             /// 0048c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00490
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00498
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0049c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 004a0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004a4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 004b0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004b4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004b8
			 0xff800000,                                                  // -inf                                        /// 004bc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004c0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 004c8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 004cc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004d0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 004d4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004d8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 004dc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004e8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004ec
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 004f0
			 0x33333333,                                                  // 4 random values                             /// 004f4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004f8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004fc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00500
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00504
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00508
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0050c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00510
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00514
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00518
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0051c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00520
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00524
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0052c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00530
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00534
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00538
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0053c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00540
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00548
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0054c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00550
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00558
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0055c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00560
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00564
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00568
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0056c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00570
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00574
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00578
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0057c
			 0x00000000,                                                  // zero                                        /// 00580
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00584
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00588
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0058c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00590
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00594
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00598
			 0x4b000000,                                                  // 8388608.0                                   /// 0059c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005a0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005a4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005a8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 005b0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 005b4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005bc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 005c0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005c4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 005cc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 005d0
			 0xffc00001,                                                  // -signaling NaN                              /// 005d4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 005d8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005dc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005e0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005e4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005ec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005f0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005fc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00600
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00604
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00608
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0060c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00610
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00614
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00618
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0061c
			 0x3f028f5c,                                                  // 0.51                                        /// 00620
			 0x0e000001,                                                  // Trailing 1s:                                /// 00624
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0062c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00630
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00634
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00638
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0063c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00644
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00648
			 0x55555555,                                                  // 4 random values                             /// 0064c
			 0x80000000,                                                  // -zero                                       /// 00650
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00654
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00658
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0065c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00660
			 0x80000000,                                                  // -zero                                       /// 00664
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00668
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0066c
			 0xffc00001,                                                  // -signaling NaN                              /// 00670
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00674
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00678
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0067c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00680
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00684
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00688
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0068c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00694
			 0x7f800000,                                                  // inf                                         /// 00698
			 0xffc00001,                                                  // -signaling NaN                              /// 0069c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 006a0
			 0x80011111,                                                  // -9.7958E-41                                 /// 006a4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 006a8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006ac
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006b0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006b4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006b8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006bc
			 0x0e000007,                                                  // Trailing 1s:                                /// 006c0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006cc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 006d4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006dc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006e0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 006ec
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006f8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006fc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00708
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0070c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00710
			 0x0e000003,                                                  // Trailing 1s:                                /// 00714
			 0x0e000003,                                                  // Trailing 1s:                                /// 00718
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0071c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00720
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00724
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00728
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00730
			 0x7f800000,                                                  // inf                                         /// 00734
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00738
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0073c
			 0x00000000,                                                  // zero                                        /// 00740
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00744
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00748
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0074c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00750
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00754
			 0x0e000003,                                                  // Trailing 1s:                                /// 00758
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0075c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00764
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00768
			 0x4b000000,                                                  // 8388608.0                                   /// 0076c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00770
			 0x00011111,                                                  // 9.7958E-41                                  /// 00774
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00778
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00784
			 0x7f800000,                                                  // inf                                         /// 00788
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0078c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00790
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00794
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00798
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0079c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 007a8
			 0x0c600000,                                                  // Leading 1s:                                 /// 007ac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007b0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 007b8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007bc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007c0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007c4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 007c8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 007cc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 007d0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 007d4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007dc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007e0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e4
			 0x4b000000,                                                  // 8388608.0                                   /// 007e8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007fc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00800
			 0xcb000000,                                                  // -8388608.0                                  /// 00804
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00808
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0080c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00810
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00814
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00818
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0081c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00820
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00824
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00828
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0082c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00830
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00834
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00838
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00840
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00844
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00848
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0084c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00850
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00854
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0085c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00864
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00868
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00870
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00874
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0087c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00880
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00884
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00888
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0088c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00890
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00894
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00898
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0089c
			 0xbf028f5c,                                                  // -0.51                                       /// 008a0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 008a4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008ac
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b0
			 0x0c700000,                                                  // Leading 1s:                                 /// 008b4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008b8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008bc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008c0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008c8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 008cc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008d0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 008d4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008d8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008dc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008e0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0x0e000007,                                                  // Trailing 1s:                                /// 008e8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008ec
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008f4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008f8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008fc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00900
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00904
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00908
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0090c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00914
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00920
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00924
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00928
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0092c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00930
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00934
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00938
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0093c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00940
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00944
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00948
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0094c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00950
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00954
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00958
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0095c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00960
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00964
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00968
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0096c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00970
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00974
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00978
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0097c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00980
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00984
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0098c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00990
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00998
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0099c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 009a8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009b4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009b8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009bc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c4
			 0x80011111,                                                  // -9.7958E-41                                 /// 009c8
			 0x80011111,                                                  // -9.7958E-41                                 /// 009cc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 009dc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009e0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e4
			 0xffc00001,                                                  // -signaling NaN                              /// 009e8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 009ec
			 0x0e000001,                                                  // Trailing 1s:                                /// 009f0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 009f4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 009f8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009fc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a00
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a04
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a08
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a0c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a10
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a14
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a18
			 0x55555555,                                                  // 4 random values                             /// 00a1c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a20
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a24
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a2c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a30
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a34
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a38
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a3c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a40
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a44
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a48
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a4c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a50
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a54
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a58
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a60
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0xcb000000,                                                  // -8388608.0                                  /// 00a6c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a70
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a74
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a78
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a7c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a80
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a88
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a90
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a94
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a98
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a9c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00aa0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00aa4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00aa8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ab0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ab4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ab8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00abc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ac0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ac4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ad0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ad4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ad8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ae0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ae4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00aec
			 0x00000000,                                                  // zero                                        /// 00af0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00af8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00afc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b00
			 0x00000000,                                                  // zero                                        /// 00b04
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b08
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b0c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b10
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b14
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b18
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b1c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b20
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b24
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b28
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b2c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b30
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b3c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b40
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b44
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b48
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b4c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b50
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b54
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b58
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b5c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b60
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b64
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b70
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b74
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b78
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b7c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b80
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b84
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b88
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b8c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b90
			 0x80000000,                                                  // -zero                                       /// 00b94
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b98
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b9c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ba0
			 0x80000000,                                                  // -zero                                       /// 00ba4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ba8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00bb0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00bb4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00bb8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00bc4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00bcc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bd0
			 0x7fc00001,                                                  // signaling NaN                               /// 00bd4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00bdc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00be4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00be8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00bec
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00bf0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00bf8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00bfc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c00
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c04
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c08
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c0c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c10
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c18
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c1c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c20
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c24
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c28
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c2c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c30
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c34
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c38
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c3c
			 0x00000000,                                                  // zero                                        /// 00c40
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c44
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c48
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c50
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c54
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c58
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c5c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c64
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c6c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c70
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c74
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c78
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c7c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c84
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c88
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c8c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c90
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c98
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c9c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ca0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ca4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ca8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00cac
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00cb0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00cb4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cbc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00cc0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00cc4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00cc8
			 0x4b000000,                                                  // 8388608.0                                   /// 00ccc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00cd0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00cd8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ce0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ce4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ce8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cec
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00cf0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00cf4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00cfc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d00
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d04
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d08
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d0c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d10
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d14
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d18
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d1c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d20
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d24
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d28
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d2c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d30
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d34
			 0x33333333,                                                  // 4 random values                             /// 00d38
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d3c
			 0x3f028f5c,                                                  // 0.51                                        /// 00d40
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d44
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d48
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d4c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0x4b000000,                                                  // 8388608.0                                   /// 00d54
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d58
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d5c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d60
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d64
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d68
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d6c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d74
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d7c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d80
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d84
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d88
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d90
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d94
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d98
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d9c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da0
			 0x7fc00001,                                                  // signaling NaN                               /// 00da4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00da8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00dac
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00db0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00db8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00dbc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00dc4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00dcc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00dd0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00dd4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00dd8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ddc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00de0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00de4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00de8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00dec
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00df0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00df4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00df8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dfc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e00
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e0c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e10
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e14
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e18
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e1c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e20
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e24
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e28
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e2c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e30
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e34
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e38
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e3c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e40
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e44
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e48
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e4c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e50
			 0x33333333,                                                  // 4 random values                             /// 00e54
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e58
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e5c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e60
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e64
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e6c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e70
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e74
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e78
			 0x7f800000,                                                  // inf                                         /// 00e7c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e80
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e84
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e88
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e8c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e94
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e98
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e9c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ea0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ea4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ea8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00eac
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00eb0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00eb4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00eb8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0xff800000,                                                  // -inf                                        /// 00ec0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ec8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ecc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ed0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00edc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ee0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ee8
			 0xffc00001,                                                  // -signaling NaN                              /// 00eec
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ef0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ef4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ef8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f00
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f04
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f08
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f0c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f14
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f18
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f1c
			 0x80000000,                                                  // -zero                                       /// 00f20
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f24
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f28
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f2c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f30
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f34
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f38
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f3c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f40
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f44
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f48
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f4c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f50
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f58
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f60
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f64
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f68
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f6c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f70
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f74
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f78
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f7c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f80
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f84
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f88
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f8c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f90
			 0xbf028f5c,                                                  // -0.51                                       /// 00f94
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f98
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f9c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00fa4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00fa8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00fac
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fb4
			 0xff800000,                                                  // -inf                                        /// 00fb8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0x00000000,                                                  // zero                                        /// 00fc0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fc4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00fc8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fcc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fd0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00fd4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00fdc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fe0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00fe4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fec
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ff0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff8
			 0x80000200                                                  // SP - 1 bit alone set in mantissa -ve        /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x4aac6c63, /// 0x0
			 0x050ff28e, /// 0x4
			 0xd906c108, /// 0x8
			 0xf6632745, /// 0xc
			 0xf69ee91e, /// 0x10
			 0x9a6ef354, /// 0x14
			 0xdc5099a4, /// 0x18
			 0x4eb8d1cc, /// 0x1c
			 0x9ba0e03b, /// 0x20
			 0x810b2e69, /// 0x24
			 0x3be2bb64, /// 0x28
			 0xe3a2e100, /// 0x2c
			 0x22c360a3, /// 0x30
			 0xdb50c2ad, /// 0x34
			 0x4ea8c728, /// 0x38
			 0x744bcfc5, /// 0x3c
			 0xc68cebf7, /// 0x40
			 0xafbc13fb, /// 0x44
			 0xc27b245d, /// 0x48
			 0x6ea3ec1f, /// 0x4c
			 0x7cdaa631, /// 0x50
			 0xfd5032c9, /// 0x54
			 0xc5447d4b, /// 0x58
			 0x0ed75bd9, /// 0x5c
			 0xbd756078, /// 0x60
			 0x1521470e, /// 0x64
			 0xa5b8c667, /// 0x68
			 0xe561ab88, /// 0x6c
			 0x8c1b6fef, /// 0x70
			 0x3c1088b6, /// 0x74
			 0x60b3b844, /// 0x78
			 0x452d3005, /// 0x7c
			 0x772b70c2, /// 0x80
			 0x64fadd1f, /// 0x84
			 0x6cd29260, /// 0x88
			 0x6b662b3b, /// 0x8c
			 0xda2e6553, /// 0x90
			 0xffbea4ec, /// 0x94
			 0xf548ea6c, /// 0x98
			 0xa2cffe3b, /// 0x9c
			 0x4a2045d7, /// 0xa0
			 0x102add94, /// 0xa4
			 0xb0b86429, /// 0xa8
			 0x2a6f73f2, /// 0xac
			 0xabf7cf48, /// 0xb0
			 0x5e7f6b76, /// 0xb4
			 0x3ca90300, /// 0xb8
			 0x5d7940c3, /// 0xbc
			 0xb177a4ce, /// 0xc0
			 0x31ec7d63, /// 0xc4
			 0xd343b524, /// 0xc8
			 0xe01ff39e, /// 0xcc
			 0xa3c9d256, /// 0xd0
			 0x088f2680, /// 0xd4
			 0x801730c0, /// 0xd8
			 0x05ac5cd2, /// 0xdc
			 0x95ee50e0, /// 0xe0
			 0x782b9ff6, /// 0xe4
			 0xb8c62289, /// 0xe8
			 0x086acb9b, /// 0xec
			 0xa2ab35bd, /// 0xf0
			 0x4972faea, /// 0xf4
			 0xe1012f76, /// 0xf8
			 0x487e4367, /// 0xfc
			 0x7b3d515b, /// 0x100
			 0xb34ada1b, /// 0x104
			 0x65d8be86, /// 0x108
			 0xd031bf92, /// 0x10c
			 0xb50dbd5a, /// 0x110
			 0x7c491699, /// 0x114
			 0x874e02f7, /// 0x118
			 0x96926091, /// 0x11c
			 0x256b0436, /// 0x120
			 0x13942792, /// 0x124
			 0xa4fbc84c, /// 0x128
			 0x02462fb4, /// 0x12c
			 0x2cac5cfe, /// 0x130
			 0x9587a1c4, /// 0x134
			 0x33ee6514, /// 0x138
			 0x7f975490, /// 0x13c
			 0x9e40576f, /// 0x140
			 0x6f43bbc5, /// 0x144
			 0x47e128b9, /// 0x148
			 0x5ce200ab, /// 0x14c
			 0x0069745d, /// 0x150
			 0xfdc513ee, /// 0x154
			 0x4ff0e40c, /// 0x158
			 0xae694915, /// 0x15c
			 0x176603aa, /// 0x160
			 0xd07fd241, /// 0x164
			 0x0132456f, /// 0x168
			 0x0bf72b37, /// 0x16c
			 0xe667b1fa, /// 0x170
			 0xe1c692b9, /// 0x174
			 0x8457c8f8, /// 0x178
			 0xf29f434b, /// 0x17c
			 0xee24198b, /// 0x180
			 0x821bb2a0, /// 0x184
			 0xb2a884df, /// 0x188
			 0x1f85c3d2, /// 0x18c
			 0xf2ffb57d, /// 0x190
			 0x18dd1faa, /// 0x194
			 0x9aeb3ae2, /// 0x198
			 0x81472c80, /// 0x19c
			 0x946820f3, /// 0x1a0
			 0xe8caaf22, /// 0x1a4
			 0x4aa12087, /// 0x1a8
			 0x43ad13bc, /// 0x1ac
			 0x8aa44644, /// 0x1b0
			 0x3a61d284, /// 0x1b4
			 0x26bf3b54, /// 0x1b8
			 0x1212aa22, /// 0x1bc
			 0xb2aeb729, /// 0x1c0
			 0xfcaae056, /// 0x1c4
			 0x583e2620, /// 0x1c8
			 0x39f4324d, /// 0x1cc
			 0xc82ead57, /// 0x1d0
			 0x94f40c4e, /// 0x1d4
			 0x20822e5f, /// 0x1d8
			 0x7788844c, /// 0x1dc
			 0x19e6b3ed, /// 0x1e0
			 0x70a989d3, /// 0x1e4
			 0x1a2fc9c3, /// 0x1e8
			 0xcb9ab690, /// 0x1ec
			 0x92a8b997, /// 0x1f0
			 0x6c7b5156, /// 0x1f4
			 0xd623f918, /// 0x1f8
			 0x8485ba52, /// 0x1fc
			 0x0b1f4ba3, /// 0x200
			 0xccd3ea07, /// 0x204
			 0xdf0c7b20, /// 0x208
			 0xc730c38e, /// 0x20c
			 0xdc7471e9, /// 0x210
			 0xa8febdf9, /// 0x214
			 0x6dd706d5, /// 0x218
			 0x5b355747, /// 0x21c
			 0x69d72866, /// 0x220
			 0x698eed0b, /// 0x224
			 0x3ce1edd2, /// 0x228
			 0x858f0032, /// 0x22c
			 0xf8da4575, /// 0x230
			 0x7142ede2, /// 0x234
			 0x010eaee0, /// 0x238
			 0xa1ea3d71, /// 0x23c
			 0xa14dccf3, /// 0x240
			 0x68e16f19, /// 0x244
			 0xf3c2a3b6, /// 0x248
			 0x23004e56, /// 0x24c
			 0x049e8389, /// 0x250
			 0x62fb258e, /// 0x254
			 0x0306ae7d, /// 0x258
			 0x55951d0d, /// 0x25c
			 0xae0a6439, /// 0x260
			 0x6134191e, /// 0x264
			 0x1f0d8890, /// 0x268
			 0x41b0502a, /// 0x26c
			 0xe923244e, /// 0x270
			 0xb7f807e0, /// 0x274
			 0x7055b91b, /// 0x278
			 0x169068d4, /// 0x27c
			 0x8babf846, /// 0x280
			 0x2de4d62f, /// 0x284
			 0x197d7ea1, /// 0x288
			 0x92cd48a8, /// 0x28c
			 0x491c3b32, /// 0x290
			 0xe44db2ad, /// 0x294
			 0xcec739cc, /// 0x298
			 0x892eed36, /// 0x29c
			 0x0deb5247, /// 0x2a0
			 0xbbe2ae66, /// 0x2a4
			 0x7b06a9cb, /// 0x2a8
			 0x09ca6237, /// 0x2ac
			 0x63e9fa31, /// 0x2b0
			 0x5ad9a4c8, /// 0x2b4
			 0xffac6f21, /// 0x2b8
			 0x8f2e22ff, /// 0x2bc
			 0x1a7110a1, /// 0x2c0
			 0x3ae75eb2, /// 0x2c4
			 0x7dee2c3b, /// 0x2c8
			 0x5cd10690, /// 0x2cc
			 0x32453829, /// 0x2d0
			 0x593893bb, /// 0x2d4
			 0xcfaead21, /// 0x2d8
			 0x49865cf0, /// 0x2dc
			 0x814205b7, /// 0x2e0
			 0x88716c5d, /// 0x2e4
			 0xb92952f3, /// 0x2e8
			 0x424dd467, /// 0x2ec
			 0x4964b5bd, /// 0x2f0
			 0xe2157dcd, /// 0x2f4
			 0xbdf4939e, /// 0x2f8
			 0x104a6100, /// 0x2fc
			 0x00129df5, /// 0x300
			 0x8d068d3b, /// 0x304
			 0x83b342c8, /// 0x308
			 0xc922a576, /// 0x30c
			 0x30594f8b, /// 0x310
			 0x13e812b0, /// 0x314
			 0x6198200c, /// 0x318
			 0x68897d74, /// 0x31c
			 0x0cf75624, /// 0x320
			 0xe07773ca, /// 0x324
			 0xa94a6e15, /// 0x328
			 0xe0143f48, /// 0x32c
			 0x08ac834a, /// 0x330
			 0xddec0af4, /// 0x334
			 0x74688620, /// 0x338
			 0xf3fdc163, /// 0x33c
			 0x3297337f, /// 0x340
			 0x56750e27, /// 0x344
			 0x56ba0746, /// 0x348
			 0x1d48b18a, /// 0x34c
			 0xc4251d32, /// 0x350
			 0x9abe8809, /// 0x354
			 0x6049bee1, /// 0x358
			 0xbce2ade0, /// 0x35c
			 0x94fca25a, /// 0x360
			 0xce0a0ddd, /// 0x364
			 0xf4219ff5, /// 0x368
			 0x766fab26, /// 0x36c
			 0xe1f30dde, /// 0x370
			 0xc0979a29, /// 0x374
			 0x2be8399e, /// 0x378
			 0xd864595b, /// 0x37c
			 0x4b2d9a66, /// 0x380
			 0x0845fbf7, /// 0x384
			 0xe4cc9a19, /// 0x388
			 0x6da6eb62, /// 0x38c
			 0x144d0608, /// 0x390
			 0x85276dbe, /// 0x394
			 0x3cad5c1a, /// 0x398
			 0xcf600696, /// 0x39c
			 0x44ebc61c, /// 0x3a0
			 0xe24b69e8, /// 0x3a4
			 0x2f3b5e1e, /// 0x3a8
			 0xa800ff64, /// 0x3ac
			 0x17da72e7, /// 0x3b0
			 0x8545b7c4, /// 0x3b4
			 0x0f910d63, /// 0x3b8
			 0xbb40357d, /// 0x3bc
			 0x2b36166a, /// 0x3c0
			 0x275c20b1, /// 0x3c4
			 0xe2606a16, /// 0x3c8
			 0xe27ed6fb, /// 0x3cc
			 0x22069680, /// 0x3d0
			 0x875e7feb, /// 0x3d4
			 0x77b08b76, /// 0x3d8
			 0x5c72c1e3, /// 0x3dc
			 0xf6dd4f49, /// 0x3e0
			 0x84c5ea31, /// 0x3e4
			 0xc32c4ccf, /// 0x3e8
			 0x24529e63, /// 0x3ec
			 0x4a3e166c, /// 0x3f0
			 0xefbfe096, /// 0x3f4
			 0x9622ed60, /// 0x3f8
			 0x88a0b429, /// 0x3fc
			 0x148d91f6, /// 0x400
			 0x13264c62, /// 0x404
			 0x6c6b6cb7, /// 0x408
			 0x3c6a6ac4, /// 0x40c
			 0x6541f139, /// 0x410
			 0x476ddec6, /// 0x414
			 0x8ac96acc, /// 0x418
			 0x0fef6c2f, /// 0x41c
			 0x11a488d6, /// 0x420
			 0x6892125d, /// 0x424
			 0x823cf35a, /// 0x428
			 0x76db4b6d, /// 0x42c
			 0xeda58bec, /// 0x430
			 0xc7b73829, /// 0x434
			 0x9a50bea0, /// 0x438
			 0x74d4f70f, /// 0x43c
			 0x7806588c, /// 0x440
			 0xcb7c5720, /// 0x444
			 0xf2cf94bb, /// 0x448
			 0x0a900e45, /// 0x44c
			 0x85b37101, /// 0x450
			 0x5eaa1723, /// 0x454
			 0x453b916a, /// 0x458
			 0x3d299b63, /// 0x45c
			 0x51e3c771, /// 0x460
			 0x7da8fef7, /// 0x464
			 0x47ef8f62, /// 0x468
			 0x789d8331, /// 0x46c
			 0x6388ebbe, /// 0x470
			 0x0a726096, /// 0x474
			 0x396da4f6, /// 0x478
			 0xc5729648, /// 0x47c
			 0x5f98df2b, /// 0x480
			 0xd2986e5f, /// 0x484
			 0x61d2125d, /// 0x488
			 0x8a12aa7c, /// 0x48c
			 0x62496e69, /// 0x490
			 0xc8d6e9ab, /// 0x494
			 0xa8327187, /// 0x498
			 0xb0559254, /// 0x49c
			 0x63f1e33b, /// 0x4a0
			 0x27510c8a, /// 0x4a4
			 0xfcf86fc8, /// 0x4a8
			 0x6f188eec, /// 0x4ac
			 0x4c2dec68, /// 0x4b0
			 0xd466ff3f, /// 0x4b4
			 0xdbf004e7, /// 0x4b8
			 0xb864dfa5, /// 0x4bc
			 0x69ae2976, /// 0x4c0
			 0xda828ce5, /// 0x4c4
			 0x6c3829ee, /// 0x4c8
			 0xdab29cd7, /// 0x4cc
			 0xc55a1387, /// 0x4d0
			 0x8f6f8ade, /// 0x4d4
			 0x147fb2ac, /// 0x4d8
			 0x9db34646, /// 0x4dc
			 0x75ed943c, /// 0x4e0
			 0x92b844e4, /// 0x4e4
			 0x6f829ed7, /// 0x4e8
			 0xc56ec73b, /// 0x4ec
			 0x6aff7425, /// 0x4f0
			 0x6bcacd39, /// 0x4f4
			 0xd77d811b, /// 0x4f8
			 0x0974aa9e, /// 0x4fc
			 0x8e9258e2, /// 0x500
			 0x9fdf2113, /// 0x504
			 0x8f2abd3c, /// 0x508
			 0xdc439d33, /// 0x50c
			 0x5a57dfeb, /// 0x510
			 0xd4ad42c1, /// 0x514
			 0xd78c55ec, /// 0x518
			 0x0c1e40db, /// 0x51c
			 0x7f11134d, /// 0x520
			 0xd7bd7bd9, /// 0x524
			 0x992bb72e, /// 0x528
			 0x871f2baa, /// 0x52c
			 0x789d1020, /// 0x530
			 0x8fedb627, /// 0x534
			 0x8c4d5a96, /// 0x538
			 0x846e9e3e, /// 0x53c
			 0xa44fd447, /// 0x540
			 0x9af64655, /// 0x544
			 0x62ed0b40, /// 0x548
			 0x8087c0c3, /// 0x54c
			 0xf0c4de99, /// 0x550
			 0x4c041804, /// 0x554
			 0x6b2dff9d, /// 0x558
			 0x738cff2b, /// 0x55c
			 0x53a66ffd, /// 0x560
			 0xe92c1d99, /// 0x564
			 0x499815ae, /// 0x568
			 0x428c6385, /// 0x56c
			 0x7745b981, /// 0x570
			 0xf8635700, /// 0x574
			 0x41f2e19a, /// 0x578
			 0x37c38cd1, /// 0x57c
			 0xc0296809, /// 0x580
			 0xa0603104, /// 0x584
			 0x557fc9bc, /// 0x588
			 0x8d8d8605, /// 0x58c
			 0x5f24b87a, /// 0x590
			 0x257ed105, /// 0x594
			 0x254ec6b2, /// 0x598
			 0x8cad0add, /// 0x59c
			 0x2a7133a2, /// 0x5a0
			 0xd7d7be3c, /// 0x5a4
			 0x6676f914, /// 0x5a8
			 0x7a37168b, /// 0x5ac
			 0xc7928186, /// 0x5b0
			 0xa4840b99, /// 0x5b4
			 0xf32574af, /// 0x5b8
			 0xd2db8558, /// 0x5bc
			 0x7a04d774, /// 0x5c0
			 0xdf8e522e, /// 0x5c4
			 0xbcbe6a93, /// 0x5c8
			 0xebe1a711, /// 0x5cc
			 0x5626063d, /// 0x5d0
			 0x619ea095, /// 0x5d4
			 0xb39c2879, /// 0x5d8
			 0x1208ecb7, /// 0x5dc
			 0x88941fe0, /// 0x5e0
			 0x7f71d173, /// 0x5e4
			 0x2cc91f2c, /// 0x5e8
			 0xb39a63f0, /// 0x5ec
			 0x4ca0ae3a, /// 0x5f0
			 0xcb2dc85e, /// 0x5f4
			 0x45260391, /// 0x5f8
			 0x721ed966, /// 0x5fc
			 0x821e87f0, /// 0x600
			 0x7ef4cc19, /// 0x604
			 0x1ec4219f, /// 0x608
			 0x86afa46b, /// 0x60c
			 0x3f6e55d8, /// 0x610
			 0x146561c7, /// 0x614
			 0xfb0f7b77, /// 0x618
			 0x3f080793, /// 0x61c
			 0x0b93c321, /// 0x620
			 0xff3d16e6, /// 0x624
			 0x899715ee, /// 0x628
			 0xb7975c70, /// 0x62c
			 0x461426b0, /// 0x630
			 0xfc78f57e, /// 0x634
			 0xdb598c8f, /// 0x638
			 0x4c1d9cb6, /// 0x63c
			 0x22ab4c1c, /// 0x640
			 0x9264bf87, /// 0x644
			 0xd201deae, /// 0x648
			 0x3b27f088, /// 0x64c
			 0x9771cae5, /// 0x650
			 0xc73c068e, /// 0x654
			 0x6d9cba9c, /// 0x658
			 0x2679499e, /// 0x65c
			 0xa54aab8b, /// 0x660
			 0x2a8ca91f, /// 0x664
			 0x862113fe, /// 0x668
			 0xd1bcf508, /// 0x66c
			 0xabfa24b0, /// 0x670
			 0xb903203f, /// 0x674
			 0xc3e43309, /// 0x678
			 0x3ab19b24, /// 0x67c
			 0xe575098b, /// 0x680
			 0x081b4e85, /// 0x684
			 0x61ed080f, /// 0x688
			 0xa3e336a5, /// 0x68c
			 0xbfd89391, /// 0x690
			 0x27af646b, /// 0x694
			 0x1fb743e6, /// 0x698
			 0x026941bd, /// 0x69c
			 0x0710bcfb, /// 0x6a0
			 0xf29fb167, /// 0x6a4
			 0x552150f6, /// 0x6a8
			 0x93188a37, /// 0x6ac
			 0xf173f8a5, /// 0x6b0
			 0x2af76744, /// 0x6b4
			 0xf44c7cca, /// 0x6b8
			 0xe34231ea, /// 0x6bc
			 0x3d3af9b7, /// 0x6c0
			 0xa0b6c91f, /// 0x6c4
			 0x9a211628, /// 0x6c8
			 0x55b712af, /// 0x6cc
			 0xd11f71d6, /// 0x6d0
			 0xa55c1fd1, /// 0x6d4
			 0x42d32336, /// 0x6d8
			 0xe1c54588, /// 0x6dc
			 0xb0db8679, /// 0x6e0
			 0xd8754964, /// 0x6e4
			 0x89a4eb13, /// 0x6e8
			 0x975c1c5e, /// 0x6ec
			 0xfac0ecc1, /// 0x6f0
			 0xd8318ffc, /// 0x6f4
			 0x99095ebe, /// 0x6f8
			 0xf7055862, /// 0x6fc
			 0x35bf0007, /// 0x700
			 0xf37a1b84, /// 0x704
			 0xca9a0393, /// 0x708
			 0xe0ef8731, /// 0x70c
			 0x108e79fc, /// 0x710
			 0xad7182d3, /// 0x714
			 0x4a5b3ba9, /// 0x718
			 0x3726fe1f, /// 0x71c
			 0x43cb4433, /// 0x720
			 0x7581109d, /// 0x724
			 0x9bc169e5, /// 0x728
			 0x4d7f6917, /// 0x72c
			 0x53cb1d98, /// 0x730
			 0xa1ad1500, /// 0x734
			 0x208ca1f4, /// 0x738
			 0x59dfd93d, /// 0x73c
			 0x86961962, /// 0x740
			 0xc03a5968, /// 0x744
			 0x1a9e5bcc, /// 0x748
			 0x1cf7aa85, /// 0x74c
			 0xf66bc537, /// 0x750
			 0xd557dd44, /// 0x754
			 0xd16ca74e, /// 0x758
			 0xb06fbdfb, /// 0x75c
			 0xe49fdf95, /// 0x760
			 0x8663869b, /// 0x764
			 0xca29e382, /// 0x768
			 0xd06fb210, /// 0x76c
			 0xbfafef63, /// 0x770
			 0x445b4bcf, /// 0x774
			 0x680bb3f3, /// 0x778
			 0xda6e1a22, /// 0x77c
			 0x76e0577a, /// 0x780
			 0xe73f1a5b, /// 0x784
			 0x125d54f8, /// 0x788
			 0xc82f4137, /// 0x78c
			 0xf1839b1e, /// 0x790
			 0x35cbb50d, /// 0x794
			 0x8039d386, /// 0x798
			 0xe8f3f018, /// 0x79c
			 0xf5dba6f2, /// 0x7a0
			 0x6f27c579, /// 0x7a4
			 0x02b1e6d8, /// 0x7a8
			 0x2af3f8d8, /// 0x7ac
			 0xa422a0ea, /// 0x7b0
			 0x0afbc1f6, /// 0x7b4
			 0xe9253957, /// 0x7b8
			 0x3e2e39e6, /// 0x7bc
			 0x1f820f44, /// 0x7c0
			 0x8df7c531, /// 0x7c4
			 0x1d35eb28, /// 0x7c8
			 0xf1107019, /// 0x7cc
			 0xcd1859c8, /// 0x7d0
			 0x3393cf3f, /// 0x7d4
			 0xf7d8864c, /// 0x7d8
			 0xe6a7ead2, /// 0x7dc
			 0x7311eee6, /// 0x7e0
			 0x2f5064a9, /// 0x7e4
			 0xd9b9c2f2, /// 0x7e8
			 0x3706a214, /// 0x7ec
			 0x49d42862, /// 0x7f0
			 0x5ca969e7, /// 0x7f4
			 0xad6290e5, /// 0x7f8
			 0x067c25c4, /// 0x7fc
			 0xe2d047c1, /// 0x800
			 0xc3ea2b48, /// 0x804
			 0x0b6f274d, /// 0x808
			 0x4480eaea, /// 0x80c
			 0x0c511731, /// 0x810
			 0x18259dbf, /// 0x814
			 0x6bc55ca0, /// 0x818
			 0xd7299e6a, /// 0x81c
			 0xbfaecf31, /// 0x820
			 0x8e36f4a7, /// 0x824
			 0xbeb3bc0b, /// 0x828
			 0x37cd0bd0, /// 0x82c
			 0x57273910, /// 0x830
			 0x61c86121, /// 0x834
			 0x103ba9e5, /// 0x838
			 0x2dda805c, /// 0x83c
			 0xdebf6ce8, /// 0x840
			 0x96affb17, /// 0x844
			 0x61c1edd6, /// 0x848
			 0x7a4cc7ac, /// 0x84c
			 0x42cfced4, /// 0x850
			 0x688dd558, /// 0x854
			 0x8af60da7, /// 0x858
			 0x47cdb8bd, /// 0x85c
			 0xc3e6c08d, /// 0x860
			 0x8a413914, /// 0x864
			 0xa607b0a4, /// 0x868
			 0x811bf7c9, /// 0x86c
			 0x2a8c7760, /// 0x870
			 0xc2fa17fb, /// 0x874
			 0x439e6926, /// 0x878
			 0x315e4564, /// 0x87c
			 0xb35ed22b, /// 0x880
			 0xffa9c0e8, /// 0x884
			 0xd9d7eda4, /// 0x888
			 0x28b40c2d, /// 0x88c
			 0x63f1c773, /// 0x890
			 0xd281082e, /// 0x894
			 0xb48e2bcf, /// 0x898
			 0x3cacdf35, /// 0x89c
			 0x6e86fa5e, /// 0x8a0
			 0xb55a9e0a, /// 0x8a4
			 0x7f6379ab, /// 0x8a8
			 0x9f6680f1, /// 0x8ac
			 0x4ac30546, /// 0x8b0
			 0x2aa12d1e, /// 0x8b4
			 0x8659b456, /// 0x8b8
			 0x028f5f87, /// 0x8bc
			 0x47bad2a7, /// 0x8c0
			 0x8ff9f18a, /// 0x8c4
			 0x5efb3e0f, /// 0x8c8
			 0xeb3afed6, /// 0x8cc
			 0x178c1f6d, /// 0x8d0
			 0xed007c86, /// 0x8d4
			 0xb8b1a27f, /// 0x8d8
			 0x878d3bc5, /// 0x8dc
			 0xaa3332af, /// 0x8e0
			 0xfd8bf845, /// 0x8e4
			 0x0eb81a81, /// 0x8e8
			 0x4659bc6f, /// 0x8ec
			 0x5796d212, /// 0x8f0
			 0x2f0c4a52, /// 0x8f4
			 0x572b506d, /// 0x8f8
			 0x961e1ab7, /// 0x8fc
			 0xa7fe25be, /// 0x900
			 0x2eb1c7f5, /// 0x904
			 0xaaed16ae, /// 0x908
			 0x6d4457f1, /// 0x90c
			 0x42bb1ff9, /// 0x910
			 0x3e5424ff, /// 0x914
			 0xe4f93768, /// 0x918
			 0x44877668, /// 0x91c
			 0xf490f2af, /// 0x920
			 0xf78fc253, /// 0x924
			 0xca2f0e69, /// 0x928
			 0x9b0aecc3, /// 0x92c
			 0xde329af9, /// 0x930
			 0x2bce7b84, /// 0x934
			 0xd0531717, /// 0x938
			 0x0baef5dc, /// 0x93c
			 0x30531233, /// 0x940
			 0x144779dc, /// 0x944
			 0x18e34a18, /// 0x948
			 0xcbbae9f9, /// 0x94c
			 0xe33b0708, /// 0x950
			 0xd452e487, /// 0x954
			 0xcb580022, /// 0x958
			 0xe84c6c57, /// 0x95c
			 0x95b3b01a, /// 0x960
			 0xf3b0889d, /// 0x964
			 0xe3b22213, /// 0x968
			 0x4bb8dc06, /// 0x96c
			 0x8049cdc7, /// 0x970
			 0xef79de37, /// 0x974
			 0x43eedf5b, /// 0x978
			 0xc7392ae5, /// 0x97c
			 0x6631b297, /// 0x980
			 0xb2184de7, /// 0x984
			 0x1c2ccc55, /// 0x988
			 0xa687b195, /// 0x98c
			 0xb3a698b8, /// 0x990
			 0x90f2d1e6, /// 0x994
			 0x0acee337, /// 0x998
			 0x0f9f9b45, /// 0x99c
			 0xacb9cb41, /// 0x9a0
			 0xc8358615, /// 0x9a4
			 0xcd7b7a6a, /// 0x9a8
			 0xd87dbc11, /// 0x9ac
			 0xf316bcbf, /// 0x9b0
			 0xc68dad8b, /// 0x9b4
			 0x2adb447c, /// 0x9b8
			 0x64f078e1, /// 0x9bc
			 0x136d16ce, /// 0x9c0
			 0xfa27361d, /// 0x9c4
			 0x19d2601c, /// 0x9c8
			 0x8119003e, /// 0x9cc
			 0x062860f0, /// 0x9d0
			 0x6fe81104, /// 0x9d4
			 0xcd7db1d3, /// 0x9d8
			 0xc54d7b95, /// 0x9dc
			 0xe4298bc7, /// 0x9e0
			 0xe35f859d, /// 0x9e4
			 0xdea7326e, /// 0x9e8
			 0xd58f8732, /// 0x9ec
			 0xb3b870db, /// 0x9f0
			 0x88ac0d2f, /// 0x9f4
			 0x24684289, /// 0x9f8
			 0xa43c6d74, /// 0x9fc
			 0x7edad4e1, /// 0xa00
			 0x04af7d21, /// 0xa04
			 0xdb49a34e, /// 0xa08
			 0xeaa741f0, /// 0xa0c
			 0x4752c9bf, /// 0xa10
			 0x2b94129c, /// 0xa14
			 0x655b1b9b, /// 0xa18
			 0x2d128791, /// 0xa1c
			 0x9b089259, /// 0xa20
			 0x6453e89f, /// 0xa24
			 0x59409656, /// 0xa28
			 0x7439b451, /// 0xa2c
			 0xcac5147f, /// 0xa30
			 0xeed2c05a, /// 0xa34
			 0xa61767ba, /// 0xa38
			 0x093fb151, /// 0xa3c
			 0x84d2c5e8, /// 0xa40
			 0x56baa7f3, /// 0xa44
			 0x7b41a9f1, /// 0xa48
			 0x9e274b13, /// 0xa4c
			 0xa6841fa6, /// 0xa50
			 0xe0f2859d, /// 0xa54
			 0xe9848c7a, /// 0xa58
			 0x2f7d812d, /// 0xa5c
			 0x0b24afd0, /// 0xa60
			 0x9b2b4e1f, /// 0xa64
			 0x4bd5b268, /// 0xa68
			 0x4201d9ae, /// 0xa6c
			 0x8f730a20, /// 0xa70
			 0xc3bf8138, /// 0xa74
			 0x9cba85fc, /// 0xa78
			 0x429c51dc, /// 0xa7c
			 0xb884c9d3, /// 0xa80
			 0xc53e7083, /// 0xa84
			 0xb7acb0d6, /// 0xa88
			 0xa77595d8, /// 0xa8c
			 0x9d29dcfd, /// 0xa90
			 0xfd55da7e, /// 0xa94
			 0x771a1185, /// 0xa98
			 0x1abb3143, /// 0xa9c
			 0xe0137163, /// 0xaa0
			 0xe687b066, /// 0xaa4
			 0xbb085d85, /// 0xaa8
			 0x8cc0490c, /// 0xaac
			 0x42ae1d62, /// 0xab0
			 0x001d4b57, /// 0xab4
			 0xe56483cf, /// 0xab8
			 0x57ccdfdb, /// 0xabc
			 0xa38a0e4a, /// 0xac0
			 0xaa08751c, /// 0xac4
			 0x1bb72ec2, /// 0xac8
			 0x4cae6d06, /// 0xacc
			 0x6f7c3613, /// 0xad0
			 0x96a79c92, /// 0xad4
			 0x3ec5a741, /// 0xad8
			 0xe135b355, /// 0xadc
			 0x36ccdb46, /// 0xae0
			 0x8876910b, /// 0xae4
			 0x2f19b393, /// 0xae8
			 0xcf8edaca, /// 0xaec
			 0x79ad430a, /// 0xaf0
			 0xaebe54cb, /// 0xaf4
			 0xd98a1786, /// 0xaf8
			 0x09b1bb1d, /// 0xafc
			 0x18924e92, /// 0xb00
			 0x8e1c6601, /// 0xb04
			 0xdab9fc48, /// 0xb08
			 0xeea67386, /// 0xb0c
			 0x5600183e, /// 0xb10
			 0x96e0e5f0, /// 0xb14
			 0xc3050874, /// 0xb18
			 0x69f38d95, /// 0xb1c
			 0x2f592110, /// 0xb20
			 0x98d6ab57, /// 0xb24
			 0xc65496dc, /// 0xb28
			 0x16f3b9a4, /// 0xb2c
			 0x7b1175cb, /// 0xb30
			 0x246665ab, /// 0xb34
			 0x9d715712, /// 0xb38
			 0xf2480cc1, /// 0xb3c
			 0x63f7f77d, /// 0xb40
			 0xd1252f14, /// 0xb44
			 0x156e931d, /// 0xb48
			 0xe682e42a, /// 0xb4c
			 0x4242132f, /// 0xb50
			 0xe78f191a, /// 0xb54
			 0x61979298, /// 0xb58
			 0x4163e314, /// 0xb5c
			 0x54afa31e, /// 0xb60
			 0xd6dbde04, /// 0xb64
			 0x994a204e, /// 0xb68
			 0xdb889bdd, /// 0xb6c
			 0x9888a1a6, /// 0xb70
			 0xd28dc400, /// 0xb74
			 0x91737f4a, /// 0xb78
			 0x1412209f, /// 0xb7c
			 0x68ce806b, /// 0xb80
			 0x1b552c37, /// 0xb84
			 0xa4f60938, /// 0xb88
			 0x76ea1dab, /// 0xb8c
			 0x41d25b8a, /// 0xb90
			 0x68af0308, /// 0xb94
			 0xace4adba, /// 0xb98
			 0xb16b29a2, /// 0xb9c
			 0xab9f8d9e, /// 0xba0
			 0xd16354d0, /// 0xba4
			 0x00f05274, /// 0xba8
			 0xb3d51878, /// 0xbac
			 0x42fc1bce, /// 0xbb0
			 0xff82ff6d, /// 0xbb4
			 0x91db50c8, /// 0xbb8
			 0x9b9a6d87, /// 0xbbc
			 0x51eef02a, /// 0xbc0
			 0xf64af49f, /// 0xbc4
			 0xc26a5b9c, /// 0xbc8
			 0xa0e241ec, /// 0xbcc
			 0x67b36088, /// 0xbd0
			 0x23271ecd, /// 0xbd4
			 0xa8651673, /// 0xbd8
			 0xaec99264, /// 0xbdc
			 0xf8c496c8, /// 0xbe0
			 0xfbec1760, /// 0xbe4
			 0x3ee9b4df, /// 0xbe8
			 0xbb53d529, /// 0xbec
			 0xd43bdf5d, /// 0xbf0
			 0x12f1808d, /// 0xbf4
			 0x849ac336, /// 0xbf8
			 0xecd764f8, /// 0xbfc
			 0xba2706a4, /// 0xc00
			 0xe00615f3, /// 0xc04
			 0x3cee500f, /// 0xc08
			 0xa807fdb1, /// 0xc0c
			 0xc353034e, /// 0xc10
			 0x48a91bf7, /// 0xc14
			 0x28632592, /// 0xc18
			 0x04cd2156, /// 0xc1c
			 0x26a82bab, /// 0xc20
			 0xf64891f2, /// 0xc24
			 0x95e7ba76, /// 0xc28
			 0x299b817a, /// 0xc2c
			 0xe35bb4f4, /// 0xc30
			 0x39072c4a, /// 0xc34
			 0x2a7dbdc2, /// 0xc38
			 0x6c5c675c, /// 0xc3c
			 0x2e88edee, /// 0xc40
			 0x5bcca3fe, /// 0xc44
			 0xfacdfe11, /// 0xc48
			 0xf8c99409, /// 0xc4c
			 0x53cb3986, /// 0xc50
			 0xb2d40d4f, /// 0xc54
			 0x19b5a1a1, /// 0xc58
			 0xf64e68e8, /// 0xc5c
			 0xcacda421, /// 0xc60
			 0xba547a1b, /// 0xc64
			 0xdf213f18, /// 0xc68
			 0xd921d29c, /// 0xc6c
			 0x0b208a94, /// 0xc70
			 0xd2a664a9, /// 0xc74
			 0xbc2febfe, /// 0xc78
			 0x4ee692ec, /// 0xc7c
			 0xdaa6f0a1, /// 0xc80
			 0x4bfccf44, /// 0xc84
			 0x68c933e4, /// 0xc88
			 0xc7ab6389, /// 0xc8c
			 0x7877eb32, /// 0xc90
			 0x66bfccc7, /// 0xc94
			 0xb6d3f81b, /// 0xc98
			 0x37e5fc42, /// 0xc9c
			 0xa681bdca, /// 0xca0
			 0xb981d9db, /// 0xca4
			 0xbfa4ce2b, /// 0xca8
			 0xeb47c7df, /// 0xcac
			 0x5db031f4, /// 0xcb0
			 0x07be724f, /// 0xcb4
			 0xcb337831, /// 0xcb8
			 0x7b503401, /// 0xcbc
			 0xc929ad6f, /// 0xcc0
			 0x90ac61bb, /// 0xcc4
			 0x4a7d9f9b, /// 0xcc8
			 0xe71b08aa, /// 0xccc
			 0xc9155a09, /// 0xcd0
			 0xb4bd0186, /// 0xcd4
			 0x91a18c90, /// 0xcd8
			 0x1021cb47, /// 0xcdc
			 0x3cfcd33c, /// 0xce0
			 0x529c7e42, /// 0xce4
			 0x6923275b, /// 0xce8
			 0x4cb1969e, /// 0xcec
			 0xe05cb74b, /// 0xcf0
			 0xac1a31cf, /// 0xcf4
			 0x8d4990eb, /// 0xcf8
			 0xf98b782a, /// 0xcfc
			 0x4fceb12d, /// 0xd00
			 0x0c3de432, /// 0xd04
			 0x1ef27e44, /// 0xd08
			 0x8ff6cef4, /// 0xd0c
			 0x3e8749f5, /// 0xd10
			 0x97974c25, /// 0xd14
			 0x8a22ea12, /// 0xd18
			 0x1f26db1c, /// 0xd1c
			 0x521ed5db, /// 0xd20
			 0x79846f45, /// 0xd24
			 0x4a489c1e, /// 0xd28
			 0xdb5b0a95, /// 0xd2c
			 0x8fa37dbd, /// 0xd30
			 0xc080c611, /// 0xd34
			 0x050c5cd5, /// 0xd38
			 0x9726533d, /// 0xd3c
			 0x4fd474c4, /// 0xd40
			 0xd2874c65, /// 0xd44
			 0xa328aa96, /// 0xd48
			 0xf295b623, /// 0xd4c
			 0xf2299eae, /// 0xd50
			 0x4e63ce5b, /// 0xd54
			 0xebb85696, /// 0xd58
			 0x2a9d5062, /// 0xd5c
			 0x3dea6107, /// 0xd60
			 0x5a7c5f0f, /// 0xd64
			 0xad81d4c7, /// 0xd68
			 0x62247432, /// 0xd6c
			 0x60f19a65, /// 0xd70
			 0xfe72123b, /// 0xd74
			 0xe32bbad9, /// 0xd78
			 0x7cadfdb7, /// 0xd7c
			 0x3fff3196, /// 0xd80
			 0x7523e339, /// 0xd84
			 0xa2b5fd9c, /// 0xd88
			 0xe2f1142e, /// 0xd8c
			 0x28c7c115, /// 0xd90
			 0xc998aad3, /// 0xd94
			 0x9953beba, /// 0xd98
			 0xc85eec30, /// 0xd9c
			 0x7e2d9639, /// 0xda0
			 0xfea147e6, /// 0xda4
			 0x9451fada, /// 0xda8
			 0x935010ce, /// 0xdac
			 0xcfeeedaf, /// 0xdb0
			 0x080efc57, /// 0xdb4
			 0x08a05e02, /// 0xdb8
			 0x5fed7128, /// 0xdbc
			 0x20c416b2, /// 0xdc0
			 0x85ba8e67, /// 0xdc4
			 0x53e0c907, /// 0xdc8
			 0x79967ccc, /// 0xdcc
			 0xbc13e1a3, /// 0xdd0
			 0x7f1e7342, /// 0xdd4
			 0x5009a97a, /// 0xdd8
			 0x879f6c11, /// 0xddc
			 0x4697ceeb, /// 0xde0
			 0x9d353789, /// 0xde4
			 0x3bd53422, /// 0xde8
			 0xa5d88111, /// 0xdec
			 0x60786732, /// 0xdf0
			 0x04b076ef, /// 0xdf4
			 0x30aa4724, /// 0xdf8
			 0x04cbd5a2, /// 0xdfc
			 0xacb10c6f, /// 0xe00
			 0x6164d302, /// 0xe04
			 0xd868bd7a, /// 0xe08
			 0xa21cc8d2, /// 0xe0c
			 0xc7224a40, /// 0xe10
			 0xec4156da, /// 0xe14
			 0x96cc4f1f, /// 0xe18
			 0xb0df6222, /// 0xe1c
			 0x4b4e6157, /// 0xe20
			 0x13047665, /// 0xe24
			 0xdc3a9724, /// 0xe28
			 0x433d9a9e, /// 0xe2c
			 0xe3a5dc69, /// 0xe30
			 0x477bedfa, /// 0xe34
			 0x4ea07b35, /// 0xe38
			 0x1c4ae3b3, /// 0xe3c
			 0xbd2d22e0, /// 0xe40
			 0x82a03e49, /// 0xe44
			 0x56d14424, /// 0xe48
			 0x4d16c447, /// 0xe4c
			 0xef48bada, /// 0xe50
			 0x7615e958, /// 0xe54
			 0x44850421, /// 0xe58
			 0x99cacb6d, /// 0xe5c
			 0x7f0781dd, /// 0xe60
			 0x41f7add4, /// 0xe64
			 0x2eb5fe36, /// 0xe68
			 0x7ab80a95, /// 0xe6c
			 0x31df6c06, /// 0xe70
			 0x168f5202, /// 0xe74
			 0x1f4bdeaf, /// 0xe78
			 0x6a578978, /// 0xe7c
			 0x0de2331d, /// 0xe80
			 0xde1bc184, /// 0xe84
			 0x0d9fbeb8, /// 0xe88
			 0xef345731, /// 0xe8c
			 0x2e1948f4, /// 0xe90
			 0xd83c7e51, /// 0xe94
			 0x9a5253da, /// 0xe98
			 0xe8654d5b, /// 0xe9c
			 0x9c7707e7, /// 0xea0
			 0x86f41307, /// 0xea4
			 0x1b96ba31, /// 0xea8
			 0x5dc18e11, /// 0xeac
			 0xf824bf47, /// 0xeb0
			 0x70ace36c, /// 0xeb4
			 0xacf9a234, /// 0xeb8
			 0x089212d6, /// 0xebc
			 0x3c822a2b, /// 0xec0
			 0x044a7a30, /// 0xec4
			 0x172b3e6f, /// 0xec8
			 0xe2c20774, /// 0xecc
			 0x08111ad6, /// 0xed0
			 0xe33965d0, /// 0xed4
			 0x92a35f38, /// 0xed8
			 0x18bf627d, /// 0xedc
			 0x44e6d4b2, /// 0xee0
			 0x7f9cb8d9, /// 0xee4
			 0xb9b0d66a, /// 0xee8
			 0x2d29f225, /// 0xeec
			 0xe92546e5, /// 0xef0
			 0x71961a4d, /// 0xef4
			 0xa9d69c28, /// 0xef8
			 0xc6e69a9d, /// 0xefc
			 0x88af6c47, /// 0xf00
			 0xf12f22c8, /// 0xf04
			 0x133aa6ed, /// 0xf08
			 0x205f57a1, /// 0xf0c
			 0x6454ff25, /// 0xf10
			 0x679c64c9, /// 0xf14
			 0xc9ef7c77, /// 0xf18
			 0x9a1d5a8d, /// 0xf1c
			 0xb0cb9d9c, /// 0xf20
			 0x50a8e8f4, /// 0xf24
			 0xa72f9cb0, /// 0xf28
			 0x64cb8825, /// 0xf2c
			 0x231bfb22, /// 0xf30
			 0x0087b244, /// 0xf34
			 0x1503f65c, /// 0xf38
			 0xa101cc78, /// 0xf3c
			 0xe3132d3d, /// 0xf40
			 0xdeced463, /// 0xf44
			 0x2490d324, /// 0xf48
			 0xacdaaafd, /// 0xf4c
			 0xa3015dcd, /// 0xf50
			 0x98330288, /// 0xf54
			 0xcea5b29d, /// 0xf58
			 0xfd6d03c6, /// 0xf5c
			 0x522394c7, /// 0xf60
			 0xac135982, /// 0xf64
			 0x3f586f79, /// 0xf68
			 0xd1a5f9af, /// 0xf6c
			 0xf736f038, /// 0xf70
			 0x09a965bd, /// 0xf74
			 0x94b7ce19, /// 0xf78
			 0x7f737385, /// 0xf7c
			 0xc64dea2f, /// 0xf80
			 0x56e9094a, /// 0xf84
			 0x38fd8732, /// 0xf88
			 0x51aa36b7, /// 0xf8c
			 0x20adaebe, /// 0xf90
			 0x64e549e5, /// 0xf94
			 0xa1f13e0f, /// 0xf98
			 0x5b4ed815, /// 0xf9c
			 0xdecd8e9d, /// 0xfa0
			 0xe03dc2b5, /// 0xfa4
			 0x1b203e61, /// 0xfa8
			 0xb7220e3d, /// 0xfac
			 0xfe39d028, /// 0xfb0
			 0x2c6ece37, /// 0xfb4
			 0x9cd65ec3, /// 0xfb8
			 0x48f7482a, /// 0xfbc
			 0xcb14230d, /// 0xfc0
			 0x5dc530e9, /// 0xfc4
			 0x8a458e0f, /// 0xfc8
			 0x317a254a, /// 0xfcc
			 0x8debf43b, /// 0xfd0
			 0xba7d3b83, /// 0xfd4
			 0x942ca66a, /// 0xfd8
			 0x5b4fe847, /// 0xfdc
			 0x274ee805, /// 0xfe0
			 0x9ecc1190, /// 0xfe4
			 0x8c9e9f8d, /// 0xfe8
			 0xafc7d7c7, /// 0xfec
			 0x76357007, /// 0xff0
			 0x9a592e6b, /// 0xff4
			 0xa29b39f6, /// 0xff8
			 0x049e6a8e /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x71f33d75, /// 0x0
			 0xe4bfced9, /// 0x4
			 0xda473e2b, /// 0x8
			 0xbe861726, /// 0xc
			 0x3ab3e10e, /// 0x10
			 0x58d185d1, /// 0x14
			 0xb5b913a6, /// 0x18
			 0x41642310, /// 0x1c
			 0xe30ae1a0, /// 0x20
			 0x7d745aa9, /// 0x24
			 0xbb39d7bb, /// 0x28
			 0xb4d3640f, /// 0x2c
			 0x5125b6e1, /// 0x30
			 0x738c9136, /// 0x34
			 0x351b2ae8, /// 0x38
			 0x0eb16ae0, /// 0x3c
			 0x731769d9, /// 0x40
			 0x619cdf1f, /// 0x44
			 0x01cf6634, /// 0x48
			 0xdab89515, /// 0x4c
			 0xd333ae17, /// 0x50
			 0x165ddd0c, /// 0x54
			 0x72f72209, /// 0x58
			 0xd5ed07c1, /// 0x5c
			 0x8cbc7a16, /// 0x60
			 0x5248789f, /// 0x64
			 0x6f5efa96, /// 0x68
			 0xb5aeac87, /// 0x6c
			 0xba628a9b, /// 0x70
			 0xfb8b6393, /// 0x74
			 0x932d672f, /// 0x78
			 0x6f8215c4, /// 0x7c
			 0xf7237f9e, /// 0x80
			 0x49d5a687, /// 0x84
			 0x1d9adf06, /// 0x88
			 0x74de93db, /// 0x8c
			 0xe6446340, /// 0x90
			 0xe2797b9a, /// 0x94
			 0xde6f5e60, /// 0x98
			 0x2fc7c1e1, /// 0x9c
			 0xbb3671d2, /// 0xa0
			 0x17c66475, /// 0xa4
			 0x9469eae9, /// 0xa8
			 0xc284e001, /// 0xac
			 0x43faa23f, /// 0xb0
			 0x71bd4122, /// 0xb4
			 0xc7dcb120, /// 0xb8
			 0xd02457c6, /// 0xbc
			 0x4f4c12f2, /// 0xc0
			 0xb648fe4a, /// 0xc4
			 0x561e31f3, /// 0xc8
			 0xa3044b6b, /// 0xcc
			 0x611b6a0f, /// 0xd0
			 0xc737ffa9, /// 0xd4
			 0x056291df, /// 0xd8
			 0xc123f8cd, /// 0xdc
			 0xbed32234, /// 0xe0
			 0x81e6c3bc, /// 0xe4
			 0xc6d4f790, /// 0xe8
			 0x367aa285, /// 0xec
			 0x9481e354, /// 0xf0
			 0xe4df4170, /// 0xf4
			 0x6f327ac4, /// 0xf8
			 0x37b103fe, /// 0xfc
			 0x9e6f9d3b, /// 0x100
			 0x0fdbdb9e, /// 0x104
			 0x1c0c87ae, /// 0x108
			 0x65e75aff, /// 0x10c
			 0x2fafafe2, /// 0x110
			 0x947aeb44, /// 0x114
			 0x2d8b13f1, /// 0x118
			 0x910574d8, /// 0x11c
			 0x13a582ad, /// 0x120
			 0x2744e67d, /// 0x124
			 0x0c082b62, /// 0x128
			 0xadaac1b2, /// 0x12c
			 0x8b7a826b, /// 0x130
			 0x8903483f, /// 0x134
			 0x5605d6bc, /// 0x138
			 0x164f5cb6, /// 0x13c
			 0xfbb51129, /// 0x140
			 0x23da04ac, /// 0x144
			 0x2dea357c, /// 0x148
			 0x2af06a5f, /// 0x14c
			 0x2ff51cd0, /// 0x150
			 0x700bfd62, /// 0x154
			 0xa94bcbab, /// 0x158
			 0xfde79ca9, /// 0x15c
			 0xd223bb1f, /// 0x160
			 0xb5a61c40, /// 0x164
			 0x0db50c49, /// 0x168
			 0xf198e972, /// 0x16c
			 0xf2e4b51e, /// 0x170
			 0x121ee52e, /// 0x174
			 0xab1a1450, /// 0x178
			 0xfea1442d, /// 0x17c
			 0xb8fae0df, /// 0x180
			 0x4549419e, /// 0x184
			 0x5118f3fd, /// 0x188
			 0x43193cd4, /// 0x18c
			 0xa84e866e, /// 0x190
			 0x33104c4f, /// 0x194
			 0x39bb0bad, /// 0x198
			 0x9744983e, /// 0x19c
			 0x467e91f2, /// 0x1a0
			 0xd59018e8, /// 0x1a4
			 0x831d3c53, /// 0x1a8
			 0xd3d96b2c, /// 0x1ac
			 0x2ce5dcb3, /// 0x1b0
			 0x30a446b9, /// 0x1b4
			 0x11084db8, /// 0x1b8
			 0xb2d3fd7d, /// 0x1bc
			 0x50f543c6, /// 0x1c0
			 0x59dfbea6, /// 0x1c4
			 0x7fd50861, /// 0x1c8
			 0xa5f1f4a7, /// 0x1cc
			 0x667de5f0, /// 0x1d0
			 0x511a68e3, /// 0x1d4
			 0x864ab1df, /// 0x1d8
			 0x4d5560b8, /// 0x1dc
			 0xd4c2e160, /// 0x1e0
			 0x96c9ea9f, /// 0x1e4
			 0x719ab430, /// 0x1e8
			 0x3301aec4, /// 0x1ec
			 0x62049dc8, /// 0x1f0
			 0x5da10217, /// 0x1f4
			 0x542882fb, /// 0x1f8
			 0x80164412, /// 0x1fc
			 0xe695a6d7, /// 0x200
			 0xea36494f, /// 0x204
			 0xddb3300e, /// 0x208
			 0x3314feee, /// 0x20c
			 0x99394b8a, /// 0x210
			 0x3f1b3fef, /// 0x214
			 0x795fa460, /// 0x218
			 0x02b3807a, /// 0x21c
			 0xb7af65d9, /// 0x220
			 0x3a220498, /// 0x224
			 0xca74d833, /// 0x228
			 0xd22d3c19, /// 0x22c
			 0x16dfae49, /// 0x230
			 0x79d00506, /// 0x234
			 0x112a5421, /// 0x238
			 0x3372978f, /// 0x23c
			 0x5f7517e8, /// 0x240
			 0x26fe4b57, /// 0x244
			 0xd81b1f54, /// 0x248
			 0x5fa61086, /// 0x24c
			 0x36d167c0, /// 0x250
			 0xe36d2ef6, /// 0x254
			 0x3134c558, /// 0x258
			 0x439bad60, /// 0x25c
			 0x93313f6d, /// 0x260
			 0x68e5aac7, /// 0x264
			 0x37e09e84, /// 0x268
			 0xd9335e9e, /// 0x26c
			 0x7cfa3fcd, /// 0x270
			 0x25a21d5d, /// 0x274
			 0x990d3d5a, /// 0x278
			 0x9834c8dc, /// 0x27c
			 0x3152c88d, /// 0x280
			 0x6b4fce46, /// 0x284
			 0xb7c303e6, /// 0x288
			 0x878e5dfe, /// 0x28c
			 0xf02095bb, /// 0x290
			 0x2566575b, /// 0x294
			 0xfb08b976, /// 0x298
			 0xe1a301e0, /// 0x29c
			 0x32750084, /// 0x2a0
			 0x031830bd, /// 0x2a4
			 0x1d4cb5d4, /// 0x2a8
			 0xa1cc06a5, /// 0x2ac
			 0x3721cce5, /// 0x2b0
			 0x26121cea, /// 0x2b4
			 0xd5036fd2, /// 0x2b8
			 0x9825ca21, /// 0x2bc
			 0xfe1bbcbe, /// 0x2c0
			 0x2cd08c4f, /// 0x2c4
			 0xec06403d, /// 0x2c8
			 0xbefdacb7, /// 0x2cc
			 0x1c7440fa, /// 0x2d0
			 0x86916144, /// 0x2d4
			 0xad1810b9, /// 0x2d8
			 0xed1e8f52, /// 0x2dc
			 0x5d89704b, /// 0x2e0
			 0xee9a0bd2, /// 0x2e4
			 0x85984a8f, /// 0x2e8
			 0x5d03b38f, /// 0x2ec
			 0xd417aaed, /// 0x2f0
			 0x2d1444b1, /// 0x2f4
			 0xb7f33d4f, /// 0x2f8
			 0xe06cf99f, /// 0x2fc
			 0x18cf42ea, /// 0x300
			 0xe18f9303, /// 0x304
			 0xb8d83c3a, /// 0x308
			 0xb857ba1f, /// 0x30c
			 0xb20022ff, /// 0x310
			 0x0ce91d7f, /// 0x314
			 0x8ea3968f, /// 0x318
			 0x1cb055db, /// 0x31c
			 0x7269505d, /// 0x320
			 0x563b056a, /// 0x324
			 0x6d0911e0, /// 0x328
			 0xaee826a7, /// 0x32c
			 0xc6f160f7, /// 0x330
			 0x8eb8e410, /// 0x334
			 0xf68e5003, /// 0x338
			 0xd99f9052, /// 0x33c
			 0x59a3904c, /// 0x340
			 0x73a5daa1, /// 0x344
			 0x83f0df25, /// 0x348
			 0x5761d721, /// 0x34c
			 0x57df64e8, /// 0x350
			 0x8d26b5b1, /// 0x354
			 0xb2791575, /// 0x358
			 0x5bfcfd3a, /// 0x35c
			 0xde0eaad6, /// 0x360
			 0xf2779a03, /// 0x364
			 0xd6f2ed5a, /// 0x368
			 0x7235946f, /// 0x36c
			 0x8832dc1f, /// 0x370
			 0xa725980f, /// 0x374
			 0x97ab85f0, /// 0x378
			 0x587bc6cc, /// 0x37c
			 0x0a4f09f2, /// 0x380
			 0x9ba9e938, /// 0x384
			 0x341ef853, /// 0x388
			 0x33efe95f, /// 0x38c
			 0x92b5017b, /// 0x390
			 0xe0397182, /// 0x394
			 0x0ac9157f, /// 0x398
			 0xdc641645, /// 0x39c
			 0x746f28f9, /// 0x3a0
			 0xcc563326, /// 0x3a4
			 0xabfc888e, /// 0x3a8
			 0xa083f723, /// 0x3ac
			 0xc4f3f459, /// 0x3b0
			 0xf02ae0b5, /// 0x3b4
			 0x0ae5bf0e, /// 0x3b8
			 0x9c4ab745, /// 0x3bc
			 0x4c866cf4, /// 0x3c0
			 0x6aba1d90, /// 0x3c4
			 0x3d138820, /// 0x3c8
			 0xd74678f1, /// 0x3cc
			 0xe2862aa9, /// 0x3d0
			 0xa554ce97, /// 0x3d4
			 0xbecd7dba, /// 0x3d8
			 0x332f38be, /// 0x3dc
			 0x5093daa4, /// 0x3e0
			 0xa6fe7167, /// 0x3e4
			 0x5e4f123c, /// 0x3e8
			 0x878a687b, /// 0x3ec
			 0xbc8d353c, /// 0x3f0
			 0x94df969e, /// 0x3f4
			 0x24883e90, /// 0x3f8
			 0xe7521d8f, /// 0x3fc
			 0x07bd51ac, /// 0x400
			 0xb405edc6, /// 0x404
			 0x82b7e5dc, /// 0x408
			 0xd9d53343, /// 0x40c
			 0x5da116c7, /// 0x410
			 0xa2a31b80, /// 0x414
			 0xcb8f5f48, /// 0x418
			 0x1d00af58, /// 0x41c
			 0xd9c8e06f, /// 0x420
			 0x744ccb7f, /// 0x424
			 0x1e036909, /// 0x428
			 0x90dbb5ed, /// 0x42c
			 0x18c429dc, /// 0x430
			 0x0ee28991, /// 0x434
			 0xd6fbb1c5, /// 0x438
			 0xf26a50d8, /// 0x43c
			 0xb2013829, /// 0x440
			 0x0a875635, /// 0x444
			 0x31c48f97, /// 0x448
			 0x2985abf1, /// 0x44c
			 0x5d76e019, /// 0x450
			 0x02ef7974, /// 0x454
			 0x2d3332bb, /// 0x458
			 0x1e11601e, /// 0x45c
			 0xa2a38101, /// 0x460
			 0x06c96374, /// 0x464
			 0x64ef723c, /// 0x468
			 0x41ce94fb, /// 0x46c
			 0x50f74ecb, /// 0x470
			 0x7ec35820, /// 0x474
			 0x03060ac5, /// 0x478
			 0x780d9646, /// 0x47c
			 0x7e760dee, /// 0x480
			 0xe180b6e0, /// 0x484
			 0xaa179d87, /// 0x488
			 0xdfb142f5, /// 0x48c
			 0xbfac46bb, /// 0x490
			 0xac1e0fa0, /// 0x494
			 0x9fdce4f0, /// 0x498
			 0x3275da64, /// 0x49c
			 0xd651bd6a, /// 0x4a0
			 0x23bdfd49, /// 0x4a4
			 0xf109940e, /// 0x4a8
			 0x5108dcdf, /// 0x4ac
			 0x1f543dfb, /// 0x4b0
			 0xb01460c6, /// 0x4b4
			 0x8e992648, /// 0x4b8
			 0xa9aad6fd, /// 0x4bc
			 0xd524f348, /// 0x4c0
			 0xa0882575, /// 0x4c4
			 0xba05dba7, /// 0x4c8
			 0x85641f3d, /// 0x4cc
			 0x24cb3a1e, /// 0x4d0
			 0x125d0b14, /// 0x4d4
			 0x044b63a7, /// 0x4d8
			 0xd343a794, /// 0x4dc
			 0x3db7d59d, /// 0x4e0
			 0x6abb8583, /// 0x4e4
			 0x400f7f80, /// 0x4e8
			 0x5cc5c2dc, /// 0x4ec
			 0xbb3c364b, /// 0x4f0
			 0x59d0f672, /// 0x4f4
			 0x90a193f1, /// 0x4f8
			 0x70dd5563, /// 0x4fc
			 0x269da719, /// 0x500
			 0x53debe89, /// 0x504
			 0xe2d5623f, /// 0x508
			 0x20dc0fc1, /// 0x50c
			 0x15911c93, /// 0x510
			 0x1d9268df, /// 0x514
			 0xae8ab041, /// 0x518
			 0x2d923118, /// 0x51c
			 0xabedcb81, /// 0x520
			 0x259c7569, /// 0x524
			 0x5100d515, /// 0x528
			 0x0df0c7b7, /// 0x52c
			 0xc5134201, /// 0x530
			 0x1102ccce, /// 0x534
			 0xeb8b95bb, /// 0x538
			 0xa31937d8, /// 0x53c
			 0xb7801aff, /// 0x540
			 0x4b84356a, /// 0x544
			 0xdcd11a29, /// 0x548
			 0x31cc1e5d, /// 0x54c
			 0x4e5e6e37, /// 0x550
			 0x26bba2d7, /// 0x554
			 0xba83c6a4, /// 0x558
			 0x292ddfd0, /// 0x55c
			 0x3a63c075, /// 0x560
			 0x205b68c2, /// 0x564
			 0x855fcea9, /// 0x568
			 0x30611105, /// 0x56c
			 0x8b743416, /// 0x570
			 0xc45f5e84, /// 0x574
			 0x6022eb6f, /// 0x578
			 0x3878cee9, /// 0x57c
			 0xca9f0cc4, /// 0x580
			 0x568e3700, /// 0x584
			 0x03a6683d, /// 0x588
			 0x8fb9273b, /// 0x58c
			 0xc8e89713, /// 0x590
			 0xe0c6df10, /// 0x594
			 0x63c32a63, /// 0x598
			 0x79fbdb5c, /// 0x59c
			 0x9a148e0a, /// 0x5a0
			 0xb6dae1fe, /// 0x5a4
			 0xe77f0327, /// 0x5a8
			 0x97c91ad5, /// 0x5ac
			 0x89f4f01c, /// 0x5b0
			 0x034b4734, /// 0x5b4
			 0xe28d502b, /// 0x5b8
			 0x2262d6a9, /// 0x5bc
			 0x7842c8f6, /// 0x5c0
			 0x16eb4214, /// 0x5c4
			 0x06cb7839, /// 0x5c8
			 0xec0254ab, /// 0x5cc
			 0x673bd5b7, /// 0x5d0
			 0xe10da2d2, /// 0x5d4
			 0x7cbf2ba9, /// 0x5d8
			 0x1d2b47e5, /// 0x5dc
			 0x36f40219, /// 0x5e0
			 0xa526791b, /// 0x5e4
			 0xfb49c548, /// 0x5e8
			 0xeaaa042d, /// 0x5ec
			 0xb2528b8a, /// 0x5f0
			 0xcbc3696e, /// 0x5f4
			 0x9d89f09c, /// 0x5f8
			 0xe6bdba34, /// 0x5fc
			 0xf72706dc, /// 0x600
			 0x2086c001, /// 0x604
			 0x82f0353f, /// 0x608
			 0xf96a2dec, /// 0x60c
			 0x5344852e, /// 0x610
			 0x2f4c5578, /// 0x614
			 0x8e7c13b9, /// 0x618
			 0xd9043790, /// 0x61c
			 0xc5f9b3a5, /// 0x620
			 0x43a476c0, /// 0x624
			 0xf112e861, /// 0x628
			 0x3ac14125, /// 0x62c
			 0xba45345c, /// 0x630
			 0xc0e5bc47, /// 0x634
			 0x0af14b90, /// 0x638
			 0xda04cb53, /// 0x63c
			 0xbce00256, /// 0x640
			 0x25ffb1d5, /// 0x644
			 0x2375544d, /// 0x648
			 0x375bcc51, /// 0x64c
			 0x5700014b, /// 0x650
			 0x48a49598, /// 0x654
			 0xd608b8f2, /// 0x658
			 0x6fe6b4c5, /// 0x65c
			 0x0770e3b7, /// 0x660
			 0x38a1a06f, /// 0x664
			 0x9da6cd8e, /// 0x668
			 0x91859ae5, /// 0x66c
			 0x6e88a9e2, /// 0x670
			 0x8b0c6f02, /// 0x674
			 0x13cf80dc, /// 0x678
			 0x4ab6a7de, /// 0x67c
			 0xac9bd6cb, /// 0x680
			 0x3e9ec9df, /// 0x684
			 0xeb2e44a3, /// 0x688
			 0x67ec54a5, /// 0x68c
			 0x1fc6d12f, /// 0x690
			 0xcf328970, /// 0x694
			 0x54ccdafa, /// 0x698
			 0x37353cf8, /// 0x69c
			 0x3336e0f2, /// 0x6a0
			 0x82c164a8, /// 0x6a4
			 0xe37f9470, /// 0x6a8
			 0xa18909b6, /// 0x6ac
			 0x38680170, /// 0x6b0
			 0x1353cb76, /// 0x6b4
			 0x5796912d, /// 0x6b8
			 0xd81fb088, /// 0x6bc
			 0x0a53df8e, /// 0x6c0
			 0x16cbb7d1, /// 0x6c4
			 0x36928d47, /// 0x6c8
			 0xa5de195a, /// 0x6cc
			 0x9b523758, /// 0x6d0
			 0x47a91a2a, /// 0x6d4
			 0x42782f2c, /// 0x6d8
			 0x949c9839, /// 0x6dc
			 0x0179d2f6, /// 0x6e0
			 0x2d6a6fdf, /// 0x6e4
			 0x069ae381, /// 0x6e8
			 0xd1ad8636, /// 0x6ec
			 0x1f0069ba, /// 0x6f0
			 0xfab7879d, /// 0x6f4
			 0xf5f4154e, /// 0x6f8
			 0xca95959d, /// 0x6fc
			 0xda2b186c, /// 0x700
			 0xbb076348, /// 0x704
			 0x41708658, /// 0x708
			 0x4d099831, /// 0x70c
			 0x7a6f5bf7, /// 0x710
			 0xde16a640, /// 0x714
			 0x2f5f17f0, /// 0x718
			 0x744f715d, /// 0x71c
			 0x136019f2, /// 0x720
			 0xf42f0219, /// 0x724
			 0x35f8983c, /// 0x728
			 0x56eba7e2, /// 0x72c
			 0x9961c455, /// 0x730
			 0xf1cd7e6d, /// 0x734
			 0xc43afa1a, /// 0x738
			 0x453dc9a5, /// 0x73c
			 0xb8a598cf, /// 0x740
			 0x9aba0447, /// 0x744
			 0xfb53cc96, /// 0x748
			 0x3be5efa3, /// 0x74c
			 0x5f9eb1c3, /// 0x750
			 0xd7922a11, /// 0x754
			 0xaa04b921, /// 0x758
			 0xb03ebf4e, /// 0x75c
			 0x89d70f10, /// 0x760
			 0x796292d6, /// 0x764
			 0xf8d58eee, /// 0x768
			 0xd50591f6, /// 0x76c
			 0xe39eec7a, /// 0x770
			 0x5b4236ce, /// 0x774
			 0x5cc9c1ed, /// 0x778
			 0xc1cd9b8d, /// 0x77c
			 0xbaba9a11, /// 0x780
			 0x0a52f5fe, /// 0x784
			 0x4aeb0bed, /// 0x788
			 0x61c17149, /// 0x78c
			 0x6204dd81, /// 0x790
			 0x5fd58614, /// 0x794
			 0x12050e24, /// 0x798
			 0x8437dc48, /// 0x79c
			 0xf7526dae, /// 0x7a0
			 0x6878c82a, /// 0x7a4
			 0x48d29679, /// 0x7a8
			 0x18d231e8, /// 0x7ac
			 0xeb150bec, /// 0x7b0
			 0x6b853f36, /// 0x7b4
			 0xbc2dfe7d, /// 0x7b8
			 0xc4dd5e47, /// 0x7bc
			 0x8077cfb1, /// 0x7c0
			 0x15a8e2c8, /// 0x7c4
			 0x8dbd2ca8, /// 0x7c8
			 0xb04297e8, /// 0x7cc
			 0x09533e44, /// 0x7d0
			 0x26ec05bf, /// 0x7d4
			 0x738e2124, /// 0x7d8
			 0x1492ffd6, /// 0x7dc
			 0x74c38593, /// 0x7e0
			 0xd368fd36, /// 0x7e4
			 0x84cfe9b3, /// 0x7e8
			 0x6a64fb28, /// 0x7ec
			 0xa0f24ab1, /// 0x7f0
			 0xb0ec7e6a, /// 0x7f4
			 0x0c372101, /// 0x7f8
			 0x68fd8d92, /// 0x7fc
			 0x4552804b, /// 0x800
			 0x85b502a3, /// 0x804
			 0xb87628e2, /// 0x808
			 0x154bcfdc, /// 0x80c
			 0xc8d83957, /// 0x810
			 0x1bab4040, /// 0x814
			 0x8351b77b, /// 0x818
			 0x1cd180f5, /// 0x81c
			 0x98d313bf, /// 0x820
			 0x31498a8a, /// 0x824
			 0xef1a2530, /// 0x828
			 0xf678ec67, /// 0x82c
			 0x1b456ff2, /// 0x830
			 0xf4e828ec, /// 0x834
			 0x1ce76a98, /// 0x838
			 0x9d7c6af8, /// 0x83c
			 0x2f6267d1, /// 0x840
			 0x723bb5a4, /// 0x844
			 0x75128e81, /// 0x848
			 0xf164807e, /// 0x84c
			 0x02ede1b4, /// 0x850
			 0xebb9b136, /// 0x854
			 0xa96e4e36, /// 0x858
			 0xc41facab, /// 0x85c
			 0x32cbd529, /// 0x860
			 0x7601232c, /// 0x864
			 0xc6ee1624, /// 0x868
			 0x9682eabb, /// 0x86c
			 0x9565280e, /// 0x870
			 0xaaf6503d, /// 0x874
			 0xf368fcc0, /// 0x878
			 0xe225d70f, /// 0x87c
			 0xa5db7048, /// 0x880
			 0xb6385f7d, /// 0x884
			 0xf6199087, /// 0x888
			 0x22f47757, /// 0x88c
			 0x1d99db65, /// 0x890
			 0x52fe8ca7, /// 0x894
			 0xf661ac78, /// 0x898
			 0x7e6b2433, /// 0x89c
			 0x31a590dc, /// 0x8a0
			 0x8aee089b, /// 0x8a4
			 0xa22f22cc, /// 0x8a8
			 0x7cf56604, /// 0x8ac
			 0x4db32e29, /// 0x8b0
			 0xcfc62977, /// 0x8b4
			 0xb1ea39fe, /// 0x8b8
			 0x231b71c2, /// 0x8bc
			 0x8a58d0d5, /// 0x8c0
			 0xea53b40f, /// 0x8c4
			 0xd2ca3a31, /// 0x8c8
			 0x26956450, /// 0x8cc
			 0x8922424f, /// 0x8d0
			 0x271cc47a, /// 0x8d4
			 0xdbd834a6, /// 0x8d8
			 0x47b7f7d8, /// 0x8dc
			 0x418e280d, /// 0x8e0
			 0xf17fc790, /// 0x8e4
			 0xcb294471, /// 0x8e8
			 0x14c6a174, /// 0x8ec
			 0x32b13cb3, /// 0x8f0
			 0xab38bed0, /// 0x8f4
			 0x0ec3138a, /// 0x8f8
			 0xc82ac3ca, /// 0x8fc
			 0x58b95e1c, /// 0x900
			 0xa7ddf709, /// 0x904
			 0xe9d740c5, /// 0x908
			 0x9e8db975, /// 0x90c
			 0xa60a7e4a, /// 0x910
			 0x0775a661, /// 0x914
			 0xe7d65a66, /// 0x918
			 0xca3b18c1, /// 0x91c
			 0x42144fff, /// 0x920
			 0x21818640, /// 0x924
			 0x93c95c5a, /// 0x928
			 0x9d2980ae, /// 0x92c
			 0x262f7810, /// 0x930
			 0xf5bd19be, /// 0x934
			 0x6eaff262, /// 0x938
			 0xe5191a56, /// 0x93c
			 0x4d3fd0e2, /// 0x940
			 0xc24da6a3, /// 0x944
			 0x69ce3009, /// 0x948
			 0x464c3f28, /// 0x94c
			 0x8fb75a67, /// 0x950
			 0x9a1595a1, /// 0x954
			 0xaaa065c4, /// 0x958
			 0xcc5d8e71, /// 0x95c
			 0x24d544a5, /// 0x960
			 0x628f576f, /// 0x964
			 0x51dd254c, /// 0x968
			 0x579d6720, /// 0x96c
			 0xd60967d6, /// 0x970
			 0xa2a69e5d, /// 0x974
			 0x8e9aa9fb, /// 0x978
			 0x5cf33374, /// 0x97c
			 0x9da153f6, /// 0x980
			 0xf58d7b20, /// 0x984
			 0x4234f8a4, /// 0x988
			 0xce1053e1, /// 0x98c
			 0x3c6fd64c, /// 0x990
			 0xaffca907, /// 0x994
			 0x0b829164, /// 0x998
			 0x46fd5311, /// 0x99c
			 0x382c1eee, /// 0x9a0
			 0x9cf45c0c, /// 0x9a4
			 0xafe065dc, /// 0x9a8
			 0x18cb6cc2, /// 0x9ac
			 0x18e268ee, /// 0x9b0
			 0x7f7b8f00, /// 0x9b4
			 0x13cc8ada, /// 0x9b8
			 0x6821b86e, /// 0x9bc
			 0xce51c121, /// 0x9c0
			 0xc457d1c6, /// 0x9c4
			 0x03180184, /// 0x9c8
			 0x32755b4c, /// 0x9cc
			 0x14f9dfb9, /// 0x9d0
			 0x16c1ae5f, /// 0x9d4
			 0x2c521309, /// 0x9d8
			 0x34e77076, /// 0x9dc
			 0x448a69ed, /// 0x9e0
			 0xf8d2793a, /// 0x9e4
			 0x1972f1db, /// 0x9e8
			 0x28318d41, /// 0x9ec
			 0x2570280d, /// 0x9f0
			 0x8427ddb4, /// 0x9f4
			 0x84bdd9ec, /// 0x9f8
			 0x2395d320, /// 0x9fc
			 0x780cd609, /// 0xa00
			 0xe1a1ca32, /// 0xa04
			 0xd1015bf2, /// 0xa08
			 0x3a54426a, /// 0xa0c
			 0xca0bcbd4, /// 0xa10
			 0xadf4f30e, /// 0xa14
			 0x3dd3b465, /// 0xa18
			 0x32e48379, /// 0xa1c
			 0x08fe1bd4, /// 0xa20
			 0xba30dc68, /// 0xa24
			 0x4d565f24, /// 0xa28
			 0xf2484285, /// 0xa2c
			 0x51ef7a9c, /// 0xa30
			 0xcdf97657, /// 0xa34
			 0x9c262f05, /// 0xa38
			 0x3c776960, /// 0xa3c
			 0x1378cdba, /// 0xa40
			 0x51b00a0b, /// 0xa44
			 0xa6fb94c9, /// 0xa48
			 0x3f4c756b, /// 0xa4c
			 0x80b32bc6, /// 0xa50
			 0xb34eda82, /// 0xa54
			 0x3d2a832e, /// 0xa58
			 0x93bf5426, /// 0xa5c
			 0xeea243ce, /// 0xa60
			 0x9a426442, /// 0xa64
			 0xb057371c, /// 0xa68
			 0xa43ada4b, /// 0xa6c
			 0x990b8b82, /// 0xa70
			 0x42935f07, /// 0xa74
			 0xd9ea6e14, /// 0xa78
			 0x4ae01781, /// 0xa7c
			 0x2f44da29, /// 0xa80
			 0xce100aa3, /// 0xa84
			 0xc40242df, /// 0xa88
			 0xdcaf0af5, /// 0xa8c
			 0x676918be, /// 0xa90
			 0xdd5199a6, /// 0xa94
			 0xc5ed6138, /// 0xa98
			 0xe84b5490, /// 0xa9c
			 0xab0fbd4d, /// 0xaa0
			 0xe35eb814, /// 0xaa4
			 0x1aa983b3, /// 0xaa8
			 0xbca706bc, /// 0xaac
			 0x16a425fe, /// 0xab0
			 0x58527f7d, /// 0xab4
			 0x76f65397, /// 0xab8
			 0x4714b5c9, /// 0xabc
			 0x45b3c065, /// 0xac0
			 0x853acdfa, /// 0xac4
			 0x24611e33, /// 0xac8
			 0x86c0e48a, /// 0xacc
			 0xae3e7321, /// 0xad0
			 0x7a6b28ed, /// 0xad4
			 0x1fccf981, /// 0xad8
			 0x0f784553, /// 0xadc
			 0x5bf82c3e, /// 0xae0
			 0x4ead5be1, /// 0xae4
			 0x5ce56ecb, /// 0xae8
			 0x2c4b60d9, /// 0xaec
			 0x8cf8ee6f, /// 0xaf0
			 0x7ad9c5dc, /// 0xaf4
			 0x867ca9fa, /// 0xaf8
			 0x2f5f8e55, /// 0xafc
			 0xe4a1304b, /// 0xb00
			 0x69042c29, /// 0xb04
			 0x292ce4a7, /// 0xb08
			 0xcdc1122a, /// 0xb0c
			 0xb0aa36e1, /// 0xb10
			 0x257a8df8, /// 0xb14
			 0xd9ff5132, /// 0xb18
			 0xb1ddca78, /// 0xb1c
			 0x0b82c75c, /// 0xb20
			 0x824e26b1, /// 0xb24
			 0x426ec974, /// 0xb28
			 0x8a6be2d0, /// 0xb2c
			 0xf027e6dc, /// 0xb30
			 0x409a35d1, /// 0xb34
			 0x3fd268d8, /// 0xb38
			 0x6ee3a3e6, /// 0xb3c
			 0x47318f4c, /// 0xb40
			 0x05d24371, /// 0xb44
			 0x9ead6560, /// 0xb48
			 0xcb1b14ec, /// 0xb4c
			 0xae35dd3e, /// 0xb50
			 0x6893f98b, /// 0xb54
			 0x97e48fcd, /// 0xb58
			 0x3ec5b822, /// 0xb5c
			 0xeaa3d139, /// 0xb60
			 0x56103e8a, /// 0xb64
			 0x64248e09, /// 0xb68
			 0xdc0b3196, /// 0xb6c
			 0x670fac19, /// 0xb70
			 0xfc56b53d, /// 0xb74
			 0xfeee3e52, /// 0xb78
			 0xfca0739b, /// 0xb7c
			 0x8ce0236c, /// 0xb80
			 0x8a6416e2, /// 0xb84
			 0x4a60d7d5, /// 0xb88
			 0xa9f5230a, /// 0xb8c
			 0xca87d5ba, /// 0xb90
			 0x978f5c90, /// 0xb94
			 0x0b670ede, /// 0xb98
			 0x34614d34, /// 0xb9c
			 0xf3bf3a68, /// 0xba0
			 0x1fa63c98, /// 0xba4
			 0x52c56d59, /// 0xba8
			 0x9f79bc13, /// 0xbac
			 0x98bd0dc7, /// 0xbb0
			 0xada3c22f, /// 0xbb4
			 0x88c82f43, /// 0xbb8
			 0x3579fe9b, /// 0xbbc
			 0x5edcc98a, /// 0xbc0
			 0xfbad9f25, /// 0xbc4
			 0x25f375fd, /// 0xbc8
			 0x9193a765, /// 0xbcc
			 0x4ec7e6a1, /// 0xbd0
			 0xa28899ec, /// 0xbd4
			 0x52d53438, /// 0xbd8
			 0x27abb036, /// 0xbdc
			 0x0d8f3a50, /// 0xbe0
			 0xfd1fe822, /// 0xbe4
			 0xda466dbe, /// 0xbe8
			 0x065fa5c3, /// 0xbec
			 0x2d23e181, /// 0xbf0
			 0x3701513b, /// 0xbf4
			 0xcebeea2e, /// 0xbf8
			 0xd7d3c429, /// 0xbfc
			 0xa98678e9, /// 0xc00
			 0x85d3c63a, /// 0xc04
			 0xd97f338c, /// 0xc08
			 0x14344f22, /// 0xc0c
			 0x2dd4da74, /// 0xc10
			 0x1ad205e3, /// 0xc14
			 0xf5902c8f, /// 0xc18
			 0xd29ab6a7, /// 0xc1c
			 0x3050a813, /// 0xc20
			 0x9a6ed720, /// 0xc24
			 0x40494c89, /// 0xc28
			 0xd4908478, /// 0xc2c
			 0x21a95ae9, /// 0xc30
			 0xc3b42444, /// 0xc34
			 0xf9cbb321, /// 0xc38
			 0xd215540a, /// 0xc3c
			 0xf6a1f520, /// 0xc40
			 0x7f3d2fa9, /// 0xc44
			 0xb257e0d6, /// 0xc48
			 0x80d904e4, /// 0xc4c
			 0x3f0ea889, /// 0xc50
			 0xecc5a3fd, /// 0xc54
			 0x95065000, /// 0xc58
			 0xe5ae84be, /// 0xc5c
			 0x3f83e816, /// 0xc60
			 0x9abe2768, /// 0xc64
			 0x12c92e84, /// 0xc68
			 0x5f1e64e4, /// 0xc6c
			 0xb6b1fdd6, /// 0xc70
			 0xe188c52f, /// 0xc74
			 0x37a0ea01, /// 0xc78
			 0x1c740897, /// 0xc7c
			 0x650b169a, /// 0xc80
			 0xf93352f7, /// 0xc84
			 0xe5a3ef43, /// 0xc88
			 0x4cbe6954, /// 0xc8c
			 0xdb625646, /// 0xc90
			 0xc58337b7, /// 0xc94
			 0xcb02d9ef, /// 0xc98
			 0x9e8e8232, /// 0xc9c
			 0x68af19c4, /// 0xca0
			 0xa1f55e1e, /// 0xca4
			 0xeb60bea3, /// 0xca8
			 0x8056aee0, /// 0xcac
			 0xadae86c9, /// 0xcb0
			 0xfbf49d28, /// 0xcb4
			 0x54bb207e, /// 0xcb8
			 0x40c47a8a, /// 0xcbc
			 0x48491c2c, /// 0xcc0
			 0xb3a693ca, /// 0xcc4
			 0xa3972346, /// 0xcc8
			 0x44d3543d, /// 0xccc
			 0x71b313a2, /// 0xcd0
			 0x1ab33b74, /// 0xcd4
			 0x70ec521e, /// 0xcd8
			 0x41cc3bcd, /// 0xcdc
			 0x737a1a87, /// 0xce0
			 0x5839f8d8, /// 0xce4
			 0x7b54c780, /// 0xce8
			 0xd0bb1aa2, /// 0xcec
			 0x3b55080c, /// 0xcf0
			 0xdce781db, /// 0xcf4
			 0x6a5c06a7, /// 0xcf8
			 0xf9bcc350, /// 0xcfc
			 0xa13d93a4, /// 0xd00
			 0x56f023d7, /// 0xd04
			 0x29924750, /// 0xd08
			 0x4ed205e8, /// 0xd0c
			 0xc11867de, /// 0xd10
			 0xcf9c8e57, /// 0xd14
			 0x5cf2aa30, /// 0xd18
			 0xda7349b1, /// 0xd1c
			 0xee468334, /// 0xd20
			 0xd3ee29dd, /// 0xd24
			 0xaf05fcac, /// 0xd28
			 0xde7679ec, /// 0xd2c
			 0x1237d061, /// 0xd30
			 0x86b71783, /// 0xd34
			 0xbefa2a4d, /// 0xd38
			 0x3f6e05a9, /// 0xd3c
			 0xecf889b9, /// 0xd40
			 0x867b6f5f, /// 0xd44
			 0xa8828901, /// 0xd48
			 0x9533824c, /// 0xd4c
			 0x2f16014a, /// 0xd50
			 0x20768566, /// 0xd54
			 0x5259478e, /// 0xd58
			 0xf8a0d0d6, /// 0xd5c
			 0x56b74a9b, /// 0xd60
			 0xdaf63281, /// 0xd64
			 0x43781dcb, /// 0xd68
			 0x95b6fa54, /// 0xd6c
			 0xb86d6918, /// 0xd70
			 0x1187201c, /// 0xd74
			 0xb8ceba48, /// 0xd78
			 0x573f887a, /// 0xd7c
			 0xb12be5fe, /// 0xd80
			 0x6ce1348e, /// 0xd84
			 0x60c529c8, /// 0xd88
			 0x1daa83d6, /// 0xd8c
			 0xce881b69, /// 0xd90
			 0xd4c4c520, /// 0xd94
			 0x50745cba, /// 0xd98
			 0x6b119296, /// 0xd9c
			 0x27c56df6, /// 0xda0
			 0x78ae429c, /// 0xda4
			 0x3b1eb70e, /// 0xda8
			 0x50a95f75, /// 0xdac
			 0xff7273fd, /// 0xdb0
			 0xa3ad61cc, /// 0xdb4
			 0x820d30cb, /// 0xdb8
			 0xb210b04a, /// 0xdbc
			 0x667f7e75, /// 0xdc0
			 0xb08efdc3, /// 0xdc4
			 0x689e4b0d, /// 0xdc8
			 0x69517d50, /// 0xdcc
			 0xc223c02d, /// 0xdd0
			 0xd44cc9f9, /// 0xdd4
			 0xe7bb6c2d, /// 0xdd8
			 0x85f39218, /// 0xddc
			 0x02d5bd1f, /// 0xde0
			 0xb47ec656, /// 0xde4
			 0x9b0d7668, /// 0xde8
			 0xc0114e88, /// 0xdec
			 0xba993ec0, /// 0xdf0
			 0x1900c223, /// 0xdf4
			 0xa29f79df, /// 0xdf8
			 0x4d836587, /// 0xdfc
			 0xfc30fba3, /// 0xe00
			 0xd898db2a, /// 0xe04
			 0xf2a8012b, /// 0xe08
			 0x9dd82efd, /// 0xe0c
			 0x278f992a, /// 0xe10
			 0xad53faa5, /// 0xe14
			 0x9350c6e3, /// 0xe18
			 0x95a79208, /// 0xe1c
			 0xc7ac3fb3, /// 0xe20
			 0x633d5a3d, /// 0xe24
			 0xa594d223, /// 0xe28
			 0x4e261432, /// 0xe2c
			 0x9a14b04e, /// 0xe30
			 0xba79594f, /// 0xe34
			 0xab43df7f, /// 0xe38
			 0xf724359a, /// 0xe3c
			 0xfaafb6d6, /// 0xe40
			 0x955ddb89, /// 0xe44
			 0x92907d70, /// 0xe48
			 0xca1f01e5, /// 0xe4c
			 0xbc702886, /// 0xe50
			 0xedb72e30, /// 0xe54
			 0xfc5342f1, /// 0xe58
			 0x50140836, /// 0xe5c
			 0xd2083768, /// 0xe60
			 0x54dff796, /// 0xe64
			 0xb39ef7f8, /// 0xe68
			 0x9f0ef173, /// 0xe6c
			 0xf31455ee, /// 0xe70
			 0x693e7213, /// 0xe74
			 0xdece994e, /// 0xe78
			 0x9b4ca383, /// 0xe7c
			 0xf716b843, /// 0xe80
			 0xe5e1e71d, /// 0xe84
			 0x92a6f5bf, /// 0xe88
			 0xf8864b76, /// 0xe8c
			 0x5bfb0449, /// 0xe90
			 0x82d341cc, /// 0xe94
			 0xa8857628, /// 0xe98
			 0xd1bc7d8c, /// 0xe9c
			 0x4673536c, /// 0xea0
			 0x2ed6a28b, /// 0xea4
			 0x6b69e28d, /// 0xea8
			 0xe17e5ed9, /// 0xeac
			 0x54a73793, /// 0xeb0
			 0x45887bd1, /// 0xeb4
			 0xfe012a7c, /// 0xeb8
			 0xe29a767d, /// 0xebc
			 0x436925d0, /// 0xec0
			 0x451b4a3d, /// 0xec4
			 0xe28b24a9, /// 0xec8
			 0xc94a5f8b, /// 0xecc
			 0xb24b8453, /// 0xed0
			 0xf8140fda, /// 0xed4
			 0x1f335dd6, /// 0xed8
			 0x393022c5, /// 0xedc
			 0x08fc5d2d, /// 0xee0
			 0xa7e87629, /// 0xee4
			 0x0ef38f7a, /// 0xee8
			 0xe1f6f538, /// 0xeec
			 0xc19e13c4, /// 0xef0
			 0x71299b3f, /// 0xef4
			 0x0b4a2192, /// 0xef8
			 0x16d22960, /// 0xefc
			 0x134a3aae, /// 0xf00
			 0x284fa3ef, /// 0xf04
			 0xe247b7f7, /// 0xf08
			 0xaccdc428, /// 0xf0c
			 0x878d498a, /// 0xf10
			 0x46e98988, /// 0xf14
			 0x52195e96, /// 0xf18
			 0xa5f7a7fc, /// 0xf1c
			 0xd7ea17ba, /// 0xf20
			 0x49f9166e, /// 0xf24
			 0xfa7a6cf1, /// 0xf28
			 0xa47c5367, /// 0xf2c
			 0x38ceb77c, /// 0xf30
			 0xfab9633f, /// 0xf34
			 0x73368a8f, /// 0xf38
			 0x3a3fba96, /// 0xf3c
			 0x6f9332f2, /// 0xf40
			 0xcfd93be8, /// 0xf44
			 0x4af47057, /// 0xf48
			 0x5cd6dbed, /// 0xf4c
			 0x381358c9, /// 0xf50
			 0xe14eba71, /// 0xf54
			 0x75ada0fe, /// 0xf58
			 0x1b6c7cc1, /// 0xf5c
			 0xd4e1f35b, /// 0xf60
			 0x24fc38b3, /// 0xf64
			 0x49a7b421, /// 0xf68
			 0x3b6d42e9, /// 0xf6c
			 0xc8d9f64c, /// 0xf70
			 0x4f4c851a, /// 0xf74
			 0x4e58225a, /// 0xf78
			 0xe0b22d59, /// 0xf7c
			 0x35395f0a, /// 0xf80
			 0xe954cbf4, /// 0xf84
			 0xe6fded6f, /// 0xf88
			 0xf4a4de28, /// 0xf8c
			 0x9d03af1e, /// 0xf90
			 0x91a2e82d, /// 0xf94
			 0xe1842f4e, /// 0xf98
			 0xe64b8cdd, /// 0xf9c
			 0x7f8d1f05, /// 0xfa0
			 0xa8b4a3a2, /// 0xfa4
			 0x464831b9, /// 0xfa8
			 0xe002abc4, /// 0xfac
			 0xb60bea2d, /// 0xfb0
			 0x196dfc93, /// 0xfb4
			 0xb7c5fb46, /// 0xfb8
			 0xb2a57deb, /// 0xfbc
			 0x3537ee7c, /// 0xfc0
			 0xe6b68f2c, /// 0xfc4
			 0x896833c2, /// 0xfc8
			 0x03469204, /// 0xfcc
			 0x7b03f787, /// 0xfd0
			 0x39b36cd3, /// 0xfd4
			 0xa4cd167a, /// 0xfd8
			 0xafb84d67, /// 0xfdc
			 0x9ae1c288, /// 0xfe0
			 0x5ac8c48c, /// 0xfe4
			 0x82f3fa49, /// 0xfe8
			 0xd5fd0216, /// 0xfec
			 0x893712d0, /// 0xff0
			 0xdffecd71, /// 0xff4
			 0x20c12352, /// 0xff8
			 0xf90764c2 /// last
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
			 0x0000043c,
			 0x00000400,
			 0x000003a8,
			 0x00000208,
			 0x00000590,
			 0x0000011c,
			 0x0000059c,
			 0x000007b8,

			 /// vpu register f2
			 0x41a00000,
			 0x41900000,
			 0x40800000,
			 0x41f80000,
			 0x41880000,
			 0x41e80000,
			 0x42000000,
			 0x41400000,

			 /// vpu register f3
			 0x41d80000,
			 0x41500000,
			 0x41700000,
			 0x41500000,
			 0x40e00000,
			 0x41c00000,
			 0x41900000,
			 0x41800000,

			 /// vpu register f4
			 0x41300000,
			 0x40c00000,
			 0x41d80000,
			 0x41700000,
			 0x42000000,
			 0x41a80000,
			 0x41300000,
			 0x41000000,

			 /// vpu register f5
			 0x41d00000,
			 0x41700000,
			 0x41200000,
			 0x40000000,
			 0x41b00000,
			 0x41d80000,
			 0x41000000,
			 0x41a80000,

			 /// vpu register f6
			 0x41e80000,
			 0x41b80000,
			 0x41200000,
			 0x40c00000,
			 0x41c00000,
			 0x3f800000,
			 0x41000000,
			 0x40a00000,

			 /// vpu register f7
			 0x41000000,
			 0x41100000,
			 0x41c80000,
			 0x41800000,
			 0x42000000,
			 0x41800000,
			 0x41f80000,
			 0x41900000,

			 /// vpu register f8
			 0x3f800000,
			 0x41b00000,
			 0x41400000,
			 0x41100000,
			 0x41100000,
			 0x41a00000,
			 0x41f00000,
			 0x40a00000,

			 /// vpu register f9
			 0x41980000,
			 0x41c80000,
			 0x41e00000,
			 0x41d80000,
			 0x41c80000,
			 0x41400000,
			 0x42000000,
			 0x40800000,

			 /// vpu register f10
			 0x41b80000,
			 0x41900000,
			 0x41d80000,
			 0x41c80000,
			 0x40e00000,
			 0x41600000,
			 0x41100000,
			 0x41000000,

			 /// vpu register f11
			 0x41d80000,
			 0x41e00000,
			 0x40c00000,
			 0x41f80000,
			 0x41f80000,
			 0x41980000,
			 0x42000000,
			 0x40a00000,

			 /// vpu register f12
			 0x41d80000,
			 0x3f800000,
			 0x40000000,
			 0x40c00000,
			 0x41b80000,
			 0x41800000,
			 0x40400000,
			 0x41980000,

			 /// vpu register f13
			 0x41b80000,
			 0x41300000,
			 0x41a00000,
			 0x41a00000,
			 0x3f800000,
			 0x41000000,
			 0x41c00000,
			 0x41d80000,

			 /// vpu register f14
			 0x41980000,
			 0x41e80000,
			 0x41600000,
			 0x41c00000,
			 0x41500000,
			 0x41c80000,
			 0x41c80000,
			 0x41f80000,

			 /// vpu register f15
			 0x41200000,
			 0x41000000,
			 0x41d80000,
			 0x40a00000,
			 0x40c00000,
			 0x41e80000,
			 0x41e00000,
			 0x41880000,

			 /// vpu register f16
			 0x41b80000,
			 0x41400000,
			 0x40c00000,
			 0x41e80000,
			 0x3f800000,
			 0x41200000,
			 0x40e00000,
			 0x41a00000,

			 /// vpu register f17
			 0x41700000,
			 0x41500000,
			 0x42000000,
			 0x42000000,
			 0x41a00000,
			 0x41100000,
			 0x41400000,
			 0x40c00000,

			 /// vpu register f18
			 0x40400000,
			 0x40e00000,
			 0x41a00000,
			 0x41980000,
			 0x41e00000,
			 0x41800000,
			 0x41e80000,
			 0x41a80000,

			 /// vpu register f19
			 0x41b00000,
			 0x41100000,
			 0x41600000,
			 0x40000000,
			 0x41100000,
			 0x41300000,
			 0x40000000,
			 0x41000000,

			 /// vpu register f20
			 0x41800000,
			 0x41e00000,
			 0x42000000,
			 0x41500000,
			 0x41f80000,
			 0x41980000,
			 0x41300000,
			 0x41a80000,

			 /// vpu register f21
			 0x42000000,
			 0x41980000,
			 0x41200000,
			 0x41600000,
			 0x41800000,
			 0x41a00000,
			 0x42000000,
			 0x41400000,

			 /// vpu register f22
			 0x41400000,
			 0x41a80000,
			 0x40000000,
			 0x41e00000,
			 0x41c00000,
			 0x41d00000,
			 0x41d00000,
			 0x41e00000,

			 /// vpu register f23
			 0x41e80000,
			 0x41f00000,
			 0x41b80000,
			 0x41100000,
			 0x41d00000,
			 0x41400000,
			 0x41a00000,
			 0x41b00000,

			 /// vpu register f24
			 0x41100000,
			 0x41d80000,
			 0x40c00000,
			 0x42000000,
			 0x40800000,
			 0x41300000,
			 0x41500000,
			 0x41e00000,

			 /// vpu register f25
			 0x41400000,
			 0x42000000,
			 0x3f800000,
			 0x40e00000,
			 0x41300000,
			 0x40400000,
			 0x41900000,
			 0x40c00000,

			 /// vpu register f26
			 0x41400000,
			 0x41c80000,
			 0x41500000,
			 0x40400000,
			 0x40000000,
			 0x41300000,
			 0x41100000,
			 0x41300000,

			 /// vpu register f27
			 0x41c80000,
			 0x41a00000,
			 0x41700000,
			 0x41c00000,
			 0x41880000,
			 0x41100000,
			 0x40a00000,
			 0x41a00000,

			 /// vpu register f28
			 0x41500000,
			 0x3f800000,
			 0x40800000,
			 0x41d80000,
			 0x41b00000,
			 0x41a80000,
			 0x41e80000,
			 0x40a00000,

			 /// vpu register f29
			 0x40000000,
			 0x41b80000,
			 0x41300000,
			 0x41a00000,
			 0x41f80000,
			 0x40000000,
			 0x41d00000,
			 0x41d00000,

			 /// vpu register f30
			 0x40a00000,
			 0x41100000,
			 0x41100000,
			 0x41b80000,
			 0x40000000,
			 0x41500000,
			 0x41300000,
			 0x41980000,

			 /// vpu register f31
			 0x41a80000,
			 0x40c00000,
			 0x40c00000,
			 0x41d80000,
			 0x41f00000,
			 0x41d80000,
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
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f30, f1 (x13)\n"                         ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f20, f1 (x11)\n"                         ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f29, f1 (x12)\n"                         ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f26, f1 (x11)\n"                         ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f22, f1 (x12)\n"                         ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f29, f1 (x12)\n"                         ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f18, f1 (x12)\n"                         ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f8, f1 (x13)\n"                          ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f29, f1 (x15)\n"                         ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f9, f1 (x15)\n"                          ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f28, f1 (x15)\n"                         ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f15, f1 (x11)\n"                         ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f25, f1 (x15)\n"                         ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f26, f1 (x11)\n"                         ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f25, f1 (x15)\n"                         ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f6, f1 (x13)\n"                          ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f2, f1 (x11)\n"                          ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f12, f1 (x11)\n"                         ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f16, f1 (x14)\n"                         ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f23, f1 (x13)\n"                         ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f17, f1 (x14)\n"                         ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f21, f1 (x12)\n"                         ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f16, f1 (x15)\n"                         ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f18, f1 (x15)\n"                         ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f28, f1 (x13)\n"                         ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f27, f1 (x12)\n"                         ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f14, f1 (x13)\n"                         ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f30, f1 (x11)\n"                         ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f16, f1 (x15)\n"                         ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f25, f1 (x13)\n"                         ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f12, f1 (x14)\n"                         ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f18, f1 (x14)\n"                         ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f27, f1 (x13)\n"                         ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f4, f1 (x14)\n"                          ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f5, f1 (x14)\n"                          ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f15, f1 (x11)\n"                         ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f6, f1 (x13)\n"                          ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f19, f1 (x11)\n"                         ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f16, f1 (x12)\n"                         ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f28, f1 (x15)\n"                         ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f12, f1 (x14)\n"                         ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f12, f1 (x12)\n"                         ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f14, f1 (x15)\n"                         ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f19, f1 (x11)\n"                         ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f25, f1 (x15)\n"                         ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f13, f1 (x13)\n"                         ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f18, f1 (x12)\n"                         ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f30, f1 (x15)\n"                         ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f3, f1 (x12)\n"                          ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f26, f1 (x14)\n"                         ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f14, f1 (x13)\n"                         ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f30, f1 (x11)\n"                         ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f21, f1 (x15)\n"                         ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f14, f1 (x11)\n"                         ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f24, f1 (x11)\n"                         ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f27, f1 (x12)\n"                         ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f2, f1 (x12)\n"                          ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f16, f1 (x15)\n"                         ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f28, f1 (x13)\n"                         ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f30, f1 (x11)\n"                         ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f5, f1 (x12)\n"                          ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f3, f1 (x11)\n"                          ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f11, f1 (x11)\n"                         ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f25, f1 (x12)\n"                         ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f17, f1 (x12)\n"                         ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f8, f1 (x12)\n"                          ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f17, f1 (x12)\n"                         ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f18, f1 (x12)\n"                         ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f17, f1 (x11)\n"                         ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f30, f1 (x11)\n"                         ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f12, f1 (x12)\n"                         ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f18, f1 (x11)\n"                         ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f22, f1 (x13)\n"                         ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f18, f1 (x12)\n"                         ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f14, f1 (x14)\n"                         ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f15, f1 (x11)\n"                         ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f20, f1 (x13)\n"                         ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f29, f1 (x14)\n"                         ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f28, f1 (x11)\n"                         ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f26, f1 (x12)\n"                         ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f20, f1 (x13)\n"                         ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f6, f1 (x11)\n"                          ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f29, f1 (x14)\n"                         ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f26, f1 (x15)\n"                         ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f28, f1 (x11)\n"                         ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f15, f1 (x12)\n"                         ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f5, f1 (x14)\n"                          ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f10, f1 (x13)\n"                         ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f20, f1 (x14)\n"                         ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f13, f1 (x11)\n"                         ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f6, f1 (x15)\n"                          ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f16, f1 (x11)\n"                         ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f5, f1 (x15)\n"                          ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f26, f1 (x15)\n"                         ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f21, f1 (x12)\n"                         ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f17, f1 (x15)\n"                         ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f14, f1 (x15)\n"                         ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f15, f1 (x11)\n"                         ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famominl.pi f18, f1 (x15)\n"                         ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
