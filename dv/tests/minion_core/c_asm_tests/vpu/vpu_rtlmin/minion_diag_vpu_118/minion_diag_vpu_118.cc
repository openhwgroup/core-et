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
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00000
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00004
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0000c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00010
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00014
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0001c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00020
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00024
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0xff800000,                                                  // -inf                                        /// 0002c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00030
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00038
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0003c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00040
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00044
			 0x3f028f5c,                                                  // 0.51                                        /// 00048
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0004c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00050
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00054
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00058
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0005c
			 0x33333333,                                                  // 4 random values                             /// 00060
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00064
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00068
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0006c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00070
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00074
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00078
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0007c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00080
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00084
			 0x80011111,                                                  // -9.7958E-41                                 /// 00088
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0008c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00090
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00094
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0009c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000a0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000a8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000b0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000b8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c0
			 0x0c780000,                                                  // Leading 1s:                                 /// 000c4
			 0x0e000007,                                                  // Trailing 1s:                                /// 000c8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 000cc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000d0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000d4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x0e000003,                                                  // Trailing 1s:                                /// 000dc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000e0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 000e4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000ec
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000f0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0x80011111,                                                  // -9.7958E-41                                 /// 000f8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000fc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x0e000001,                                                  // Trailing 1s:                                /// 00104
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00108
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0010c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00110
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00118
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0011c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00120
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00124
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00128
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0012c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00130
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00134
			 0x7f800000,                                                  // inf                                         /// 00138
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0013c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00144
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00148
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0014c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00150
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00158
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00160
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00164
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00168
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0016c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00170
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00174
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0017c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00180
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00184
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00188
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0018c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00190
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00194
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00198
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0019c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 001a4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001a8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 001ac
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 001b0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001b4
			 0x0e000001,                                                  // Trailing 1s:                                /// 001b8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001bc
			 0x00000000,                                                  // zero                                        /// 001c0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001c8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001d0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d4
			 0xcb000000,                                                  // -8388608.0                                  /// 001d8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 001dc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e0
			 0x7fc00001,                                                  // signaling NaN                               /// 001e4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 001e8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001ec
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001fc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00200
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00204
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00208
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0020c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00210
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0021c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00220
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00224
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00228
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0022c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00230
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00234
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00238
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0023c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00240
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00244
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0024c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00250
			 0x00000000,                                                  // zero                                        /// 00254
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00258
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0025c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00264
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00268
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00270
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00274
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00278
			 0xff800000,                                                  // -inf                                        /// 0027c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00280
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00284
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00288
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0028c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00290
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00294
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00298
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0029c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002a0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002a4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002a8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 002ac
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 002b0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 002b4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 002b8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002bc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002c0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002c4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002c8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002cc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 002d4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002d8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002dc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002e0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 002e4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002e8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 002ec
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002f0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002f4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f8
			 0x4b000000,                                                  // 8388608.0                                   /// 002fc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00300
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00304
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00308
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0030c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00314
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00318
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0031c
			 0x00000000,                                                  // zero                                        /// 00320
			 0x0c700000,                                                  // Leading 1s:                                 /// 00324
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00328
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0032c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00330
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00334
			 0x80011111,                                                  // -9.7958E-41                                 /// 00338
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00340
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00344
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0x80000000,                                                  // -zero                                       /// 0034c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00350
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00354
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00358
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0035c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00360
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00364
			 0x80000000,                                                  // -zero                                       /// 00368
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00370
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00374
			 0x0e000001,                                                  // Trailing 1s:                                /// 00378
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0037c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00380
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00384
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00388
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0038c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00390
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00394
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0039c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003a4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003a8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003b0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 003c0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 003c4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003c8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 003cc
			 0x80000000,                                                  // -zero                                       /// 003d0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003d4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003d8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003dc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003e0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003ec
			 0x0e007fff,                                                  // Trailing 1s:                                /// 003f0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 003f4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003f8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003fc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00400
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00404
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00408
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0040c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00414
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00418
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0041c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00420
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00424
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00428
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0042c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00430
			 0xcb000000,                                                  // -8388608.0                                  /// 00434
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00438
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0043c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00444
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00448
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0044c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00450
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00454
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00458
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0045c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00460
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00464
			 0x80000000,                                                  // -zero                                       /// 00468
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0046c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00470
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00474
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00478
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0047c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00480
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00484
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00488
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0048c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00494
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00498
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a0
			 0x55555555,                                                  // 4 random values                             /// 004a4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 004a8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004ac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 004b4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b8
			 0x80000000,                                                  // -zero                                       /// 004bc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004c0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004c8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 004cc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 004d0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004d4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004d8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 004e0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004e8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004ec
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 004f0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 004f8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004fc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00500
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00504
			 0x0c600000,                                                  // Leading 1s:                                 /// 00508
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0050c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00510
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00514
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0051c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00520
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00524
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00528
			 0x55555555,                                                  // 4 random values                             /// 0052c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00530
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00534
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00538
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00540
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x0e000007,                                                  // Trailing 1s:                                /// 00548
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0054c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00550
			 0x0c780000,                                                  // Leading 1s:                                 /// 00554
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00558
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0055c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00560
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00564
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00568
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0056c
			 0x33333333,                                                  // 4 random values                             /// 00570
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00574
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00578
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0057c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00580
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00584
			 0x0c700000,                                                  // Leading 1s:                                 /// 00588
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0058c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00590
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00594
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00598
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0059c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005a0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 005a8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 005ac
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 005b0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 005b4
			 0x80011111,                                                  // -9.7958E-41                                 /// 005b8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005bc
			 0x0e000007,                                                  // Trailing 1s:                                /// 005c0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005c8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005cc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005d4
			 0xcb000000,                                                  // -8388608.0                                  /// 005d8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005dc
			 0x0c700000,                                                  // Leading 1s:                                 /// 005e0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005e4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005e8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 005ec
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005f0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00600
			 0xff800000,                                                  // -inf                                        /// 00604
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00608
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0060c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00610
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00614
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0061c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00620
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00624
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00628
			 0xff800000,                                                  // -inf                                        /// 0062c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00630
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00634
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00638
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0063c
			 0xcb000000,                                                  // -8388608.0                                  /// 00640
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00644
			 0x80011111,                                                  // -9.7958E-41                                 /// 00648
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00650
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00654
			 0x0c700000,                                                  // Leading 1s:                                 /// 00658
			 0x80011111,                                                  // -9.7958E-41                                 /// 0065c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00660
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00664
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00668
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0066c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00670
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00674
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00678
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0067c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00680
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00684
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00688
			 0x0e000007,                                                  // Trailing 1s:                                /// 0068c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00690
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00694
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00698
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 006a0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 006a4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 006ac
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006b0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 006bc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006c0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 006c4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 006c8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006cc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006d0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006d4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 006d8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 006dc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006e0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006e4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 006ec
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 006f0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006f8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 006fc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00704
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0x0c600000,                                                  // Leading 1s:                                 /// 0070c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0x80000000,                                                  // -zero                                       /// 00714
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00718
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0071c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00720
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00724
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00728
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0072c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00730
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00734
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00738
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0073c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00744
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00748
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0074c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00750
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00754
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00758
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0075c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00760
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00764
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00768
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0076c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00770
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00778
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0077c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00780
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00784
			 0x4b000000,                                                  // 8388608.0                                   /// 00788
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0078c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00790
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00794
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00798
			 0x0e000007,                                                  // Trailing 1s:                                /// 0079c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 007a8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 007ac
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007b0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007b4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007b8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 007bc
			 0x80011111,                                                  // -9.7958E-41                                 /// 007c0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007c4
			 0x33333333,                                                  // 4 random values                             /// 007c8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 007cc
			 0x7f800000,                                                  // inf                                         /// 007d0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 007d4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 007d8
			 0x7fc00001,                                                  // signaling NaN                               /// 007dc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007e0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 007e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ec
			 0xff800000,                                                  // -inf                                        /// 007f0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 007f4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 007f8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00800
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00804
			 0x55555555,                                                  // 4 random values                             /// 00808
			 0x0c780000,                                                  // Leading 1s:                                 /// 0080c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00810
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00814
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0081c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00820
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00824
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00828
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00830
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00834
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00838
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x7fc00001,                                                  // signaling NaN                               /// 00840
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00844
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00848
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0xcb000000,                                                  // -8388608.0                                  /// 00850
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00854
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00858
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0085c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00860
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00864
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0086c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00870
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00874
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00878
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0087c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00880
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00884
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0088c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00894
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00898
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0089c
			 0x7fc00001,                                                  // signaling NaN                               /// 008a0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008a4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008a8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 008ac
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008b8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008bc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 008c0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 008c4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008c8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008cc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008d0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d4
			 0x0c780000,                                                  // Leading 1s:                                 /// 008d8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 008dc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008e4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 008e8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ec
			 0x0e00007f,                                                  // Trailing 1s:                                /// 008f0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 008f4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 008f8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008fc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00900
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00904
			 0x80011111,                                                  // -9.7958E-41                                 /// 00908
			 0x3f028f5c,                                                  // 0.51                                        /// 0090c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00910
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00918
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0091c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00920
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00924
			 0x80011111,                                                  // -9.7958E-41                                 /// 00928
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0092c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00930
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00934
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00938
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0093c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00940
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00944
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00948
			 0x4b000000,                                                  // 8388608.0                                   /// 0094c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00950
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00954
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00958
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0095c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00960
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00968
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00970
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00974
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00978
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0097c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00980
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00984
			 0x55555555,                                                  // 4 random values                             /// 00988
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0098c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00990
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00994
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00998
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0099c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009a4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009a8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 009ac
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009b0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009b4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 009b8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009bc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009c0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009c4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 009c8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 009cc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009d4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009d8
			 0x00011111,                                                  // 9.7958E-41                                  /// 009dc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009e0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009e4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009e8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009ec
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009f4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009f8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009fc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a00
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a04
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a08
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a0c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a10
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a14
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a18
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a1c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a20
			 0x7f800000,                                                  // inf                                         /// 00a24
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a28
			 0x33333333,                                                  // 4 random values                             /// 00a2c
			 0xffc00001,                                                  // -signaling NaN                              /// 00a30
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a34
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a38
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a3c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a40
			 0x7f800000,                                                  // inf                                         /// 00a44
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a48
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a4c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a50
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a5c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a64
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a68
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a6c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a70
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a74
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a78
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a7c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x55555555,                                                  // 4 random values                             /// 00a84
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a88
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a8c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a90
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a94
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a98
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a9c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00aa4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00aa8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00aac
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ab0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ab4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ab8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00abc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00acc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ad0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ad4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ad8
			 0xff800000,                                                  // -inf                                        /// 00adc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ae0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ae4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00aec
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00af0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af8
			 0x3f028f5c,                                                  // 0.51                                        /// 00afc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b00
			 0x33333333,                                                  // 4 random values                             /// 00b04
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b08
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b0c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b10
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b14
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b18
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b1c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b20
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b28
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b2c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b30
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b38
			 0x80000000,                                                  // -zero                                       /// 00b3c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b40
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b44
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b48
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b4c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b50
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b54
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b58
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b5c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b60
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b64
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b68
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b6c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b70
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b74
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b7c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b80
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b84
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b88
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b8c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b90
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b94
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b98
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b9c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ba0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ba4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ba8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bac
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00bb0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00bb4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00bc0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0x4b000000,                                                  // 8388608.0                                   /// 00bc8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00bcc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bd4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00bdc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00be0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00be4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00bec
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00bf0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00bf4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00bf8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00bfc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c00
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c04
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c08
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c0c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c18
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c1c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c24
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c28
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c2c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c34
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c38
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c3c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c40
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c44
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c48
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c4c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c54
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c58
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c5c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c60
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c64
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c74
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c78
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c7c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c80
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c84
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c88
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c8c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c90
			 0x80000000,                                                  // -zero                                       /// 00c94
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c98
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c9c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca4
			 0x55555555,                                                  // 4 random values                             /// 00ca8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00cac
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00cb0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cb4
			 0x4b000000,                                                  // 8388608.0                                   /// 00cb8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00cbc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00cc0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00cc4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ccc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00cd0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00cd4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00cd8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00cdc
			 0x4b000000,                                                  // 8388608.0                                   /// 00ce0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ce8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00cec
			 0x33333333,                                                  // 4 random values                             /// 00cf0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00cf4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00cfc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d00
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d04
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d08
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d0c
			 0x7fc00001,                                                  // signaling NaN                               /// 00d10
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d14
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d1c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d20
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d24
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d28
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d2c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d30
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d34
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d38
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d44
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d48
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d4c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d50
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d54
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d5c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d60
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d64
			 0xbf028f5c,                                                  // -0.51                                       /// 00d68
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d6c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d70
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d74
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d78
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d7c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d80
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d84
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d8c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d94
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d98
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d9c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00da0
			 0x00000000,                                                  // zero                                        /// 00da4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00db0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00db8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00dbc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc0
			 0x7fc00001,                                                  // signaling NaN                               /// 00dc4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00dc8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00dcc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00dd0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00dd4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00dd8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ddc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00de4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00dec
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00df0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00df4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00df8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00dfc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e00
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e04
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e08
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e0c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e10
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e14
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e18
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e20
			 0x4b000000,                                                  // 8388608.0                                   /// 00e24
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e28
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e2c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e30
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e34
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e38
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e3c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e40
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e44
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e48
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e4c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e50
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e58
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e5c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e60
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e64
			 0x33333333,                                                  // 4 random values                             /// 00e68
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e70
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e74
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e7c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e80
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e84
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e88
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e8c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e90
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0xcb000000,                                                  // -8388608.0                                  /// 00e98
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e9c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea4
			 0xffc00001,                                                  // -signaling NaN                              /// 00ea8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00eac
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00eb0
			 0x4b000000,                                                  // 8388608.0                                   /// 00eb4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ebc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ec0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ec4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ed0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ed4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00edc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ee8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00eec
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ef0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ef4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f00
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f04
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f08
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f0c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f10
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f14
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f18
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f1c
			 0x7f800000,                                                  // inf                                         /// 00f20
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f24
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f28
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f2c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f30
			 0x7f800000,                                                  // inf                                         /// 00f34
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f38
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f3c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f40
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f44
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f48
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f4c
			 0xcb000000,                                                  // -8388608.0                                  /// 00f50
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f54
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f58
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f5c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f60
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f64
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f68
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f6c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f70
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f74
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f7c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f80
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f84
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f88
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f8c
			 0xcb000000,                                                  // -8388608.0                                  /// 00f90
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f94
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f98
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f9c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fa0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fa4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00fa8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00fac
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00fb0
			 0xbf028f5c,                                                  // -0.51                                       /// 00fb4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00fb8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fbc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00fc4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00fc8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00fcc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00fd0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00fd4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00fd8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00fdc
			 0xff800000,                                                  // -inf                                        /// 00fe0
			 0x7f800000,                                                  // inf                                         /// 00fe4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00fec
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ff0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ff4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ff8
			 0x7f7ffffe // max norm - 2ulp                               // max norm +ve                                /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00000
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00008
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0000c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00010
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00014
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00018
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0001c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00020
			 0x0c600000,                                                  // Leading 1s:                                 /// 00024
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00028
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0002c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00030
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00034
			 0xff800000,                                                  // -inf                                        /// 00038
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0003c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00040
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00044
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0004c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00054
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00058
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0005c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00060
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00064
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00068
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0006c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00070
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00074
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00078
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00080
			 0x80000000,                                                  // -zero                                       /// 00084
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0008c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00090
			 0x0e000007,                                                  // Trailing 1s:                                /// 00094
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0009c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000a0
			 0x7fc00001,                                                  // signaling NaN                               /// 000a4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000a8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000ac
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 000b0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 000b4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000b8
			 0xffc00001,                                                  // -signaling NaN                              /// 000bc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000c0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0xbf028f5c,                                                  // -0.51                                       /// 000c8
			 0x0e000007,                                                  // Trailing 1s:                                /// 000cc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000d0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000d4
			 0x0c400000,                                                  // Leading 1s:                                 /// 000d8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000dc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000e0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000e4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 000ec
			 0xCCCCCCCC,                                                  // 4 random values                             /// 000f0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000f4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000f8
			 0x00000000,                                                  // zero                                        /// 000fc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00100
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00104
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00108
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00110
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00114
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0011c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00120
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00124
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00128
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0012c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00130
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00134
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00138
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0013c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00140
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00144
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0014c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00150
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0x80011111,                                                  // -9.7958E-41                                 /// 00158
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0015c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00160
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00164
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00168
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0016c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00170
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00174
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00178
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0017c
			 0xcb000000,                                                  // -8388608.0                                  /// 00180
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00184
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00188
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0018c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00190
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00194
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00198
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0019c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 001a0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001a4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001a8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 001ac
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001b0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001b8
			 0x0e000003,                                                  // Trailing 1s:                                /// 001bc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c0
			 0x80000000,                                                  // -zero                                       /// 001c4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001c8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001cc
			 0x4b000000,                                                  // 8388608.0                                   /// 001d0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001d4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 001dc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 001e0
			 0x7fc00001,                                                  // signaling NaN                               /// 001e4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001ec
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001f4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001fc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00204
			 0x00011111,                                                  // 9.7958E-41                                  /// 00208
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0020c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00210
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00214
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0021c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00220
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00224
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00228
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00230
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00234
			 0xff800000,                                                  // -inf                                        /// 00238
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0023c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00240
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00244
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00248
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0024c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00254
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00258
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0025c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00260
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00264
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0026c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00270
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00274
			 0x0c700000,                                                  // Leading 1s:                                 /// 00278
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0027c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00280
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00288
			 0x0c600000,                                                  // Leading 1s:                                 /// 0028c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00290
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00294
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00298
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0029c
			 0x0c700000,                                                  // Leading 1s:                                 /// 002a0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 002ac
			 0x0c400000,                                                  // Leading 1s:                                 /// 002b0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002b4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002b8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 002bc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002c0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 002c4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 002c8
			 0x0c600000,                                                  // Leading 1s:                                 /// 002cc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002d0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002d8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 002dc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 002e0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 002e4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 002e8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002ec
			 0x0c600000,                                                  // Leading 1s:                                 /// 002f0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002f4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 002f8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00300
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00304
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00308
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0030c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00310
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00318
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0031c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00320
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00324
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00328
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0032c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00330
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00334
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00338
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0033c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00344
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00348
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0034c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0xffc00001,                                                  // -signaling NaN                              /// 00354
			 0xbf028f5c,                                                  // -0.51                                       /// 00358
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0035c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00360
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00364
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00368
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0036c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00370
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00374
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0037c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00380
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00384
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00388
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0038c
			 0x4b000000,                                                  // 8388608.0                                   /// 00390
			 0x0e000001,                                                  // Trailing 1s:                                /// 00394
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00398
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0039c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003a0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003a4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003a8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 003ac
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003b0
			 0x55555555,                                                  // 4 random values                             /// 003b4
			 0x0e000003,                                                  // Trailing 1s:                                /// 003b8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003bc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0xbf028f5c,                                                  // -0.51                                       /// 003c8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003cc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003d0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003d8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003dc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003e8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ec
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 003f0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003f4
			 0x7fc00001,                                                  // signaling NaN                               /// 003f8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003fc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00400
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00404
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00408
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0040c
			 0x7f800000,                                                  // inf                                         /// 00410
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00414
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00418
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0041c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00420
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00424
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00428
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0042c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00430
			 0x0c600000,                                                  // Leading 1s:                                 /// 00434
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00438
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0043c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00440
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00444
			 0x7f800000,                                                  // inf                                         /// 00448
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0044c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00450
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00454
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0045c
			 0xffc00001,                                                  // -signaling NaN                              /// 00460
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00464
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00468
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00470
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00474
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00478
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0047c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00480
			 0x7f800000,                                                  // inf                                         /// 00484
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00488
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0048c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00494
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0049c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004a0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 004a4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004a8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 004b0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 004b4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004b8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004bc
			 0x0e000001,                                                  // Trailing 1s:                                /// 004c0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004c4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004c8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004cc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004d0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 004d8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004dc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004e4
			 0x0c780000,                                                  // Leading 1s:                                 /// 004e8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 004ec
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 004f4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004fc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00504
			 0x7f800000,                                                  // inf                                         /// 00508
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0050c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00510
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00514
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00518
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0051c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00520
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00524
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00528
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0052c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00530
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00534
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00538
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00540
			 0x7f800000,                                                  // inf                                         /// 00544
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00548
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0054c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00550
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00558
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0055c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00560
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00564
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00568
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0056c
			 0x3f028f5c,                                                  // 0.51                                        /// 00570
			 0xff800000,                                                  // -inf                                        /// 00574
			 0x0c700000,                                                  // Leading 1s:                                 /// 00578
			 0x0c400000,                                                  // Leading 1s:                                 /// 0057c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00584
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00588
			 0x0e000003,                                                  // Trailing 1s:                                /// 0058c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00590
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00594
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00598
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0059c
			 0x7f800000,                                                  // inf                                         /// 005a0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 005a4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 005ac
			 0x4b000000,                                                  // 8388608.0                                   /// 005b0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005b4
			 0xff800000,                                                  // -inf                                        /// 005b8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 005bc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005c0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 005c4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005c8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005cc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005d0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 005d4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005d8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005dc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005e0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 005e4
			 0x7fc00001,                                                  // signaling NaN                               /// 005e8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 005ec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005f4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 005f8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005fc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00600
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00604
			 0x0e000003,                                                  // Trailing 1s:                                /// 00608
			 0x7f800000,                                                  // inf                                         /// 0060c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00610
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00614
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00618
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0061c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00620
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00624
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00628
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0062c
			 0xcb000000,                                                  // -8388608.0                                  /// 00630
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00638
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0063c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00640
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00644
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00648
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0064c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00654
			 0xff800000,                                                  // -inf                                        /// 00658
			 0x3f028f5c,                                                  // 0.51                                        /// 0065c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00660
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00664
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00668
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00670
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00674
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0067c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00680
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00684
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0068c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00690
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00694
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00698
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0069c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006a0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006a8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006ac
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006b0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 006b8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006bc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 006c0
			 0x80011111,                                                  // -9.7958E-41                                 /// 006c4
			 0x3f028f5c,                                                  // 0.51                                        /// 006c8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0x33333333,                                                  // 4 random values                             /// 006d0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006d4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006d8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006dc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 006e4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 006e8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006ec
			 0x0c400000,                                                  // Leading 1s:                                 /// 006f0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 006f4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006f8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006fc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00700
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00704
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x0c700000,                                                  // Leading 1s:                                 /// 0070c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00710
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00714
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00718
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0071c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00720
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00724
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00728
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0072c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00730
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00734
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0073c
			 0x3f028f5c,                                                  // 0.51                                        /// 00740
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00748
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0074c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00750
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00754
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00758
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0075c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00760
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00764
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00768
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0076c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00770
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00774
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00778
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00780
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00784
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0078c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00790
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00794
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00798
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0079c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007a4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007a8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 007ac
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007b0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007b4
			 0x4b000000,                                                  // 8388608.0                                   /// 007b8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007bc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 007c0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007c8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007cc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 007d4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007d8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 007dc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007e0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 007e8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007ec
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007f0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 007f4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 007f8
			 0x80011111,                                                  // -9.7958E-41                                 /// 007fc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00800
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00804
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00808
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0080c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00810
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00814
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00818
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0081c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00820
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00824
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00828
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0082c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00834
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00838
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0083c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00840
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00844
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00848
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0084c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00850
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0x0e000003,                                                  // Trailing 1s:                                /// 00858
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0085c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00864
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00868
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0086c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00870
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00874
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00878
			 0x0e000003,                                                  // Trailing 1s:                                /// 0087c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00880
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00884
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00888
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00890
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00894
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00898
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 008a0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 008a4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 008a8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ac
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008b0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 008b4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008b8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008bc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 008c0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008c4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008c8
			 0x00000000,                                                  // zero                                        /// 008cc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 008d4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 008d8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 008dc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008e0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008e4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008e8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008ec
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 008f0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008f4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008f8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008fc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00900
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00904
			 0x80000000,                                                  // -zero                                       /// 00908
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00910
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00914
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0091c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00920
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00924
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00928
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0092c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00930
			 0xcb000000,                                                  // -8388608.0                                  /// 00934
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00938
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0093c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00940
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00944
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00948
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0094c
			 0xffc00001,                                                  // -signaling NaN                              /// 00950
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00954
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00958
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0095c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00960
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00964
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00968
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0096c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00970
			 0x00011111,                                                  // 9.7958E-41                                  /// 00974
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00978
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0097c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00980
			 0x4b000000,                                                  // 8388608.0                                   /// 00984
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0098c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00990
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0099c
			 0x7f800000,                                                  // inf                                         /// 009a0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009a4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009a8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 009ac
			 0x00000000,                                                  // zero                                        /// 009b0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 009bc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009c0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009c4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 009c8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009d0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009d4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009d8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009dc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009e0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009e4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009ec
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 009f4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 009f8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0xff800000,                                                  // -inf                                        /// 00a04
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a08
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a0c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a10
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a14
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a18
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a1c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a24
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a28
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a30
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a34
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a38
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a3c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a40
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a44
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a48
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a4c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a50
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a54
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a5c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a60
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a64
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a68
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a6c
			 0x7f800000,                                                  // inf                                         /// 00a70
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a74
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a78
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a7c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a80
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a84
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a8c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a90
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a94
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a98
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a9c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00aa4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00aac
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ab0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ab4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ab8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ac4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ac8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad0
			 0x7fc00001,                                                  // signaling NaN                               /// 00ad4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0x80000000,                                                  // -zero                                       /// 00ae0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ae4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ae8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00aec
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00af0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00af4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00af8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00afc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b00
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b04
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b08
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b0c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b10
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b14
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b18
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b1c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b20
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b24
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b28
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b30
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b34
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b38
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b3c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b40
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b44
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b4c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b50
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b54
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b58
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b5c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b64
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b6c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b70
			 0x3f028f5c,                                                  // 0.51                                        /// 00b74
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b78
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b88
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b90
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b94
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b9c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ba8
			 0x33333333,                                                  // 4 random values                             /// 00bac
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bb0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00bc4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00bc8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00bcc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00bd0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00bdc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00be0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00be4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00be8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0x00000000,                                                  // zero                                        /// 00bf0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bf4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00bf8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00bfc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c00
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c04
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c08
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c0c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c18
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c1c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c20
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c24
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c28
			 0x7fc00001,                                                  // signaling NaN                               /// 00c2c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c34
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c38
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c3c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c40
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c48
			 0xcb000000,                                                  // -8388608.0                                  /// 00c4c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c50
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c54
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c58
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c60
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c64
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c68
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c70
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c74
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c7c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c84
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c88
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c8c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c90
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c94
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c98
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c9c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ca0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ca4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ca8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00cac
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00cb4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00cb8
			 0xbf028f5c,                                                  // -0.51                                       /// 00cbc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00cc0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00cc4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00cc8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ccc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00cd0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00cd4
			 0xcb000000,                                                  // -8388608.0                                  /// 00cd8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00cdc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ce0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ce4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00cec
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00cf0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00cf4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cf8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00cfc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d00
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d04
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d08
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d0c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d10
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d14
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d18
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d1c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d20
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d24
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d28
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d2c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d30
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d34
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d38
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d3c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d40
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d44
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d48
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d4c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d50
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d54
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d58
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d5c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d64
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d68
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d6c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d70
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d74
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d78
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d7c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d80
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d84
			 0x4b000000,                                                  // 8388608.0                                   /// 00d88
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d8c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d90
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d94
			 0x80000000,                                                  // -zero                                       /// 00d98
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d9c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00da0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00da4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00dac
			 0x7fc00001,                                                  // signaling NaN                               /// 00db0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00db8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00dbc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00dc0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00dc4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00dc8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00dcc
			 0x7fc00001,                                                  // signaling NaN                               /// 00dd0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ddc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00de0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00df0
			 0xff800000,                                                  // -inf                                        /// 00df4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00df8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00dfc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e00
			 0xffc00001,                                                  // -signaling NaN                              /// 00e04
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e08
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e0c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e10
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e14
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e18
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e1c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e20
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e24
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e28
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e2c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e30
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e34
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e38
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e3c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e40
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e44
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e48
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e4c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e50
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e54
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e58
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e5c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e60
			 0xffc00001,                                                  // -signaling NaN                              /// 00e64
			 0x7fc00001,                                                  // signaling NaN                               /// 00e68
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e70
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e74
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e80
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e84
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e88
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e8c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e94
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e9c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ea0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ea8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00eac
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00eb0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00eb4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ebc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ec0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ec4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ec8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ecc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ed0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ed4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00edc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ee0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ee8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eec
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ef0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ef4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ef8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00efc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f00
			 0xff800000,                                                  // -inf                                        /// 00f04
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f08
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f0c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f10
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f18
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f1c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f20
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f24
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f28
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f2c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f30
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f34
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f38
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f44
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f48
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f4c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f50
			 0xcb000000,                                                  // -8388608.0                                  /// 00f54
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f58
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f5c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f60
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f68
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f6c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f70
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f74
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f78
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f7c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f80
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f84
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f88
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f8c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f90
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f94
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f98
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f9c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00fa0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00fa4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fac
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00fb0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00fb4
			 0x33333333,                                                  // 4 random values                             /// 00fb8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fc0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00fc4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00fc8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fcc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00fd0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00fd4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00fd8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00fdc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fe0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fe4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fe8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fec
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ff0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ff4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ff8
			 0x007ffffe // max subnorm - 1ulp                            // SP +ve numbers                              /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00000
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00004
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00008
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0000c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00010
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00014
			 0xffc00001,                                                  // -signaling NaN                              /// 00018
			 0x80000000,                                                  // -zero                                       /// 0001c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00020
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00024
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0002c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00030
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00034
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00038
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0003c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00040
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00044
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0004c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00050
			 0x7f800000,                                                  // inf                                         /// 00054
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00058
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0005c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00060
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00068
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0006c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00070
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00074
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00078
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00084
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00088
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0008c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00090
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00094
			 0xcb000000,                                                  // -8388608.0                                  /// 00098
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0009c
			 0x00000000,                                                  // zero                                        /// 000a0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 000a4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 000a8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0x0c780000,                                                  // Leading 1s:                                 /// 000b0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000bc
			 0x0c600000,                                                  // Leading 1s:                                 /// 000c0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 000c4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 000c8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000cc
			 0x55555555,                                                  // 4 random values                             /// 000d0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 000d8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000dc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000e0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 000e8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ec
			 0x0e000003,                                                  // Trailing 1s:                                /// 000f0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f4
			 0x3f028f5c,                                                  // 0.51                                        /// 000f8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000fc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00100
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00104
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00108
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00110
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00114
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0011c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00120
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00124
			 0x55555555,                                                  // 4 random values                             /// 00128
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0012c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00130
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00134
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0013c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00144
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0014c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00150
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00154
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00158
			 0x80011111,                                                  // -9.7958E-41                                 /// 0015c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00160
			 0x7f800000,                                                  // inf                                         /// 00164
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00168
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0016c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00170
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00174
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00178
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0017c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00184
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00188
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0018c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00190
			 0x4b000000,                                                  // 8388608.0                                   /// 00194
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00198
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0019c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 001a4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001a8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0x80000000,                                                  // -zero                                       /// 001b0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001b4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 001b8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 001c0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001c4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001cc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001d4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 001d8
			 0xcb000000,                                                  // -8388608.0                                  /// 001dc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 001e0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001e4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 001f4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001f8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001fc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00204
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00208
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00210
			 0x00011111,                                                  // 9.7958E-41                                  /// 00214
			 0x55555555,                                                  // 4 random values                             /// 00218
			 0x0c400000,                                                  // Leading 1s:                                 /// 0021c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00220
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00224
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00228
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0022c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00230
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00234
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00238
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0023c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00240
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00244
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00248
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0024c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00250
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00254
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00258
			 0x00011111,                                                  // 9.7958E-41                                  /// 0025c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00260
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00264
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0026c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00270
			 0x0e000007,                                                  // Trailing 1s:                                /// 00274
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00278
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0027c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00280
			 0xbf028f5c,                                                  // -0.51                                       /// 00284
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00288
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0028c
			 0x7fc00001,                                                  // signaling NaN                               /// 00290
			 0x7fc00001,                                                  // signaling NaN                               /// 00294
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00298
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0029c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002a0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 002a4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002a8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002ac
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 002b0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 002b8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002bc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002c0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002c4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 002cc
			 0x55555555,                                                  // 4 random values                             /// 002d0
			 0x00011111,                                                  // 9.7958E-41                                  /// 002d4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002d8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002dc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002e8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002f4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002f8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 002fc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00300
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00304
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00308
			 0xffc00001,                                                  // -signaling NaN                              /// 0030c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00310
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00314
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00318
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00320
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00324
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00328
			 0x0e000007,                                                  // Trailing 1s:                                /// 0032c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00330
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00338
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00340
			 0x0c700000,                                                  // Leading 1s:                                 /// 00344
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00348
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0034c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00350
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00354
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00358
			 0xffc00001,                                                  // -signaling NaN                              /// 0035c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00360
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00364
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00368
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0036c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00370
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00374
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0037c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00380
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00384
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00388
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0038c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00390
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00394
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0039c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 003a0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003a4
			 0x0c600000,                                                  // Leading 1s:                                 /// 003a8
			 0x0c700000,                                                  // Leading 1s:                                 /// 003ac
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 003b0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003b8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x00011111,                                                  // 9.7958E-41                                  /// 003c0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003c4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003c8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003cc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003d4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003d8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003e0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003e4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 003e8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 003ec
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003f0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003f4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003fc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00404
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00408
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0040c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00410
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00414
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00418
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0041c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00420
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00424
			 0x80000000,                                                  // -zero                                       /// 00428
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0042c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00430
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00434
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00438
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0043c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00444
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00448
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0044c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00450
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00454
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00458
			 0x4b000000,                                                  // 8388608.0                                   /// 0045c
			 0x55555555,                                                  // 4 random values                             /// 00460
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00464
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0046c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00470
			 0x0e000001,                                                  // Trailing 1s:                                /// 00474
			 0x00000000,                                                  // zero                                        /// 00478
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0047c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00480
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00484
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00488
			 0x80000000,                                                  // -zero                                       /// 0048c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00494
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0049c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 004a0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004a8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004ac
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004b0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 004b4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004bc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004c0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004c4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004c8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004cc
			 0xff800000,                                                  // -inf                                        /// 004d0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004d8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004dc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004e0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004e4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004ec
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004f4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004f8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 004fc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00500
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00504
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0050c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00510
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00514
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00518
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0051c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00520
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00524
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00528
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0052c
			 0x4b000000,                                                  // 8388608.0                                   /// 00530
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00534
			 0x33333333,                                                  // 4 random values                             /// 00538
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0053c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00540
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00544
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00548
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0054c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00550
			 0x0c600000,                                                  // Leading 1s:                                 /// 00554
			 0x0c400000,                                                  // Leading 1s:                                 /// 00558
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0055c
			 0x80000000,                                                  // -zero                                       /// 00560
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00564
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00568
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00570
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00578
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0057c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00580
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00584
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00588
			 0x0c600000,                                                  // Leading 1s:                                 /// 0058c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00590
			 0xbf028f5c,                                                  // -0.51                                       /// 00594
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00598
			 0x4b000000,                                                  // 8388608.0                                   /// 0059c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005a0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005a4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 005a8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005b4
			 0x0e000003,                                                  // Trailing 1s:                                /// 005b8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 005c0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005c4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005c8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005cc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005d0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005d4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005d8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 005dc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005e4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005ec
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005f0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 005f4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 005f8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00600
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00604
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00608
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0060c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00610
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00614
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00618
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0061c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00620
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00624
			 0x80000000,                                                  // -zero                                       /// 00628
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0x7f800000,                                                  // inf                                         /// 00630
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00634
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00638
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0063c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0x3f028f5c,                                                  // 0.51                                        /// 00644
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00648
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0064c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00650
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00654
			 0x0c780000,                                                  // Leading 1s:                                 /// 00658
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0065c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00660
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00664
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00668
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0066c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00674
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00680
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00684
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0068c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0x0c400000,                                                  // Leading 1s:                                 /// 00694
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00698
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0069c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006a0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006a4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 006a8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006ac
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 006b0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006b4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006b8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006bc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 006c0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006c4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006c8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006cc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 006d0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006d4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 006d8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006dc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006e0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 006e4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 006e8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ec
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006f0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006fc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00700
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00704
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0070c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00710
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00718
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0071c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0xbf028f5c,                                                  // -0.51                                       /// 00724
			 0x0e000001,                                                  // Trailing 1s:                                /// 00728
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0072c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00730
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00734
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0073c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00740
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00744
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00748
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0074c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00750
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00754
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00758
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0075c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00760
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00764
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00768
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00770
			 0x4b000000,                                                  // 8388608.0                                   /// 00774
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00778
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0077c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00784
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00788
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0078c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00794
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00798
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0079c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 007a4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007ac
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007b4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007b8
			 0x80000000,                                                  // -zero                                       /// 007bc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007c0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 007c4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007c8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 007cc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007d4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 007d8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007dc
			 0x3f028f5c,                                                  // 0.51                                        /// 007e0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 007e4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 007e8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007f0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007f4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007f8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007fc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x7fc00001,                                                  // signaling NaN                               /// 00804
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00808
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0080c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00810
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00814
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00818
			 0x00000000,                                                  // zero                                        /// 0081c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00820
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00824
			 0x0e000007,                                                  // Trailing 1s:                                /// 00828
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0082c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00830
			 0x00011111,                                                  // 9.7958E-41                                  /// 00834
			 0x33333333,                                                  // 4 random values                             /// 00838
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0083c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00840
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00844
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00848
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0084c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00858
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0085c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00860
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00864
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x3f028f5c,                                                  // 0.51                                        /// 0086c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00870
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00874
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00878
			 0xff800000,                                                  // -inf                                        /// 0087c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00880
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00884
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0x0c400000,                                                  // Leading 1s:                                 /// 0088c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00890
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00894
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008a0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 008a4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008a8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008ac
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 008b4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008bc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008c0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008c4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008c8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 008cc
			 0x0c600000,                                                  // Leading 1s:                                 /// 008d0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d4
			 0xbf028f5c,                                                  // -0.51                                       /// 008d8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008dc
			 0x55555555,                                                  // 4 random values                             /// 008e0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 008e4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 008ec
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008f0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008f4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008fc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00900
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00904
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00908
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00910
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00918
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0091c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0x7fc00001,                                                  // signaling NaN                               /// 00924
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00928
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0092c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00930
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00934
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00938
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0093c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00940
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00944
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00948
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0094c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00950
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00954
			 0x80000000,                                                  // -zero                                       /// 00958
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0095c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00960
			 0x00011111,                                                  // 9.7958E-41                                  /// 00964
			 0x33333333,                                                  // 4 random values                             /// 00968
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0096c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00970
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0097c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00980
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00984
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00988
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0098c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00990
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00994
			 0xcb000000,                                                  // -8388608.0                                  /// 00998
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0099c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009a0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0x33333333,                                                  // 4 random values                             /// 009a8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009ac
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009b4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009b8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 009bc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009c0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 009c4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009c8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 009cc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 009d0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 009d4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009dc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009e0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009e4
			 0x7f800000,                                                  // inf                                         /// 009e8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 009ec
			 0x0e00007f,                                                  // Trailing 1s:                                /// 009f0
			 0xbf028f5c,                                                  // -0.51                                       /// 009f4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f8
			 0x0e000003,                                                  // Trailing 1s:                                /// 009fc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a00
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a08
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a0c
			 0xbf028f5c,                                                  // -0.51                                       /// 00a10
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a14
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a18
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a1c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a20
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a24
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a28
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a2c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a34
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a3c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a40
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a44
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a48
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a4c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a50
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a54
			 0x55555555,                                                  // 4 random values                             /// 00a58
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a60
			 0x80000000,                                                  // -zero                                       /// 00a64
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a68
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a6c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a70
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a74
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a78
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a7c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a84
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a88
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a8c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a90
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a94
			 0xffc00001,                                                  // -signaling NaN                              /// 00a98
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a9c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00aa4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00aa8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00aac
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ab0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ab4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00abc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ac0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ac4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ac8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00acc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ad0
			 0x3f028f5c,                                                  // 0.51                                        /// 00ad4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00adc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ae0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ae4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ae8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00af0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00af4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00af8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00afc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b00
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b04
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b08
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b0c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b10
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b14
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b18
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b1c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b20
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b2c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b30
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b34
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b38
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b3c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b40
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b48
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b50
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b54
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b58
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b5c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b64
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b68
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b6c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b70
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b74
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b78
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b7c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b80
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b84
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b8c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b90
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b94
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ba0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ba4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00bac
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00bb0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00bb4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bb8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bc0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00bc8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00bcc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00bd0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00bd4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00bd8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00bdc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00be0
			 0xffc00001,                                                  // -signaling NaN                              /// 00be4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00be8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00bec
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00bf0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00bf4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0x00000000,                                                  // zero                                        /// 00c00
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c04
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c0c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c10
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c18
			 0x33333333,                                                  // 4 random values                             /// 00c1c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c20
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c24
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c28
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c2c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c30
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c34
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c38
			 0xffc00001,                                                  // -signaling NaN                              /// 00c3c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c40
			 0xff800000,                                                  // -inf                                        /// 00c44
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c48
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c4c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c50
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c54
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c58
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c5c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c60
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c64
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c68
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c6c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c70
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c74
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c78
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c7c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c80
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c84
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c88
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c8c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c90
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c94
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c98
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c9c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ca0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ca4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ca8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00cac
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00cb0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00cb4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00cb8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00cbc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00cc0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00cc4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00cc8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ccc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00cd0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00cd4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00cd8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00cdc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ce0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ce4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ce8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00cec
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00cf0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00cf4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00cf8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00cfc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d00
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d04
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d08
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d0c
			 0x55555555,                                                  // 4 random values                             /// 00d10
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d14
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d18
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d1c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d20
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d24
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d28
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d2c
			 0xffc00001,                                                  // -signaling NaN                              /// 00d30
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d34
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d38
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d44
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d48
			 0x7fc00001,                                                  // signaling NaN                               /// 00d4c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d54
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d58
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d5c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d64
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d68
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d6c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d78
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d7c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d80
			 0x7f800000,                                                  // inf                                         /// 00d84
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d88
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d8c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d90
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d94
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d98
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d9c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00da0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00da4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00da8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00dac
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00db0
			 0xffc00001,                                                  // -signaling NaN                              /// 00db4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00db8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00dbc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00dc0
			 0x3f028f5c,                                                  // 0.51                                        /// 00dc4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dcc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00dd0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00dd4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00dd8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ddc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00de0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00dec
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00df4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00df8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00dfc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e00
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e04
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e08
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e0c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e10
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e14
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e18
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e1c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e24
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e28
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e2c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e30
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e34
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e38
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e3c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e44
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e48
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e4c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e50
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e54
			 0xbf028f5c,                                                  // -0.51                                       /// 00e58
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e5c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e64
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e68
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e6c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e78
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e7c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e80
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e84
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e88
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e8c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e90
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0xff800000,                                                  // -inf                                        /// 00e98
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e9c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ea0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ea4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ea8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00eac
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00eb0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb8
			 0x80000000,                                                  // -zero                                       /// 00ebc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ec0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ecc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ed4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ed8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ee0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ee4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ef0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ef8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00efc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f00
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f04
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f08
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f0c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f10
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f14
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f18
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f1c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f20
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f24
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f28
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f2c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f30
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f34
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f38
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f3c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f40
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f44
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x3f028f5c,                                                  // 0.51                                        /// 00f4c
			 0xff800000,                                                  // -inf                                        /// 00f50
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f54
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f58
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f5c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f60
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f64
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f68
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f6c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f74
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f78
			 0xffc00001,                                                  // -signaling NaN                              /// 00f7c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f80
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f84
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f88
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f8c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f90
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f94
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f98
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00fa0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00fa8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00fac
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00fb0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00fb4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00fb8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00fbc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00fc4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00fc8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fcc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00fd0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00fd4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00fd8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00fdc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fe0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fe8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00fec
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ff0
			 0x3f028f5c,                                                  // 0.51                                        /// 00ff4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ff8
			 0x7fc00000                                                  // cquiet NaN                                  /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00000
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00004
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00008
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0000c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00010
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00018
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0001c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00020
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00028
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0002c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00030
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00034
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00038
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0003c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0x4b000000,                                                  // 8388608.0                                   /// 00044
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00048
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0004c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0xbf028f5c,                                                  // -0.51                                       /// 00054
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00058
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0005c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00060
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00068
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0006c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00070
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00074
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00078
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0007c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00080
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00084
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00088
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0008c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00090
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00098
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0009c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000a0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000a4
			 0x7f800000,                                                  // inf                                         /// 000a8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 000ac
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000b0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 000b4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000bc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000c0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000c4
			 0x3f028f5c,                                                  // 0.51                                        /// 000c8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 000cc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000d0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000d4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000dc
			 0xffc00001,                                                  // -signaling NaN                              /// 000e0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000e4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000e8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000f8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 000fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00104
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00108
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0010c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00110
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00114
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00118
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0011c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00120
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00124
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0x00011111,                                                  // 9.7958E-41                                  /// 0012c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00130
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00134
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00138
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0013c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00140
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00144
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00148
			 0xcb000000,                                                  // -8388608.0                                  /// 0014c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00150
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00158
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0015c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00160
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00164
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00168
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0016c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00170
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00174
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00178
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0017c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00180
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00184
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00188
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0018c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00190
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00194
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00198
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0019c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001a0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 001a4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001a8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001ac
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 001b0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001b4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001b8
			 0x55555555,                                                  // 4 random values                             /// 001bc
			 0x4b000000,                                                  // 8388608.0                                   /// 001c0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 001c4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001cc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001d0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 001d4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 001dc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001e0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001e4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 001ec
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001f0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001f4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 001f8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001fc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00200
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00204
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00208
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0020c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00210
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00214
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00218
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0021c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00220
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00224
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00228
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0022c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00230
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00234
			 0x80000000,                                                  // -zero                                       /// 00238
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00240
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00244
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00248
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0024c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00250
			 0x33333333,                                                  // 4 random values                             /// 00254
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00258
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0025c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00260
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00264
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00268
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0026c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00270
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00274
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00278
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0027c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00280
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00284
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00288
			 0x0c780000,                                                  // Leading 1s:                                 /// 0028c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00290
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00298
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0029c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 002a0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 002a4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002a8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002ac
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 002b0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002b4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002b8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 002bc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 002c8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002cc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002d0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 002d4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 002d8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002dc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 002e0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002e4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 002e8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 002ec
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002f4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002f8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00300
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00304
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00308
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0030c
			 0x33333333,                                                  // 4 random values                             /// 00310
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00314
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00318
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0031c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00320
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00324
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00328
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0032c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00330
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00334
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00338
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0033c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00340
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00344
			 0x7fc00001,                                                  // signaling NaN                               /// 00348
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0034c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00350
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00354
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00358
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0035c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00360
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00364
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00368
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00370
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00374
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00378
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0037c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00384
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0038c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00390
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00394
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0039c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003a4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003a8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003b0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 003b4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003b8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003bc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003c0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003c4
			 0x0c780000,                                                  // Leading 1s:                                 /// 003c8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 003cc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003d4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 003d8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 003dc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 003e0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003e4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 003e8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 003ec
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 003f0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 003f4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0x0e000007,                                                  // Trailing 1s:                                /// 003fc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00400
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00404
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00408
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00414
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00418
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0041c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00420
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00424
			 0xbf028f5c,                                                  // -0.51                                       /// 00428
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0042c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00430
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00434
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00438
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00440
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00444
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00448
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0044c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00450
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00454
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0045c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00460
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00464
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00468
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0046c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00470
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00474
			 0x33333333,                                                  // 4 random values                             /// 00478
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0047c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00480
			 0x0c600000,                                                  // Leading 1s:                                 /// 00484
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00488
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0048c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00490
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00494
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00498
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0049c
			 0x80011111,                                                  // -9.7958E-41                                 /// 004a0
			 0x55555555,                                                  // 4 random values                             /// 004a4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 004ac
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004b0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004b4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 004bc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004c0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 004c4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c8
			 0x0c400000,                                                  // Leading 1s:                                 /// 004cc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004d4
			 0x0c400000,                                                  // Leading 1s:                                 /// 004d8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 004e0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 004e4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 004e8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 004ec
			 0xff800000,                                                  // -inf                                        /// 004f0
			 0x0e000007,                                                  // Trailing 1s:                                /// 004f4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004f8
			 0x00011111,                                                  // 9.7958E-41                                  /// 004fc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00500
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00504
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0050c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00510
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00514
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0051c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00520
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00524
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00528
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0052c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00530
			 0x0c700000,                                                  // Leading 1s:                                 /// 00534
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00538
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00540
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00548
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0054c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00550
			 0x80000000,                                                  // -zero                                       /// 00554
			 0xcb000000,                                                  // -8388608.0                                  /// 00558
			 0x0c700000,                                                  // Leading 1s:                                 /// 0055c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00560
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00564
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00568
			 0xbf028f5c,                                                  // -0.51                                       /// 0056c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00570
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00574
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00578
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0057c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00580
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00584
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00588
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0058c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00590
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00594
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00598
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0059c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005a0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005a4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 005a8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005ac
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 005b0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005bc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 005c4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 005c8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005cc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005d0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005d4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0x0c600000,                                                  // Leading 1s:                                 /// 005dc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005e8
			 0x0e000007,                                                  // Trailing 1s:                                /// 005ec
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005f0
			 0x80000000,                                                  // -zero                                       /// 005f4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005f8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 005fc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00600
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00604
			 0x0c700000,                                                  // Leading 1s:                                 /// 00608
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0060c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00610
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00614
			 0xff800000,                                                  // -inf                                        /// 00618
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00620
			 0x0e000003,                                                  // Trailing 1s:                                /// 00624
			 0x55555555,                                                  // 4 random values                             /// 00628
			 0x80000000,                                                  // -zero                                       /// 0062c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00630
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00638
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0063c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00640
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00644
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00648
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0064c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00650
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00654
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00658
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00660
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00668
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0066c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00678
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0067c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00680
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00684
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00688
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0068c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00690
			 0x0e000003,                                                  // Trailing 1s:                                /// 00694
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00698
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 006a0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006a4
			 0x0e000007,                                                  // Trailing 1s:                                /// 006a8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006ac
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006b4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006b8
			 0x7fc00001,                                                  // signaling NaN                               /// 006bc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 006c0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006c4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006cc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 006d0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 006d8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006dc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 006e0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006e8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 006ec
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006f0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006f4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006f8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006fc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00700
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00704
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0070c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00710
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00714
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00718
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0071c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00728
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00730
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00734
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0x0c400000,                                                  // Leading 1s:                                 /// 0073c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00740
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00744
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00748
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0x55555555,                                                  // 4 random values                             /// 00750
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00754
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00758
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0075c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00760
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00764
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00768
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0076c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00770
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00774
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00778
			 0x3f028f5c,                                                  // 0.51                                        /// 0077c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00780
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00784
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00794
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0079c
			 0x0c700000,                                                  // Leading 1s:                                 /// 007a0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007a8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 007ac
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 007b8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007bc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007c0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007c8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 007cc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007d0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007d4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007d8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007dc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007e0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007e4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007e8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007ec
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007f4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 007f8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007fc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00804
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00808
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0080c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00810
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00814
			 0x7fc00001,                                                  // signaling NaN                               /// 00818
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0081c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00820
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00824
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00828
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0082c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00830
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00834
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00838
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0083c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00844
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0084c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00850
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0085c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00860
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00864
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00868
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0086c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00870
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00874
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00878
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00880
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00884
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0088c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00890
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00894
			 0x0e000007,                                                  // Trailing 1s:                                /// 00898
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0089c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008a8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 008ac
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008b0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008b4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008b8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008bc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0xff800000,                                                  // -inf                                        /// 008c4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 008c8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008cc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008d0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 008d4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 008d8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 008dc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008e4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008e8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008ec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008f0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008f4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008fc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00900
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00904
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00908
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0090c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00910
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00914
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00918
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0091c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00920
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00924
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00928
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0092c
			 0xcb000000,                                                  // -8388608.0                                  /// 00930
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00934
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00938
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0093c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00940
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00948
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0094c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00950
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00954
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00958
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0095c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00964
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00968
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0096c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00970
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00974
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00978
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0097c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00980
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00984
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00988
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00990
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00994
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00998
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0099c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 009a8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009b0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b4
			 0xcb000000,                                                  // -8388608.0                                  /// 009b8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009bc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 009c4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 009cc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009d0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009d4
			 0x0c400000,                                                  // Leading 1s:                                 /// 009d8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009dc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0xff800000,                                                  // -inf                                        /// 009e4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 009e8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009ec
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009f0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009f4
			 0x0e000007,                                                  // Trailing 1s:                                /// 009f8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009fc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a00
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a04
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a08
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a0c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a10
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a14
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a18
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a1c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a20
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a24
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a2c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a30
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a34
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a3c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a40
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a44
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a48
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a4c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a50
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a58
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a5c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a60
			 0x55555555,                                                  // 4 random values                             /// 00a64
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a68
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a6c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a70
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a74
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a78
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a7c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a80
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0xcb000000,                                                  // -8388608.0                                  /// 00a88
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a8c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a90
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a94
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a98
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a9c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00aa0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00aa8
			 0xffc00001,                                                  // -signaling NaN                              /// 00aac
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ab0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ab4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ab8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00abc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ac4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00acc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ad0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ad4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ad8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00adc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ae0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ae4
			 0x7f800000,                                                  // inf                                         /// 00ae8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00aec
			 0x3f028f5c,                                                  // 0.51                                        /// 00af0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00af4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00af8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00afc
			 0x3f028f5c,                                                  // 0.51                                        /// 00b00
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b04
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b08
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b0c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b10
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b14
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b18
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b1c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b20
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b28
			 0xffc00001,                                                  // -signaling NaN                              /// 00b2c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b30
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b34
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b38
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b40
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b44
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b48
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b4c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b50
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b54
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b58
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b5c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b64
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b68
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b6c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b70
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b74
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b80
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b84
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b88
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b8c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b90
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b94
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b98
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b9c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ba4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ba8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00bb0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00bb8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00bc0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00bc4
			 0x3f028f5c,                                                  // 0.51                                        /// 00bc8
			 0x3f028f5c,                                                  // 0.51                                        /// 00bcc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00bd0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bd8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bdc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0xffc00001,                                                  // -signaling NaN                              /// 00be4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00be8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00bf0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00bf8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c00
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c04
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c08
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c0c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c14
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c18
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c1c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x3f028f5c,                                                  // 0.51                                        /// 00c24
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c28
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c30
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c34
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c38
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c40
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c48
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c50
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c54
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c58
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c5c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c60
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c64
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c70
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c74
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c78
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c7c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c80
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c84
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c88
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c8c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c90
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c94
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c9c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ca0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ca4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ca8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00cac
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00cb4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cbc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00cc8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ccc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cd0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00cd8
			 0x80000000,                                                  // -zero                                       /// 00cdc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ce0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ce4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ce8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00cf0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00cf4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00cfc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d00
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d04
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d08
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d0c
			 0x33333333,                                                  // 4 random values                             /// 00d10
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d14
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d18
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d20
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d28
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d2c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d30
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d34
			 0x55555555,                                                  // 4 random values                             /// 00d38
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d40
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d44
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d48
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d4c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d50
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d54
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d58
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d5c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d60
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d64
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d6c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0x3f028f5c,                                                  // 0.51                                        /// 00d74
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d78
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d7c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d80
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d84
			 0xbf028f5c,                                                  // -0.51                                       /// 00d88
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d8c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d90
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d94
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d98
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d9c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00da0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00da4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00dac
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00db0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00db4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00db8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00dbc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc0
			 0x33333333,                                                  // 4 random values                             /// 00dc4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00dc8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dcc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00dd0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0x00000000,                                                  // zero                                        /// 00dd8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ddc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00de8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00dec
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00df0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00df8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dfc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e00
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e04
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e08
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e0c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e10
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e14
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e18
			 0x3f028f5c,                                                  // 0.51                                        /// 00e1c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e20
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e24
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e28
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e2c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e30
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e34
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e3c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e40
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e44
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e48
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e4c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e50
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e54
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e58
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e5c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e60
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e64
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e68
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e6c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0x80000000,                                                  // -zero                                       /// 00e74
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e7c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e84
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e88
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e8c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e90
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e94
			 0x3f028f5c,                                                  // 0.51                                        /// 00e98
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e9c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ea0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ea8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00eac
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00eb4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ebc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0xcb000000,                                                  // -8388608.0                                  /// 00ec8
			 0x3f028f5c,                                                  // 0.51                                        /// 00ecc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ed0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ed4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ed8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00edc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ee0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ee4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00eec
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ef0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ef4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00efc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f00
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f04
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f08
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f14
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f1c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f20
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f24
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f28
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f2c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f30
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f34
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f40
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f44
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f48
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f4c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f54
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f58
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f5c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f60
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f64
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f68
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f6c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0xcb000000,                                                  // -8388608.0                                  /// 00f78
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f7c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f80
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f84
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f88
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f8c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f90
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f94
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f98
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f9c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fa0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fa4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fa8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00fac
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00fb0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00fb4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00fbc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00fc0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fc4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00fc8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fcc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00fe0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00fe8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fec
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ff0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ff4
			 0x4b000000,                                                  // 8388608.0                                   /// 00ff8
			 0x00100000                                                  // SP - 1 bit alone set in mantissa +ve        /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0xff5ea673, /// 0x0
			 0x760cff0c, /// 0x4
			 0x707826a2, /// 0x8
			 0x21182740, /// 0xc
			 0xbc5fedef, /// 0x10
			 0xe8526ce6, /// 0x14
			 0x50c6e968, /// 0x18
			 0x3e0de0c5, /// 0x1c
			 0x7002bbb3, /// 0x20
			 0xfa146766, /// 0x24
			 0x4cc378e2, /// 0x28
			 0x51f4ab0e, /// 0x2c
			 0xef1319d8, /// 0x30
			 0xe8926c03, /// 0x34
			 0x746b0b90, /// 0x38
			 0xf832dff3, /// 0x3c
			 0x835bc39a, /// 0x40
			 0x06b14b4e, /// 0x44
			 0x16ebe394, /// 0x48
			 0x81b2fb43, /// 0x4c
			 0xf774d80d, /// 0x50
			 0xd9875a1a, /// 0x54
			 0x90c3fcc0, /// 0x58
			 0x9b374243, /// 0x5c
			 0x981ccd00, /// 0x60
			 0x792521a5, /// 0x64
			 0xa009aeef, /// 0x68
			 0x33b19e90, /// 0x6c
			 0xfd98373a, /// 0x70
			 0x42b40bac, /// 0x74
			 0xcc1e84da, /// 0x78
			 0x709c107f, /// 0x7c
			 0x26559ab7, /// 0x80
			 0xe666ad85, /// 0x84
			 0xd4c4f6e0, /// 0x88
			 0x26d4c097, /// 0x8c
			 0xc749df9e, /// 0x90
			 0xd61a7b43, /// 0x94
			 0xc2f85dbf, /// 0x98
			 0xf5953d50, /// 0x9c
			 0xdf06a932, /// 0xa0
			 0x70767ff7, /// 0xa4
			 0x43ca1dd0, /// 0xa8
			 0x7367aae6, /// 0xac
			 0x5607419b, /// 0xb0
			 0xf58a2b3b, /// 0xb4
			 0x0b631de1, /// 0xb8
			 0xf6a147d4, /// 0xbc
			 0x0259bbf4, /// 0xc0
			 0xb88dfddd, /// 0xc4
			 0xbd528e0c, /// 0xc8
			 0xc85d62f2, /// 0xcc
			 0x52ee7aa2, /// 0xd0
			 0xb665896c, /// 0xd4
			 0x0f95fe03, /// 0xd8
			 0x6b33f349, /// 0xdc
			 0x59b550b7, /// 0xe0
			 0xffac8621, /// 0xe4
			 0xad4c1108, /// 0xe8
			 0x5a5fbf06, /// 0xec
			 0x23d3d9d2, /// 0xf0
			 0xd6ad2442, /// 0xf4
			 0x4e942ff8, /// 0xf8
			 0xe2deddc5, /// 0xfc
			 0xa7bc2dfa, /// 0x100
			 0x6aa74a72, /// 0x104
			 0xdb90d89f, /// 0x108
			 0x8ff4c32e, /// 0x10c
			 0xe36d8598, /// 0x110
			 0x4f70a168, /// 0x114
			 0x6c2de6e9, /// 0x118
			 0x8b9807c7, /// 0x11c
			 0x6e242cf7, /// 0x120
			 0x9af5ac91, /// 0x124
			 0x8ba732b4, /// 0x128
			 0xef8219c2, /// 0x12c
			 0x8c28a87f, /// 0x130
			 0xcc39444c, /// 0x134
			 0xefd6917d, /// 0x138
			 0x4e37fd4c, /// 0x13c
			 0x4690b11b, /// 0x140
			 0x44c21f11, /// 0x144
			 0x828fff34, /// 0x148
			 0xb2c41e4d, /// 0x14c
			 0xdd53d8c9, /// 0x150
			 0x77b77192, /// 0x154
			 0x5cbbf4b8, /// 0x158
			 0xcc86e4c7, /// 0x15c
			 0xff366801, /// 0x160
			 0x426ac6bd, /// 0x164
			 0xccdcc106, /// 0x168
			 0xcfc066ca, /// 0x16c
			 0x4706044f, /// 0x170
			 0xce633cea, /// 0x174
			 0xe075b5aa, /// 0x178
			 0xfcef8cbd, /// 0x17c
			 0x6aca8134, /// 0x180
			 0x75381b5d, /// 0x184
			 0x07e8f1b4, /// 0x188
			 0xa1ef2deb, /// 0x18c
			 0xe7ce5dac, /// 0x190
			 0x1667241f, /// 0x194
			 0xb645bd93, /// 0x198
			 0x2a878a3b, /// 0x19c
			 0xc05ac58e, /// 0x1a0
			 0x79c3009e, /// 0x1a4
			 0xd4b92cc3, /// 0x1a8
			 0x56731bf8, /// 0x1ac
			 0xc5749bc4, /// 0x1b0
			 0x6f12029e, /// 0x1b4
			 0x4e09c787, /// 0x1b8
			 0xd2aa7da4, /// 0x1bc
			 0x68b01d5d, /// 0x1c0
			 0x7d7ea1ef, /// 0x1c4
			 0x2e739d2b, /// 0x1c8
			 0x3c0d7b29, /// 0x1cc
			 0xa7bfec33, /// 0x1d0
			 0x4a13d29c, /// 0x1d4
			 0xad6403f1, /// 0x1d8
			 0x62322ce1, /// 0x1dc
			 0xac9fe527, /// 0x1e0
			 0xf6b764c2, /// 0x1e4
			 0xfd2ee804, /// 0x1e8
			 0xac609ced, /// 0x1ec
			 0x382f56f9, /// 0x1f0
			 0x5f17631b, /// 0x1f4
			 0x523cfba5, /// 0x1f8
			 0x865c4447, /// 0x1fc
			 0x229ee893, /// 0x200
			 0xa8e3a7f2, /// 0x204
			 0xd17fb663, /// 0x208
			 0xbcf9adbb, /// 0x20c
			 0xbcdfc9f7, /// 0x210
			 0x41558f90, /// 0x214
			 0xb8573573, /// 0x218
			 0x611e8875, /// 0x21c
			 0x8fb9df14, /// 0x220
			 0xe0354d62, /// 0x224
			 0x39e30ce3, /// 0x228
			 0xb519ee1b, /// 0x22c
			 0x02ff3c7c, /// 0x230
			 0x36fd3a6b, /// 0x234
			 0x72c46e56, /// 0x238
			 0x07bcb0ce, /// 0x23c
			 0x2c897d25, /// 0x240
			 0xc54335ee, /// 0x244
			 0xee4e6669, /// 0x248
			 0x66556734, /// 0x24c
			 0x0999d9c1, /// 0x250
			 0x08dc51cf, /// 0x254
			 0x89442623, /// 0x258
			 0xdc5b9745, /// 0x25c
			 0x4905a3c3, /// 0x260
			 0x344547e1, /// 0x264
			 0x24bf1fe9, /// 0x268
			 0x829f83ef, /// 0x26c
			 0xea4347ea, /// 0x270
			 0x0c45591b, /// 0x274
			 0x67e4b8d0, /// 0x278
			 0x1921e888, /// 0x27c
			 0x4aa5f44a, /// 0x280
			 0xc9c21c59, /// 0x284
			 0xb64633fd, /// 0x288
			 0x80603f62, /// 0x28c
			 0xf0a3aa05, /// 0x290
			 0xf4450a72, /// 0x294
			 0xc1a4f428, /// 0x298
			 0xa2ee33f0, /// 0x29c
			 0xa0edd355, /// 0x2a0
			 0x0f2b209f, /// 0x2a4
			 0xaf55cc0b, /// 0x2a8
			 0x22d48302, /// 0x2ac
			 0x26f93086, /// 0x2b0
			 0x73628f40, /// 0x2b4
			 0x7c2fee10, /// 0x2b8
			 0xdbd29ce6, /// 0x2bc
			 0x3aa187ea, /// 0x2c0
			 0x97449edb, /// 0x2c4
			 0xff5bbfe8, /// 0x2c8
			 0xeae78763, /// 0x2cc
			 0xfb2136b2, /// 0x2d0
			 0xd46815ec, /// 0x2d4
			 0xb0ff3702, /// 0x2d8
			 0x699dcb44, /// 0x2dc
			 0xab5666b3, /// 0x2e0
			 0xe82a3b79, /// 0x2e4
			 0x4beeaa0d, /// 0x2e8
			 0xf10dd831, /// 0x2ec
			 0x5a5f3674, /// 0x2f0
			 0xef0de15a, /// 0x2f4
			 0x5eff836a, /// 0x2f8
			 0x69fc733c, /// 0x2fc
			 0x0da703e0, /// 0x300
			 0x0d5110b2, /// 0x304
			 0xb55bf318, /// 0x308
			 0xbeb7cfcb, /// 0x30c
			 0xbbbf40bf, /// 0x310
			 0x410ee76d, /// 0x314
			 0x7593f2a2, /// 0x318
			 0x5c39a255, /// 0x31c
			 0x7078961b, /// 0x320
			 0xd6971e36, /// 0x324
			 0x9d75d9aa, /// 0x328
			 0x0bb30540, /// 0x32c
			 0xf6dacae8, /// 0x330
			 0x49287873, /// 0x334
			 0xe5001483, /// 0x338
			 0x8cc53afa, /// 0x33c
			 0xa4dc6bc8, /// 0x340
			 0x55a9fe28, /// 0x344
			 0x205e4adb, /// 0x348
			 0x2d1e1520, /// 0x34c
			 0xb8a5bc21, /// 0x350
			 0xb4ecc7e7, /// 0x354
			 0x08dbe98a, /// 0x358
			 0x0a87066b, /// 0x35c
			 0xa717ade4, /// 0x360
			 0xae1c0a7e, /// 0x364
			 0xd9a995d7, /// 0x368
			 0xa496fbd9, /// 0x36c
			 0x4c7b26ea, /// 0x370
			 0x3c1d4ac5, /// 0x374
			 0x222bd4ad, /// 0x378
			 0x21380ec2, /// 0x37c
			 0xcfa17065, /// 0x380
			 0xa2126e01, /// 0x384
			 0xb19cf90e, /// 0x388
			 0x22e1441d, /// 0x38c
			 0x5522c371, /// 0x390
			 0x47e294c7, /// 0x394
			 0x8dd89cba, /// 0x398
			 0x1d7f0b62, /// 0x39c
			 0xe033435e, /// 0x3a0
			 0x5ea67bf3, /// 0x3a4
			 0x127c8087, /// 0x3a8
			 0xf8ba0b8f, /// 0x3ac
			 0x3e3988cc, /// 0x3b0
			 0x8a40da7d, /// 0x3b4
			 0x15b71aa8, /// 0x3b8
			 0x1002214b, /// 0x3bc
			 0xeec08c2a, /// 0x3c0
			 0x24c9f85a, /// 0x3c4
			 0xd23f57d2, /// 0x3c8
			 0x42b1afc6, /// 0x3cc
			 0xbcfa1d84, /// 0x3d0
			 0x4ecff9a1, /// 0x3d4
			 0x67ced2eb, /// 0x3d8
			 0x3f0456b4, /// 0x3dc
			 0xb61657af, /// 0x3e0
			 0xc4e576c2, /// 0x3e4
			 0x409ca6fd, /// 0x3e8
			 0x0ffe448b, /// 0x3ec
			 0x668f95b5, /// 0x3f0
			 0xa7409db1, /// 0x3f4
			 0xda7d9205, /// 0x3f8
			 0xea0848ed, /// 0x3fc
			 0xc8a86843, /// 0x400
			 0x8304df15, /// 0x404
			 0xfcd3117c, /// 0x408
			 0x9d748bb7, /// 0x40c
			 0x881ef313, /// 0x410
			 0xedd3a4a6, /// 0x414
			 0x127c28e8, /// 0x418
			 0xe64adb39, /// 0x41c
			 0x75c66f36, /// 0x420
			 0x338a56b1, /// 0x424
			 0xde7162f9, /// 0x428
			 0x237c2f17, /// 0x42c
			 0xae892111, /// 0x430
			 0x78ca0a1a, /// 0x434
			 0x63732393, /// 0x438
			 0x610900a6, /// 0x43c
			 0x4dd10d28, /// 0x440
			 0x4f6c758a, /// 0x444
			 0xf296883f, /// 0x448
			 0xba5af85f, /// 0x44c
			 0x36daa1a5, /// 0x450
			 0x2d9d8f1d, /// 0x454
			 0x2d626d94, /// 0x458
			 0xebbc57fd, /// 0x45c
			 0x939c7496, /// 0x460
			 0x03aed028, /// 0x464
			 0x27a5ecb0, /// 0x468
			 0xead469a9, /// 0x46c
			 0x3a667ff5, /// 0x470
			 0xef4467f2, /// 0x474
			 0x5c9ba608, /// 0x478
			 0x0719a4d4, /// 0x47c
			 0x02266439, /// 0x480
			 0x4bd1215c, /// 0x484
			 0x1347800f, /// 0x488
			 0xb6b86cff, /// 0x48c
			 0x0e13bae0, /// 0x490
			 0x7d15a0e7, /// 0x494
			 0xf794657b, /// 0x498
			 0x6dfdbcde, /// 0x49c
			 0x3043b672, /// 0x4a0
			 0x55e76710, /// 0x4a4
			 0x2c3e3501, /// 0x4a8
			 0x63079c1f, /// 0x4ac
			 0xcf2a7f54, /// 0x4b0
			 0xb9b8967d, /// 0x4b4
			 0x4c3cbc73, /// 0x4b8
			 0x8edc0a42, /// 0x4bc
			 0x4431c02b, /// 0x4c0
			 0xafa6eeae, /// 0x4c4
			 0x1d3ee132, /// 0x4c8
			 0xf49aa471, /// 0x4cc
			 0x455c494f, /// 0x4d0
			 0xbac904a7, /// 0x4d4
			 0xdd321e60, /// 0x4d8
			 0x482b325f, /// 0x4dc
			 0x2e9c9192, /// 0x4e0
			 0x83cc68f1, /// 0x4e4
			 0x691cfa7f, /// 0x4e8
			 0x3fb2aeb2, /// 0x4ec
			 0xdd21adf3, /// 0x4f0
			 0x99c637e6, /// 0x4f4
			 0x9345d2d6, /// 0x4f8
			 0x5e3a1854, /// 0x4fc
			 0xe3b11193, /// 0x500
			 0x0ef3547f, /// 0x504
			 0xf7ffc038, /// 0x508
			 0xbd7e572d, /// 0x50c
			 0xb621f803, /// 0x510
			 0xb838bda6, /// 0x514
			 0xefa5434d, /// 0x518
			 0x079b2e7b, /// 0x51c
			 0x80ba0345, /// 0x520
			 0x19f263bf, /// 0x524
			 0x45cc40ff, /// 0x528
			 0x20036d76, /// 0x52c
			 0x76790492, /// 0x530
			 0x39956d20, /// 0x534
			 0x6fcc4c8a, /// 0x538
			 0xca291929, /// 0x53c
			 0x685294eb, /// 0x540
			 0xe46d9552, /// 0x544
			 0xe048932b, /// 0x548
			 0x07e24c20, /// 0x54c
			 0x441d0777, /// 0x550
			 0x665f9553, /// 0x554
			 0x496be6c0, /// 0x558
			 0x40f09346, /// 0x55c
			 0xb7d74b8d, /// 0x560
			 0x2e963c7a, /// 0x564
			 0x0e22b881, /// 0x568
			 0x38b16d97, /// 0x56c
			 0x579fc367, /// 0x570
			 0xf1e0c929, /// 0x574
			 0x172636ba, /// 0x578
			 0x2c0ec1ac, /// 0x57c
			 0xf2e3d66c, /// 0x580
			 0xfd3c72f3, /// 0x584
			 0x5d3c4199, /// 0x588
			 0xab832232, /// 0x58c
			 0x7ab62596, /// 0x590
			 0x6808837f, /// 0x594
			 0x1f66f048, /// 0x598
			 0x399d0afd, /// 0x59c
			 0x2ea199f8, /// 0x5a0
			 0x87a5d5b3, /// 0x5a4
			 0x7966c1ca, /// 0x5a8
			 0x18dbb06c, /// 0x5ac
			 0xf64c5664, /// 0x5b0
			 0x40bb4ec3, /// 0x5b4
			 0x1c0501cc, /// 0x5b8
			 0x20f28d38, /// 0x5bc
			 0x8becbb19, /// 0x5c0
			 0x923967d7, /// 0x5c4
			 0x3cb89c0d, /// 0x5c8
			 0x5a06a343, /// 0x5cc
			 0x16b5de8f, /// 0x5d0
			 0xf3d623cd, /// 0x5d4
			 0x1f92f44d, /// 0x5d8
			 0x53af14a1, /// 0x5dc
			 0x1d8283da, /// 0x5e0
			 0x659df884, /// 0x5e4
			 0xb8021395, /// 0x5e8
			 0x7c31270b, /// 0x5ec
			 0x1386ee70, /// 0x5f0
			 0xc8a6a13b, /// 0x5f4
			 0xe3323920, /// 0x5f8
			 0x6633ad56, /// 0x5fc
			 0xa0f74eac, /// 0x600
			 0xb31cbd89, /// 0x604
			 0x6b1840b6, /// 0x608
			 0xdec9a7c3, /// 0x60c
			 0x3247ec1d, /// 0x610
			 0xde31a10f, /// 0x614
			 0xd8e600b4, /// 0x618
			 0x1f197678, /// 0x61c
			 0x43fa516b, /// 0x620
			 0xb8a4bc73, /// 0x624
			 0xb7a087ae, /// 0x628
			 0xd3e7d161, /// 0x62c
			 0x2a2db482, /// 0x630
			 0xe7aa5842, /// 0x634
			 0xffa801f0, /// 0x638
			 0xd23c050c, /// 0x63c
			 0xc3e42d86, /// 0x640
			 0xdf1b5705, /// 0x644
			 0x01b4bf4b, /// 0x648
			 0xfad98e4c, /// 0x64c
			 0xa51d08c1, /// 0x650
			 0xba1de557, /// 0x654
			 0x1df34466, /// 0x658
			 0x04edce9d, /// 0x65c
			 0x8dda02fa, /// 0x660
			 0xe551ebff, /// 0x664
			 0xc670c578, /// 0x668
			 0xcb3ae0cc, /// 0x66c
			 0xfafec745, /// 0x670
			 0xcf518a38, /// 0x674
			 0xf4412d99, /// 0x678
			 0x01510412, /// 0x67c
			 0x27bb0447, /// 0x680
			 0x11c862ca, /// 0x684
			 0x06f897dd, /// 0x688
			 0x0a87baae, /// 0x68c
			 0x9d981409, /// 0x690
			 0x07c3aa13, /// 0x694
			 0x7c3d5db9, /// 0x698
			 0xf57ea85c, /// 0x69c
			 0xc468a7c0, /// 0x6a0
			 0x541501cd, /// 0x6a4
			 0x7b88200c, /// 0x6a8
			 0x8908e38b, /// 0x6ac
			 0xeb9ec34c, /// 0x6b0
			 0x5dfcc3df, /// 0x6b4
			 0x3e17d680, /// 0x6b8
			 0x37970201, /// 0x6bc
			 0x9da473e4, /// 0x6c0
			 0x2b89b949, /// 0x6c4
			 0x088fbee6, /// 0x6c8
			 0xdc5986a3, /// 0x6cc
			 0x9f8c7aef, /// 0x6d0
			 0x14775ed6, /// 0x6d4
			 0x0e0d09dc, /// 0x6d8
			 0xdda291ee, /// 0x6dc
			 0x3900adb6, /// 0x6e0
			 0x20530ad0, /// 0x6e4
			 0x600e2f06, /// 0x6e8
			 0xdc8da3d6, /// 0x6ec
			 0xed5a9f7f, /// 0x6f0
			 0x3a80dede, /// 0x6f4
			 0xaf7eecbb, /// 0x6f8
			 0xdc182d7c, /// 0x6fc
			 0x8ab65d87, /// 0x700
			 0x2f6a4534, /// 0x704
			 0x44111eeb, /// 0x708
			 0xda7bfac6, /// 0x70c
			 0xd0e78461, /// 0x710
			 0xc5c270ac, /// 0x714
			 0xbf910b4e, /// 0x718
			 0x47b812ff, /// 0x71c
			 0xf4e0caca, /// 0x720
			 0x6d2d8ad9, /// 0x724
			 0x4b1f90eb, /// 0x728
			 0x626cc365, /// 0x72c
			 0x0b63347b, /// 0x730
			 0xc93425bd, /// 0x734
			 0x8eda526a, /// 0x738
			 0xd55ea803, /// 0x73c
			 0xbdd3198e, /// 0x740
			 0xc4176f5f, /// 0x744
			 0xfbd7b052, /// 0x748
			 0xe1a135e0, /// 0x74c
			 0x812cf3eb, /// 0x750
			 0xcfb0e6a0, /// 0x754
			 0x2c445112, /// 0x758
			 0x992f0677, /// 0x75c
			 0x1c47591d, /// 0x760
			 0xa4554b27, /// 0x764
			 0x5763362f, /// 0x768
			 0xd6da5432, /// 0x76c
			 0xc6873546, /// 0x770
			 0xcaac8e37, /// 0x774
			 0x8ce1af6d, /// 0x778
			 0xd5d395da, /// 0x77c
			 0x848b34ff, /// 0x780
			 0x0449847a, /// 0x784
			 0xf49cfd3c, /// 0x788
			 0x1a24b669, /// 0x78c
			 0xd892640a, /// 0x790
			 0x250af065, /// 0x794
			 0x96bbe8c0, /// 0x798
			 0x58588d22, /// 0x79c
			 0x4b892cf1, /// 0x7a0
			 0xf6582d0a, /// 0x7a4
			 0x515c3425, /// 0x7a8
			 0x05a2a1ca, /// 0x7ac
			 0x96f88a29, /// 0x7b0
			 0xf881b874, /// 0x7b4
			 0xa0f50c93, /// 0x7b8
			 0x6d292345, /// 0x7bc
			 0x96226c92, /// 0x7c0
			 0x1da34711, /// 0x7c4
			 0xa834989b, /// 0x7c8
			 0x61fa9c80, /// 0x7cc
			 0x03d1e986, /// 0x7d0
			 0xbeb5c23a, /// 0x7d4
			 0xcd28bb89, /// 0x7d8
			 0xdb9dd2a0, /// 0x7dc
			 0x59e8573e, /// 0x7e0
			 0x197b9159, /// 0x7e4
			 0x751acfdb, /// 0x7e8
			 0x44e475ec, /// 0x7ec
			 0x3adcbdda, /// 0x7f0
			 0x5a4d6d49, /// 0x7f4
			 0x5f94b43e, /// 0x7f8
			 0x9006bd88, /// 0x7fc
			 0xb131bd18, /// 0x800
			 0x5c0f2397, /// 0x804
			 0xbbfb1b23, /// 0x808
			 0x1d9cd335, /// 0x80c
			 0xdf8276b5, /// 0x810
			 0x564b92ee, /// 0x814
			 0xe77e97ba, /// 0x818
			 0xf2f30bd7, /// 0x81c
			 0xc43ab079, /// 0x820
			 0x2f21abd3, /// 0x824
			 0xd1ca353c, /// 0x828
			 0xdfa973f4, /// 0x82c
			 0xd6dbedc7, /// 0x830
			 0x5a619ee6, /// 0x834
			 0xa01ffac5, /// 0x838
			 0xf2135258, /// 0x83c
			 0x8b4572d7, /// 0x840
			 0xb3d4918b, /// 0x844
			 0x8b95013b, /// 0x848
			 0xe382f963, /// 0x84c
			 0xc4dc444d, /// 0x850
			 0x799af5c3, /// 0x854
			 0x54fea6b5, /// 0x858
			 0x30d09561, /// 0x85c
			 0x88ea3746, /// 0x860
			 0x90ee95e9, /// 0x864
			 0x65d15487, /// 0x868
			 0xc4ddc21a, /// 0x86c
			 0x211bf9c3, /// 0x870
			 0xb6444aa4, /// 0x874
			 0xe0fd4ecf, /// 0x878
			 0x107bfa28, /// 0x87c
			 0x3e6418a6, /// 0x880
			 0x23047c27, /// 0x884
			 0x51a17eb9, /// 0x888
			 0xa681225b, /// 0x88c
			 0x15d5368d, /// 0x890
			 0x3d08603a, /// 0x894
			 0x61e6091d, /// 0x898
			 0x22f9388a, /// 0x89c
			 0x5a5eb7e8, /// 0x8a0
			 0x2e9f1c51, /// 0x8a4
			 0x7e15a298, /// 0x8a8
			 0x10556941, /// 0x8ac
			 0x9066a4e0, /// 0x8b0
			 0xb05c5f36, /// 0x8b4
			 0x2aaa880a, /// 0x8b8
			 0xf2d57b7d, /// 0x8bc
			 0xafa5d72d, /// 0x8c0
			 0x137156b8, /// 0x8c4
			 0x032ab6d0, /// 0x8c8
			 0x0e9870b4, /// 0x8cc
			 0x698f4648, /// 0x8d0
			 0x7090a696, /// 0x8d4
			 0x03d7fd76, /// 0x8d8
			 0x63611ee9, /// 0x8dc
			 0x42f6eac9, /// 0x8e0
			 0xb9ba3afe, /// 0x8e4
			 0x096d352f, /// 0x8e8
			 0x4da37508, /// 0x8ec
			 0x61d745ac, /// 0x8f0
			 0xc8211694, /// 0x8f4
			 0x7969ee8a, /// 0x8f8
			 0x55fec022, /// 0x8fc
			 0x819def4b, /// 0x900
			 0x7f3baba4, /// 0x904
			 0xc92068d4, /// 0x908
			 0xe24416bd, /// 0x90c
			 0xe0a81b61, /// 0x910
			 0xf7097ac9, /// 0x914
			 0x88bf60e0, /// 0x918
			 0x82d98362, /// 0x91c
			 0x14afeb6e, /// 0x920
			 0x12814dc8, /// 0x924
			 0xfe1c8363, /// 0x928
			 0xfd669d5c, /// 0x92c
			 0x874994d6, /// 0x930
			 0x9483081a, /// 0x934
			 0x5c14547e, /// 0x938
			 0x644cc7ba, /// 0x93c
			 0x96d8b9f5, /// 0x940
			 0xa7a4059b, /// 0x944
			 0x8b8c2f3f, /// 0x948
			 0xa8e227f4, /// 0x94c
			 0x40ae3f8c, /// 0x950
			 0xdf314bc0, /// 0x954
			 0xc3c50023, /// 0x958
			 0xded6b294, /// 0x95c
			 0xb8bcd699, /// 0x960
			 0x75e9afe3, /// 0x964
			 0xa160c28a, /// 0x968
			 0xb8c37438, /// 0x96c
			 0xc2290583, /// 0x970
			 0xd1f59302, /// 0x974
			 0xb0045c40, /// 0x978
			 0x49f6f09c, /// 0x97c
			 0x8739291d, /// 0x980
			 0x5d9badf4, /// 0x984
			 0x804afd12, /// 0x988
			 0xe5fbe7fe, /// 0x98c
			 0x9fc49883, /// 0x990
			 0x5ad26ad6, /// 0x994
			 0x7d5f7ac8, /// 0x998
			 0xc7ee83fe, /// 0x99c
			 0x24571ecf, /// 0x9a0
			 0xa841af4a, /// 0x9a4
			 0x5ef43b2b, /// 0x9a8
			 0x02fa8228, /// 0x9ac
			 0xaa6235c8, /// 0x9b0
			 0x1e4e6d4a, /// 0x9b4
			 0x435f6f82, /// 0x9b8
			 0x482d5e82, /// 0x9bc
			 0xf1f0d9be, /// 0x9c0
			 0x233196ea, /// 0x9c4
			 0xa726578a, /// 0x9c8
			 0xd29db25b, /// 0x9cc
			 0x33211c58, /// 0x9d0
			 0x8179f0b7, /// 0x9d4
			 0xbb867a96, /// 0x9d8
			 0x7282d922, /// 0x9dc
			 0xaa50674a, /// 0x9e0
			 0x23934158, /// 0x9e4
			 0x10741f0f, /// 0x9e8
			 0x285bea2d, /// 0x9ec
			 0xf75b50f4, /// 0x9f0
			 0xcdaef21b, /// 0x9f4
			 0x7f993baf, /// 0x9f8
			 0x43c93ecc, /// 0x9fc
			 0x1542cb30, /// 0xa00
			 0x5caec605, /// 0xa04
			 0x1661e42b, /// 0xa08
			 0x14e70a4e, /// 0xa0c
			 0x3caa1fb5, /// 0xa10
			 0xff45ae48, /// 0xa14
			 0xd30e8fde, /// 0xa18
			 0x0d9bea0d, /// 0xa1c
			 0x9c680a97, /// 0xa20
			 0x0b3f8397, /// 0xa24
			 0xc0b74442, /// 0xa28
			 0x62d0071d, /// 0xa2c
			 0x12338a7f, /// 0xa30
			 0x7b3c9c5d, /// 0xa34
			 0x10663698, /// 0xa38
			 0x9aefc308, /// 0xa3c
			 0xf3a744f8, /// 0xa40
			 0x82da5e70, /// 0xa44
			 0xa4de6fb6, /// 0xa48
			 0x03f1323a, /// 0xa4c
			 0xc897693a, /// 0xa50
			 0xdbefe03b, /// 0xa54
			 0xca3325eb, /// 0xa58
			 0xc409aad0, /// 0xa5c
			 0x95651152, /// 0xa60
			 0x346ad93f, /// 0xa64
			 0x0c1a30e6, /// 0xa68
			 0x0b7232b3, /// 0xa6c
			 0xaad7871a, /// 0xa70
			 0x42e49d0d, /// 0xa74
			 0xa700da90, /// 0xa78
			 0x95a5967d, /// 0xa7c
			 0x4fbc1ffa, /// 0xa80
			 0x465d13c6, /// 0xa84
			 0xe2a10f54, /// 0xa88
			 0x83e7473e, /// 0xa8c
			 0x7571b123, /// 0xa90
			 0x9cb263de, /// 0xa94
			 0xf84bb598, /// 0xa98
			 0xf79f1367, /// 0xa9c
			 0x3e7c3ce5, /// 0xaa0
			 0xfbd2e54e, /// 0xaa4
			 0x4ff2902b, /// 0xaa8
			 0x6e328f49, /// 0xaac
			 0x603e8426, /// 0xab0
			 0x97a6eb40, /// 0xab4
			 0xb97cae9d, /// 0xab8
			 0xbe37deac, /// 0xabc
			 0xf29ee702, /// 0xac0
			 0x5caf5b46, /// 0xac4
			 0x3b020555, /// 0xac8
			 0xc8ab6180, /// 0xacc
			 0x0ec97d39, /// 0xad0
			 0xba1936bb, /// 0xad4
			 0x204a6d4e, /// 0xad8
			 0xf6ed0be4, /// 0xadc
			 0xdc180f41, /// 0xae0
			 0x17ae0d5e, /// 0xae4
			 0x1d4408f1, /// 0xae8
			 0x47f4f035, /// 0xaec
			 0x85e1d2ea, /// 0xaf0
			 0x08ce671b, /// 0xaf4
			 0x9924811d, /// 0xaf8
			 0x318bafd3, /// 0xafc
			 0xeb4b13b7, /// 0xb00
			 0x08fb7a92, /// 0xb04
			 0x136111f0, /// 0xb08
			 0x4a896d58, /// 0xb0c
			 0x445e9922, /// 0xb10
			 0x0261569c, /// 0xb14
			 0x310d026e, /// 0xb18
			 0xcb71b993, /// 0xb1c
			 0xd9b235da, /// 0xb20
			 0xfd7778e6, /// 0xb24
			 0x4cb2b2df, /// 0xb28
			 0x7ea040e3, /// 0xb2c
			 0x76f9263c, /// 0xb30
			 0x537619ed, /// 0xb34
			 0x7c3d11ba, /// 0xb38
			 0x8132eeab, /// 0xb3c
			 0x3a7f18ad, /// 0xb40
			 0x3eeda620, /// 0xb44
			 0x9b7c60e8, /// 0xb48
			 0xeddbb680, /// 0xb4c
			 0x77fad093, /// 0xb50
			 0x086e1895, /// 0xb54
			 0x5c991800, /// 0xb58
			 0x1fa86cd0, /// 0xb5c
			 0x816be835, /// 0xb60
			 0x4be3c7a0, /// 0xb64
			 0x95302c80, /// 0xb68
			 0x4acfc4a5, /// 0xb6c
			 0x1a8d3ada, /// 0xb70
			 0x2f5022dd, /// 0xb74
			 0xf81f5a9a, /// 0xb78
			 0x9562ac99, /// 0xb7c
			 0xd8e95c67, /// 0xb80
			 0x82bcfc2a, /// 0xb84
			 0xc8d87564, /// 0xb88
			 0x45cc8f1c, /// 0xb8c
			 0x24bad1e3, /// 0xb90
			 0x3cd435bf, /// 0xb94
			 0xf8474d8b, /// 0xb98
			 0x7c00976c, /// 0xb9c
			 0xf1543650, /// 0xba0
			 0xca9e4cc5, /// 0xba4
			 0xe417144c, /// 0xba8
			 0x72810e79, /// 0xbac
			 0xf7fab605, /// 0xbb0
			 0xd40226f0, /// 0xbb4
			 0x934e3217, /// 0xbb8
			 0x844ac481, /// 0xbbc
			 0xbb08f5a3, /// 0xbc0
			 0x84ebbe25, /// 0xbc4
			 0x83c3ca73, /// 0xbc8
			 0x1b502725, /// 0xbcc
			 0x54b37385, /// 0xbd0
			 0x8bf2b766, /// 0xbd4
			 0x73558343, /// 0xbd8
			 0x3ecf4aac, /// 0xbdc
			 0x35c69be7, /// 0xbe0
			 0xb7c88edd, /// 0xbe4
			 0x4051888e, /// 0xbe8
			 0xb13d3475, /// 0xbec
			 0xe897bc86, /// 0xbf0
			 0xc81b6e7e, /// 0xbf4
			 0x13c3fde6, /// 0xbf8
			 0xb48d56fc, /// 0xbfc
			 0x600cc398, /// 0xc00
			 0xe6c09f74, /// 0xc04
			 0xda395bc6, /// 0xc08
			 0x9380442c, /// 0xc0c
			 0xbd6fd869, /// 0xc10
			 0x166d1d6c, /// 0xc14
			 0x12fd9c24, /// 0xc18
			 0x8601f190, /// 0xc1c
			 0x707db47c, /// 0xc20
			 0xc5db66eb, /// 0xc24
			 0x8bb4de65, /// 0xc28
			 0xe14d32f8, /// 0xc2c
			 0x0815148b, /// 0xc30
			 0x788d4cc6, /// 0xc34
			 0x991dbe83, /// 0xc38
			 0x9e135297, /// 0xc3c
			 0x72a16a68, /// 0xc40
			 0xa515feba, /// 0xc44
			 0xff2f5ffb, /// 0xc48
			 0x4c7f6e4e, /// 0xc4c
			 0x6c9c1772, /// 0xc50
			 0xd47e8ec7, /// 0xc54
			 0x83a08e3a, /// 0xc58
			 0x12825870, /// 0xc5c
			 0x5b19f89b, /// 0xc60
			 0x88f8b691, /// 0xc64
			 0x30b076d9, /// 0xc68
			 0xd5ae8d9f, /// 0xc6c
			 0x6298330f, /// 0xc70
			 0xfa224314, /// 0xc74
			 0x1a823be5, /// 0xc78
			 0xff22d4e3, /// 0xc7c
			 0x7148d194, /// 0xc80
			 0x5f8e9fb9, /// 0xc84
			 0x84841224, /// 0xc88
			 0xe1f959ce, /// 0xc8c
			 0xa632cacb, /// 0xc90
			 0x1966e1f2, /// 0xc94
			 0x90486ae3, /// 0xc98
			 0xc561e9a8, /// 0xc9c
			 0x45be2454, /// 0xca0
			 0x41301214, /// 0xca4
			 0x14ad5c66, /// 0xca8
			 0xb5bc9118, /// 0xcac
			 0x66b0a19a, /// 0xcb0
			 0xfb53f3da, /// 0xcb4
			 0x08aa95de, /// 0xcb8
			 0xb8f678f7, /// 0xcbc
			 0x4e4f71b6, /// 0xcc0
			 0x13f271f3, /// 0xcc4
			 0xf38aa6a2, /// 0xcc8
			 0x981b4845, /// 0xccc
			 0x8d20c41b, /// 0xcd0
			 0x32d1fb6e, /// 0xcd4
			 0xd838cf86, /// 0xcd8
			 0xd14b99e7, /// 0xcdc
			 0x435a90f8, /// 0xce0
			 0x68c4c4af, /// 0xce4
			 0x3b0f75a2, /// 0xce8
			 0x3473f3ba, /// 0xcec
			 0xa209592b, /// 0xcf0
			 0x51caded1, /// 0xcf4
			 0xcdca478d, /// 0xcf8
			 0x109a9139, /// 0xcfc
			 0x24d4aac7, /// 0xd00
			 0xe5ce273d, /// 0xd04
			 0x4649e3af, /// 0xd08
			 0xd2b21096, /// 0xd0c
			 0x2acb0ffa, /// 0xd10
			 0x472df907, /// 0xd14
			 0x2bfb7d21, /// 0xd18
			 0x54b34fb1, /// 0xd1c
			 0xf9c3e139, /// 0xd20
			 0x2c2679e4, /// 0xd24
			 0x6a70b25f, /// 0xd28
			 0x724c2d13, /// 0xd2c
			 0xb6e952e9, /// 0xd30
			 0x243cf9d8, /// 0xd34
			 0x6d4dfd7d, /// 0xd38
			 0x4ce92182, /// 0xd3c
			 0x097b9562, /// 0xd40
			 0xe5bead5f, /// 0xd44
			 0x39a337ff, /// 0xd48
			 0xf91aa97e, /// 0xd4c
			 0xe5fb373d, /// 0xd50
			 0x4f464453, /// 0xd54
			 0x9f9b4819, /// 0xd58
			 0xbffcc657, /// 0xd5c
			 0x3aeaa669, /// 0xd60
			 0xfdbf0a35, /// 0xd64
			 0x5fad0325, /// 0xd68
			 0xda90879d, /// 0xd6c
			 0x9a6b2dfc, /// 0xd70
			 0x16f9bae4, /// 0xd74
			 0x55d66651, /// 0xd78
			 0x3e3291f8, /// 0xd7c
			 0x9bb2ce44, /// 0xd80
			 0xff075dcb, /// 0xd84
			 0x85241729, /// 0xd88
			 0x10ee8ccb, /// 0xd8c
			 0x1dd06d0f, /// 0xd90
			 0x260f3c48, /// 0xd94
			 0x630bdc5b, /// 0xd98
			 0x9853dcff, /// 0xd9c
			 0x568a14dd, /// 0xda0
			 0x87c80fb4, /// 0xda4
			 0x2c810599, /// 0xda8
			 0x0e7e15f2, /// 0xdac
			 0x47d322ac, /// 0xdb0
			 0x76d009e8, /// 0xdb4
			 0x7abfb9f6, /// 0xdb8
			 0xc285ac57, /// 0xdbc
			 0xcd997f21, /// 0xdc0
			 0xbdec764b, /// 0xdc4
			 0xf565a7d9, /// 0xdc8
			 0x3769b4ad, /// 0xdcc
			 0xf3228968, /// 0xdd0
			 0x42257697, /// 0xdd4
			 0x3596f552, /// 0xdd8
			 0x9866c7e6, /// 0xddc
			 0x72482df7, /// 0xde0
			 0x65c070f7, /// 0xde4
			 0xb574752d, /// 0xde8
			 0xc0cbc3b3, /// 0xdec
			 0x8920af57, /// 0xdf0
			 0x804335ae, /// 0xdf4
			 0xa52c9942, /// 0xdf8
			 0x488e0029, /// 0xdfc
			 0xc90a9f91, /// 0xe00
			 0x026c156c, /// 0xe04
			 0x70eafbea, /// 0xe08
			 0xaa2983e5, /// 0xe0c
			 0x9e4a2a67, /// 0xe10
			 0x837af7d5, /// 0xe14
			 0xb95d771a, /// 0xe18
			 0x52438a5d, /// 0xe1c
			 0x49acf875, /// 0xe20
			 0xa46b5604, /// 0xe24
			 0x007f40cc, /// 0xe28
			 0x3a52f05f, /// 0xe2c
			 0x63fafb35, /// 0xe30
			 0x136da192, /// 0xe34
			 0x0076cb3c, /// 0xe38
			 0x0b8eaf4b, /// 0xe3c
			 0x5f4f4f03, /// 0xe40
			 0x5c6119a1, /// 0xe44
			 0xa3b9f806, /// 0xe48
			 0xef501396, /// 0xe4c
			 0x1e12abde, /// 0xe50
			 0xa0cdc05a, /// 0xe54
			 0x038a2388, /// 0xe58
			 0xfb1a03fd, /// 0xe5c
			 0xbc1cfa29, /// 0xe60
			 0x12ef366a, /// 0xe64
			 0xdfaf8186, /// 0xe68
			 0x46f93201, /// 0xe6c
			 0x248a23ce, /// 0xe70
			 0x3cc80e96, /// 0xe74
			 0x3b3c506b, /// 0xe78
			 0xd8ebb196, /// 0xe7c
			 0x646c3108, /// 0xe80
			 0x87ae3068, /// 0xe84
			 0xbe1865d6, /// 0xe88
			 0x38041e9c, /// 0xe8c
			 0xe08c08b7, /// 0xe90
			 0xba5237ba, /// 0xe94
			 0xe5703d43, /// 0xe98
			 0x8f6429b7, /// 0xe9c
			 0xe7b2438d, /// 0xea0
			 0xa7606ef1, /// 0xea4
			 0xf54e9658, /// 0xea8
			 0xf32a20e6, /// 0xeac
			 0x5762a383, /// 0xeb0
			 0xfd8f6d0e, /// 0xeb4
			 0xdc85262c, /// 0xeb8
			 0x10f57c9f, /// 0xebc
			 0x4471b435, /// 0xec0
			 0x00ffff63, /// 0xec4
			 0xb7332159, /// 0xec8
			 0x7a6b6179, /// 0xecc
			 0xc4a900de, /// 0xed0
			 0x2b0fe597, /// 0xed4
			 0x7a274fcf, /// 0xed8
			 0x0540471d, /// 0xedc
			 0x2d260bad, /// 0xee0
			 0xac05e84f, /// 0xee4
			 0xd6bff3b0, /// 0xee8
			 0xd3f2a682, /// 0xeec
			 0xbb05860a, /// 0xef0
			 0xc34bbf14, /// 0xef4
			 0x5e1aab54, /// 0xef8
			 0x0055828d, /// 0xefc
			 0xbc6a3f19, /// 0xf00
			 0x1adbd8f1, /// 0xf04
			 0x92700e62, /// 0xf08
			 0x53f05389, /// 0xf0c
			 0xaa9209cb, /// 0xf10
			 0xba15c848, /// 0xf14
			 0x3a6ec7de, /// 0xf18
			 0x8d0d49b9, /// 0xf1c
			 0x3d06b2c1, /// 0xf20
			 0xc08153d6, /// 0xf24
			 0xa3e4c0cd, /// 0xf28
			 0xeaa808cf, /// 0xf2c
			 0x9393617d, /// 0xf30
			 0x9a59a37d, /// 0xf34
			 0xf44185c9, /// 0xf38
			 0xeda87ba3, /// 0xf3c
			 0xf6c84be4, /// 0xf40
			 0x50dbe2fa, /// 0xf44
			 0x4ae3baae, /// 0xf48
			 0x465713d1, /// 0xf4c
			 0xf4c93b56, /// 0xf50
			 0x1ece5498, /// 0xf54
			 0x0f223d1f, /// 0xf58
			 0xef3014b5, /// 0xf5c
			 0x114cc9de, /// 0xf60
			 0x017233c2, /// 0xf64
			 0xe94d3569, /// 0xf68
			 0xc82c4532, /// 0xf6c
			 0x8eb908cd, /// 0xf70
			 0x0df941b0, /// 0xf74
			 0x07b7a9ad, /// 0xf78
			 0x03cd9e9b, /// 0xf7c
			 0x5ce07e20, /// 0xf80
			 0xc2872459, /// 0xf84
			 0x02a543c5, /// 0xf88
			 0xc4355493, /// 0xf8c
			 0x50f2c35c, /// 0xf90
			 0xf59c6dfe, /// 0xf94
			 0x1533ca7a, /// 0xf98
			 0xd7705fb7, /// 0xf9c
			 0xb9570fc5, /// 0xfa0
			 0x2f7e29ad, /// 0xfa4
			 0x2100eac3, /// 0xfa8
			 0xa5aea0bd, /// 0xfac
			 0x6235a022, /// 0xfb0
			 0xb5e576c4, /// 0xfb4
			 0xd84b89b7, /// 0xfb8
			 0xa4bacd52, /// 0xfbc
			 0x76d77236, /// 0xfc0
			 0xfb3c3f0f, /// 0xfc4
			 0x55d24a55, /// 0xfc8
			 0x3bfc458c, /// 0xfcc
			 0xcdc7c2e4, /// 0xfd0
			 0x1e03fd47, /// 0xfd4
			 0xe826b831, /// 0xfd8
			 0x0bf4fed4, /// 0xfdc
			 0xeb96505a, /// 0xfe0
			 0x2397ed42, /// 0xfe4
			 0xad010579, /// 0xfe8
			 0xb25390bb, /// 0xfec
			 0xd12b9c64, /// 0xff0
			 0x77203999, /// 0xff4
			 0x5cfff360, /// 0xff8
			 0x249657ff /// last
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
			 0x000000e4,
			 0x00000448,
			 0x000005a8,
			 0x00000504,
			 0x00000288,
			 0x00000038,
			 0x00000688,
			 0x00000678,

			 /// vpu register f2
			 0x41d00000,
			 0x41a00000,
			 0x41f00000,
			 0x3f800000,
			 0x41c00000,
			 0x41900000,
			 0x41f00000,
			 0x41000000,

			 /// vpu register f3
			 0x41b80000,
			 0x41b00000,
			 0x40400000,
			 0x42000000,
			 0x40c00000,
			 0x40e00000,
			 0x41600000,
			 0x41100000,

			 /// vpu register f4
			 0x40000000,
			 0x41c00000,
			 0x41700000,
			 0x41e00000,
			 0x41d80000,
			 0x40e00000,
			 0x41000000,
			 0x41400000,

			 /// vpu register f5
			 0x3f800000,
			 0x41b80000,
			 0x42000000,
			 0x41b80000,
			 0x41000000,
			 0x40a00000,
			 0x41200000,
			 0x41a00000,

			 /// vpu register f6
			 0x42000000,
			 0x40000000,
			 0x41e80000,
			 0x41200000,
			 0x41100000,
			 0x41900000,
			 0x41d80000,
			 0x40e00000,

			 /// vpu register f7
			 0x41700000,
			 0x41f80000,
			 0x41c00000,
			 0x41000000,
			 0x41900000,
			 0x41400000,
			 0x41c80000,
			 0x41100000,

			 /// vpu register f8
			 0x41b00000,
			 0x41400000,
			 0x41100000,
			 0x40400000,
			 0x41880000,
			 0x41a00000,
			 0x3f800000,
			 0x41400000,

			 /// vpu register f9
			 0x40800000,
			 0x40000000,
			 0x41e00000,
			 0x41d00000,
			 0x41200000,
			 0x41e00000,
			 0x41e80000,
			 0x41500000,

			 /// vpu register f10
			 0x41900000,
			 0x41100000,
			 0x40800000,
			 0x41a00000,
			 0x41a80000,
			 0x41e80000,
			 0x40c00000,
			 0x41200000,

			 /// vpu register f11
			 0x40800000,
			 0x41d80000,
			 0x41000000,
			 0x41c00000,
			 0x41d80000,
			 0x41c00000,
			 0x41400000,
			 0x41400000,

			 /// vpu register f12
			 0x41100000,
			 0x41e80000,
			 0x41b00000,
			 0x41e00000,
			 0x41d80000,
			 0x41f80000,
			 0x41a00000,
			 0x41d00000,

			 /// vpu register f13
			 0x41100000,
			 0x41c80000,
			 0x40000000,
			 0x41200000,
			 0x41d80000,
			 0x41b00000,
			 0x41100000,
			 0x41f00000,

			 /// vpu register f14
			 0x42000000,
			 0x41200000,
			 0x41700000,
			 0x41100000,
			 0x41880000,
			 0x41500000,
			 0x41200000,
			 0x41b00000,

			 /// vpu register f15
			 0x41980000,
			 0x41800000,
			 0x41d00000,
			 0x41000000,
			 0x41d00000,
			 0x41e00000,
			 0x41b00000,
			 0x41900000,

			 /// vpu register f16
			 0x40c00000,
			 0x41700000,
			 0x41700000,
			 0x42000000,
			 0x40a00000,
			 0x41b80000,
			 0x41b00000,
			 0x40800000,

			 /// vpu register f17
			 0x41b80000,
			 0x40400000,
			 0x41b00000,
			 0x41a80000,
			 0x41d80000,
			 0x41600000,
			 0x41400000,
			 0x41c00000,

			 /// vpu register f18
			 0x41b00000,
			 0x41f80000,
			 0x41900000,
			 0x41b80000,
			 0x40800000,
			 0x41c80000,
			 0x42000000,
			 0x41000000,

			 /// vpu register f19
			 0x41500000,
			 0x41a80000,
			 0x40000000,
			 0x41700000,
			 0x41f00000,
			 0x41d00000,
			 0x41c80000,
			 0x40a00000,

			 /// vpu register f20
			 0x41300000,
			 0x41100000,
			 0x40c00000,
			 0x41000000,
			 0x41300000,
			 0x41b00000,
			 0x41b00000,
			 0x42000000,

			 /// vpu register f21
			 0x41d00000,
			 0x41e80000,
			 0x41900000,
			 0x41100000,
			 0x41a80000,
			 0x40000000,
			 0x41000000,
			 0x41800000,

			 /// vpu register f22
			 0x42000000,
			 0x40c00000,
			 0x41600000,
			 0x41b00000,
			 0x41c80000,
			 0x40400000,
			 0x40400000,
			 0x41c80000,

			 /// vpu register f23
			 0x40c00000,
			 0x40a00000,
			 0x40000000,
			 0x41900000,
			 0x41a00000,
			 0x41100000,
			 0x41d00000,
			 0x40e00000,

			 /// vpu register f24
			 0x41300000,
			 0x42000000,
			 0x41900000,
			 0x41e00000,
			 0x41e80000,
			 0x41600000,
			 0x41a80000,
			 0x41880000,

			 /// vpu register f25
			 0x41d80000,
			 0x40400000,
			 0x41600000,
			 0x41b00000,
			 0x40c00000,
			 0x40400000,
			 0x41d80000,
			 0x41a00000,

			 /// vpu register f26
			 0x41c00000,
			 0x40c00000,
			 0x41d80000,
			 0x41f80000,
			 0x41700000,
			 0x41800000,
			 0x40e00000,
			 0x41500000,

			 /// vpu register f27
			 0x41c80000,
			 0x41b00000,
			 0x41d80000,
			 0x40e00000,
			 0x40a00000,
			 0x40000000,
			 0x41c00000,
			 0x41c80000,

			 /// vpu register f28
			 0x40400000,
			 0x41d80000,
			 0x41a00000,
			 0x40400000,
			 0x41c80000,
			 0x41500000,
			 0x41b00000,
			 0x41a00000,

			 /// vpu register f29
			 0x41700000,
			 0x40400000,
			 0x41500000,
			 0x41880000,
			 0x42000000,
			 0x41500000,
			 0x41d00000,
			 0x41c80000,

			 /// vpu register f30
			 0x40a00000,
			 0x41a80000,
			 0x41c80000,
			 0x41500000,
			 0x41d80000,
			 0x41800000,
			 0x41c00000,
			 0x3f800000,

			 /// vpu register f31
			 0x41e00000,
			 0x41100000,
			 0x40a00000,
			 0x41300000,
			 0x41100000,
			 0x40c00000,
			 0x41600000,
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
		"fnmadd.s f17, f19, f30, f16, rup\n"                  ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f30, f9, f6, f30, rdn\n"                    ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f12, f16, f13, f26, rup\n"                  ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f24, f12, f19, f4, dyn\n"                   ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f5, f23, f29, f21, rne\n"                   ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f13, f23, f8, f24, rmm\n"                   ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f27, f4, f24, f26, rmm\n"                   ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f24, f24, f19, f9, dyn\n"                   ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f13, f14, f23, f13, rup\n"                  ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f19, f22, f8, f9, rmm\n"                    ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f6, f24, f9, f30, rne\n"                    ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f21, f1, f8, f0, rtz\n"                     ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f6, f20, f30, f7, rdn\n"                    ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f19, f24, f22, f2, rdn\n"                   ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f5, f11, f12, f8, dyn\n"                    ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f24, f18, f0, f6, rne\n"                    ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f24, f17, f23, f2, rmm\n"                   ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f6, f21, f3, f13, dyn\n"                    ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f29, f25, f28, f22, dyn\n"                  ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f30, f29, f15, f13, rne\n"                  ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f24, f24, f17, f7, dyn\n"                   ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f8, f26, f9, f19, rdn\n"                    ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f20, f17, f1, f30, rmm\n"                   ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f28, f0, f8, f27, rdn\n"                    ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f8, f12, f6, f24, rdn\n"                    ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f2, f5, f1, f18, rup\n"                     ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f7, f23, f13, f0, rtz\n"                    ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f7, f7, f12, f13, rtz\n"                    ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f27, f15, f13, f27, rtz\n"                  ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f10, f7, f30, f26, rdn\n"                   ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f14, f11, f21, f14, rtz\n"                  ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f25, f15, f19, f6, rne\n"                   ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f25, f26, f27, f10, rne\n"                  ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f4, f20, f22, f20, rne\n"                   ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f20, f17, f15, f16, dyn\n"                  ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f6, f23, f11, f16, rtz\n"                   ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f19, f15, f1, f28, dyn\n"                   ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f6, f7, f29, f8, rmm\n"                     ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f23, f20, f6, f0, rdn\n"                    ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f11, f2, f14, f16, rdn\n"                   ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f21, f5, f12, f30, rmm\n"                   ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f22, f0, f16, f23, rdn\n"                   ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f20, f26, f20, f22, rne\n"                  ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f17, f14, f14, f18, rne\n"                  ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f20, f10, f18, f26, dyn\n"                  ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f23, f25, f0, f20, rup\n"                   ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f25, f11, f9, f27, rdn\n"                   ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f2, f13, f3, f22, rmm\n"                    ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f10, f13, f3, f25, rup\n"                   ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f8, f8, f16, f17, rup\n"                    ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f3, f3, f16, f20, rup\n"                    ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f14, f18, f15, f2, rup\n"                   ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f29, f1, f18, f15, rne\n"                   ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f22, f25, f5, f11, rmm\n"                   ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f29, f8, f21, f8, dyn\n"                    ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f16, f22, f15, f23, rdn\n"                  ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f13, f14, f14, f13, rdn\n"                  ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f6, f24, f1, f28, rtz\n"                    ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f18, f12, f5, f0, rup\n"                    ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f5, f0, f23, f13, rne\n"                    ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f23, f28, f17, f17, dyn\n"                  ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f24, f8, f16, f7, rdn\n"                    ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f15, f0, f7, f26, rdn\n"                    ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f15, f17, f20, f6, rtz\n"                   ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f21, f1, f5, f10, rdn\n"                    ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f17, f24, f0, f2, rup\n"                    ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f12, f28, f30, f19, rmm\n"                  ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f4, f26, f15, f1, rne\n"                    ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f23, f30, f19, f29, rmm\n"                  ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f9, f8, f13, f3, rmm\n"                     ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f26, f2, f0, f14, rdn\n"                    ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f27, f14, f12, f28, rup\n"                  ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f28, f4, f28, f1, rup\n"                    ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f10, f14, f9, f19, rtz\n"                   ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f19, f11, f2, f15, rmm\n"                   ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f26, f1, f5, f8, rup\n"                     ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f11, f8, f12, f9, rtz\n"                    ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f19, f25, f14, f5, rtz\n"                   ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f4, f19, f5, f23, rne\n"                    ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f18, f8, f23, f0, rne\n"                    ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f30, f1, f19, f2, rup\n"                    ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f25, f9, f4, f4, rdn\n"                     ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f21, f2, f4, f29, rup\n"                    ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f28, f20, f11, f0, rmm\n"                   ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f5, f14, f22, f30, rtz\n"                   ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f11, f13, f5, f26, rmm\n"                   ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f8, f3, f3, f14, rdn\n"                     ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f28, f16, f16, f23, dyn\n"                  ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f9, f18, f29, f1, rtz\n"                    ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f6, f6, f23, f7, rdn\n"                     ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f23, f17, f0, f22, rtz\n"                   ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f29, f25, f10, f5, rne\n"                   ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f15, f28, f19, f24, rtz\n"                  ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f11, f14, f26, f11, rtz\n"                  ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f7, f0, f7, f7, rne\n"                      ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f16, f10, f7, f28, rtz\n"                   ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f16, f15, f12, f29, rdn\n"                  ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f23, f26, f5, f22, rmm\n"                   ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmadd.s f22, f21, f12, f25, rtz\n"                  ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
