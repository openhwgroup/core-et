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
			 0xcb000000,                                                  // -8388608.0                                  /// 00000
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00004
			 0x00011111,                                                  // 9.7958E-41                                  /// 00008
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0000c
			 0x33333333,                                                  // 4 random values                             /// 00010
			 0x0e000003,                                                  // Trailing 1s:                                /// 00014
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0x0c700000,                                                  // Leading 1s:                                 /// 0001c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00020
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00028
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0002c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00030
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x0c400000,                                                  // Leading 1s:                                 /// 00038
			 0x80000000,                                                  // -zero                                       /// 0003c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00040
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00044
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00048
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0004c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00050
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00054
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00058
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0005c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00060
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00064
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0006c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00070
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00074
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00078
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00080
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00084
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00088
			 0x33333333,                                                  // 4 random values                             /// 0008c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00090
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0009c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000a4
			 0x7fc00001,                                                  // signaling NaN                               /// 000a8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000ac
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000b0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000b4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 000b8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 000bc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000c0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 000c8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000cc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000d0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 000d4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000d8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 000dc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 000e0
			 0x0e000003,                                                  // Trailing 1s:                                /// 000e4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 000e8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 000ec
			 0x7fc00000,                                                  // cquiet NaN                                  /// 000f0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 000f4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000f8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00104
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00108
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0010c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00114
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00118
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0011c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00124
			 0x55555555,                                                  // 4 random values                             /// 00128
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0012c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00130
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00134
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00138
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0013c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00144
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00148
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0014c
			 0x7fc00001,                                                  // signaling NaN                               /// 00150
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00154
			 0x80011111,                                                  // -9.7958E-41                                 /// 00158
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0015c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00160
			 0x0e000007,                                                  // Trailing 1s:                                /// 00164
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00168
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00170
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00174
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00178
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00180
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00184
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00188
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0018c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00190
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00198
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0019c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 001a0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 001a4
			 0xbf028f5c,                                                  // -0.51                                       /// 001a8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001ac
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001b4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 001b8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 001bc
			 0xcb000000,                                                  // -8388608.0                                  /// 001c0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 001c8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 001cc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 001dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 001e8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0xcb000000,                                                  // -8388608.0                                  /// 001f0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001f4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 001f8
			 0x3f028f5c,                                                  // 0.51                                        /// 001fc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00200
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00204
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00208
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0020c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00214
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00218
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00220
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00224
			 0x0e000007,                                                  // Trailing 1s:                                /// 00228
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0022c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00230
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00234
			 0x7f800000,                                                  // inf                                         /// 00238
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00240
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00248
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0024c
			 0x4b000000,                                                  // 8388608.0                                   /// 00250
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00254
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00258
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00260
			 0x7f800000,                                                  // inf                                         /// 00264
			 0x55555555,                                                  // 4 random values                             /// 00268
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0026c
			 0x4b000000,                                                  // 8388608.0                                   /// 00270
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00278
			 0x80011111,                                                  // -9.7958E-41                                 /// 0027c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00280
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00284
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00288
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0028c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00290
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00298
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0029c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002a4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002a8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002b0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002b4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002b8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002bc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 002c0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002c4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002c8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002cc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 002d0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002d4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 002d8
			 0x00011111,                                                  // 9.7958E-41                                  /// 002dc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 002e0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 002e4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002e8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002ec
			 0xffc00001,                                                  // -signaling NaN                              /// 002f0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 002f4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 002f8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002fc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00300
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00304
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00308
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0030c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00310
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00318
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0031c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00320
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00324
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00328
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0032c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00330
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00334
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00338
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0033c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00340
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00344
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00348
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0034c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00350
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00354
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00358
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00360
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00368
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0036c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00374
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00378
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0037c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00380
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00384
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0038c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00390
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00394
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00398
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0039c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 003a0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 003a4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 003a8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003ac
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003b8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003bc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 003c0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003c4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 003c8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 003cc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 003d4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003d8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 003dc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003e0
			 0xbf028f5c,                                                  // -0.51                                       /// 003e4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003e8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003ec
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 003f0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 003f4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003f8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 003fc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00400
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00404
			 0x55555555,                                                  // 4 random values                             /// 00408
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0040c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00410
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00414
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00418
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0041c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00420
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00424
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00428
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0042c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00430
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00434
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00440
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00448
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0044c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00450
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00454
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00458
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0045c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00460
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00464
			 0xff800000,                                                  // -inf                                        /// 00468
			 0x00000000,                                                  // zero                                        /// 0046c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00470
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00474
			 0x0e000007,                                                  // Trailing 1s:                                /// 00478
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0047c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00480
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00484
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00488
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0048c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00490
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00494
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00498
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0049c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004a0
			 0x0e000003,                                                  // Trailing 1s:                                /// 004a4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004a8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004ac
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b0
			 0xcb000000,                                                  // -8388608.0                                  /// 004b4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004b8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004bc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004c0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004c4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 004c8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004cc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004d0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 004d4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004d8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004dc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 004e0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004e4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004e8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004ec
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004f8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 004fc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00500
			 0x0c780000,                                                  // Leading 1s:                                 /// 00504
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00508
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0050c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00510
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00514
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00518
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0051c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00524
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0052c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00530
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00534
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00538
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0053c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00540
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00544
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00548
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0054c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00550
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00554
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00558
			 0xff800000,                                                  // -inf                                        /// 0055c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00560
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00564
			 0x0e000001,                                                  // Trailing 1s:                                /// 00568
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0056c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00570
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00574
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00578
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0057c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00580
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00584
			 0x0c400000,                                                  // Leading 1s:                                 /// 00588
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0058c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00590
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00598
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0059c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0x7f800000,                                                  // inf                                         /// 005a4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 005a8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005ac
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005b0
			 0x00011111,                                                  // 9.7958E-41                                  /// 005b4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005b8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005bc
			 0x0e000003,                                                  // Trailing 1s:                                /// 005c0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005c4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005c8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 005cc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 005d0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005d8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0xcb000000,                                                  // -8388608.0                                  /// 005e0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 005e4
			 0x0e000003,                                                  // Trailing 1s:                                /// 005e8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 005ec
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005f0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005f4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005fc
			 0xbf028f5c,                                                  // -0.51                                       /// 00600
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00604
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00608
			 0x55555555,                                                  // 4 random values                             /// 0060c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00610
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00614
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0061c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00620
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00624
			 0x55555555,                                                  // 4 random values                             /// 00628
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00634
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00638
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0063c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0xcb000000,                                                  // -8388608.0                                  /// 00644
			 0x00000000,                                                  // zero                                        /// 00648
			 0x7fc00001,                                                  // signaling NaN                               /// 0064c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00654
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00658
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0065c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00660
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00668
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0066c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00670
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00674
			 0x7fc00001,                                                  // signaling NaN                               /// 00678
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0067c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00680
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00684
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00688
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0068c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00690
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00694
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00698
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0069c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 006a4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006a8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006ac
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 006b0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006b4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 006b8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006bc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006c4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0x0e000007,                                                  // Trailing 1s:                                /// 006cc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006d0
			 0xffc00001,                                                  // -signaling NaN                              /// 006d4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006dc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 006e0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 006e4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 006e8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 006ec
			 0x0e000001,                                                  // Trailing 1s:                                /// 006f0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006f4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 006f8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 006fc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00700
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00704
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0070c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00710
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00714
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0071c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00720
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00724
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00728
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0072c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00734
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00738
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00740
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00744
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00748
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0074c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00750
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00754
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00758
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0075c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00760
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00764
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00768
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00770
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00774
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0077c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0078c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00798
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0079c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 007a0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 007a8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ac
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007b0
			 0xffc00001,                                                  // -signaling NaN                              /// 007b4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 007b8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 007bc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 007c0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007c4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007cc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007d0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 007d4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007dc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 007e4
			 0x0c400000,                                                  // Leading 1s:                                 /// 007e8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 007ec
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0xcb000000,                                                  // -8388608.0                                  /// 007f4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007f8
			 0x0e000007,                                                  // Trailing 1s:                                /// 007fc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00800
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00804
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00808
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0080c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00810
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00814
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0081c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00820
			 0x00011111,                                                  // 9.7958E-41                                  /// 00824
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00828
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0082c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00834
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00838
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0083c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00840
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00844
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00848
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0084c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00850
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00854
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00858
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0085c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00860
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00864
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00868
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0086c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00870
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00874
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00878
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0087c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00880
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00888
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0088c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00894
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00898
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0089c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008a0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008a4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 008a8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ac
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 008b4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008b8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 008bc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008c0
			 0x80000000,                                                  // -zero                                       /// 008c4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008c8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008cc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008d0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 008d4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008d8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008dc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008e0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 008e4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008ec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 008f4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 008f8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 008fc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00900
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00904
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00908
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00910
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00914
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00918
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00920
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00924
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00928
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0092c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00930
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00934
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00938
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0093c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00940
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00944
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00948
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0094c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00950
			 0xff800000,                                                  // -inf                                        /// 00954
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00958
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0095c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00964
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00968
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0096c
			 0x7fc00001,                                                  // signaling NaN                               /// 00970
			 0x0c600000,                                                  // Leading 1s:                                 /// 00974
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00978
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0097c
			 0x7fc00001,                                                  // signaling NaN                               /// 00980
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00984
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00988
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0098c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00990
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00998
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0099c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 009a8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009ac
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009b4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009bc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 009c0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009c4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009c8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009d0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009d4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 009d8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 009dc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009e0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ec
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009f0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 009f4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009fc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a08
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a0c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a10
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a14
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a18
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a1c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a20
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a24
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a28
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a30
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a34
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a38
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a3c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a40
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a44
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a48
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a50
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x80000000,                                                  // -zero                                       /// 00a58
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a5c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a60
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a64
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a68
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a70
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a74
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a78
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a7c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a80
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a84
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a88
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a8c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a90
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a94
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a98
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a9c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00aa4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00aac
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ab4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00abc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ac0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ac4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ac8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00acc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ad0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ad4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ad8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00adc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ae0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ae4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ae8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00aec
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00af4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00af8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b00
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b04
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b08
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b0c
			 0x7f800000,                                                  // inf                                         /// 00b10
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b14
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b18
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b20
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b24
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b28
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b2c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b30
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b34
			 0xffc00001,                                                  // -signaling NaN                              /// 00b38
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b3c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b40
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b44
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b48
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b50
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b54
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b58
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b5c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b60
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b64
			 0xcb000000,                                                  // -8388608.0                                  /// 00b68
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b6c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b70
			 0x7f800000,                                                  // inf                                         /// 00b74
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b78
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b80
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b84
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b88
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b8c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b90
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b94
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b98
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b9c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ba0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ba4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00bac
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00bb4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00bb8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00bbc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00bc0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00bc8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00bcc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00bd0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00bd4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00bd8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00bdc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00be0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00be4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00be8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bec
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00bf0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00bf4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00bf8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00bfc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c00
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c04
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c08
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c0c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c10
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c18
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c1c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c20
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c24
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c28
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c2c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c30
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c34
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c38
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c3c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c40
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c48
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c50
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c54
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c58
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c5c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c60
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c64
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c68
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c6c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c70
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c74
			 0xffc00001,                                                  // -signaling NaN                              /// 00c78
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c7c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c80
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c84
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c88
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c8c
			 0xbf028f5c,                                                  // -0.51                                       /// 00c90
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c94
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c98
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c9c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ca0
			 0x33333333,                                                  // 4 random values                             /// 00ca4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ca8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cac
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00cb4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00cb8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cbc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00cc0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00cc4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00cc8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ccc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00cd0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cd4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00cdc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ce0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ce4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ce8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00cec
			 0x00011111,                                                  // 9.7958E-41                                  /// 00cf0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00cf8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00cfc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d00
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d04
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d08
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d0c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d10
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d14
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d18
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d1c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d20
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d24
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d28
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d2c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d30
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d34
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d38
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d3c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d40
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d44
			 0xff800000,                                                  // -inf                                        /// 00d48
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d4c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d50
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d54
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d58
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d5c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d60
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d64
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d6c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d70
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d74
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d78
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d7c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d80
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d84
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d88
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d8c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d90
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d94
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d98
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d9c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00da4
			 0x7fc00001,                                                  // signaling NaN                               /// 00da8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00dac
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00db0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00db4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00dbc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00dc0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00dc4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dcc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00dd0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00dd4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00dd8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ddc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00de0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00de4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00de8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00dec
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00df0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00df4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00df8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00dfc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e00
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e04
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e08
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e0c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e10
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e14
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e18
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e1c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e20
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e24
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e28
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e2c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e30
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e34
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e38
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e3c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e40
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e44
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e48
			 0x00000000,                                                  // zero                                        /// 00e4c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e50
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e54
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e58
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e5c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e60
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e64
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e68
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e6c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0x4b000000,                                                  // 8388608.0                                   /// 00e74
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e78
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e7c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e80
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e84
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e88
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e8c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e90
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e94
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e98
			 0x4b000000,                                                  // 8388608.0                                   /// 00e9c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ea4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00eac
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00eb8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec0
			 0x7f800000,                                                  // inf                                         /// 00ec4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ec8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ecc
			 0x80000000,                                                  // -zero                                       /// 00ed0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ed4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ed8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00edc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ee0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ee8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00eec
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ef0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ef8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00efc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f00
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f04
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f08
			 0xffc00001,                                                  // -signaling NaN                              /// 00f0c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f10
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f14
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f18
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f1c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f24
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f2c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f30
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f34
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f38
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f40
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x55555555,                                                  // 4 random values                             /// 00f4c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f50
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f54
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f58
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f5c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f64
			 0x7f800000,                                                  // inf                                         /// 00f68
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f6c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f70
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f74
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f78
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f7c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f80
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f84
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f88
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f8c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f94
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f98
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f9c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00fa0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00fa4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00fa8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00fac
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00fb0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00fb8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00fbc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00fc0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fcc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00fd0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00fd4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00fd8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fdc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00fe4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00fe8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ff0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ff4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ff8
			 0x00000008                                                  // SP - 1 bit alone set in mantissa +ve        /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x408bf84b, /// 0x0
			 0x97ee6c8c, /// 0x4
			 0x93310063, /// 0x8
			 0xc09d29c3, /// 0xc
			 0x8010f727, /// 0x10
			 0x5fdc7eb6, /// 0x14
			 0x6f0b4635, /// 0x18
			 0xbc9ec09d, /// 0x1c
			 0xcdea001f, /// 0x20
			 0x8b28ee7d, /// 0x24
			 0xe2aa323c, /// 0x28
			 0x66686cb7, /// 0x2c
			 0xe3782bef, /// 0x30
			 0xa33e4cda, /// 0x34
			 0x866b1578, /// 0x38
			 0x73531aa2, /// 0x3c
			 0x5f91dcf5, /// 0x40
			 0xa390af11, /// 0x44
			 0x79bac85a, /// 0x48
			 0x3aad00cd, /// 0x4c
			 0xb4bfd0ff, /// 0x50
			 0x3f45bd27, /// 0x54
			 0xc8563d55, /// 0x58
			 0x7979226f, /// 0x5c
			 0x76d95039, /// 0x60
			 0x0c072224, /// 0x64
			 0x171ca7fa, /// 0x68
			 0x2f27e06d, /// 0x6c
			 0x90630cb7, /// 0x70
			 0x4ad7b58e, /// 0x74
			 0x6143856f, /// 0x78
			 0x1eacd3f6, /// 0x7c
			 0x27293bf8, /// 0x80
			 0x476b7799, /// 0x84
			 0x2fe29824, /// 0x88
			 0x6854a836, /// 0x8c
			 0x184a6a42, /// 0x90
			 0xb3beff09, /// 0x94
			 0xe838ca11, /// 0x98
			 0xad95bb77, /// 0x9c
			 0x6a610ae2, /// 0xa0
			 0xd3382ced, /// 0xa4
			 0x2cb4e024, /// 0xa8
			 0x5db4a4c3, /// 0xac
			 0xc41fc928, /// 0xb0
			 0x986c6f37, /// 0xb4
			 0xf566de98, /// 0xb8
			 0xd0091dc7, /// 0xbc
			 0xc066ae1f, /// 0xc0
			 0xa5468d2b, /// 0xc4
			 0x368500fb, /// 0xc8
			 0xd5aec365, /// 0xcc
			 0x7f68ef26, /// 0xd0
			 0xbe206435, /// 0xd4
			 0x7697968e, /// 0xd8
			 0xf6341451, /// 0xdc
			 0x08af0c9b, /// 0xe0
			 0x5d8e309c, /// 0xe4
			 0x5e0fe8a6, /// 0xe8
			 0xad3b2605, /// 0xec
			 0xfc996b59, /// 0xf0
			 0x52cf10e7, /// 0xf4
			 0xd77fd8cb, /// 0xf8
			 0xde9c4174, /// 0xfc
			 0xffb32b1a, /// 0x100
			 0x3730ac97, /// 0x104
			 0x2ecc29ba, /// 0x108
			 0x959d05ca, /// 0x10c
			 0x32a3218b, /// 0x110
			 0x79702ce7, /// 0x114
			 0xe6872627, /// 0x118
			 0x3c2feb5a, /// 0x11c
			 0x1bfab36a, /// 0x120
			 0x796fca07, /// 0x124
			 0x0a3a4f98, /// 0x128
			 0x13ef71a6, /// 0x12c
			 0xbabdc064, /// 0x130
			 0x775c48a5, /// 0x134
			 0x4e763031, /// 0x138
			 0xee283aa0, /// 0x13c
			 0x41ed174a, /// 0x140
			 0x1fc663a7, /// 0x144
			 0xc4957519, /// 0x148
			 0xf1c63a58, /// 0x14c
			 0x11440e1c, /// 0x150
			 0x3fb0c41a, /// 0x154
			 0x1d574969, /// 0x158
			 0x066e2130, /// 0x15c
			 0x4cac4ea9, /// 0x160
			 0xa4a4c75a, /// 0x164
			 0x4449222f, /// 0x168
			 0x99e7a97a, /// 0x16c
			 0x2f2c2489, /// 0x170
			 0x5442be6c, /// 0x174
			 0x11839b28, /// 0x178
			 0x93d16c31, /// 0x17c
			 0xfc0870b5, /// 0x180
			 0x58db52a5, /// 0x184
			 0x7ddb6874, /// 0x188
			 0xdf07cd9b, /// 0x18c
			 0xe166eae4, /// 0x190
			 0x6c302a35, /// 0x194
			 0x01d51879, /// 0x198
			 0x1b93939b, /// 0x19c
			 0x04facdb4, /// 0x1a0
			 0x09b093a2, /// 0x1a4
			 0x3183bc60, /// 0x1a8
			 0x413b025d, /// 0x1ac
			 0xed9ffc07, /// 0x1b0
			 0x82896a95, /// 0x1b4
			 0x29b4174f, /// 0x1b8
			 0x6021054b, /// 0x1bc
			 0x730b2d83, /// 0x1c0
			 0xf459cf16, /// 0x1c4
			 0x72b2901e, /// 0x1c8
			 0x7660a937, /// 0x1cc
			 0x425409d8, /// 0x1d0
			 0x660c2427, /// 0x1d4
			 0x080f36d1, /// 0x1d8
			 0x570a03ee, /// 0x1dc
			 0x9948b4eb, /// 0x1e0
			 0x6d2edc76, /// 0x1e4
			 0xb03f2ddf, /// 0x1e8
			 0xd5e4a3e9, /// 0x1ec
			 0x3f2c8dba, /// 0x1f0
			 0xad628158, /// 0x1f4
			 0xebe2ddd8, /// 0x1f8
			 0xc16b21e4, /// 0x1fc
			 0x01c1ee54, /// 0x200
			 0x36193042, /// 0x204
			 0xea3a474b, /// 0x208
			 0xd9a885c6, /// 0x20c
			 0x344997bf, /// 0x210
			 0xa3989fb8, /// 0x214
			 0x1330551d, /// 0x218
			 0xb0719128, /// 0x21c
			 0x1634ac58, /// 0x220
			 0x14c9bfbe, /// 0x224
			 0xe246ed69, /// 0x228
			 0xbeb92ba7, /// 0x22c
			 0x6839b53a, /// 0x230
			 0x4dfa5d90, /// 0x234
			 0x9f1a78d5, /// 0x238
			 0xa29e3998, /// 0x23c
			 0xe50dbbeb, /// 0x240
			 0xff7374d2, /// 0x244
			 0x4a29cdb0, /// 0x248
			 0xd1f0ca42, /// 0x24c
			 0x7f134344, /// 0x250
			 0xf2bf30b6, /// 0x254
			 0x583589ef, /// 0x258
			 0x5785d344, /// 0x25c
			 0xece06a01, /// 0x260
			 0xecbb0ca9, /// 0x264
			 0x8fd66590, /// 0x268
			 0xc3613618, /// 0x26c
			 0x7a430fa1, /// 0x270
			 0x3344a879, /// 0x274
			 0x86e5ccd2, /// 0x278
			 0x2a192479, /// 0x27c
			 0xc57a70a4, /// 0x280
			 0xd054888d, /// 0x284
			 0x59d68ca2, /// 0x288
			 0x96b570af, /// 0x28c
			 0x44a6c1f3, /// 0x290
			 0xfd9d1a10, /// 0x294
			 0x804e369c, /// 0x298
			 0x3529c29f, /// 0x29c
			 0x2768f21e, /// 0x2a0
			 0x2e9ae91c, /// 0x2a4
			 0x844e20e2, /// 0x2a8
			 0xbfb54e5a, /// 0x2ac
			 0x7a6b4430, /// 0x2b0
			 0x32294d5d, /// 0x2b4
			 0x3b518984, /// 0x2b8
			 0x94ca6d87, /// 0x2bc
			 0xa37a9b9f, /// 0x2c0
			 0xf536e643, /// 0x2c4
			 0x387997de, /// 0x2c8
			 0x4e78972a, /// 0x2cc
			 0x9affa79f, /// 0x2d0
			 0xe2f175c5, /// 0x2d4
			 0x540d1b64, /// 0x2d8
			 0x547dfbc8, /// 0x2dc
			 0x2bff0dee, /// 0x2e0
			 0xe44d6496, /// 0x2e4
			 0xe1cff7c5, /// 0x2e8
			 0x00c8cf6b, /// 0x2ec
			 0xe1ca366d, /// 0x2f0
			 0x645603eb, /// 0x2f4
			 0x305f49bd, /// 0x2f8
			 0x527fa149, /// 0x2fc
			 0xb29d003d, /// 0x300
			 0x9af15cfe, /// 0x304
			 0xe3a1da57, /// 0x308
			 0x80be2001, /// 0x30c
			 0x29073a07, /// 0x310
			 0x221c900b, /// 0x314
			 0xf2ee0bf8, /// 0x318
			 0xe6b8c557, /// 0x31c
			 0xb5c890a7, /// 0x320
			 0xd0d8c661, /// 0x324
			 0xbcf7ae85, /// 0x328
			 0xa98ab138, /// 0x32c
			 0xa117e385, /// 0x330
			 0xad9d9c45, /// 0x334
			 0xff250fff, /// 0x338
			 0x99bbae17, /// 0x33c
			 0x139688ee, /// 0x340
			 0x56462e8f, /// 0x344
			 0x2f7f30c7, /// 0x348
			 0x8969fd78, /// 0x34c
			 0x84f7af80, /// 0x350
			 0x937be37c, /// 0x354
			 0xbd1de4af, /// 0x358
			 0x5d788e9d, /// 0x35c
			 0xa90bfc70, /// 0x360
			 0x7a0265c9, /// 0x364
			 0x41716305, /// 0x368
			 0xe4b2bcdf, /// 0x36c
			 0x9a7655c7, /// 0x370
			 0x1f4d644c, /// 0x374
			 0x210086e7, /// 0x378
			 0xad1a2884, /// 0x37c
			 0x81d8d9b1, /// 0x380
			 0x3b2bef2e, /// 0x384
			 0xc0a5e2e4, /// 0x388
			 0x57f40236, /// 0x38c
			 0xdaa8ead9, /// 0x390
			 0x05015b60, /// 0x394
			 0x7d5c7ba8, /// 0x398
			 0x0ff36233, /// 0x39c
			 0x8094c6e3, /// 0x3a0
			 0xf89a37e5, /// 0x3a4
			 0xa52b5d6c, /// 0x3a8
			 0xe19dca2e, /// 0x3ac
			 0x84cd54f7, /// 0x3b0
			 0xf5604610, /// 0x3b4
			 0xb9e74c24, /// 0x3b8
			 0x01433ce2, /// 0x3bc
			 0x9a7a9ac3, /// 0x3c0
			 0xc2fa41a0, /// 0x3c4
			 0x9acd50a2, /// 0x3c8
			 0xa239ef9e, /// 0x3cc
			 0xeb4b2c4d, /// 0x3d0
			 0xff1ba6e8, /// 0x3d4
			 0x3718f28a, /// 0x3d8
			 0x88891ec5, /// 0x3dc
			 0x0c475ff6, /// 0x3e0
			 0x68ad134a, /// 0x3e4
			 0xb85c955b, /// 0x3e8
			 0x0fdf5d85, /// 0x3ec
			 0x6dfa194c, /// 0x3f0
			 0xb77e3df0, /// 0x3f4
			 0x7da01c04, /// 0x3f8
			 0x98f45a67, /// 0x3fc
			 0xd6cafc9b, /// 0x400
			 0x97f67551, /// 0x404
			 0xa5f6457b, /// 0x408
			 0x8ea62b85, /// 0x40c
			 0x9394d298, /// 0x410
			 0x97acfd5f, /// 0x414
			 0x3f102cf2, /// 0x418
			 0xac11440f, /// 0x41c
			 0xf1cc95a7, /// 0x420
			 0xb5e033c6, /// 0x424
			 0xae38b55f, /// 0x428
			 0x77d9f2fe, /// 0x42c
			 0x5fb42548, /// 0x430
			 0x33eba8a7, /// 0x434
			 0xe51b62a0, /// 0x438
			 0x93d61456, /// 0x43c
			 0x0e73951a, /// 0x440
			 0xc2d99e9c, /// 0x444
			 0x788f5428, /// 0x448
			 0xeb240a51, /// 0x44c
			 0xde01bce2, /// 0x450
			 0x720be4b7, /// 0x454
			 0x343abe09, /// 0x458
			 0xf1c009c7, /// 0x45c
			 0x33edb51c, /// 0x460
			 0x4b652acb, /// 0x464
			 0xd2e43daf, /// 0x468
			 0x1a7c43f8, /// 0x46c
			 0x454cc19b, /// 0x470
			 0x373d8e7f, /// 0x474
			 0x01b12a74, /// 0x478
			 0xe2839564, /// 0x47c
			 0xb4bc031e, /// 0x480
			 0xddefc0af, /// 0x484
			 0xdb1a4dd2, /// 0x488
			 0xeea796e9, /// 0x48c
			 0x195993aa, /// 0x490
			 0x7056e529, /// 0x494
			 0xd090b96f, /// 0x498
			 0x21527e53, /// 0x49c
			 0xa0d2d82a, /// 0x4a0
			 0xc4492441, /// 0x4a4
			 0x72e8a3ce, /// 0x4a8
			 0x4763d2f6, /// 0x4ac
			 0x3a0d747b, /// 0x4b0
			 0x6188ecdc, /// 0x4b4
			 0x16796ebc, /// 0x4b8
			 0xdce514d9, /// 0x4bc
			 0x51f9305f, /// 0x4c0
			 0x5ac25c93, /// 0x4c4
			 0x69b07e97, /// 0x4c8
			 0x9d87d3ad, /// 0x4cc
			 0x7536bdcb, /// 0x4d0
			 0x62019254, /// 0x4d4
			 0x0d5e5255, /// 0x4d8
			 0x8a97fc56, /// 0x4dc
			 0x97d5d90a, /// 0x4e0
			 0x3c0a9852, /// 0x4e4
			 0xbe407dc7, /// 0x4e8
			 0x98c85af8, /// 0x4ec
			 0x3ec030df, /// 0x4f0
			 0x09d47e90, /// 0x4f4
			 0xa7eae61c, /// 0x4f8
			 0x6bb27d36, /// 0x4fc
			 0x52586265, /// 0x500
			 0xd92b2f6b, /// 0x504
			 0xf766ff4f, /// 0x508
			 0x6093f16c, /// 0x50c
			 0xe62e51d9, /// 0x510
			 0x7bd41a58, /// 0x514
			 0x67115d43, /// 0x518
			 0x70ffb23a, /// 0x51c
			 0xc6dd9bc1, /// 0x520
			 0x9fc95666, /// 0x524
			 0x084811bd, /// 0x528
			 0xd5efcc47, /// 0x52c
			 0xe0aeeae3, /// 0x530
			 0x306c42af, /// 0x534
			 0x0b97df26, /// 0x538
			 0x1081ca18, /// 0x53c
			 0xdab13423, /// 0x540
			 0x1b215e73, /// 0x544
			 0xeec40281, /// 0x548
			 0x1cb4a63f, /// 0x54c
			 0x456c9dfc, /// 0x550
			 0x0b24fada, /// 0x554
			 0x9b069d96, /// 0x558
			 0x96dd9020, /// 0x55c
			 0x2d296e08, /// 0x560
			 0x69dcea5b, /// 0x564
			 0x65a525f5, /// 0x568
			 0x159c1025, /// 0x56c
			 0x82261984, /// 0x570
			 0x447f2217, /// 0x574
			 0xa9e15065, /// 0x578
			 0x617ddc8f, /// 0x57c
			 0x732615f5, /// 0x580
			 0x0b3f58d8, /// 0x584
			 0x500ad880, /// 0x588
			 0xc0996fc8, /// 0x58c
			 0xe37713a4, /// 0x590
			 0xb969dacd, /// 0x594
			 0xf29f55ee, /// 0x598
			 0x95523e54, /// 0x59c
			 0x0a40096b, /// 0x5a0
			 0x78cd3a27, /// 0x5a4
			 0x8bf3af3f, /// 0x5a8
			 0x0b6188b9, /// 0x5ac
			 0xd129c598, /// 0x5b0
			 0x2f729997, /// 0x5b4
			 0xde07f58e, /// 0x5b8
			 0x52cdee2a, /// 0x5bc
			 0x5947a674, /// 0x5c0
			 0xe326027d, /// 0x5c4
			 0xc4b7d0e3, /// 0x5c8
			 0xdf4d812d, /// 0x5cc
			 0x8ebfa777, /// 0x5d0
			 0x194a24a4, /// 0x5d4
			 0x0d2d1eaa, /// 0x5d8
			 0x2bf0c3d5, /// 0x5dc
			 0x3b357d7d, /// 0x5e0
			 0xaccbc229, /// 0x5e4
			 0x610e2bfd, /// 0x5e8
			 0x48bfe3ce, /// 0x5ec
			 0xfeeedc66, /// 0x5f0
			 0xb31d0115, /// 0x5f4
			 0x197211ac, /// 0x5f8
			 0x854d1ba1, /// 0x5fc
			 0x7ce5dd28, /// 0x600
			 0x4f7de3e5, /// 0x604
			 0xbf3dcc57, /// 0x608
			 0xa8520236, /// 0x60c
			 0xba767148, /// 0x610
			 0xe9e3082b, /// 0x614
			 0x3e624ebf, /// 0x618
			 0xfd89c860, /// 0x61c
			 0xbca7d00b, /// 0x620
			 0x5cf3dcbd, /// 0x624
			 0x8f523af8, /// 0x628
			 0x01da3d45, /// 0x62c
			 0x54dd8310, /// 0x630
			 0x39226fde, /// 0x634
			 0x919bc834, /// 0x638
			 0xc98cb308, /// 0x63c
			 0xac4e045b, /// 0x640
			 0xff64224b, /// 0x644
			 0xd3389f87, /// 0x648
			 0x47f4aa29, /// 0x64c
			 0xaa4d1e2e, /// 0x650
			 0x6bba9c7f, /// 0x654
			 0x16032bdc, /// 0x658
			 0x2e1c027d, /// 0x65c
			 0x72486d84, /// 0x660
			 0x431841cf, /// 0x664
			 0xc799bab0, /// 0x668
			 0x5b6bd108, /// 0x66c
			 0x406f8926, /// 0x670
			 0xecdfd01e, /// 0x674
			 0x2deae775, /// 0x678
			 0x75edcb9e, /// 0x67c
			 0xe4764b93, /// 0x680
			 0xd6447f9c, /// 0x684
			 0xaea206b7, /// 0x688
			 0x43b92835, /// 0x68c
			 0xb4752b45, /// 0x690
			 0xdb4157fe, /// 0x694
			 0x6d5fe003, /// 0x698
			 0x228796e0, /// 0x69c
			 0xcb38425e, /// 0x6a0
			 0x2919a378, /// 0x6a4
			 0xdb4ab254, /// 0x6a8
			 0x61b4fed8, /// 0x6ac
			 0x6c242b07, /// 0x6b0
			 0x63456b26, /// 0x6b4
			 0xff6afa06, /// 0x6b8
			 0x1e5d4639, /// 0x6bc
			 0x41c9caf6, /// 0x6c0
			 0x33cbcd11, /// 0x6c4
			 0xea7ba8c3, /// 0x6c8
			 0xe3536935, /// 0x6cc
			 0x010dc636, /// 0x6d0
			 0xaf91d58d, /// 0x6d4
			 0x84577dbc, /// 0x6d8
			 0x74bac63c, /// 0x6dc
			 0x4dd79af4, /// 0x6e0
			 0x9f4c2e38, /// 0x6e4
			 0x404342f4, /// 0x6e8
			 0x47bc6b75, /// 0x6ec
			 0x5625b1a2, /// 0x6f0
			 0xb99d46d2, /// 0x6f4
			 0xe89e414a, /// 0x6f8
			 0xa52a31e2, /// 0x6fc
			 0x2813e2a1, /// 0x700
			 0x6d1fd713, /// 0x704
			 0xd9e877e2, /// 0x708
			 0xf08da3a4, /// 0x70c
			 0xcd0b03f4, /// 0x710
			 0x9b2a04e8, /// 0x714
			 0x2a3e31ac, /// 0x718
			 0x1e5aa14e, /// 0x71c
			 0xc5b72653, /// 0x720
			 0x10238aee, /// 0x724
			 0x05256ee6, /// 0x728
			 0xe5c6af51, /// 0x72c
			 0x56becfe3, /// 0x730
			 0x9b15fc89, /// 0x734
			 0x067f1c2f, /// 0x738
			 0x7b912777, /// 0x73c
			 0x824b62f5, /// 0x740
			 0xb5757abc, /// 0x744
			 0x832403ac, /// 0x748
			 0x06f1a078, /// 0x74c
			 0x65645127, /// 0x750
			 0x4238abbe, /// 0x754
			 0xe1a29e39, /// 0x758
			 0xe24d8a88, /// 0x75c
			 0x6e019116, /// 0x760
			 0xb649f859, /// 0x764
			 0xdfd740e4, /// 0x768
			 0x121a04d5, /// 0x76c
			 0x8a07ba6f, /// 0x770
			 0x74aa725a, /// 0x774
			 0x0283d7a4, /// 0x778
			 0x745e5cb9, /// 0x77c
			 0x1ebd2de6, /// 0x780
			 0xb43a223f, /// 0x784
			 0xd33a0945, /// 0x788
			 0x6dc24972, /// 0x78c
			 0x2426fb86, /// 0x790
			 0x5c64471f, /// 0x794
			 0xb5a6528f, /// 0x798
			 0xc826457b, /// 0x79c
			 0x4be6eea4, /// 0x7a0
			 0x7eb195fa, /// 0x7a4
			 0xf06bfbe0, /// 0x7a8
			 0xa9870251, /// 0x7ac
			 0x328b8393, /// 0x7b0
			 0x3edb928a, /// 0x7b4
			 0xa74f70e4, /// 0x7b8
			 0xe06cd45a, /// 0x7bc
			 0x0a0336ec, /// 0x7c0
			 0x2ef33617, /// 0x7c4
			 0x2f2009af, /// 0x7c8
			 0x5003ff48, /// 0x7cc
			 0x80282f7e, /// 0x7d0
			 0x4ead98ce, /// 0x7d4
			 0xfdd388b5, /// 0x7d8
			 0xfed92b90, /// 0x7dc
			 0xa27f91a3, /// 0x7e0
			 0x90705185, /// 0x7e4
			 0x28ad0e7e, /// 0x7e8
			 0x880a466c, /// 0x7ec
			 0x7c578090, /// 0x7f0
			 0x4aa52886, /// 0x7f4
			 0xa8be1181, /// 0x7f8
			 0x583b396a, /// 0x7fc
			 0x7dbddc5d, /// 0x800
			 0x4227998e, /// 0x804
			 0xe77f448f, /// 0x808
			 0x2d761bf1, /// 0x80c
			 0xfaa6f825, /// 0x810
			 0x3f2fcf67, /// 0x814
			 0x491ce617, /// 0x818
			 0xdd885400, /// 0x81c
			 0xa877d9f1, /// 0x820
			 0x474d140e, /// 0x824
			 0xda12be06, /// 0x828
			 0x69ad2c77, /// 0x82c
			 0x39f3b45b, /// 0x830
			 0x5910842a, /// 0x834
			 0xf482fc44, /// 0x838
			 0x9d216da0, /// 0x83c
			 0xeec3e775, /// 0x840
			 0xc26b667d, /// 0x844
			 0x03142f74, /// 0x848
			 0x1258e731, /// 0x84c
			 0xd3e97071, /// 0x850
			 0x6b34c616, /// 0x854
			 0x1585e870, /// 0x858
			 0xdf9ef326, /// 0x85c
			 0x0108c7ff, /// 0x860
			 0x7a5cd632, /// 0x864
			 0x40d4abb6, /// 0x868
			 0xaa4bcce2, /// 0x86c
			 0x8874871f, /// 0x870
			 0x9e1a3ff6, /// 0x874
			 0xd0dcdf4c, /// 0x878
			 0xc2cf7d3b, /// 0x87c
			 0x90e81171, /// 0x880
			 0x261c3e07, /// 0x884
			 0x81666d93, /// 0x888
			 0x832c7e30, /// 0x88c
			 0x4295bb38, /// 0x890
			 0x1063380a, /// 0x894
			 0xc9f3d733, /// 0x898
			 0x3d1879a9, /// 0x89c
			 0x752f5c37, /// 0x8a0
			 0x058e4da3, /// 0x8a4
			 0x2f5272a5, /// 0x8a8
			 0x9383bdc6, /// 0x8ac
			 0x341ec33c, /// 0x8b0
			 0xa006f975, /// 0x8b4
			 0xe9018e48, /// 0x8b8
			 0xe5cae851, /// 0x8bc
			 0x7aabc39a, /// 0x8c0
			 0x303f17c9, /// 0x8c4
			 0x76134230, /// 0x8c8
			 0x15d5dbb8, /// 0x8cc
			 0xbd6ac47b, /// 0x8d0
			 0x63287d45, /// 0x8d4
			 0x1130cfbe, /// 0x8d8
			 0xd50efeb3, /// 0x8dc
			 0xa0b2f236, /// 0x8e0
			 0x704ef8fa, /// 0x8e4
			 0x889586ef, /// 0x8e8
			 0x9e843ae1, /// 0x8ec
			 0xc44587ed, /// 0x8f0
			 0xbf7ba699, /// 0x8f4
			 0xb30871ff, /// 0x8f8
			 0xbcb9c638, /// 0x8fc
			 0x9b35bfaa, /// 0x900
			 0xcb7fad97, /// 0x904
			 0x243d980d, /// 0x908
			 0xe8301fe7, /// 0x90c
			 0xa7b5ade1, /// 0x910
			 0x6314f636, /// 0x914
			 0x46f7011e, /// 0x918
			 0x2e3b172c, /// 0x91c
			 0xf26e2b83, /// 0x920
			 0xb1a79911, /// 0x924
			 0x86b31f85, /// 0x928
			 0xd52fee5c, /// 0x92c
			 0xae7e7a25, /// 0x930
			 0xcdf122aa, /// 0x934
			 0x7f1a7cfd, /// 0x938
			 0xd674e7b9, /// 0x93c
			 0x5b7617c3, /// 0x940
			 0x7f5966db, /// 0x944
			 0x07339aa9, /// 0x948
			 0x92285339, /// 0x94c
			 0xe4406eed, /// 0x950
			 0x04da2fe1, /// 0x954
			 0xe8c727ab, /// 0x958
			 0x8bde0410, /// 0x95c
			 0x55d0820e, /// 0x960
			 0x52f29582, /// 0x964
			 0x841c588d, /// 0x968
			 0x9fc65f56, /// 0x96c
			 0x640a98cd, /// 0x970
			 0x31d7eb37, /// 0x974
			 0x2c39a723, /// 0x978
			 0x1578282f, /// 0x97c
			 0x879cebf3, /// 0x980
			 0xc3bfd8ca, /// 0x984
			 0x66452c56, /// 0x988
			 0xe1025b26, /// 0x98c
			 0x83534261, /// 0x990
			 0x359e59f4, /// 0x994
			 0x467a5246, /// 0x998
			 0xeed2f2cb, /// 0x99c
			 0x387c8614, /// 0x9a0
			 0x1edc83ac, /// 0x9a4
			 0x0e8bb474, /// 0x9a8
			 0x276108cd, /// 0x9ac
			 0xb42184fe, /// 0x9b0
			 0xbecb9ab4, /// 0x9b4
			 0xdc90bf39, /// 0x9b8
			 0xac3456b8, /// 0x9bc
			 0xcfb9f38f, /// 0x9c0
			 0x99d04b64, /// 0x9c4
			 0x44fdfd36, /// 0x9c8
			 0xf6e18777, /// 0x9cc
			 0xb134b50b, /// 0x9d0
			 0xf4d88dc0, /// 0x9d4
			 0xb44ed957, /// 0x9d8
			 0x0c42c949, /// 0x9dc
			 0xd12e3a0f, /// 0x9e0
			 0xd78b308d, /// 0x9e4
			 0xfd09afd8, /// 0x9e8
			 0x3681f05b, /// 0x9ec
			 0xbfb1835c, /// 0x9f0
			 0xaa8b848a, /// 0x9f4
			 0x4356336a, /// 0x9f8
			 0x6c6eb666, /// 0x9fc
			 0x0c6a8d87, /// 0xa00
			 0xb14239cd, /// 0xa04
			 0x240f540e, /// 0xa08
			 0x78421a6f, /// 0xa0c
			 0x44f7d9b3, /// 0xa10
			 0x79b83f0f, /// 0xa14
			 0x0c887afd, /// 0xa18
			 0x7bdf7e0a, /// 0xa1c
			 0xcb81f388, /// 0xa20
			 0xccdd879a, /// 0xa24
			 0xb2dc16ea, /// 0xa28
			 0x927a11a2, /// 0xa2c
			 0x2db64999, /// 0xa30
			 0x9db472cc, /// 0xa34
			 0x2008e864, /// 0xa38
			 0xc496b810, /// 0xa3c
			 0x02d75191, /// 0xa40
			 0x9df0ef16, /// 0xa44
			 0xddfb885e, /// 0xa48
			 0xf7febc25, /// 0xa4c
			 0x8b97473c, /// 0xa50
			 0x3709c1e8, /// 0xa54
			 0x7bdf98de, /// 0xa58
			 0x5413a9a5, /// 0xa5c
			 0xc0b91ab5, /// 0xa60
			 0xfe073940, /// 0xa64
			 0x2f0e8a82, /// 0xa68
			 0xf54746ec, /// 0xa6c
			 0xe64f29de, /// 0xa70
			 0x81ecc54d, /// 0xa74
			 0x25aee37c, /// 0xa78
			 0xaf83b1c7, /// 0xa7c
			 0xdeac2937, /// 0xa80
			 0xff997eb8, /// 0xa84
			 0xc39a7735, /// 0xa88
			 0xb0f98cd8, /// 0xa8c
			 0xaf8b3de6, /// 0xa90
			 0x6948b686, /// 0xa94
			 0x9c9fa1d5, /// 0xa98
			 0xefaba974, /// 0xa9c
			 0x30613c9d, /// 0xaa0
			 0x32fbcfa3, /// 0xaa4
			 0x3221c7ec, /// 0xaa8
			 0xb66c3731, /// 0xaac
			 0xb4274924, /// 0xab0
			 0x7395044c, /// 0xab4
			 0x6d505b85, /// 0xab8
			 0xb661b382, /// 0xabc
			 0xb53eed8a, /// 0xac0
			 0x95744e80, /// 0xac4
			 0xda6f9597, /// 0xac8
			 0x054368df, /// 0xacc
			 0xfafbaa1b, /// 0xad0
			 0x4412b70c, /// 0xad4
			 0x00aeba67, /// 0xad8
			 0xf7519f83, /// 0xadc
			 0xc81f52bd, /// 0xae0
			 0xc7523570, /// 0xae4
			 0x7631c1fb, /// 0xae8
			 0x3672acd2, /// 0xaec
			 0x768a48d9, /// 0xaf0
			 0x8470be3c, /// 0xaf4
			 0x958bb1e7, /// 0xaf8
			 0x52401170, /// 0xafc
			 0x112b83f8, /// 0xb00
			 0xebbfe4d4, /// 0xb04
			 0x01145a61, /// 0xb08
			 0xf764cff8, /// 0xb0c
			 0x8e5560db, /// 0xb10
			 0xad66ca91, /// 0xb14
			 0x31f8d790, /// 0xb18
			 0xd7fed43b, /// 0xb1c
			 0x2ef1a836, /// 0xb20
			 0xdb60f1fe, /// 0xb24
			 0xfacb175b, /// 0xb28
			 0x0d945472, /// 0xb2c
			 0x23145fb0, /// 0xb30
			 0x1ee5eb38, /// 0xb34
			 0x18718eed, /// 0xb38
			 0x428dd766, /// 0xb3c
			 0x65da5aa7, /// 0xb40
			 0x60c44750, /// 0xb44
			 0xeeddce6e, /// 0xb48
			 0x198f5d34, /// 0xb4c
			 0xb17041aa, /// 0xb50
			 0x7c61118b, /// 0xb54
			 0x4d820d96, /// 0xb58
			 0x57fd4fb0, /// 0xb5c
			 0x3ce70587, /// 0xb60
			 0x9d882efb, /// 0xb64
			 0xf915a12f, /// 0xb68
			 0x1510b5fe, /// 0xb6c
			 0x4bdaad49, /// 0xb70
			 0x808b1044, /// 0xb74
			 0x80350b32, /// 0xb78
			 0x6f9b61d4, /// 0xb7c
			 0xe8960b77, /// 0xb80
			 0x9592d5ad, /// 0xb84
			 0x387316a3, /// 0xb88
			 0x8f46e0d0, /// 0xb8c
			 0xca9dae23, /// 0xb90
			 0xd26fb276, /// 0xb94
			 0x8c06d9db, /// 0xb98
			 0x1219065b, /// 0xb9c
			 0x0db6c0f9, /// 0xba0
			 0x1c86e6ae, /// 0xba4
			 0x7bf5f478, /// 0xba8
			 0xb3241edf, /// 0xbac
			 0xb5d12c55, /// 0xbb0
			 0xcb279684, /// 0xbb4
			 0xfe5fe54b, /// 0xbb8
			 0xc8abe8dc, /// 0xbbc
			 0x871351f9, /// 0xbc0
			 0xe9f6dd2a, /// 0xbc4
			 0x370ef58b, /// 0xbc8
			 0x141f4d51, /// 0xbcc
			 0x5e2435a9, /// 0xbd0
			 0x7ed5df6d, /// 0xbd4
			 0x82edfa25, /// 0xbd8
			 0x6e944e2e, /// 0xbdc
			 0xa910cb8d, /// 0xbe0
			 0x74807120, /// 0xbe4
			 0xaf7c74ff, /// 0xbe8
			 0x3434e2a6, /// 0xbec
			 0x3d321f16, /// 0xbf0
			 0xe1dcc334, /// 0xbf4
			 0x7a888ba7, /// 0xbf8
			 0x52974cfb, /// 0xbfc
			 0xbf3351d4, /// 0xc00
			 0xb8758781, /// 0xc04
			 0xe0d4aca3, /// 0xc08
			 0x7087a780, /// 0xc0c
			 0x63df4575, /// 0xc10
			 0x944238ec, /// 0xc14
			 0x5a458de1, /// 0xc18
			 0x09d5f1c1, /// 0xc1c
			 0xf9363502, /// 0xc20
			 0x7d55bf1b, /// 0xc24
			 0xa1d5e279, /// 0xc28
			 0x2dd1f710, /// 0xc2c
			 0x145faa5a, /// 0xc30
			 0x50ad719f, /// 0xc34
			 0x77cfcf43, /// 0xc38
			 0x6375351c, /// 0xc3c
			 0xe259a658, /// 0xc40
			 0x54f6c62d, /// 0xc44
			 0x1d778e68, /// 0xc48
			 0x1f350f82, /// 0xc4c
			 0xfea8eda6, /// 0xc50
			 0xb5de6480, /// 0xc54
			 0x18859d86, /// 0xc58
			 0xd9d702c3, /// 0xc5c
			 0xa8f32e82, /// 0xc60
			 0x208bb507, /// 0xc64
			 0x2864abed, /// 0xc68
			 0xac7c9158, /// 0xc6c
			 0x47e88c48, /// 0xc70
			 0x01924462, /// 0xc74
			 0x6e084666, /// 0xc78
			 0x762a1335, /// 0xc7c
			 0x5edd7100, /// 0xc80
			 0xb7bf6320, /// 0xc84
			 0x83a0cbb2, /// 0xc88
			 0x0058f4c6, /// 0xc8c
			 0xf7210179, /// 0xc90
			 0x146e35cc, /// 0xc94
			 0x56177890, /// 0xc98
			 0xed6a317f, /// 0xc9c
			 0xc899e011, /// 0xca0
			 0x69d23df3, /// 0xca4
			 0xeb8c62ed, /// 0xca8
			 0x97ccfccf, /// 0xcac
			 0x87d0ed73, /// 0xcb0
			 0x62e71f80, /// 0xcb4
			 0x82213a6d, /// 0xcb8
			 0x61e4733d, /// 0xcbc
			 0x06645f58, /// 0xcc0
			 0xb58212fd, /// 0xcc4
			 0x9bbe36ad, /// 0xcc8
			 0xc660449d, /// 0xccc
			 0xa1707e09, /// 0xcd0
			 0x4001b0c1, /// 0xcd4
			 0x4f42cc0a, /// 0xcd8
			 0x1e28c09e, /// 0xcdc
			 0x9ce3b20f, /// 0xce0
			 0xabc3976b, /// 0xce4
			 0x3ac11e9a, /// 0xce8
			 0x0542c4b0, /// 0xcec
			 0xe6f2974b, /// 0xcf0
			 0xff46a609, /// 0xcf4
			 0x886083ff, /// 0xcf8
			 0x493ae230, /// 0xcfc
			 0x7e8b397a, /// 0xd00
			 0x1aaf3daa, /// 0xd04
			 0x110cb477, /// 0xd08
			 0xde09983f, /// 0xd0c
			 0x4a656e9d, /// 0xd10
			 0x535ffd6f, /// 0xd14
			 0x61140c94, /// 0xd18
			 0xf014a278, /// 0xd1c
			 0x041acc05, /// 0xd20
			 0x4dc9d6a2, /// 0xd24
			 0xc606cb77, /// 0xd28
			 0xcef408cb, /// 0xd2c
			 0x1cec62d4, /// 0xd30
			 0xf81c5cf7, /// 0xd34
			 0x2aa2de72, /// 0xd38
			 0x36f853d3, /// 0xd3c
			 0x00ee1d9e, /// 0xd40
			 0x401aad96, /// 0xd44
			 0xa92c6bcc, /// 0xd48
			 0x23871ec5, /// 0xd4c
			 0x8257c308, /// 0xd50
			 0x02596ece, /// 0xd54
			 0x1f5c888c, /// 0xd58
			 0x786ddb83, /// 0xd5c
			 0xbe02a7c0, /// 0xd60
			 0x857e7437, /// 0xd64
			 0x340bdda9, /// 0xd68
			 0x9f6cd858, /// 0xd6c
			 0x540cbc9e, /// 0xd70
			 0xa0d7813b, /// 0xd74
			 0xe2a56f5a, /// 0xd78
			 0xb8cb9c0d, /// 0xd7c
			 0xca817829, /// 0xd80
			 0x61c4517e, /// 0xd84
			 0x0a55e468, /// 0xd88
			 0x888bb7f9, /// 0xd8c
			 0xf7f120eb, /// 0xd90
			 0x06714f00, /// 0xd94
			 0x794b389f, /// 0xd98
			 0x40d687e8, /// 0xd9c
			 0xd24a097a, /// 0xda0
			 0xb8acd987, /// 0xda4
			 0xf93c9279, /// 0xda8
			 0xe4f82c71, /// 0xdac
			 0x1e02584c, /// 0xdb0
			 0x91e35012, /// 0xdb4
			 0x2bd9e6bb, /// 0xdb8
			 0x6bd21b76, /// 0xdbc
			 0x1e7a842c, /// 0xdc0
			 0x34e8da23, /// 0xdc4
			 0x98427465, /// 0xdc8
			 0x77b2b68e, /// 0xdcc
			 0x72b09092, /// 0xdd0
			 0x88c66eba, /// 0xdd4
			 0x05acc2b8, /// 0xdd8
			 0x01686287, /// 0xddc
			 0x360ba1a2, /// 0xde0
			 0x54ada9f0, /// 0xde4
			 0xed3fc5ef, /// 0xde8
			 0xbc7812f7, /// 0xdec
			 0x0a332d29, /// 0xdf0
			 0xf14f656d, /// 0xdf4
			 0x0b0f09c6, /// 0xdf8
			 0x7f86cf0f, /// 0xdfc
			 0xb482199f, /// 0xe00
			 0x4a0ba3b6, /// 0xe04
			 0x735d9a80, /// 0xe08
			 0x32797ca3, /// 0xe0c
			 0xfe2c98cb, /// 0xe10
			 0x7839ab93, /// 0xe14
			 0x33717141, /// 0xe18
			 0xc2f2d418, /// 0xe1c
			 0xc8c7e0ac, /// 0xe20
			 0xefa1c049, /// 0xe24
			 0x43309545, /// 0xe28
			 0xeeb10ac3, /// 0xe2c
			 0x07f0d9fd, /// 0xe30
			 0x636f9c85, /// 0xe34
			 0x459aa9e3, /// 0xe38
			 0x1a53494e, /// 0xe3c
			 0x6f2f0e84, /// 0xe40
			 0xaed3bff3, /// 0xe44
			 0xd5c56975, /// 0xe48
			 0x98985d39, /// 0xe4c
			 0x438ad846, /// 0xe50
			 0x53fcf42e, /// 0xe54
			 0x289f8607, /// 0xe58
			 0x79bf6ec4, /// 0xe5c
			 0xdf974053, /// 0xe60
			 0x63dfa26c, /// 0xe64
			 0x60ebcd64, /// 0xe68
			 0x91221922, /// 0xe6c
			 0x659b0555, /// 0xe70
			 0x54062741, /// 0xe74
			 0x3a046155, /// 0xe78
			 0xd5727486, /// 0xe7c
			 0xb5b08de6, /// 0xe80
			 0x41f1e40e, /// 0xe84
			 0x00a06056, /// 0xe88
			 0xc1ae7026, /// 0xe8c
			 0x233185d7, /// 0xe90
			 0xbb4adf3b, /// 0xe94
			 0xf6dcd0ec, /// 0xe98
			 0x11c12eb2, /// 0xe9c
			 0xc6b80efb, /// 0xea0
			 0xce7f58a6, /// 0xea4
			 0x73dd22d1, /// 0xea8
			 0xc446db8e, /// 0xeac
			 0x6710e2ac, /// 0xeb0
			 0xf4168ddb, /// 0xeb4
			 0xd2637944, /// 0xeb8
			 0x6d4d06e0, /// 0xebc
			 0x04ee1e11, /// 0xec0
			 0x04ff848e, /// 0xec4
			 0x2a908b2f, /// 0xec8
			 0x5e496ae3, /// 0xecc
			 0xff25c7e0, /// 0xed0
			 0xfecc115b, /// 0xed4
			 0x6c524a37, /// 0xed8
			 0xe432fbeb, /// 0xedc
			 0x5c1f6688, /// 0xee0
			 0x7686053d, /// 0xee4
			 0xb741789c, /// 0xee8
			 0x5bff4c34, /// 0xeec
			 0xc8adfbc2, /// 0xef0
			 0x225ebe2e, /// 0xef4
			 0x1a7124e2, /// 0xef8
			 0x563f514f, /// 0xefc
			 0x5868d93c, /// 0xf00
			 0xce9e5ed6, /// 0xf04
			 0xf69cf1fe, /// 0xf08
			 0x0d84010a, /// 0xf0c
			 0x4cb9dcce, /// 0xf10
			 0x0a08b7e4, /// 0xf14
			 0x0279680b, /// 0xf18
			 0x1cf2aae7, /// 0xf1c
			 0x958cf04e, /// 0xf20
			 0x6fe5ace0, /// 0xf24
			 0x2a7db59a, /// 0xf28
			 0x365de76e, /// 0xf2c
			 0xdc669cf5, /// 0xf30
			 0x90edc935, /// 0xf34
			 0x4c2a9962, /// 0xf38
			 0x5a482ac5, /// 0xf3c
			 0x4bc8e5ab, /// 0xf40
			 0x413fdede, /// 0xf44
			 0x91e0dee9, /// 0xf48
			 0xd7f6e61f, /// 0xf4c
			 0x31490b1a, /// 0xf50
			 0x936fb682, /// 0xf54
			 0xa3926c96, /// 0xf58
			 0xb9cfc2dc, /// 0xf5c
			 0xef46baec, /// 0xf60
			 0x01c93b27, /// 0xf64
			 0xbabe3a0d, /// 0xf68
			 0x1b62ef6e, /// 0xf6c
			 0x68f35127, /// 0xf70
			 0xee96d917, /// 0xf74
			 0x8aa8f874, /// 0xf78
			 0xb763c530, /// 0xf7c
			 0x97d7257e, /// 0xf80
			 0xe9e50d7f, /// 0xf84
			 0x989158fa, /// 0xf88
			 0xb9834a40, /// 0xf8c
			 0xc16d3979, /// 0xf90
			 0x509b9f01, /// 0xf94
			 0xdc7b7e5b, /// 0xf98
			 0x7e52e074, /// 0xf9c
			 0x8289c1c0, /// 0xfa0
			 0xf871f97a, /// 0xfa4
			 0x16e61062, /// 0xfa8
			 0xd550d9f0, /// 0xfac
			 0xc1c9d40e, /// 0xfb0
			 0x9ab77905, /// 0xfb4
			 0xa8c99504, /// 0xfb8
			 0xc6a7ebbb, /// 0xfbc
			 0x311fb7d7, /// 0xfc0
			 0xe320181e, /// 0xfc4
			 0x4fc38d91, /// 0xfc8
			 0x9a132d51, /// 0xfcc
			 0x892163f9, /// 0xfd0
			 0xff25c0cd, /// 0xfd4
			 0xce664d28, /// 0xfd8
			 0xecf0dddc, /// 0xfdc
			 0x0ad51f0e, /// 0xfe0
			 0x82aae621, /// 0xfe4
			 0x42880d3e, /// 0xfe8
			 0xfe3948cb, /// 0xfec
			 0x37f8b990, /// 0xff0
			 0x988a7b68, /// 0xff4
			 0x259ee212, /// 0xff8
			 0xd1686c3f /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0xa1b9087f, /// 0x0
			 0x8c469d0e, /// 0x4
			 0x4a6c0856, /// 0x8
			 0xf27f9ba1, /// 0xc
			 0xd8c0ba83, /// 0x10
			 0x4e56b61f, /// 0x14
			 0xcf56560e, /// 0x18
			 0x366909e3, /// 0x1c
			 0x5d10356b, /// 0x20
			 0x545b52e7, /// 0x24
			 0xb9c0b2a1, /// 0x28
			 0x202fcd34, /// 0x2c
			 0x4fc2082d, /// 0x30
			 0xe5a3b091, /// 0x34
			 0x4329c274, /// 0x38
			 0xbf6d215f, /// 0x3c
			 0xb48ae58f, /// 0x40
			 0x4dc77cf3, /// 0x44
			 0xba592686, /// 0x48
			 0x26b2e913, /// 0x4c
			 0x488c597a, /// 0x50
			 0x782e8554, /// 0x54
			 0xb7df635d, /// 0x58
			 0x548e91b9, /// 0x5c
			 0xc404e590, /// 0x60
			 0x76b2c138, /// 0x64
			 0xfb6b89b7, /// 0x68
			 0x18e0d322, /// 0x6c
			 0xbdaad234, /// 0x70
			 0x742f8754, /// 0x74
			 0x804cbb6d, /// 0x78
			 0x86487be7, /// 0x7c
			 0x75dd89b0, /// 0x80
			 0x1feb00fc, /// 0x84
			 0x7a40ff94, /// 0x88
			 0x43ecd122, /// 0x8c
			 0xf3bd2981, /// 0x90
			 0x58bb93eb, /// 0x94
			 0x62b84fcb, /// 0x98
			 0x10ed8d58, /// 0x9c
			 0x3fed45de, /// 0xa0
			 0x25757e9f, /// 0xa4
			 0xc803e17f, /// 0xa8
			 0x448e14c5, /// 0xac
			 0x85fb041d, /// 0xb0
			 0x5505042a, /// 0xb4
			 0xe85e1858, /// 0xb8
			 0x4ecae27d, /// 0xbc
			 0x2b3eaa32, /// 0xc0
			 0xdba25a62, /// 0xc4
			 0x5e45e192, /// 0xc8
			 0x3e6e7dd1, /// 0xcc
			 0x421cbd01, /// 0xd0
			 0x4bf9ad7c, /// 0xd4
			 0x4fdd1226, /// 0xd8
			 0x463308c7, /// 0xdc
			 0xe739a553, /// 0xe0
			 0x47e021ec, /// 0xe4
			 0x6b70bafb, /// 0xe8
			 0xcae883db, /// 0xec
			 0xb6a96766, /// 0xf0
			 0x33cb0dc6, /// 0xf4
			 0x369b4b50, /// 0xf8
			 0x20ee238c, /// 0xfc
			 0xb609c201, /// 0x100
			 0xd165aeff, /// 0x104
			 0x91b1965c, /// 0x108
			 0x1e11c096, /// 0x10c
			 0x0e15246d, /// 0x110
			 0x5644fd4d, /// 0x114
			 0xf08e0776, /// 0x118
			 0x06633964, /// 0x11c
			 0xe4e70d5b, /// 0x120
			 0x36cfb317, /// 0x124
			 0xa2c52f4b, /// 0x128
			 0x28a91f91, /// 0x12c
			 0x16cdc277, /// 0x130
			 0x14995a27, /// 0x134
			 0x209005d9, /// 0x138
			 0x507a5457, /// 0x13c
			 0x526ef2c4, /// 0x140
			 0x4d2d6698, /// 0x144
			 0x13c300ac, /// 0x148
			 0x74b209ba, /// 0x14c
			 0x7935f3cb, /// 0x150
			 0xed290d40, /// 0x154
			 0x50b75c54, /// 0x158
			 0xf9e0607c, /// 0x15c
			 0x8a4c3a7d, /// 0x160
			 0xee3902a4, /// 0x164
			 0xf257882e, /// 0x168
			 0x88918f6a, /// 0x16c
			 0x82fb0cb1, /// 0x170
			 0xa29d6ec1, /// 0x174
			 0x443da949, /// 0x178
			 0x9129ff66, /// 0x17c
			 0xcfaac7f7, /// 0x180
			 0xf5ba8688, /// 0x184
			 0x2a18fa44, /// 0x188
			 0xde002cbb, /// 0x18c
			 0xa44aae5c, /// 0x190
			 0x42d79c84, /// 0x194
			 0xba1c544d, /// 0x198
			 0x040d01cc, /// 0x19c
			 0x7a701541, /// 0x1a0
			 0x0e7233b1, /// 0x1a4
			 0x16a8c91e, /// 0x1a8
			 0x38f401cd, /// 0x1ac
			 0x669a48ec, /// 0x1b0
			 0xb6ab2901, /// 0x1b4
			 0x7c8535f7, /// 0x1b8
			 0x0bd94962, /// 0x1bc
			 0x5d0945c8, /// 0x1c0
			 0x52bb6594, /// 0x1c4
			 0xac4a8a15, /// 0x1c8
			 0xbbce7d51, /// 0x1cc
			 0xb45a707f, /// 0x1d0
			 0x1d735e73, /// 0x1d4
			 0xb24b25b3, /// 0x1d8
			 0xda561af4, /// 0x1dc
			 0x91456782, /// 0x1e0
			 0x858cd0dc, /// 0x1e4
			 0xd7724d40, /// 0x1e8
			 0xe0853059, /// 0x1ec
			 0xa90ce4e7, /// 0x1f0
			 0x61e7b913, /// 0x1f4
			 0x77c6e4ad, /// 0x1f8
			 0x0e0551cb, /// 0x1fc
			 0x319b695a, /// 0x200
			 0x5a95b9c1, /// 0x204
			 0x2e5591cb, /// 0x208
			 0x0de8049f, /// 0x20c
			 0x38be13b8, /// 0x210
			 0x7cc0ec07, /// 0x214
			 0xfb2c6aff, /// 0x218
			 0x230ae5cc, /// 0x21c
			 0xc70f16db, /// 0x220
			 0x139a4087, /// 0x224
			 0xf09e794a, /// 0x228
			 0xfc22b2a4, /// 0x22c
			 0x32f45ca3, /// 0x230
			 0xaad17478, /// 0x234
			 0x6dda6541, /// 0x238
			 0xacfb34c5, /// 0x23c
			 0xe01e86b6, /// 0x240
			 0xac656c18, /// 0x244
			 0x6a5d2eac, /// 0x248
			 0x579e6a5a, /// 0x24c
			 0xcd1abf8e, /// 0x250
			 0x3754b29e, /// 0x254
			 0xabc47ce0, /// 0x258
			 0x9e891b07, /// 0x25c
			 0x3ffab09d, /// 0x260
			 0xea3a4c40, /// 0x264
			 0x27d4233e, /// 0x268
			 0x1c17f34d, /// 0x26c
			 0xf0e0ce25, /// 0x270
			 0x7bf2ab06, /// 0x274
			 0xf788f8e9, /// 0x278
			 0xec55ec14, /// 0x27c
			 0xa7651422, /// 0x280
			 0x9f54bc52, /// 0x284
			 0x047847e4, /// 0x288
			 0x4da3cb2f, /// 0x28c
			 0x4110ee05, /// 0x290
			 0x2a8a5ca7, /// 0x294
			 0x03845f15, /// 0x298
			 0xcba93609, /// 0x29c
			 0x3008a64c, /// 0x2a0
			 0xc0a37da2, /// 0x2a4
			 0x8ee3e2c3, /// 0x2a8
			 0xccf7afd6, /// 0x2ac
			 0x16d02700, /// 0x2b0
			 0xa401b62e, /// 0x2b4
			 0xe49ab0c6, /// 0x2b8
			 0xc9dd1b2f, /// 0x2bc
			 0x96293b19, /// 0x2c0
			 0x76f3caf4, /// 0x2c4
			 0x65e1aa55, /// 0x2c8
			 0x7f965f22, /// 0x2cc
			 0xd9297208, /// 0x2d0
			 0x144fed28, /// 0x2d4
			 0x2fb95763, /// 0x2d8
			 0xf1e6739f, /// 0x2dc
			 0x649c2a18, /// 0x2e0
			 0xf8e64e3b, /// 0x2e4
			 0x23781965, /// 0x2e8
			 0xa428198f, /// 0x2ec
			 0x62d85d2d, /// 0x2f0
			 0x1bb9363a, /// 0x2f4
			 0xc4d87573, /// 0x2f8
			 0x5af578fc, /// 0x2fc
			 0xcb2d0673, /// 0x300
			 0x09ad1a9c, /// 0x304
			 0x560b4895, /// 0x308
			 0x50ba1a35, /// 0x30c
			 0x429899a9, /// 0x310
			 0x8e3dcf4f, /// 0x314
			 0x06cca3ad, /// 0x318
			 0xbf98b27d, /// 0x31c
			 0x3da6ac4f, /// 0x320
			 0x8ed80574, /// 0x324
			 0x885cec78, /// 0x328
			 0x8f4e85fa, /// 0x32c
			 0x500755e6, /// 0x330
			 0x62e965dc, /// 0x334
			 0xf8e96ec9, /// 0x338
			 0x629c4f3e, /// 0x33c
			 0xdf21e5c3, /// 0x340
			 0x721f6cf4, /// 0x344
			 0xa3fdf874, /// 0x348
			 0x23e0a8d7, /// 0x34c
			 0x8ac51e97, /// 0x350
			 0x9cbc05fa, /// 0x354
			 0x470a3a97, /// 0x358
			 0x64977c82, /// 0x35c
			 0xab782d6d, /// 0x360
			 0xbd5e6264, /// 0x364
			 0xfc4aa85a, /// 0x368
			 0xac4f997d, /// 0x36c
			 0x09edb300, /// 0x370
			 0x7e2c5ab8, /// 0x374
			 0xbd0b8e07, /// 0x378
			 0x0106d329, /// 0x37c
			 0xbe23abd8, /// 0x380
			 0x0205e3c8, /// 0x384
			 0xa609ad6f, /// 0x388
			 0x35d284f6, /// 0x38c
			 0x71ceb3c6, /// 0x390
			 0xbbed4a7b, /// 0x394
			 0x6376b1bc, /// 0x398
			 0xc575a637, /// 0x39c
			 0xdad298df, /// 0x3a0
			 0x07dc756f, /// 0x3a4
			 0xaa3535ac, /// 0x3a8
			 0x433bdbc9, /// 0x3ac
			 0x8910719e, /// 0x3b0
			 0x3b9d116b, /// 0x3b4
			 0xd9e23e76, /// 0x3b8
			 0x4cf10c57, /// 0x3bc
			 0x5b74faaa, /// 0x3c0
			 0xf0ae2701, /// 0x3c4
			 0x00eb7b92, /// 0x3c8
			 0x574c5065, /// 0x3cc
			 0xa492534c, /// 0x3d0
			 0x80d6c8ac, /// 0x3d4
			 0xc3a50744, /// 0x3d8
			 0x06f0ff10, /// 0x3dc
			 0x79eb498a, /// 0x3e0
			 0x1690955e, /// 0x3e4
			 0x7230533c, /// 0x3e8
			 0xe0fed9b0, /// 0x3ec
			 0xdd1edcbb, /// 0x3f0
			 0xdbde3234, /// 0x3f4
			 0xc8c82d50, /// 0x3f8
			 0x936c4ea8, /// 0x3fc
			 0x2a9e9594, /// 0x400
			 0xa0e74c3a, /// 0x404
			 0xea7ab86f, /// 0x408
			 0x89bcdd08, /// 0x40c
			 0x8f84011a, /// 0x410
			 0x45571332, /// 0x414
			 0xbfa7ff50, /// 0x418
			 0x8de46697, /// 0x41c
			 0xcfddef55, /// 0x420
			 0xd170138c, /// 0x424
			 0xe71c9219, /// 0x428
			 0x812a615a, /// 0x42c
			 0x9acbedbf, /// 0x430
			 0x4d26e9aa, /// 0x434
			 0x0ea1125e, /// 0x438
			 0x9258f8f4, /// 0x43c
			 0xd378dbc3, /// 0x440
			 0x271d1cbf, /// 0x444
			 0xf18188be, /// 0x448
			 0xb9105937, /// 0x44c
			 0xd03e1e1a, /// 0x450
			 0xdac7501b, /// 0x454
			 0xd3741515, /// 0x458
			 0x4dda72e8, /// 0x45c
			 0x2560d758, /// 0x460
			 0x5f0eaac4, /// 0x464
			 0x55fa5273, /// 0x468
			 0x84272a24, /// 0x46c
			 0x2a12a7ac, /// 0x470
			 0x934b6c14, /// 0x474
			 0x85316264, /// 0x478
			 0x19e84812, /// 0x47c
			 0x949b9775, /// 0x480
			 0xde507e3b, /// 0x484
			 0x5e5a3b25, /// 0x488
			 0x088d8817, /// 0x48c
			 0x192075fc, /// 0x490
			 0xfc2721ea, /// 0x494
			 0xeda868f1, /// 0x498
			 0x15046028, /// 0x49c
			 0x1135de6a, /// 0x4a0
			 0x85cb9b4d, /// 0x4a4
			 0xb9877e37, /// 0x4a8
			 0x31ca5400, /// 0x4ac
			 0x4d082830, /// 0x4b0
			 0x921dc8f8, /// 0x4b4
			 0x42409ece, /// 0x4b8
			 0xd5ef10d1, /// 0x4bc
			 0xcf76016a, /// 0x4c0
			 0xb8122e97, /// 0x4c4
			 0x79d9b7d2, /// 0x4c8
			 0x02c76d47, /// 0x4cc
			 0x715a2ab6, /// 0x4d0
			 0xb865ce1d, /// 0x4d4
			 0x032b08b5, /// 0x4d8
			 0xc492d505, /// 0x4dc
			 0x260d1746, /// 0x4e0
			 0xb9193c3b, /// 0x4e4
			 0x7471dc1b, /// 0x4e8
			 0x2f4986f6, /// 0x4ec
			 0x1a8b603c, /// 0x4f0
			 0x1f0a6ae2, /// 0x4f4
			 0x8f0fa005, /// 0x4f8
			 0xb0483fb0, /// 0x4fc
			 0x59512728, /// 0x500
			 0x19bf93fd, /// 0x504
			 0x4d00934a, /// 0x508
			 0xa6e29b9f, /// 0x50c
			 0x3c05aecb, /// 0x510
			 0x74cd0014, /// 0x514
			 0xb9a5bc21, /// 0x518
			 0x942023ba, /// 0x51c
			 0xa46fae46, /// 0x520
			 0x61c13cc4, /// 0x524
			 0x0c958aaf, /// 0x528
			 0x2a5a9540, /// 0x52c
			 0x46641012, /// 0x530
			 0x5aa44754, /// 0x534
			 0xa12a2056, /// 0x538
			 0x2a203d23, /// 0x53c
			 0xc1401e79, /// 0x540
			 0x81655df7, /// 0x544
			 0x36588d8a, /// 0x548
			 0xb2df29ca, /// 0x54c
			 0x8674a3b3, /// 0x550
			 0x71d8807f, /// 0x554
			 0x2fc39c4e, /// 0x558
			 0x0571b56a, /// 0x55c
			 0x9b0a52d5, /// 0x560
			 0xd07ac65b, /// 0x564
			 0x7e5a199e, /// 0x568
			 0x88825854, /// 0x56c
			 0x722f3f11, /// 0x570
			 0x4c9c7ce2, /// 0x574
			 0xa233ec54, /// 0x578
			 0x459e1102, /// 0x57c
			 0x3fd72f7d, /// 0x580
			 0xa4ffffa5, /// 0x584
			 0xa751542e, /// 0x588
			 0x1ef93f91, /// 0x58c
			 0x56727a76, /// 0x590
			 0x7a2d54eb, /// 0x594
			 0x0333c97d, /// 0x598
			 0x352ecc40, /// 0x59c
			 0xdfd31925, /// 0x5a0
			 0x6363d35f, /// 0x5a4
			 0x2aa658b5, /// 0x5a8
			 0x20ee5a52, /// 0x5ac
			 0x6f1dc6b8, /// 0x5b0
			 0xa7dd93d8, /// 0x5b4
			 0x0ca37ac6, /// 0x5b8
			 0x9260a5b0, /// 0x5bc
			 0x07d26f25, /// 0x5c0
			 0xe0bb5e59, /// 0x5c4
			 0x7df71175, /// 0x5c8
			 0x6fa0ddb7, /// 0x5cc
			 0x999cbb9f, /// 0x5d0
			 0x5765d670, /// 0x5d4
			 0x3f89784c, /// 0x5d8
			 0x73d2223b, /// 0x5dc
			 0xb17ae753, /// 0x5e0
			 0xedefc76d, /// 0x5e4
			 0x55171115, /// 0x5e8
			 0x55612528, /// 0x5ec
			 0x362cc69e, /// 0x5f0
			 0x4c9878c0, /// 0x5f4
			 0x08f08d0f, /// 0x5f8
			 0x0047f61c, /// 0x5fc
			 0x378c2238, /// 0x600
			 0x351c07ac, /// 0x604
			 0x22ddd72a, /// 0x608
			 0xcbe96c59, /// 0x60c
			 0xab6b6f1f, /// 0x610
			 0xaa5d804c, /// 0x614
			 0x1385e00e, /// 0x618
			 0xf042d359, /// 0x61c
			 0x157aa3ce, /// 0x620
			 0xd82d3cad, /// 0x624
			 0x43112fa8, /// 0x628
			 0x132eb23f, /// 0x62c
			 0xe59d53d1, /// 0x630
			 0x664b2155, /// 0x634
			 0x1f8f50ca, /// 0x638
			 0x064bfb35, /// 0x63c
			 0xc57b8b8c, /// 0x640
			 0x171ad345, /// 0x644
			 0x71b5aef0, /// 0x648
			 0x76bc61fc, /// 0x64c
			 0x10e0c76b, /// 0x650
			 0xcf30e85d, /// 0x654
			 0xcc56a4de, /// 0x658
			 0xb4fa8e84, /// 0x65c
			 0x82df045a, /// 0x660
			 0x6f2fe905, /// 0x664
			 0x13a8287f, /// 0x668
			 0x1a3f5196, /// 0x66c
			 0x4be85d6f, /// 0x670
			 0xb4cdf1d2, /// 0x674
			 0x13e80257, /// 0x678
			 0xfc9dcf3f, /// 0x67c
			 0xb297e8d8, /// 0x680
			 0x7f48fa45, /// 0x684
			 0x25fd1f33, /// 0x688
			 0x255e43c8, /// 0x68c
			 0xe7cecff3, /// 0x690
			 0xae4698af, /// 0x694
			 0x4a5aac4a, /// 0x698
			 0x9f659904, /// 0x69c
			 0x8441707c, /// 0x6a0
			 0x282a060b, /// 0x6a4
			 0xb816b1bc, /// 0x6a8
			 0x2e6c4063, /// 0x6ac
			 0x065cd6b1, /// 0x6b0
			 0x8d5b1e72, /// 0x6b4
			 0x2a109989, /// 0x6b8
			 0xf4e91cc3, /// 0x6bc
			 0x1caea994, /// 0x6c0
			 0xd7d3e281, /// 0x6c4
			 0x0a8c7d1b, /// 0x6c8
			 0xb8b5d954, /// 0x6cc
			 0x67cea373, /// 0x6d0
			 0x0009ead1, /// 0x6d4
			 0xe7103451, /// 0x6d8
			 0x73274fbd, /// 0x6dc
			 0xae411e7e, /// 0x6e0
			 0x461f933b, /// 0x6e4
			 0xca972747, /// 0x6e8
			 0xa9134418, /// 0x6ec
			 0xa57d42d2, /// 0x6f0
			 0x3e222217, /// 0x6f4
			 0x5db72e43, /// 0x6f8
			 0x12b2c26e, /// 0x6fc
			 0x357f0ef0, /// 0x700
			 0x0684acf9, /// 0x704
			 0xadb38c77, /// 0x708
			 0xecdf84cb, /// 0x70c
			 0xaf764afc, /// 0x710
			 0x4a5ba3ce, /// 0x714
			 0x11f7cb5d, /// 0x718
			 0x2ec6d17a, /// 0x71c
			 0x59634a85, /// 0x720
			 0xa7911373, /// 0x724
			 0x5ef23551, /// 0x728
			 0x09de4cf0, /// 0x72c
			 0xe6d74d9e, /// 0x730
			 0x5ac858f5, /// 0x734
			 0x5b00517a, /// 0x738
			 0x4395a3bc, /// 0x73c
			 0xfde934e0, /// 0x740
			 0x73b78578, /// 0x744
			 0x89b26d2a, /// 0x748
			 0xa27891b1, /// 0x74c
			 0xba1ad953, /// 0x750
			 0x47eaaaee, /// 0x754
			 0x230af759, /// 0x758
			 0x71787cff, /// 0x75c
			 0x0f1bc65f, /// 0x760
			 0x490f5bc0, /// 0x764
			 0xa53262ac, /// 0x768
			 0x68930722, /// 0x76c
			 0x40195d41, /// 0x770
			 0x92a3f8fc, /// 0x774
			 0xfca83b05, /// 0x778
			 0xd3163373, /// 0x77c
			 0x0567bb49, /// 0x780
			 0x440b17dd, /// 0x784
			 0x47532833, /// 0x788
			 0xaa36a3f1, /// 0x78c
			 0x648de905, /// 0x790
			 0x887d8a5a, /// 0x794
			 0x6cfa2f96, /// 0x798
			 0x30729e15, /// 0x79c
			 0x3fdcfa73, /// 0x7a0
			 0xf0d4ee15, /// 0x7a4
			 0xac2707e1, /// 0x7a8
			 0x6f81c2dc, /// 0x7ac
			 0xe9dacd31, /// 0x7b0
			 0x08158f3d, /// 0x7b4
			 0x7b97eab2, /// 0x7b8
			 0x8ab70a55, /// 0x7bc
			 0x7cb3f267, /// 0x7c0
			 0x36a83cad, /// 0x7c4
			 0x88df7e54, /// 0x7c8
			 0x63083c73, /// 0x7cc
			 0x9abd9dbd, /// 0x7d0
			 0x2f2ef687, /// 0x7d4
			 0xe299fdd4, /// 0x7d8
			 0x4e5811ee, /// 0x7dc
			 0x2866f8c5, /// 0x7e0
			 0x556e273c, /// 0x7e4
			 0x721e0ec4, /// 0x7e8
			 0x9210b6b5, /// 0x7ec
			 0x923b451d, /// 0x7f0
			 0x2f96d658, /// 0x7f4
			 0x31ef8518, /// 0x7f8
			 0x3bc7438a, /// 0x7fc
			 0xcc258e60, /// 0x800
			 0x678335c2, /// 0x804
			 0x694f2606, /// 0x808
			 0x31559619, /// 0x80c
			 0x25492d8e, /// 0x810
			 0x770ba8e9, /// 0x814
			 0x27f76567, /// 0x818
			 0xb71ce69e, /// 0x81c
			 0xe69ca5f8, /// 0x820
			 0x744ccdf7, /// 0x824
			 0x1eeef4b1, /// 0x828
			 0x717c8faa, /// 0x82c
			 0x7c31d712, /// 0x830
			 0x7305f4be, /// 0x834
			 0x1d17ec77, /// 0x838
			 0x2ef65c46, /// 0x83c
			 0x8331d0e5, /// 0x840
			 0xbb71911c, /// 0x844
			 0xdf09b7eb, /// 0x848
			 0x8731c146, /// 0x84c
			 0xb5b63b06, /// 0x850
			 0x454480cb, /// 0x854
			 0xc4870134, /// 0x858
			 0x49401dd7, /// 0x85c
			 0x9fce5dbc, /// 0x860
			 0x6287b4ce, /// 0x864
			 0x54410a5a, /// 0x868
			 0x241102ec, /// 0x86c
			 0x8aecaa51, /// 0x870
			 0x817261b4, /// 0x874
			 0x60f9dc74, /// 0x878
			 0x2bf82fcb, /// 0x87c
			 0x235e306e, /// 0x880
			 0x8c931d6e, /// 0x884
			 0x56da580d, /// 0x888
			 0xdabc5610, /// 0x88c
			 0x7cdee192, /// 0x890
			 0x32f80954, /// 0x894
			 0xd3647639, /// 0x898
			 0xcd9df8f8, /// 0x89c
			 0x675360ac, /// 0x8a0
			 0x15dd7c4b, /// 0x8a4
			 0xc835684e, /// 0x8a8
			 0xb5c5b864, /// 0x8ac
			 0x2fd714d4, /// 0x8b0
			 0x5f8e983b, /// 0x8b4
			 0xc82148df, /// 0x8b8
			 0x8b787368, /// 0x8bc
			 0xcb39bd5c, /// 0x8c0
			 0x7708dea4, /// 0x8c4
			 0x9949c8e4, /// 0x8c8
			 0xb0770979, /// 0x8cc
			 0x8ec6477c, /// 0x8d0
			 0x8fb69a4e, /// 0x8d4
			 0x2fb329e2, /// 0x8d8
			 0x33231fb8, /// 0x8dc
			 0xaaf9daf6, /// 0x8e0
			 0x84f478bb, /// 0x8e4
			 0x94fc3757, /// 0x8e8
			 0x78b838bc, /// 0x8ec
			 0x5eb7a4cf, /// 0x8f0
			 0x595d07ed, /// 0x8f4
			 0x616c4a3e, /// 0x8f8
			 0xb549aff1, /// 0x8fc
			 0xd614c39a, /// 0x900
			 0x0adab0d9, /// 0x904
			 0xc0e77317, /// 0x908
			 0x4a07448a, /// 0x90c
			 0x0a4a53e7, /// 0x910
			 0x567c0a19, /// 0x914
			 0x47ed6111, /// 0x918
			 0xbb9703d4, /// 0x91c
			 0x14a65116, /// 0x920
			 0xb10a443e, /// 0x924
			 0x3af0de95, /// 0x928
			 0xb0c5b59b, /// 0x92c
			 0x8acb2574, /// 0x930
			 0x38174787, /// 0x934
			 0x44bf5a5b, /// 0x938
			 0x0fa8e6a6, /// 0x93c
			 0x53f7209a, /// 0x940
			 0xf6b21b1a, /// 0x944
			 0x8b63084b, /// 0x948
			 0xda295077, /// 0x94c
			 0xca9fffba, /// 0x950
			 0xb61994c9, /// 0x954
			 0x34f167be, /// 0x958
			 0x825e7a1e, /// 0x95c
			 0xd8013146, /// 0x960
			 0xf4bce2ed, /// 0x964
			 0x8a392a4a, /// 0x968
			 0x4a8cf199, /// 0x96c
			 0x478adc73, /// 0x970
			 0x4a256116, /// 0x974
			 0x4bdac0a2, /// 0x978
			 0xb6788145, /// 0x97c
			 0x34ef9fc1, /// 0x980
			 0x1c6b9356, /// 0x984
			 0x46ab4ad6, /// 0x988
			 0x652b6950, /// 0x98c
			 0xc5a1e5aa, /// 0x990
			 0xc59fdd74, /// 0x994
			 0x96e07473, /// 0x998
			 0x8c880433, /// 0x99c
			 0x57a8ed75, /// 0x9a0
			 0x9e53534b, /// 0x9a4
			 0x0ce76acd, /// 0x9a8
			 0x9c3af21e, /// 0x9ac
			 0x97c4a111, /// 0x9b0
			 0xef3ed564, /// 0x9b4
			 0x3ae4156d, /// 0x9b8
			 0x9c2234d0, /// 0x9bc
			 0xf886b934, /// 0x9c0
			 0x44d3da24, /// 0x9c4
			 0xda436649, /// 0x9c8
			 0xf26dd29c, /// 0x9cc
			 0xe76eaedd, /// 0x9d0
			 0x7a8bc5a0, /// 0x9d4
			 0x9b977bd5, /// 0x9d8
			 0x45d9026f, /// 0x9dc
			 0x8bb33ea4, /// 0x9e0
			 0x74de6040, /// 0x9e4
			 0xf5e6cbf4, /// 0x9e8
			 0xa6cb228d, /// 0x9ec
			 0xce878fa4, /// 0x9f0
			 0xcf9df563, /// 0x9f4
			 0xd9c984fb, /// 0x9f8
			 0xadbb3189, /// 0x9fc
			 0xf547287e, /// 0xa00
			 0x7bd5c56a, /// 0xa04
			 0x1ce29186, /// 0xa08
			 0x81fd9432, /// 0xa0c
			 0xc31e81e4, /// 0xa10
			 0x058e725a, /// 0xa14
			 0x0184c886, /// 0xa18
			 0x0f3bec4a, /// 0xa1c
			 0x175c6541, /// 0xa20
			 0x9f4fe444, /// 0xa24
			 0x3c2e05e8, /// 0xa28
			 0x1f3733a9, /// 0xa2c
			 0x75c78719, /// 0xa30
			 0x616930ec, /// 0xa34
			 0x67dff797, /// 0xa38
			 0x76db9bb6, /// 0xa3c
			 0x77ae6fcf, /// 0xa40
			 0xf21873e8, /// 0xa44
			 0xa9dc9b60, /// 0xa48
			 0x4fe0ca8e, /// 0xa4c
			 0xc74ae88f, /// 0xa50
			 0x6c9a7f46, /// 0xa54
			 0xc53fb259, /// 0xa58
			 0xf9cba3fe, /// 0xa5c
			 0xf5e4698b, /// 0xa60
			 0x0e35cc86, /// 0xa64
			 0x03e1a840, /// 0xa68
			 0x821cd168, /// 0xa6c
			 0xe151e2fd, /// 0xa70
			 0x50f5b59a, /// 0xa74
			 0xf7fd269c, /// 0xa78
			 0xf1115edd, /// 0xa7c
			 0x7978c07d, /// 0xa80
			 0x090ef807, /// 0xa84
			 0x608964df, /// 0xa88
			 0x279c3785, /// 0xa8c
			 0xccd7481f, /// 0xa90
			 0xfbbd69fc, /// 0xa94
			 0xd4b3e943, /// 0xa98
			 0x703f1f4f, /// 0xa9c
			 0xbe7d8895, /// 0xaa0
			 0xcd533940, /// 0xaa4
			 0x9e0faa2d, /// 0xaa8
			 0xabf280d0, /// 0xaac
			 0x477ae711, /// 0xab0
			 0xc51849dc, /// 0xab4
			 0x16857a21, /// 0xab8
			 0x6b55868f, /// 0xabc
			 0xc8dc9ec0, /// 0xac0
			 0x6afce7c5, /// 0xac4
			 0x377a6d21, /// 0xac8
			 0xba41a27a, /// 0xacc
			 0x4158884d, /// 0xad0
			 0x3c1dec6d, /// 0xad4
			 0xfa29952d, /// 0xad8
			 0xae21730f, /// 0xadc
			 0x1fadd8d0, /// 0xae0
			 0x33370990, /// 0xae4
			 0xfaf3496f, /// 0xae8
			 0x34d7589c, /// 0xaec
			 0x35281654, /// 0xaf0
			 0xef866fcf, /// 0xaf4
			 0x5df76633, /// 0xaf8
			 0x5bc3e0e4, /// 0xafc
			 0x59677151, /// 0xb00
			 0x7c3438e0, /// 0xb04
			 0x029d0bfd, /// 0xb08
			 0x787a3f19, /// 0xb0c
			 0xd38a681e, /// 0xb10
			 0xfccbc206, /// 0xb14
			 0x57818d9e, /// 0xb18
			 0x8fcbdfc8, /// 0xb1c
			 0xf64d7a84, /// 0xb20
			 0xbd9e75e4, /// 0xb24
			 0x0761ca32, /// 0xb28
			 0x8eb497be, /// 0xb2c
			 0xc44053ef, /// 0xb30
			 0x3404a21b, /// 0xb34
			 0x4de1c746, /// 0xb38
			 0x2dd3e662, /// 0xb3c
			 0x9b6e3ab0, /// 0xb40
			 0x019f838f, /// 0xb44
			 0x5582507d, /// 0xb48
			 0xfda36ec7, /// 0xb4c
			 0x16181b1a, /// 0xb50
			 0xa2ed0bb2, /// 0xb54
			 0x1c1174af, /// 0xb58
			 0x0b629f6b, /// 0xb5c
			 0xa697e9ab, /// 0xb60
			 0x2a53a4e9, /// 0xb64
			 0x4ae7bef9, /// 0xb68
			 0x5b632acb, /// 0xb6c
			 0xcd0e53d5, /// 0xb70
			 0xf60a1a92, /// 0xb74
			 0xdba9dbe7, /// 0xb78
			 0x6a6d1dbe, /// 0xb7c
			 0xec780594, /// 0xb80
			 0x9a7e0b97, /// 0xb84
			 0xb4748cc8, /// 0xb88
			 0xdcd287db, /// 0xb8c
			 0x447b46d0, /// 0xb90
			 0x16b13a21, /// 0xb94
			 0xb6552ab7, /// 0xb98
			 0xc43dc924, /// 0xb9c
			 0x968488da, /// 0xba0
			 0x92c7141e, /// 0xba4
			 0xc339bbf1, /// 0xba8
			 0x4dd69fcd, /// 0xbac
			 0x5dc971bb, /// 0xbb0
			 0xc4b2db4e, /// 0xbb4
			 0x7d350b64, /// 0xbb8
			 0x7aff2f75, /// 0xbbc
			 0x873c9ea0, /// 0xbc0
			 0xad5abca9, /// 0xbc4
			 0xdbb72cff, /// 0xbc8
			 0x80c47cb9, /// 0xbcc
			 0x2f9f5213, /// 0xbd0
			 0x3d032c00, /// 0xbd4
			 0x5106ef9d, /// 0xbd8
			 0x7779e825, /// 0xbdc
			 0xf97f90ad, /// 0xbe0
			 0x0cd28e82, /// 0xbe4
			 0x4424b493, /// 0xbe8
			 0x448a05c7, /// 0xbec
			 0xa3c67316, /// 0xbf0
			 0x1c1d6215, /// 0xbf4
			 0xea1d1688, /// 0xbf8
			 0x2e2e86db, /// 0xbfc
			 0xc4b69672, /// 0xc00
			 0x726514fd, /// 0xc04
			 0xad715d04, /// 0xc08
			 0x4a2c1f5d, /// 0xc0c
			 0x8cac0f0b, /// 0xc10
			 0xe7018d7d, /// 0xc14
			 0x0faaf1dc, /// 0xc18
			 0x5addec1a, /// 0xc1c
			 0x0d31dfee, /// 0xc20
			 0x392aa86e, /// 0xc24
			 0xec05c3db, /// 0xc28
			 0x6efe6b8c, /// 0xc2c
			 0x7cb8fdbb, /// 0xc30
			 0x8df59647, /// 0xc34
			 0xab8a5c16, /// 0xc38
			 0xca30b862, /// 0xc3c
			 0xd5770558, /// 0xc40
			 0xd8e247d6, /// 0xc44
			 0x98156681, /// 0xc48
			 0xf0fd6d53, /// 0xc4c
			 0xe4b8bec1, /// 0xc50
			 0xb0fb360e, /// 0xc54
			 0x70ca4a02, /// 0xc58
			 0x0917b9b8, /// 0xc5c
			 0xceb38ea9, /// 0xc60
			 0x667ef4d5, /// 0xc64
			 0x9301a595, /// 0xc68
			 0x8e98cd86, /// 0xc6c
			 0x821ae0de, /// 0xc70
			 0x0f2ba935, /// 0xc74
			 0x8e6430b9, /// 0xc78
			 0x634602e9, /// 0xc7c
			 0xaa5c2b72, /// 0xc80
			 0x62ea97e1, /// 0xc84
			 0xe6c29444, /// 0xc88
			 0xe05f05eb, /// 0xc8c
			 0x23e11bf0, /// 0xc90
			 0x6be2c426, /// 0xc94
			 0x0ec7785b, /// 0xc98
			 0xf1631cd7, /// 0xc9c
			 0x1a6be3f0, /// 0xca0
			 0xbac7de8f, /// 0xca4
			 0xf8444fca, /// 0xca8
			 0x98715af9, /// 0xcac
			 0xc01b831e, /// 0xcb0
			 0x188b1c03, /// 0xcb4
			 0x056d4a9a, /// 0xcb8
			 0x620863f3, /// 0xcbc
			 0x0c885d52, /// 0xcc0
			 0x72f1e0ed, /// 0xcc4
			 0xf82564e6, /// 0xcc8
			 0xb7f4df10, /// 0xccc
			 0x08006f2f, /// 0xcd0
			 0xc8e1fdf3, /// 0xcd4
			 0x89446cb7, /// 0xcd8
			 0x93ff17b1, /// 0xcdc
			 0xca7bc514, /// 0xce0
			 0x9a8c0923, /// 0xce4
			 0xc9c6e635, /// 0xce8
			 0x0a564718, /// 0xcec
			 0xce0d5b8a, /// 0xcf0
			 0x4df1b812, /// 0xcf4
			 0xfffd45d0, /// 0xcf8
			 0x11d10552, /// 0xcfc
			 0x3bc175d8, /// 0xd00
			 0xc57abc39, /// 0xd04
			 0xf4e0b44d, /// 0xd08
			 0x95ff4835, /// 0xd0c
			 0x99b86785, /// 0xd10
			 0x1710e222, /// 0xd14
			 0x0eed69a2, /// 0xd18
			 0x86e304c2, /// 0xd1c
			 0x76ed86a2, /// 0xd20
			 0x2482b74c, /// 0xd24
			 0x516f2cbd, /// 0xd28
			 0x3a2db690, /// 0xd2c
			 0x9ee9aee5, /// 0xd30
			 0xdfd7e824, /// 0xd34
			 0x06ba745f, /// 0xd38
			 0x8fd13514, /// 0xd3c
			 0x69898de7, /// 0xd40
			 0x847a6f02, /// 0xd44
			 0x68730844, /// 0xd48
			 0xa3141b92, /// 0xd4c
			 0x45da27cc, /// 0xd50
			 0x8cbc68e2, /// 0xd54
			 0x0fdd41ed, /// 0xd58
			 0xeedafa71, /// 0xd5c
			 0xc29a88b1, /// 0xd60
			 0x1680b024, /// 0xd64
			 0x3327fa07, /// 0xd68
			 0x9fecea34, /// 0xd6c
			 0xfd5706e3, /// 0xd70
			 0x7c79e620, /// 0xd74
			 0x5d9bdf14, /// 0xd78
			 0x97f28d5a, /// 0xd7c
			 0x3298d2a6, /// 0xd80
			 0xbcb5eb1d, /// 0xd84
			 0xa18f280c, /// 0xd88
			 0x9a4200f4, /// 0xd8c
			 0xca52a7f3, /// 0xd90
			 0x8252f532, /// 0xd94
			 0xb4c22f0a, /// 0xd98
			 0xb78b8608, /// 0xd9c
			 0x82421141, /// 0xda0
			 0x6de45534, /// 0xda4
			 0x72621e49, /// 0xda8
			 0x69f88091, /// 0xdac
			 0xa887dc14, /// 0xdb0
			 0x3aa09789, /// 0xdb4
			 0x51930542, /// 0xdb8
			 0xfdbbc5f6, /// 0xdbc
			 0xea4b25f2, /// 0xdc0
			 0x9e53a117, /// 0xdc4
			 0x81e91379, /// 0xdc8
			 0x224c732c, /// 0xdcc
			 0x04816d39, /// 0xdd0
			 0xce1e919b, /// 0xdd4
			 0x25d93b7b, /// 0xdd8
			 0x745238c9, /// 0xddc
			 0x294240d8, /// 0xde0
			 0xba69a35c, /// 0xde4
			 0xa400b2e9, /// 0xde8
			 0xe8c4f199, /// 0xdec
			 0x4223119d, /// 0xdf0
			 0x1d4f6cc2, /// 0xdf4
			 0x5c862f23, /// 0xdf8
			 0x3b7a7fef, /// 0xdfc
			 0x336d1c9e, /// 0xe00
			 0x02c0f8de, /// 0xe04
			 0xebf3a9ee, /// 0xe08
			 0x8d8ca176, /// 0xe0c
			 0x9226a51d, /// 0xe10
			 0x055ced86, /// 0xe14
			 0xf25010a3, /// 0xe18
			 0xaef5b45d, /// 0xe1c
			 0xd0709e84, /// 0xe20
			 0x7fcf4c35, /// 0xe24
			 0xb3803e6b, /// 0xe28
			 0x1657028f, /// 0xe2c
			 0x4272151f, /// 0xe30
			 0x89bc19b0, /// 0xe34
			 0xf656cbce, /// 0xe38
			 0x3e06ed8a, /// 0xe3c
			 0x628f4dfb, /// 0xe40
			 0x8fa358d4, /// 0xe44
			 0xcf7c4e14, /// 0xe48
			 0x96f90724, /// 0xe4c
			 0x788ad753, /// 0xe50
			 0xc82c2097, /// 0xe54
			 0xe48cf51e, /// 0xe58
			 0xa8dd59bf, /// 0xe5c
			 0x04955e66, /// 0xe60
			 0x15e08091, /// 0xe64
			 0x36031778, /// 0xe68
			 0x8892f3fe, /// 0xe6c
			 0x1476d645, /// 0xe70
			 0x27cb46e5, /// 0xe74
			 0x1f191182, /// 0xe78
			 0xa1a8e584, /// 0xe7c
			 0xacec1b96, /// 0xe80
			 0xffcfaadd, /// 0xe84
			 0x23112e02, /// 0xe88
			 0xba93b8ff, /// 0xe8c
			 0xbb0fd1b3, /// 0xe90
			 0x29115ac9, /// 0xe94
			 0xd4394231, /// 0xe98
			 0xa803c81f, /// 0xe9c
			 0x832e5271, /// 0xea0
			 0x01bec5d2, /// 0xea4
			 0x1bd0503e, /// 0xea8
			 0x41599c67, /// 0xeac
			 0x28da2983, /// 0xeb0
			 0xc12f2e9d, /// 0xeb4
			 0x372e28f2, /// 0xeb8
			 0x8874a9f1, /// 0xebc
			 0x75396cc2, /// 0xec0
			 0x7ecce562, /// 0xec4
			 0x76447d09, /// 0xec8
			 0x25af01e0, /// 0xecc
			 0xc53b029c, /// 0xed0
			 0x48f49a9b, /// 0xed4
			 0x1f7e8eea, /// 0xed8
			 0x5569ab7e, /// 0xedc
			 0xeea56ecf, /// 0xee0
			 0x4655057c, /// 0xee4
			 0x339e1029, /// 0xee8
			 0x9b841ca5, /// 0xeec
			 0x278554d3, /// 0xef0
			 0xccd2be92, /// 0xef4
			 0x0f2e6191, /// 0xef8
			 0x46f46b4e, /// 0xefc
			 0x35a0dee9, /// 0xf00
			 0xec46c6ff, /// 0xf04
			 0xd3cf5c9a, /// 0xf08
			 0xf5c50f1f, /// 0xf0c
			 0x3a7f5722, /// 0xf10
			 0x22d7da2c, /// 0xf14
			 0xa1320846, /// 0xf18
			 0x837a3262, /// 0xf1c
			 0xccdb3f64, /// 0xf20
			 0xb752eac5, /// 0xf24
			 0xc6a5c1ff, /// 0xf28
			 0xc394c2ec, /// 0xf2c
			 0xdf8aab90, /// 0xf30
			 0x932bbe4a, /// 0xf34
			 0xbd1f337f, /// 0xf38
			 0xa7194ce4, /// 0xf3c
			 0x13be00a1, /// 0xf40
			 0xbb086f9e, /// 0xf44
			 0xce1fce5a, /// 0xf48
			 0x32a729fc, /// 0xf4c
			 0xf9106f17, /// 0xf50
			 0xec5dc9fa, /// 0xf54
			 0x4aebcbf3, /// 0xf58
			 0x49a9b7ed, /// 0xf5c
			 0xb615d5ee, /// 0xf60
			 0x368f333c, /// 0xf64
			 0x9c17a85b, /// 0xf68
			 0x5f9aba16, /// 0xf6c
			 0xd55ff175, /// 0xf70
			 0x494d5703, /// 0xf74
			 0xa3dfebfb, /// 0xf78
			 0x7f7dafa8, /// 0xf7c
			 0xa1ec959d, /// 0xf80
			 0xdb64f55e, /// 0xf84
			 0x0ce408f2, /// 0xf88
			 0x2f34c12d, /// 0xf8c
			 0x34a118ab, /// 0xf90
			 0x9742c32a, /// 0xf94
			 0xccd67d1a, /// 0xf98
			 0x4dd1ba69, /// 0xf9c
			 0x6dff1341, /// 0xfa0
			 0x3247c510, /// 0xfa4
			 0x1f7718fc, /// 0xfa8
			 0x36b1e134, /// 0xfac
			 0xb9229c94, /// 0xfb0
			 0x1b064560, /// 0xfb4
			 0x44d2b834, /// 0xfb8
			 0x1360c0d0, /// 0xfbc
			 0x83cc47ac, /// 0xfc0
			 0x2b46c706, /// 0xfc4
			 0xeec5c3b3, /// 0xfc8
			 0xf0d17b0b, /// 0xfcc
			 0x65316b14, /// 0xfd0
			 0xeda27db3, /// 0xfd4
			 0x656633db, /// 0xfd8
			 0x3a298b7f, /// 0xfdc
			 0x3b780905, /// 0xfe0
			 0xfd525014, /// 0xfe4
			 0x8fe0e13f, /// 0xfe8
			 0x47a63166, /// 0xfec
			 0x52026a83, /// 0xff0
			 0x117b0fad, /// 0xff4
			 0x19434013, /// 0xff8
			 0x142cd2bd /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00004
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00008
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0000c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00010
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0001c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00020
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00024
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0002c
			 0x4b000000,                                                  // 8388608.0                                   /// 00030
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00034
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00038
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0003c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00040
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00044
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00048
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0004c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00050
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00054
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00058
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0005c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00060
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00064
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00068
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0006c
			 0xbf028f5c,                                                  // -0.51                                       /// 00070
			 0x00011111,                                                  // 9.7958E-41                                  /// 00074
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00078
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0007c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00084
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0008c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00090
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0009c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000a0
			 0x55555555,                                                  // 4 random values                             /// 000a4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 000a8
			 0xff800000,                                                  // -inf                                        /// 000ac
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 000b0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000c0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 000c4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000cc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x55555555,                                                  // 4 random values                             /// 000d4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000d8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000e4
			 0x7fc00001,                                                  // signaling NaN                               /// 000e8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000ec
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000f0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000f4
			 0x33333333,                                                  // 4 random values                             /// 000f8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00100
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00104
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0010c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00110
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00118
			 0x7fc00001,                                                  // signaling NaN                               /// 0011c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00120
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00124
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00128
			 0x00000000,                                                  // zero                                        /// 0012c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00130
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00134
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00138
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0013c
			 0x7f800000,                                                  // inf                                         /// 00140
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0014c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00150
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00158
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0015c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00160
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00164
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00168
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0016c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00174
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00178
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00180
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00184
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00188
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0018c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00190
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00194
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00198
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0019c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001a0
			 0x3f028f5c,                                                  // 0.51                                        /// 001a4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 001a8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ac
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001b0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0xff800000,                                                  // -inf                                        /// 001c0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001c4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001c8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001cc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001d0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 001d4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 001d8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001dc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001e4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001e8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 001ec
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001f4
			 0x0c700000,                                                  // Leading 1s:                                 /// 001f8
			 0x00000000,                                                  // zero                                        /// 001fc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00200
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00204
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00208
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0020c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00210
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00214
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00218
			 0xbf028f5c,                                                  // -0.51                                       /// 0021c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00220
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00224
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00228
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0022c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00230
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00234
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00238
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0023c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00240
			 0xcb000000,                                                  // -8388608.0                                  /// 00244
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00248
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0024c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00250
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00254
			 0x0e000001,                                                  // Trailing 1s:                                /// 00258
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00260
			 0x4b000000,                                                  // 8388608.0                                   /// 00264
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00268
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0026c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00270
			 0x7fc00001,                                                  // signaling NaN                               /// 00274
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00280
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00284
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00288
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0028c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00290
			 0x0c600000,                                                  // Leading 1s:                                 /// 00294
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00298
			 0x7fc00001,                                                  // signaling NaN                               /// 0029c
			 0x80000000,                                                  // -zero                                       /// 002a0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002a4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002a8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002ac
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002b4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 002b8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002bc
			 0x4b000000,                                                  // 8388608.0                                   /// 002c0
			 0xff800000,                                                  // -inf                                        /// 002c4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002c8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002cc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002d4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 002dc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002e0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002e4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002e8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ec
			 0x0e000001,                                                  // Trailing 1s:                                /// 002f0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 002f4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002f8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00304
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00308
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0030c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00310
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0x80011111,                                                  // -9.7958E-41                                 /// 00318
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0031c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00320
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00324
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00328
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00330
			 0x55555555,                                                  // 4 random values                             /// 00334
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00338
			 0x0c780000,                                                  // Leading 1s:                                 /// 0033c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00340
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00344
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00348
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0034c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00350
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00354
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00358
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0035c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00360
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00364
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00368
			 0x0e000001,                                                  // Trailing 1s:                                /// 0036c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00370
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00374
			 0x4b000000,                                                  // 8388608.0                                   /// 00378
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0037c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00380
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00384
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00388
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0038c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00390
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00398
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003a0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 003a4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003a8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 003ac
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003b0
			 0x55555555,                                                  // 4 random values                             /// 003b4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003bc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 003c0
			 0x3f028f5c,                                                  // 0.51                                        /// 003c4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003c8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003cc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003d0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003d8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 003dc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003e0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003e4
			 0x0e000007,                                                  // Trailing 1s:                                /// 003e8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003ec
			 0xffc00001,                                                  // -signaling NaN                              /// 003f0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003f4
			 0x0e000007,                                                  // Trailing 1s:                                /// 003f8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00400
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00404
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00408
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0040c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00410
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00414
			 0xbf028f5c,                                                  // -0.51                                       /// 00418
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0041c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00420
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00424
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00428
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0042c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00430
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00434
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00438
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00440
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00444
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00448
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0044c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00450
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00454
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00458
			 0x0e000003,                                                  // Trailing 1s:                                /// 0045c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00460
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00468
			 0x0e000007,                                                  // Trailing 1s:                                /// 0046c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00470
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00474
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00478
			 0x0c780000,                                                  // Leading 1s:                                 /// 0047c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00480
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00484
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00488
			 0xbf028f5c,                                                  // -0.51                                       /// 0048c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00490
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00494
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00498
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0049c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 004a4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 004a8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004ac
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 004b4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b8
			 0xffc00001,                                                  // -signaling NaN                              /// 004bc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 004c0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004c4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004c8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 004cc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004d4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004dc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 004e0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 004e4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004e8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004ec
			 0xCCCCCCCC,                                                  // 4 random values                             /// 004f0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004f4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004f8
			 0x0c780000,                                                  // Leading 1s:                                 /// 004fc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00500
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00504
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00508
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0050c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00510
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00514
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00518
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0051c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00520
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00524
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00528
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0052c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00530
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00534
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00538
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0053c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00544
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00548
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0054c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00550
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00558
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0055c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00564
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00568
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0056c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00570
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00574
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0057c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00580
			 0x3f028f5c,                                                  // 0.51                                        /// 00584
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00590
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00598
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0059c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 005a0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005a4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 005a8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005b4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005b8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 005bc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005c4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005c8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005cc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005d8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005dc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005e0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005e4
			 0x0c780000,                                                  // Leading 1s:                                 /// 005e8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005f0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005f8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00600
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00604
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00608
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0060c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00610
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00614
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0061c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00620
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00624
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0062c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00630
			 0x0c400000,                                                  // Leading 1s:                                 /// 00634
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0063c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00640
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00644
			 0xff800000,                                                  // -inf                                        /// 00648
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00650
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00654
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00658
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0065c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00660
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00664
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0066c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00670
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00674
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00678
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00680
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00684
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00688
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0068c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00690
			 0x0c400000,                                                  // Leading 1s:                                 /// 00694
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00698
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0069c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006a0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006a4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 006a8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006ac
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006b0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006b4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006b8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006bc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 006c0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0x0e000007,                                                  // Trailing 1s:                                /// 006cc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006d0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006d8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006dc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006e4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e8
			 0x00011111,                                                  // 9.7958E-41                                  /// 006ec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006f8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006fc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00700
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00704
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00708
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0070c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00714
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00718
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0071c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00720
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00724
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00728
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0072c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0073c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00740
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00744
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00748
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0074c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00750
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00754
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00758
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0075c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00760
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00764
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00768
			 0x55555555,                                                  // 4 random values                             /// 0076c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00770
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00774
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00778
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0077c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00780
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00784
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00788
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0078c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00790
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00794
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 007a0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ac
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007b8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007bc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007c0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007c8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007cc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 007d0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 007d4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007d8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007dc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007e0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007ec
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007f4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 007fc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00800
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00804
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00808
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0080c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00814
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00818
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0081c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00820
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00824
			 0xcb000000,                                                  // -8388608.0                                  /// 00828
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x80000000,                                                  // -zero                                       /// 00834
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00838
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0083c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00840
			 0x0c600000,                                                  // Leading 1s:                                 /// 00844
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00848
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0084c
			 0x4b000000,                                                  // 8388608.0                                   /// 00850
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00854
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00858
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0085c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00864
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00868
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0086c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00870
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00874
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00878
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0087c
			 0x4b000000,                                                  // 8388608.0                                   /// 00880
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00884
			 0x0c400000,                                                  // Leading 1s:                                 /// 00888
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0088c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00890
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00894
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00898
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0089c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 008a0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008a4
			 0x7fc00001,                                                  // signaling NaN                               /// 008a8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 008ac
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008b0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008b4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008b8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 008bc
			 0x0c780000,                                                  // Leading 1s:                                 /// 008c0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008c4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008c8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008cc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 008d0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008d4
			 0xffc00001,                                                  // -signaling NaN                              /// 008d8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008dc
			 0x0c780000,                                                  // Leading 1s:                                 /// 008e0
			 0x0e000007,                                                  // Trailing 1s:                                /// 008e4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 008ec
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008f0
			 0x0c700000,                                                  // Leading 1s:                                 /// 008f4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008f8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 008fc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00900
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00908
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0090c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00910
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00914
			 0x33333333,                                                  // 4 random values                             /// 00918
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0091c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00920
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00924
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00928
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0092c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00934
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00938
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0093c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00940
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00944
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00948
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0094c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00950
			 0x33333333,                                                  // 4 random values                             /// 00954
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00958
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0095c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00960
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00964
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00968
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0096c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00970
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00974
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00978
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0097c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00980
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00984
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00988
			 0x0c400000,                                                  // Leading 1s:                                 /// 0098c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00990
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00994
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00998
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0099c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0xbf028f5c,                                                  // -0.51                                       /// 009a8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0x4b000000,                                                  // 8388608.0                                   /// 009b0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009b4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009bc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 009c0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d8
			 0x0e000003,                                                  // Trailing 1s:                                /// 009dc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0x00011111,                                                  // 9.7958E-41                                  /// 009e8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009ec
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f0
			 0xffc00001,                                                  // -signaling NaN                              /// 009f4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 009f8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009fc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a00
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a04
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a08
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a0c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a10
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a14
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a18
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a1c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a20
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a24
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a28
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a30
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a34
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a38
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a3c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a40
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a44
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a48
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a4c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a50
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a54
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a58
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a5c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a60
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a64
			 0x4b000000,                                                  // 8388608.0                                   /// 00a68
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a6c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a70
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a78
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a7c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a80
			 0xffc00001,                                                  // -signaling NaN                              /// 00a84
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a88
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a8c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a90
			 0xff800000,                                                  // -inf                                        /// 00a94
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a98
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a9c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00aa0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00aa4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00aa8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00aac
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ab0
			 0xcb000000,                                                  // -8388608.0                                  /// 00ab4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ab8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00abc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ac0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ac4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00acc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ad0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ad4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ad8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00adc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ae4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ae8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0x80011111,                                                  // -9.7958E-41                                 /// 00af0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00af4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00af8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00afc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b00
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b04
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b08
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b0c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b10
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b14
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b18
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b1c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b20
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b28
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b2c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b30
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b38
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b3c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b40
			 0xff800000,                                                  // -inf                                        /// 00b44
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b4c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b50
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b54
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b58
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b5c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b60
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b64
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b6c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b70
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b74
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b78
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b7c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b84
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b88
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b8c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b90
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b94
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b98
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b9c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ba8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00bac
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00bb4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00bb8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bc0
			 0xffc00001,                                                  // -signaling NaN                              /// 00bc4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00bc8
			 0xcb000000,                                                  // -8388608.0                                  /// 00bcc
			 0x33333333,                                                  // 4 random values                             /// 00bd0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bd4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00bdc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00be4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00be8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bec
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf0
			 0x00000000,                                                  // zero                                        /// 00bf4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00bf8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00bfc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c00
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c04
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c0c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c10
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c18
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c1c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c20
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c24
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c28
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c2c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c30
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c34
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c38
			 0x80000000,                                                  // -zero                                       /// 00c3c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c40
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c44
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c48
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c4c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c50
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c54
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c58
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c5c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c64
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c68
			 0x4b000000,                                                  // 8388608.0                                   /// 00c6c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c70
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c74
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c78
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c80
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c84
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c88
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c8c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c94
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c98
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c9c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ca0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ca4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ca8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cac
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00cb8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cbc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00cc0
			 0x55555555,                                                  // 4 random values                             /// 00cc4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00cc8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ccc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00cd0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00cd4
			 0x80000000,                                                  // -zero                                       /// 00cd8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00cdc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ce0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ce4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ce8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00cec
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00cf0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00cf8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cfc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d00
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d04
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d08
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d0c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d10
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d14
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d18
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d1c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d20
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d24
			 0xcb000000,                                                  // -8388608.0                                  /// 00d28
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d2c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d30
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d34
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d38
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d3c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d40
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d44
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d48
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d4c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d50
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d58
			 0x3f028f5c,                                                  // 0.51                                        /// 00d5c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d60
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d64
			 0xffc00001,                                                  // -signaling NaN                              /// 00d68
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d6c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d70
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d74
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d78
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d7c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d80
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d84
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d88
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d8c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d94
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d98
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d9c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00da0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00da4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00da8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00db4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00db8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dbc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00dc0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00dc4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00dc8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00dcc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00dd0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00dd8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ddc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00de0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00de4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00de8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00dec
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00df0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00df4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00dfc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e00
			 0xcb000000,                                                  // -8388608.0                                  /// 00e04
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e08
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e0c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e10
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e14
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e18
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e1c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e24
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e28
			 0xffc00001,                                                  // -signaling NaN                              /// 00e2c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e30
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e34
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e38
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e3c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e40
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e44
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e48
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e4c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e50
			 0x55555555,                                                  // 4 random values                             /// 00e54
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e58
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e5c
			 0x7f800000,                                                  // inf                                         /// 00e60
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e64
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e68
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e70
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e74
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e7c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e80
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e84
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e88
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e8c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e90
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e94
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e98
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e9c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ea4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00eac
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00eb0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00eb4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00eb8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ebc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ecc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ed0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ed4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00edc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ee0
			 0xcb000000,                                                  // -8388608.0                                  /// 00ee4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ee8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00eec
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ef0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ef4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ef8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00efc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f00
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f04
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f08
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f0c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f10
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f14
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f18
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f1c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f20
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f24
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f28
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f2c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f30
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0xcb000000,                                                  // -8388608.0                                  /// 00f3c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f40
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f48
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f4c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f54
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f58
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f60
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f64
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f68
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f6c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f70
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f74
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f78
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f7c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f80
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f88
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f8c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f90
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f98
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f9c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00fa0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fa8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00fac
			 0x80000000,                                                  // -zero                                       /// 00fb0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00fb4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00fbc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00fc8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fcc
			 0x4b000000,                                                  // 8388608.0                                   /// 00fd0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00fd4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00fd8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00fdc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00fe0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00fe4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fe8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fec
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ff0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ff4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ff8
			 0x0e1fffff                                                  // Trailing 1s:                                /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0xd7f9b935, /// 0x0
			 0xec0e5f6f, /// 0x4
			 0xb7f7d499, /// 0x8
			 0x98e5202a, /// 0xc
			 0xd9e53842, /// 0x10
			 0x74873056, /// 0x14
			 0x3271b49d, /// 0x18
			 0xdf919e69, /// 0x1c
			 0xab5e856c, /// 0x20
			 0x4348afd7, /// 0x24
			 0x6c7f898d, /// 0x28
			 0x9f2acbf9, /// 0x2c
			 0xb4d63b03, /// 0x30
			 0x030d4f7f, /// 0x34
			 0x7dbba323, /// 0x38
			 0xf6137ae1, /// 0x3c
			 0xf468c191, /// 0x40
			 0xf939b556, /// 0x44
			 0x198353b9, /// 0x48
			 0xd6b4418d, /// 0x4c
			 0xfdc83602, /// 0x50
			 0x785918fd, /// 0x54
			 0x69101401, /// 0x58
			 0x403d4f4c, /// 0x5c
			 0x0694e038, /// 0x60
			 0x13b32bcc, /// 0x64
			 0x9d7cd817, /// 0x68
			 0xfa984349, /// 0x6c
			 0xefcf1ac0, /// 0x70
			 0x87ef283e, /// 0x74
			 0xc2eec32c, /// 0x78
			 0xd8998fb6, /// 0x7c
			 0xd42fbde7, /// 0x80
			 0x16f8ac5f, /// 0x84
			 0xe38dabea, /// 0x88
			 0xb37a27e0, /// 0x8c
			 0x266e85c0, /// 0x90
			 0xf715c334, /// 0x94
			 0x65ac9429, /// 0x98
			 0x0abc47a1, /// 0x9c
			 0x68f7bf27, /// 0xa0
			 0xfbce82cd, /// 0xa4
			 0xd2a736fa, /// 0xa8
			 0xa91491fd, /// 0xac
			 0x1c8c6670, /// 0xb0
			 0xfc21126c, /// 0xb4
			 0xd131f21b, /// 0xb8
			 0xaf10a37b, /// 0xbc
			 0x1f2a195e, /// 0xc0
			 0x7de69e10, /// 0xc4
			 0xf2610746, /// 0xc8
			 0xaaf46508, /// 0xcc
			 0x6e465b3d, /// 0xd0
			 0x25875b27, /// 0xd4
			 0x2aaa20c9, /// 0xd8
			 0x1e0b50ca, /// 0xdc
			 0xebb2c4b2, /// 0xe0
			 0xc1e63217, /// 0xe4
			 0x7a0e0fa9, /// 0xe8
			 0x98c358a8, /// 0xec
			 0xd71daed0, /// 0xf0
			 0x67b5e6b1, /// 0xf4
			 0x07bd02cb, /// 0xf8
			 0x9f3c4fb3, /// 0xfc
			 0x97e06d76, /// 0x100
			 0xbc16decc, /// 0x104
			 0xc28c85bc, /// 0x108
			 0x623cdcdd, /// 0x10c
			 0x689de3b4, /// 0x110
			 0xe862a0fc, /// 0x114
			 0x5bff9238, /// 0x118
			 0xb98c89c0, /// 0x11c
			 0xacaba8e9, /// 0x120
			 0xc0d90ccf, /// 0x124
			 0x0e011151, /// 0x128
			 0x6f7a818f, /// 0x12c
			 0x45612d33, /// 0x130
			 0x092b3308, /// 0x134
			 0x8f45c57d, /// 0x138
			 0x80fb407d, /// 0x13c
			 0x7e4774fc, /// 0x140
			 0x7a06c666, /// 0x144
			 0x2e7bc221, /// 0x148
			 0x5875d3a3, /// 0x14c
			 0xa3dc9a5b, /// 0x150
			 0x3c31e457, /// 0x154
			 0x06208fc2, /// 0x158
			 0x010e1607, /// 0x15c
			 0xceb09a5c, /// 0x160
			 0x42935611, /// 0x164
			 0x8ab784a8, /// 0x168
			 0x1f62be8e, /// 0x16c
			 0x4c34a9a5, /// 0x170
			 0x5d909391, /// 0x174
			 0x6686f6e7, /// 0x178
			 0x1161c52c, /// 0x17c
			 0x5b9f6a09, /// 0x180
			 0x6d0e69b8, /// 0x184
			 0x3d9317e5, /// 0x188
			 0x51d56b91, /// 0x18c
			 0xbb20f6f0, /// 0x190
			 0x2291b68e, /// 0x194
			 0x2d9d8ae1, /// 0x198
			 0xe196a740, /// 0x19c
			 0xbe006038, /// 0x1a0
			 0x26cb29c2, /// 0x1a4
			 0x8b095972, /// 0x1a8
			 0xec19a114, /// 0x1ac
			 0xadd1262a, /// 0x1b0
			 0xc675b60c, /// 0x1b4
			 0xd749f91f, /// 0x1b8
			 0x860a790e, /// 0x1bc
			 0x9317fa75, /// 0x1c0
			 0x4a27af28, /// 0x1c4
			 0xbd6df15d, /// 0x1c8
			 0xa565f363, /// 0x1cc
			 0x95d9876a, /// 0x1d0
			 0xe9bebd4d, /// 0x1d4
			 0xb26d2560, /// 0x1d8
			 0x8122a933, /// 0x1dc
			 0x6b58fd9c, /// 0x1e0
			 0xa4d68530, /// 0x1e4
			 0xc159496e, /// 0x1e8
			 0x7aa50aae, /// 0x1ec
			 0xd6482770, /// 0x1f0
			 0x00847257, /// 0x1f4
			 0x006be0fe, /// 0x1f8
			 0x8c97aa5c, /// 0x1fc
			 0xc582252d, /// 0x200
			 0x18b4e312, /// 0x204
			 0xa9fb8252, /// 0x208
			 0x0361c234, /// 0x20c
			 0x2c38beae, /// 0x210
			 0x14ba438b, /// 0x214
			 0x84bbc561, /// 0x218
			 0x0ecd67aa, /// 0x21c
			 0x93c09010, /// 0x220
			 0xff77af93, /// 0x224
			 0x056b8a7f, /// 0x228
			 0x9e4c8cac, /// 0x22c
			 0x5fcc307d, /// 0x230
			 0x6f2b740e, /// 0x234
			 0x2c7f02c0, /// 0x238
			 0xc8d1fffa, /// 0x23c
			 0xf58dc6cd, /// 0x240
			 0x0f7ef3a9, /// 0x244
			 0x81bdb97a, /// 0x248
			 0xcf2dc6a7, /// 0x24c
			 0xae9af8ce, /// 0x250
			 0x1b601e16, /// 0x254
			 0xccc689d8, /// 0x258
			 0x5f8a499b, /// 0x25c
			 0xe4247764, /// 0x260
			 0x66603616, /// 0x264
			 0x8b81c14c, /// 0x268
			 0x4e364fe7, /// 0x26c
			 0xd6e37617, /// 0x270
			 0xa9000b7b, /// 0x274
			 0x467d11cb, /// 0x278
			 0x2e92a9b0, /// 0x27c
			 0xaeb52558, /// 0x280
			 0x577a5fae, /// 0x284
			 0x49c25740, /// 0x288
			 0x5f33cedc, /// 0x28c
			 0xfc223f9f, /// 0x290
			 0x6722d01c, /// 0x294
			 0xb940c227, /// 0x298
			 0x2223f232, /// 0x29c
			 0x76c96c06, /// 0x2a0
			 0x26371569, /// 0x2a4
			 0xeae31387, /// 0x2a8
			 0x66fe5efb, /// 0x2ac
			 0x07dae8d2, /// 0x2b0
			 0x1d89f75f, /// 0x2b4
			 0x0c4d429a, /// 0x2b8
			 0x78c32493, /// 0x2bc
			 0xc0e5a41a, /// 0x2c0
			 0x31c6960e, /// 0x2c4
			 0xa6d2e683, /// 0x2c8
			 0x3967df26, /// 0x2cc
			 0xf6da5c21, /// 0x2d0
			 0x07aed828, /// 0x2d4
			 0x079d1fb7, /// 0x2d8
			 0xfef2b439, /// 0x2dc
			 0x2f683c8c, /// 0x2e0
			 0xdd5eecaf, /// 0x2e4
			 0x2b216d5c, /// 0x2e8
			 0xa66295a6, /// 0x2ec
			 0x195feca9, /// 0x2f0
			 0xb7f641e1, /// 0x2f4
			 0x8335feb2, /// 0x2f8
			 0xc369e8a6, /// 0x2fc
			 0x3709a575, /// 0x300
			 0xe686aa65, /// 0x304
			 0x6f7dbdeb, /// 0x308
			 0x1de6b2b1, /// 0x30c
			 0xe12fed19, /// 0x310
			 0xdcb6d1a5, /// 0x314
			 0x0a66e5fb, /// 0x318
			 0xbbba5380, /// 0x31c
			 0xd3a7bbb1, /// 0x320
			 0xbf2f229d, /// 0x324
			 0x412f42cd, /// 0x328
			 0xd71b1621, /// 0x32c
			 0x13d139b4, /// 0x330
			 0xfb544c37, /// 0x334
			 0xe0b57fdb, /// 0x338
			 0xbda0514c, /// 0x33c
			 0x5506d5ba, /// 0x340
			 0xc823457f, /// 0x344
			 0x1bf9850f, /// 0x348
			 0x102ba859, /// 0x34c
			 0x2b6fd423, /// 0x350
			 0xbda9a69d, /// 0x354
			 0x0e378292, /// 0x358
			 0x48b2ad31, /// 0x35c
			 0x1e080cf0, /// 0x360
			 0xa56925b7, /// 0x364
			 0xb09c560b, /// 0x368
			 0xd6d3af9a, /// 0x36c
			 0xced6bf00, /// 0x370
			 0x509fa9da, /// 0x374
			 0x85cd1f4a, /// 0x378
			 0xaa8b0645, /// 0x37c
			 0x595c7753, /// 0x380
			 0xe1f72719, /// 0x384
			 0x6e10f5d9, /// 0x388
			 0x68036046, /// 0x38c
			 0x465544c2, /// 0x390
			 0x2f170059, /// 0x394
			 0x930c7fa6, /// 0x398
			 0xd468fbd1, /// 0x39c
			 0x003a9fe9, /// 0x3a0
			 0xb253a41d, /// 0x3a4
			 0xf2067c03, /// 0x3a8
			 0x64da8565, /// 0x3ac
			 0xa219eee0, /// 0x3b0
			 0x69250f9f, /// 0x3b4
			 0xcec605be, /// 0x3b8
			 0x1836fea7, /// 0x3bc
			 0xc160c9f0, /// 0x3c0
			 0x3604bf77, /// 0x3c4
			 0xa92808b9, /// 0x3c8
			 0x2bb29aa1, /// 0x3cc
			 0x0e47301f, /// 0x3d0
			 0x46cb6ae9, /// 0x3d4
			 0x9a71e5ac, /// 0x3d8
			 0xd58860d1, /// 0x3dc
			 0xc88ef952, /// 0x3e0
			 0xa977687a, /// 0x3e4
			 0x476fde2e, /// 0x3e8
			 0x02b22a03, /// 0x3ec
			 0x0f35d77d, /// 0x3f0
			 0x15987cd5, /// 0x3f4
			 0x861744fd, /// 0x3f8
			 0x3bcb5fee, /// 0x3fc
			 0x21a2e523, /// 0x400
			 0xea5faf98, /// 0x404
			 0xed056220, /// 0x408
			 0x875ec569, /// 0x40c
			 0x5af8fef6, /// 0x410
			 0xdeb83dcf, /// 0x414
			 0x88c28070, /// 0x418
			 0x4fc5b482, /// 0x41c
			 0xd69a69bf, /// 0x420
			 0x1f4fe319, /// 0x424
			 0xcca36b73, /// 0x428
			 0x23db635c, /// 0x42c
			 0xc570bc38, /// 0x430
			 0xd0cff736, /// 0x434
			 0xf5df5228, /// 0x438
			 0x5b7b8ced, /// 0x43c
			 0x1ec2bf4d, /// 0x440
			 0xec1a2886, /// 0x444
			 0x15607c6e, /// 0x448
			 0x3dc07220, /// 0x44c
			 0xb42b82de, /// 0x450
			 0x93909a06, /// 0x454
			 0x0354ef6a, /// 0x458
			 0x28a2ab5c, /// 0x45c
			 0x8574dba7, /// 0x460
			 0x9e143d05, /// 0x464
			 0xe5af15a0, /// 0x468
			 0x2f0e33be, /// 0x46c
			 0x98e438e3, /// 0x470
			 0x89cd1dd0, /// 0x474
			 0x82f2d258, /// 0x478
			 0x316f0692, /// 0x47c
			 0x6959e971, /// 0x480
			 0x87068311, /// 0x484
			 0xdbc723fd, /// 0x488
			 0xc78a2094, /// 0x48c
			 0xc6854d21, /// 0x490
			 0xfbbe04f5, /// 0x494
			 0x42d1e9ab, /// 0x498
			 0xf20733d0, /// 0x49c
			 0xafe89d06, /// 0x4a0
			 0x0888cbcf, /// 0x4a4
			 0x8f63f585, /// 0x4a8
			 0x733853f7, /// 0x4ac
			 0xc792c0d9, /// 0x4b0
			 0xaed906f3, /// 0x4b4
			 0x6b8cdd1d, /// 0x4b8
			 0x32d99bcc, /// 0x4bc
			 0xaacf2f78, /// 0x4c0
			 0xd8daa7af, /// 0x4c4
			 0xf1370d87, /// 0x4c8
			 0x2eac9ba4, /// 0x4cc
			 0x39043115, /// 0x4d0
			 0xa48a1870, /// 0x4d4
			 0xf13315cd, /// 0x4d8
			 0x3fb96782, /// 0x4dc
			 0xa83040ae, /// 0x4e0
			 0xec907987, /// 0x4e4
			 0xbb6f9621, /// 0x4e8
			 0xfb38592f, /// 0x4ec
			 0x4f15e391, /// 0x4f0
			 0x5cba1887, /// 0x4f4
			 0x59aed99f, /// 0x4f8
			 0xaac1bdaf, /// 0x4fc
			 0xe30e222d, /// 0x500
			 0x3fc37d3e, /// 0x504
			 0x7ad50b42, /// 0x508
			 0x27daf3ee, /// 0x50c
			 0x9272df58, /// 0x510
			 0x2bd4746d, /// 0x514
			 0xfbff25d4, /// 0x518
			 0x64a4bb55, /// 0x51c
			 0xb76505c3, /// 0x520
			 0x2f6c7c14, /// 0x524
			 0xc620a6a4, /// 0x528
			 0xd6254e9f, /// 0x52c
			 0xab72b5a5, /// 0x530
			 0x0a1d6194, /// 0x534
			 0x18bfc591, /// 0x538
			 0xd2ac1541, /// 0x53c
			 0x7646211b, /// 0x540
			 0x29629a77, /// 0x544
			 0xa2b73eee, /// 0x548
			 0x95427955, /// 0x54c
			 0x2109cc90, /// 0x550
			 0x54f09f7f, /// 0x554
			 0x543d0c08, /// 0x558
			 0x8459f837, /// 0x55c
			 0x35c68206, /// 0x560
			 0x5dedd3c8, /// 0x564
			 0xe4a2ebfc, /// 0x568
			 0xc51b1e28, /// 0x56c
			 0x201f92b7, /// 0x570
			 0xdba3dd44, /// 0x574
			 0x8d705e49, /// 0x578
			 0xa557be4d, /// 0x57c
			 0x1711c33c, /// 0x580
			 0xf5eff4bb, /// 0x584
			 0xbe98f1aa, /// 0x588
			 0x614f100c, /// 0x58c
			 0x43add0a8, /// 0x590
			 0xfa1e5d23, /// 0x594
			 0x8e4755f2, /// 0x598
			 0x70f52a2d, /// 0x59c
			 0x4342fd08, /// 0x5a0
			 0xbc2cac93, /// 0x5a4
			 0x88dfcba2, /// 0x5a8
			 0xc210f140, /// 0x5ac
			 0xb6124c5d, /// 0x5b0
			 0x314a2d9c, /// 0x5b4
			 0x3ab5ce31, /// 0x5b8
			 0xf63cfe2f, /// 0x5bc
			 0xa548f92f, /// 0x5c0
			 0x90abbb12, /// 0x5c4
			 0x5744bb7c, /// 0x5c8
			 0xef32f749, /// 0x5cc
			 0x04808c30, /// 0x5d0
			 0x50f5900a, /// 0x5d4
			 0xfa38468a, /// 0x5d8
			 0x66db3dd1, /// 0x5dc
			 0xd77e2871, /// 0x5e0
			 0x459d4dcb, /// 0x5e4
			 0x952e9a7e, /// 0x5e8
			 0xa402c283, /// 0x5ec
			 0x2dfb5803, /// 0x5f0
			 0x4921160e, /// 0x5f4
			 0x00d3a6ba, /// 0x5f8
			 0x6de44da1, /// 0x5fc
			 0xb1c0bf59, /// 0x600
			 0x3fd50f6d, /// 0x604
			 0xa0f06424, /// 0x608
			 0x427278c5, /// 0x60c
			 0x61e87869, /// 0x610
			 0x9b3c4881, /// 0x614
			 0x30d6b642, /// 0x618
			 0xf40507b1, /// 0x61c
			 0x7dc30860, /// 0x620
			 0xb873eedf, /// 0x624
			 0xc940141f, /// 0x628
			 0xc9bb02a3, /// 0x62c
			 0xcedfdac8, /// 0x630
			 0x9841b548, /// 0x634
			 0x43b8f5b2, /// 0x638
			 0x17becb63, /// 0x63c
			 0x9a091b91, /// 0x640
			 0x200499b4, /// 0x644
			 0x982bc616, /// 0x648
			 0x15d60936, /// 0x64c
			 0x870f55f1, /// 0x650
			 0x37275de5, /// 0x654
			 0xc68d205a, /// 0x658
			 0xb1bfb5e5, /// 0x65c
			 0x822cea2f, /// 0x660
			 0xb0ada262, /// 0x664
			 0x04182e77, /// 0x668
			 0xc93b72bf, /// 0x66c
			 0xbcc68aec, /// 0x670
			 0xc534ab7e, /// 0x674
			 0x3a4c5f2f, /// 0x678
			 0x15b8ca4a, /// 0x67c
			 0x6a3c1928, /// 0x680
			 0x5615704d, /// 0x684
			 0xc673dde2, /// 0x688
			 0x4d2a0c43, /// 0x68c
			 0x69709bdf, /// 0x690
			 0x027f584d, /// 0x694
			 0x62090ea2, /// 0x698
			 0x98c45935, /// 0x69c
			 0x3d1d38b9, /// 0x6a0
			 0xf4173fdb, /// 0x6a4
			 0x36226414, /// 0x6a8
			 0x3e692330, /// 0x6ac
			 0xb38899d9, /// 0x6b0
			 0xc34c9948, /// 0x6b4
			 0xc90f18a6, /// 0x6b8
			 0x7884a8d1, /// 0x6bc
			 0xd35fcb88, /// 0x6c0
			 0x0c2e1fad, /// 0x6c4
			 0xda7b6898, /// 0x6c8
			 0xcb69b8c4, /// 0x6cc
			 0x9848f42d, /// 0x6d0
			 0x1015c9f6, /// 0x6d4
			 0x302f6c39, /// 0x6d8
			 0x767ec72d, /// 0x6dc
			 0xbc187366, /// 0x6e0
			 0xb308cb2f, /// 0x6e4
			 0x0e4d3c97, /// 0x6e8
			 0x84041cef, /// 0x6ec
			 0x04d50c92, /// 0x6f0
			 0xc15f535f, /// 0x6f4
			 0x1ab6143d, /// 0x6f8
			 0xe31dae37, /// 0x6fc
			 0xfd072256, /// 0x700
			 0xc1b1c0da, /// 0x704
			 0xa2606f48, /// 0x708
			 0x6309848b, /// 0x70c
			 0xc1832dda, /// 0x710
			 0x215fe89b, /// 0x714
			 0x245379c4, /// 0x718
			 0x4dce481d, /// 0x71c
			 0xae377d03, /// 0x720
			 0x5d43a39f, /// 0x724
			 0x15091863, /// 0x728
			 0x6e7279a5, /// 0x72c
			 0xa4fd4ebe, /// 0x730
			 0x26eaa9ed, /// 0x734
			 0x30bfef14, /// 0x738
			 0x632b0869, /// 0x73c
			 0x9302854c, /// 0x740
			 0x9919e55f, /// 0x744
			 0xeac5d3a6, /// 0x748
			 0x9697e8ed, /// 0x74c
			 0xf64e1fae, /// 0x750
			 0x1d833f30, /// 0x754
			 0x8ccc6d73, /// 0x758
			 0xf889b9e9, /// 0x75c
			 0x1251cfce, /// 0x760
			 0x2604d241, /// 0x764
			 0x7ab6c76b, /// 0x768
			 0xf7bab844, /// 0x76c
			 0xa1a27211, /// 0x770
			 0x120048d2, /// 0x774
			 0x52a38da0, /// 0x778
			 0x0d60ab36, /// 0x77c
			 0xcddcc7a7, /// 0x780
			 0xb5c829ee, /// 0x784
			 0x1c9fdf28, /// 0x788
			 0x6e5c9be4, /// 0x78c
			 0x6f5ad4ee, /// 0x790
			 0x3920d987, /// 0x794
			 0x304c6672, /// 0x798
			 0x4954e75e, /// 0x79c
			 0xf13bcd7b, /// 0x7a0
			 0x6aa76f50, /// 0x7a4
			 0xb8b13753, /// 0x7a8
			 0x5fc53b34, /// 0x7ac
			 0xd4b48cd5, /// 0x7b0
			 0xa08c9ca2, /// 0x7b4
			 0x3314b9f7, /// 0x7b8
			 0xb4d1ac60, /// 0x7bc
			 0x14277c55, /// 0x7c0
			 0xc94e3932, /// 0x7c4
			 0x0cebf5a6, /// 0x7c8
			 0x26600a45, /// 0x7cc
			 0xf438a11b, /// 0x7d0
			 0xce6391f0, /// 0x7d4
			 0xf62ee88b, /// 0x7d8
			 0x695286d1, /// 0x7dc
			 0xbde26498, /// 0x7e0
			 0x6ad0a127, /// 0x7e4
			 0x6647f45e, /// 0x7e8
			 0x2725a1e8, /// 0x7ec
			 0xf0920a1f, /// 0x7f0
			 0x7cabf69c, /// 0x7f4
			 0x5db7d1f8, /// 0x7f8
			 0x118f423d, /// 0x7fc
			 0xf2810e54, /// 0x800
			 0xe4ee5790, /// 0x804
			 0xca278808, /// 0x808
			 0x31dc0912, /// 0x80c
			 0xcb75232e, /// 0x810
			 0xb62f4958, /// 0x814
			 0x5952f830, /// 0x818
			 0xd8922f7d, /// 0x81c
			 0xc1599023, /// 0x820
			 0x0e052592, /// 0x824
			 0x82b4c59c, /// 0x828
			 0x9e1cd7a5, /// 0x82c
			 0x236c90c8, /// 0x830
			 0x5963966b, /// 0x834
			 0xecac478f, /// 0x838
			 0x66441036, /// 0x83c
			 0xe89e4734, /// 0x840
			 0xda5151fd, /// 0x844
			 0x144d30e5, /// 0x848
			 0x1fac7072, /// 0x84c
			 0x7c759419, /// 0x850
			 0x49cdd466, /// 0x854
			 0x21a031d7, /// 0x858
			 0xf199531c, /// 0x85c
			 0x31161ec9, /// 0x860
			 0x01c0b614, /// 0x864
			 0x8fe03d5a, /// 0x868
			 0x960aa248, /// 0x86c
			 0xcd7276ae, /// 0x870
			 0x1e608ce4, /// 0x874
			 0xc6ee3a7d, /// 0x878
			 0xaeac5fc7, /// 0x87c
			 0x07f6aa1c, /// 0x880
			 0xa22cc6e3, /// 0x884
			 0x0b606f17, /// 0x888
			 0xda6081c0, /// 0x88c
			 0xe6fd2392, /// 0x890
			 0x6b7fc910, /// 0x894
			 0x56f7e99a, /// 0x898
			 0xad5e1950, /// 0x89c
			 0x58ec952d, /// 0x8a0
			 0x67487870, /// 0x8a4
			 0xf8e91fa2, /// 0x8a8
			 0xb4e55f09, /// 0x8ac
			 0xb5ee6aea, /// 0x8b0
			 0x1fce2bc5, /// 0x8b4
			 0xf0ed2355, /// 0x8b8
			 0xfd0243f1, /// 0x8bc
			 0xeaa359e5, /// 0x8c0
			 0x4fc3c38d, /// 0x8c4
			 0xe44dcfed, /// 0x8c8
			 0x91a43c5a, /// 0x8cc
			 0x2afedcff, /// 0x8d0
			 0x54ae6709, /// 0x8d4
			 0xfb1f1d39, /// 0x8d8
			 0x31b37d32, /// 0x8dc
			 0xcf09acce, /// 0x8e0
			 0xf4603c7a, /// 0x8e4
			 0x824786be, /// 0x8e8
			 0xcd3475e7, /// 0x8ec
			 0xe431cce1, /// 0x8f0
			 0x9d85a8f8, /// 0x8f4
			 0xa24b58db, /// 0x8f8
			 0xc175bf2b, /// 0x8fc
			 0x7e3dd802, /// 0x900
			 0x131b2ec4, /// 0x904
			 0xfa36edb0, /// 0x908
			 0x6b2ea6a2, /// 0x90c
			 0xe99ebc72, /// 0x910
			 0x3e89d87a, /// 0x914
			 0x520eca83, /// 0x918
			 0x6277285a, /// 0x91c
			 0x110b6681, /// 0x920
			 0x09c79e6b, /// 0x924
			 0xc13d0165, /// 0x928
			 0x067c5802, /// 0x92c
			 0x8ce5b18a, /// 0x930
			 0xb979e723, /// 0x934
			 0x4954a20f, /// 0x938
			 0x260c4810, /// 0x93c
			 0x149b3fb8, /// 0x940
			 0x472df2cb, /// 0x944
			 0x5ab97cb7, /// 0x948
			 0x3f944d8c, /// 0x94c
			 0xd837c7b1, /// 0x950
			 0xebf2e175, /// 0x954
			 0xe2b16248, /// 0x958
			 0xc6843a77, /// 0x95c
			 0x543484c0, /// 0x960
			 0xa54b04b0, /// 0x964
			 0x6cfdff9a, /// 0x968
			 0x09e30248, /// 0x96c
			 0x629978e2, /// 0x970
			 0xc60b276d, /// 0x974
			 0x8bb26fa4, /// 0x978
			 0xd9607989, /// 0x97c
			 0x53605531, /// 0x980
			 0xeca7d7c2, /// 0x984
			 0xdfc8c9a7, /// 0x988
			 0x9430f1c7, /// 0x98c
			 0xea4ac95f, /// 0x990
			 0x42b6c5da, /// 0x994
			 0xf8a43785, /// 0x998
			 0xd44bf554, /// 0x99c
			 0xc13ebb4f, /// 0x9a0
			 0x907296c6, /// 0x9a4
			 0x1d3799e8, /// 0x9a8
			 0x3f4c5f19, /// 0x9ac
			 0x053d127f, /// 0x9b0
			 0x1cb1714c, /// 0x9b4
			 0x387935a2, /// 0x9b8
			 0x160bd57e, /// 0x9bc
			 0x0556d92a, /// 0x9c0
			 0x42177468, /// 0x9c4
			 0x27f3a716, /// 0x9c8
			 0xccaa1b84, /// 0x9cc
			 0x81dc1ae2, /// 0x9d0
			 0xd360b76d, /// 0x9d4
			 0xc7cc63c6, /// 0x9d8
			 0xde2ff514, /// 0x9dc
			 0xde1070fd, /// 0x9e0
			 0x5efb6d3b, /// 0x9e4
			 0x470d0b8c, /// 0x9e8
			 0x2cb2c0ef, /// 0x9ec
			 0x4f3be427, /// 0x9f0
			 0x61aec21f, /// 0x9f4
			 0xc15b863d, /// 0x9f8
			 0x785511db, /// 0x9fc
			 0x5b12a45c, /// 0xa00
			 0x15a93725, /// 0xa04
			 0x0f39f7f9, /// 0xa08
			 0x69c05b74, /// 0xa0c
			 0x346f5c46, /// 0xa10
			 0xf3bd733c, /// 0xa14
			 0x91202f70, /// 0xa18
			 0xc4cb1d02, /// 0xa1c
			 0xc427a41c, /// 0xa20
			 0x54db1f35, /// 0xa24
			 0x28ef6cc1, /// 0xa28
			 0x5f892962, /// 0xa2c
			 0x087eef62, /// 0xa30
			 0xc30c9cb4, /// 0xa34
			 0x9810fbf6, /// 0xa38
			 0x13009d4c, /// 0xa3c
			 0x7d9d8b98, /// 0xa40
			 0x898256ad, /// 0xa44
			 0x38f8f22f, /// 0xa48
			 0x302c67d6, /// 0xa4c
			 0xeb2c714a, /// 0xa50
			 0xd1fe089f, /// 0xa54
			 0x7116255c, /// 0xa58
			 0xb115309c, /// 0xa5c
			 0x991ff9e1, /// 0xa60
			 0x27972c4b, /// 0xa64
			 0x6d305aae, /// 0xa68
			 0x296e1170, /// 0xa6c
			 0xac6d23d0, /// 0xa70
			 0x2d9cdf5a, /// 0xa74
			 0xfbee6a91, /// 0xa78
			 0x2e0bb8aa, /// 0xa7c
			 0x5526dab8, /// 0xa80
			 0x36f3bfed, /// 0xa84
			 0xaa20ddca, /// 0xa88
			 0x6d04fc43, /// 0xa8c
			 0xb64c9d44, /// 0xa90
			 0x35f6c040, /// 0xa94
			 0x1db5fd17, /// 0xa98
			 0x916d43b1, /// 0xa9c
			 0xfdf8c4df, /// 0xaa0
			 0x514619e0, /// 0xaa4
			 0x6936f660, /// 0xaa8
			 0x038c0da9, /// 0xaac
			 0xd86730da, /// 0xab0
			 0x87832bad, /// 0xab4
			 0xe1da8026, /// 0xab8
			 0x59a57bf2, /// 0xabc
			 0x33ba7103, /// 0xac0
			 0x305ae9d8, /// 0xac4
			 0xb1845e8a, /// 0xac8
			 0x3c2f0139, /// 0xacc
			 0xdfe1a66d, /// 0xad0
			 0x46b0512b, /// 0xad4
			 0xa271d16e, /// 0xad8
			 0x7c70b65c, /// 0xadc
			 0x7bd8d558, /// 0xae0
			 0x43354d40, /// 0xae4
			 0xb02c58e9, /// 0xae8
			 0x464cea72, /// 0xaec
			 0x3c9c75e8, /// 0xaf0
			 0x8bffacaf, /// 0xaf4
			 0x892b718a, /// 0xaf8
			 0x50dd4da4, /// 0xafc
			 0x717e69f5, /// 0xb00
			 0xb01a799b, /// 0xb04
			 0x548304d7, /// 0xb08
			 0x79f2f858, /// 0xb0c
			 0x5b3f2322, /// 0xb10
			 0xe0f871a3, /// 0xb14
			 0xc2528c18, /// 0xb18
			 0xd7c719ce, /// 0xb1c
			 0xabea1fb2, /// 0xb20
			 0xcfa37d4e, /// 0xb24
			 0xf156f8a8, /// 0xb28
			 0x214cbdef, /// 0xb2c
			 0x69e4f45a, /// 0xb30
			 0x35f663f0, /// 0xb34
			 0xfa8353d0, /// 0xb38
			 0x4b4eca57, /// 0xb3c
			 0xb319df24, /// 0xb40
			 0xc031cc29, /// 0xb44
			 0x873969ae, /// 0xb48
			 0x546ad234, /// 0xb4c
			 0x69ec4a4c, /// 0xb50
			 0xc4fc0065, /// 0xb54
			 0x908d0822, /// 0xb58
			 0x8111deb0, /// 0xb5c
			 0x5cb84631, /// 0xb60
			 0x71ad7e10, /// 0xb64
			 0xc53d0e09, /// 0xb68
			 0x09ec5398, /// 0xb6c
			 0x79001b67, /// 0xb70
			 0x008c3ffa, /// 0xb74
			 0x927bed52, /// 0xb78
			 0x3cd7ce97, /// 0xb7c
			 0x86caa73f, /// 0xb80
			 0x6eae843b, /// 0xb84
			 0xeb715efe, /// 0xb88
			 0xada69bf8, /// 0xb8c
			 0x4c0ab88a, /// 0xb90
			 0x239bf255, /// 0xb94
			 0xe63bae14, /// 0xb98
			 0x442d4bcf, /// 0xb9c
			 0x2d0c1d22, /// 0xba0
			 0xf5581d4a, /// 0xba4
			 0x02ca60f2, /// 0xba8
			 0xecb940ef, /// 0xbac
			 0x372e79ce, /// 0xbb0
			 0x78ef6b35, /// 0xbb4
			 0x09af9995, /// 0xbb8
			 0xce7370f4, /// 0xbbc
			 0xe0646860, /// 0xbc0
			 0xfb692ec8, /// 0xbc4
			 0x8bbf4eaf, /// 0xbc8
			 0x7215df66, /// 0xbcc
			 0x696801f5, /// 0xbd0
			 0xdcdea56e, /// 0xbd4
			 0x206bc2c3, /// 0xbd8
			 0x141f7cc4, /// 0xbdc
			 0xd992d670, /// 0xbe0
			 0x1668d3ae, /// 0xbe4
			 0x6b8c1495, /// 0xbe8
			 0x782aa7d9, /// 0xbec
			 0xf1f5585c, /// 0xbf0
			 0x49d79f17, /// 0xbf4
			 0x727274a0, /// 0xbf8
			 0xb82da68a, /// 0xbfc
			 0x99389912, /// 0xc00
			 0xc540ea5c, /// 0xc04
			 0xc961ddfc, /// 0xc08
			 0x42996e95, /// 0xc0c
			 0x9c9ff304, /// 0xc10
			 0xe5184b80, /// 0xc14
			 0xc3be417c, /// 0xc18
			 0x4b27a460, /// 0xc1c
			 0xe81827d2, /// 0xc20
			 0x1e9a9507, /// 0xc24
			 0xfade3059, /// 0xc28
			 0xa437de60, /// 0xc2c
			 0x39f80eba, /// 0xc30
			 0xb3b8ca79, /// 0xc34
			 0x7d23fdba, /// 0xc38
			 0x62415f52, /// 0xc3c
			 0x633a17e5, /// 0xc40
			 0x850721c7, /// 0xc44
			 0x728686bf, /// 0xc48
			 0x81867438, /// 0xc4c
			 0xd188a3eb, /// 0xc50
			 0x8405a6f8, /// 0xc54
			 0x6f9d2840, /// 0xc58
			 0x51c3bab2, /// 0xc5c
			 0xf2cf1ff6, /// 0xc60
			 0x34209f1e, /// 0xc64
			 0x5bdc3dd9, /// 0xc68
			 0x7d013041, /// 0xc6c
			 0xf3bc4972, /// 0xc70
			 0xda9b60c5, /// 0xc74
			 0x6dc648b6, /// 0xc78
			 0x9b7b3060, /// 0xc7c
			 0x9143d6d2, /// 0xc80
			 0x7693d012, /// 0xc84
			 0xa4df6a95, /// 0xc88
			 0x167a9d6b, /// 0xc8c
			 0x9186a6d3, /// 0xc90
			 0x3a331ffe, /// 0xc94
			 0x22fb509c, /// 0xc98
			 0x7bfba8b1, /// 0xc9c
			 0x80b972ce, /// 0xca0
			 0x22a45800, /// 0xca4
			 0xc704684d, /// 0xca8
			 0xb84fd8f9, /// 0xcac
			 0x5aa32be2, /// 0xcb0
			 0x336d61a4, /// 0xcb4
			 0x15d89b68, /// 0xcb8
			 0x39a362d5, /// 0xcbc
			 0x785a9807, /// 0xcc0
			 0x7e4b93c6, /// 0xcc4
			 0x385ec51c, /// 0xcc8
			 0xbb011b37, /// 0xccc
			 0x9fcdf501, /// 0xcd0
			 0x6fb1d9b7, /// 0xcd4
			 0x88a584c3, /// 0xcd8
			 0x9a5a9720, /// 0xcdc
			 0xaed88065, /// 0xce0
			 0x8e6c33e5, /// 0xce4
			 0x3669ed7f, /// 0xce8
			 0x8863ab0b, /// 0xcec
			 0x8d4308d7, /// 0xcf0
			 0x0fcf5d66, /// 0xcf4
			 0x1bbf31c4, /// 0xcf8
			 0x3c87c6d2, /// 0xcfc
			 0x47c88273, /// 0xd00
			 0x60933b48, /// 0xd04
			 0x02740e0b, /// 0xd08
			 0xddcb6c7e, /// 0xd0c
			 0x970c7ae0, /// 0xd10
			 0xa2b66f11, /// 0xd14
			 0x86a9c0c4, /// 0xd18
			 0x79cd3532, /// 0xd1c
			 0x889419a7, /// 0xd20
			 0xbe897cac, /// 0xd24
			 0xf6b4862a, /// 0xd28
			 0xfd6d9936, /// 0xd2c
			 0x30fd89af, /// 0xd30
			 0xd68fc2fe, /// 0xd34
			 0x0ceff69a, /// 0xd38
			 0x6c1733a0, /// 0xd3c
			 0x16a3fa7a, /// 0xd40
			 0x52a9fbd4, /// 0xd44
			 0xde92c26b, /// 0xd48
			 0x9da30972, /// 0xd4c
			 0x2364ddc0, /// 0xd50
			 0x5e278d82, /// 0xd54
			 0x52a7d2af, /// 0xd58
			 0xa5a94614, /// 0xd5c
			 0xc4a0ebdb, /// 0xd60
			 0x6b990088, /// 0xd64
			 0xc7f0c174, /// 0xd68
			 0xb1bb959c, /// 0xd6c
			 0x170a52bd, /// 0xd70
			 0x06483481, /// 0xd74
			 0x9872ebf0, /// 0xd78
			 0x23bd891f, /// 0xd7c
			 0x57ae6fb1, /// 0xd80
			 0x3acb6b4b, /// 0xd84
			 0x7193246a, /// 0xd88
			 0x62c64ee1, /// 0xd8c
			 0x72844618, /// 0xd90
			 0x10ab03a6, /// 0xd94
			 0x4aaba89e, /// 0xd98
			 0x90ea56fb, /// 0xd9c
			 0xc3cab6f6, /// 0xda0
			 0xbe8f0ead, /// 0xda4
			 0x50b40c95, /// 0xda8
			 0x2e38a664, /// 0xdac
			 0xb6dcfdde, /// 0xdb0
			 0xdb29e9d8, /// 0xdb4
			 0xe654a18f, /// 0xdb8
			 0xdfb2b86a, /// 0xdbc
			 0x73cbe706, /// 0xdc0
			 0x9778c636, /// 0xdc4
			 0x0807698d, /// 0xdc8
			 0xd1599821, /// 0xdcc
			 0x58c5c804, /// 0xdd0
			 0x4164f049, /// 0xdd4
			 0xc671efff, /// 0xdd8
			 0x67b0103a, /// 0xddc
			 0x403162bb, /// 0xde0
			 0x9e497c6a, /// 0xde4
			 0x738e65a7, /// 0xde8
			 0x216eb266, /// 0xdec
			 0x91f82b3a, /// 0xdf0
			 0xcc0cc810, /// 0xdf4
			 0xb4d3c307, /// 0xdf8
			 0xd4d1f24f, /// 0xdfc
			 0xc0ecc600, /// 0xe00
			 0xa5d8cc59, /// 0xe04
			 0xe86040e3, /// 0xe08
			 0xdd2880b4, /// 0xe0c
			 0xdc3e31c0, /// 0xe10
			 0x573d4c69, /// 0xe14
			 0x6fe51753, /// 0xe18
			 0xbca03438, /// 0xe1c
			 0xa2435f52, /// 0xe20
			 0xa9ffbadf, /// 0xe24
			 0xe2d88d5a, /// 0xe28
			 0x22a5dd5f, /// 0xe2c
			 0xa26b2b10, /// 0xe30
			 0x9bb263e1, /// 0xe34
			 0x4f10d49e, /// 0xe38
			 0x94b9aca6, /// 0xe3c
			 0x423c6e77, /// 0xe40
			 0x9e2fdfa1, /// 0xe44
			 0xce0233d0, /// 0xe48
			 0x8d8fb57a, /// 0xe4c
			 0xce544772, /// 0xe50
			 0xf42619e6, /// 0xe54
			 0x20f1b01c, /// 0xe58
			 0x01e800c7, /// 0xe5c
			 0x3ea88e89, /// 0xe60
			 0x08a1439a, /// 0xe64
			 0x38c02f61, /// 0xe68
			 0x88e65d3a, /// 0xe6c
			 0xe1a53c42, /// 0xe70
			 0x466ceb28, /// 0xe74
			 0x6d16a1d3, /// 0xe78
			 0x057eeb62, /// 0xe7c
			 0x7d7ec787, /// 0xe80
			 0xfb405eb7, /// 0xe84
			 0x04e8716e, /// 0xe88
			 0x7d4b1fc7, /// 0xe8c
			 0xed571ebb, /// 0xe90
			 0xf4e0da68, /// 0xe94
			 0x18b6dd45, /// 0xe98
			 0x2c3bdb6f, /// 0xe9c
			 0x731b8c84, /// 0xea0
			 0x5f52f9ab, /// 0xea4
			 0x8baf2724, /// 0xea8
			 0xbe6cb1c1, /// 0xeac
			 0x35af5da7, /// 0xeb0
			 0x021a9d40, /// 0xeb4
			 0xb17625e8, /// 0xeb8
			 0x66e6f555, /// 0xebc
			 0x8198e879, /// 0xec0
			 0xf9342860, /// 0xec4
			 0x0a9814ab, /// 0xec8
			 0x9a3201f8, /// 0xecc
			 0x607904ec, /// 0xed0
			 0x7f39ee95, /// 0xed4
			 0x7f28e62d, /// 0xed8
			 0x8c5209d0, /// 0xedc
			 0x894aecd8, /// 0xee0
			 0x2218d020, /// 0xee4
			 0x3be7440c, /// 0xee8
			 0x35e064ed, /// 0xeec
			 0x20adeeb7, /// 0xef0
			 0x29dfa0e4, /// 0xef4
			 0xa3296a52, /// 0xef8
			 0x3ff91c86, /// 0xefc
			 0x0d98b075, /// 0xf00
			 0x489bbf59, /// 0xf04
			 0x6dd3f8f2, /// 0xf08
			 0x37075ea2, /// 0xf0c
			 0x9874e80a, /// 0xf10
			 0xe98d07c9, /// 0xf14
			 0x7b435fbd, /// 0xf18
			 0x1e2c6a7d, /// 0xf1c
			 0xaa7b1189, /// 0xf20
			 0xd5124c1b, /// 0xf24
			 0xa29e2fd2, /// 0xf28
			 0xa8dc5d4d, /// 0xf2c
			 0xc4b2d2ef, /// 0xf30
			 0xb3867054, /// 0xf34
			 0x998dcb8c, /// 0xf38
			 0x853e12e1, /// 0xf3c
			 0xa2ca5357, /// 0xf40
			 0x7669d256, /// 0xf44
			 0x88fac1f2, /// 0xf48
			 0xa6a32d0f, /// 0xf4c
			 0xc80e0279, /// 0xf50
			 0x15ffaf15, /// 0xf54
			 0x8c9dfeb8, /// 0xf58
			 0x075befaf, /// 0xf5c
			 0x730c0b1a, /// 0xf60
			 0x7e39c48f, /// 0xf64
			 0x8ca2e078, /// 0xf68
			 0x4bed19cf, /// 0xf6c
			 0xccbbbfba, /// 0xf70
			 0x4e3320e1, /// 0xf74
			 0xa7baee2e, /// 0xf78
			 0x62d02262, /// 0xf7c
			 0xd3b3340f, /// 0xf80
			 0x8158c4b6, /// 0xf84
			 0xde3abdf1, /// 0xf88
			 0xb51b0051, /// 0xf8c
			 0x54f0fb16, /// 0xf90
			 0x006361dc, /// 0xf94
			 0x14bf984c, /// 0xf98
			 0x860d2c36, /// 0xf9c
			 0xd345f60a, /// 0xfa0
			 0x283045a9, /// 0xfa4
			 0x4a62b8d8, /// 0xfa8
			 0xdb3721d3, /// 0xfac
			 0xf5add9d1, /// 0xfb0
			 0xc21974a3, /// 0xfb4
			 0x90d225dd, /// 0xfb8
			 0x282ccd23, /// 0xfbc
			 0x29adcb38, /// 0xfc0
			 0xdece8cb3, /// 0xfc4
			 0x6dda4278, /// 0xfc8
			 0x82b8a9d8, /// 0xfcc
			 0x7c2ec906, /// 0xfd0
			 0xa75d07cb, /// 0xfd4
			 0x58266c2d, /// 0xfd8
			 0x51034ec9, /// 0xfdc
			 0x72094d1a, /// 0xfe0
			 0x0c217b51, /// 0xfe4
			 0xab77f743, /// 0xfe8
			 0x42a12c27, /// 0xfec
			 0xfa84a6c6, /// 0xff0
			 0xaae6d707, /// 0xff4
			 0x5ffc5766, /// 0xff8
			 0xb038220f /// last
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
			 0x00000484,
			 0x00000458,
			 0x00000264,
			 0x000001fc,
			 0x00000138,
			 0x00000518,
			 0x000007c8,
			 0x00000148,

			 /// vpu register f2
			 0x40e00000,
			 0x41c00000,
			 0x41e80000,
			 0x41c00000,
			 0x41200000,
			 0x41980000,
			 0x41b80000,
			 0x41d80000,

			 /// vpu register f3
			 0x41c80000,
			 0x41b00000,
			 0x41400000,
			 0x41b00000,
			 0x41880000,
			 0x41b80000,
			 0x41880000,
			 0x41b80000,

			 /// vpu register f4
			 0x41c80000,
			 0x41a00000,
			 0x41980000,
			 0x41f00000,
			 0x41500000,
			 0x41880000,
			 0x41a80000,
			 0x41d00000,

			 /// vpu register f5
			 0x41a80000,
			 0x41900000,
			 0x41f80000,
			 0x41200000,
			 0x41300000,
			 0x41600000,
			 0x41e00000,
			 0x40e00000,

			 /// vpu register f6
			 0x41880000,
			 0x41c00000,
			 0x41600000,
			 0x41f00000,
			 0x41f80000,
			 0x40000000,
			 0x41000000,
			 0x41100000,

			 /// vpu register f7
			 0x40c00000,
			 0x41200000,
			 0x3f800000,
			 0x41e00000,
			 0x41b80000,
			 0x41c00000,
			 0x41e80000,
			 0x41500000,

			 /// vpu register f8
			 0x41d00000,
			 0x41e00000,
			 0x41c00000,
			 0x41300000,
			 0x41d80000,
			 0x41000000,
			 0x41f00000,
			 0x41d00000,

			 /// vpu register f9
			 0x41a00000,
			 0x41800000,
			 0x41e00000,
			 0x41400000,
			 0x40c00000,
			 0x41b00000,
			 0x40e00000,
			 0x40000000,

			 /// vpu register f10
			 0x41100000,
			 0x41e80000,
			 0x41800000,
			 0x41600000,
			 0x41700000,
			 0x41980000,
			 0x41d00000,
			 0x41200000,

			 /// vpu register f11
			 0x41700000,
			 0x41e00000,
			 0x41e80000,
			 0x40a00000,
			 0x41980000,
			 0x41200000,
			 0x40000000,
			 0x42000000,

			 /// vpu register f12
			 0x41000000,
			 0x42000000,
			 0x41980000,
			 0x41880000,
			 0x42000000,
			 0x41500000,
			 0x41600000,
			 0x40400000,

			 /// vpu register f13
			 0x41a00000,
			 0x41c00000,
			 0x41900000,
			 0x41e00000,
			 0x40400000,
			 0x41e80000,
			 0x41000000,
			 0x41900000,

			 /// vpu register f14
			 0x41a00000,
			 0x41c00000,
			 0x41b80000,
			 0x41400000,
			 0x41d80000,
			 0x41f80000,
			 0x41400000,
			 0x41980000,

			 /// vpu register f15
			 0x41000000,
			 0x41800000,
			 0x41000000,
			 0x41700000,
			 0x41700000,
			 0x41e80000,
			 0x40000000,
			 0x41b00000,

			 /// vpu register f16
			 0x41880000,
			 0x41a80000,
			 0x40000000,
			 0x42000000,
			 0x41e00000,
			 0x41600000,
			 0x41a80000,
			 0x40000000,

			 /// vpu register f17
			 0x41400000,
			 0x40c00000,
			 0x41200000,
			 0x41d80000,
			 0x42000000,
			 0x40400000,
			 0x41980000,
			 0x41980000,

			 /// vpu register f18
			 0x41100000,
			 0x41f00000,
			 0x40c00000,
			 0x41d80000,
			 0x41900000,
			 0x41800000,
			 0x41900000,
			 0x40c00000,

			 /// vpu register f19
			 0x41a00000,
			 0x41b80000,
			 0x41a80000,
			 0x41700000,
			 0x41500000,
			 0x40800000,
			 0x41a00000,
			 0x41f00000,

			 /// vpu register f20
			 0x40000000,
			 0x41c00000,
			 0x41b00000,
			 0x41500000,
			 0x40400000,
			 0x40400000,
			 0x40000000,
			 0x41600000,

			 /// vpu register f21
			 0x41980000,
			 0x40e00000,
			 0x41a00000,
			 0x40400000,
			 0x41200000,
			 0x41200000,
			 0x41400000,
			 0x40000000,

			 /// vpu register f22
			 0x41400000,
			 0x41300000,
			 0x41e00000,
			 0x41e80000,
			 0x3f800000,
			 0x41c80000,
			 0x40000000,
			 0x40a00000,

			 /// vpu register f23
			 0x40c00000,
			 0x41900000,
			 0x41200000,
			 0x41900000,
			 0x41700000,
			 0x40400000,
			 0x41900000,
			 0x40400000,

			 /// vpu register f24
			 0x40c00000,
			 0x41d00000,
			 0x41e00000,
			 0x41b80000,
			 0x40c00000,
			 0x41300000,
			 0x42000000,
			 0x41400000,

			 /// vpu register f25
			 0x41e00000,
			 0x41f00000,
			 0x41700000,
			 0x41f80000,
			 0x41800000,
			 0x41400000,
			 0x41000000,
			 0x41700000,

			 /// vpu register f26
			 0x41400000,
			 0x40c00000,
			 0x41d00000,
			 0x41b00000,
			 0x42000000,
			 0x41000000,
			 0x42000000,
			 0x41d80000,

			 /// vpu register f27
			 0x41b80000,
			 0x40e00000,
			 0x41980000,
			 0x41c80000,
			 0x41400000,
			 0x41f00000,
			 0x41b00000,
			 0x40c00000,

			 /// vpu register f28
			 0x40c00000,
			 0x41200000,
			 0x41500000,
			 0x3f800000,
			 0x41800000,
			 0x41800000,
			 0x41e80000,
			 0x41c80000,

			 /// vpu register f29
			 0x41900000,
			 0x41b00000,
			 0x40e00000,
			 0x41b80000,
			 0x41d80000,
			 0x41c00000,
			 0x41e80000,
			 0x41a00000,

			 /// vpu register f30
			 0x40800000,
			 0x41f80000,
			 0x40e00000,
			 0x41c80000,
			 0x40800000,
			 0x41300000,
			 0x41d00000,
			 0x41000000,

			 /// vpu register f31
			 0x41700000,
			 0x41500000,
			 0x41f00000,
			 0x41000000,
			 0x42000000,
			 0x41880000,
			 0x41400000,
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
		"nop\n"
		"fscbg.ps f28, f1 (x11)\n"                            ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f4, f1 (x11)\n"                             ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f10, f1 (x15)\n"                            ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f10, f1 (x15)\n"                            ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f4, f1 (x13)\n"                             ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f22, f1 (x12)\n"                            ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f14, f1 (x14)\n"                            ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f21, f1 (x15)\n"                            ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f14, f1 (x12)\n"                            ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f11, f1 (x13)\n"                            ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f19, f1 (x12)\n"                            ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f4, f1 (x15)\n"                             ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f15, f1 (x11)\n"                            ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f20, f1 (x12)\n"                            ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f28, f1 (x15)\n"                            ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f14, f1 (x11)\n"                            ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f26, f1 (x11)\n"                            ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f4, f1 (x15)\n"                             ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f23, f1 (x13)\n"                            ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f22, f1 (x15)\n"                            ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f5, f1 (x14)\n"                             ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f27, f1 (x13)\n"                            ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f11, f1 (x11)\n"                            ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f7, f1 (x13)\n"                             ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f27, f1 (x14)\n"                            ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f3, f1 (x12)\n"                             ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f10, f1 (x14)\n"                            ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f11, f1 (x14)\n"                            ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f4, f1 (x14)\n"                             ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f20, f1 (x15)\n"                            ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f4, f1 (x14)\n"                             ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f1, f1 (x15)\n"                             ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f6, f1 (x15)\n"                             ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f28, f1 (x14)\n"                            ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f7, f1 (x13)\n"                             ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f28, f1 (x12)\n"                            ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f25, f1 (x15)\n"                            ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f4, f1 (x13)\n"                             ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f24, f1 (x13)\n"                            ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f25, f1 (x13)\n"                            ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f29, f1 (x14)\n"                            ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f27, f1 (x11)\n"                            ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f20, f1 (x13)\n"                            ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f16, f1 (x14)\n"                            ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f14, f1 (x11)\n"                            ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f1, f1 (x11)\n"                             ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f3, f1 (x15)\n"                             ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f12, f1 (x14)\n"                            ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f12, f1 (x12)\n"                            ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f24, f1 (x15)\n"                            ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f26, f1 (x14)\n"                            ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f22, f1 (x12)\n"                            ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f8, f1 (x11)\n"                             ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f12, f1 (x14)\n"                            ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f6, f1 (x15)\n"                             ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f28, f1 (x15)\n"                            ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f13, f1 (x15)\n"                            ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f30, f1 (x12)\n"                            ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f29, f1 (x12)\n"                            ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f25, f1 (x11)\n"                            ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f24, f1 (x13)\n"                            ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f22, f1 (x12)\n"                            ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f22, f1 (x14)\n"                            ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f24, f1 (x11)\n"                            ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f22, f1 (x15)\n"                            ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f3, f1 (x15)\n"                             ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f17, f1 (x15)\n"                            ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f15, f1 (x11)\n"                            ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f28, f1 (x14)\n"                            ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f29, f1 (x15)\n"                            ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f26, f1 (x15)\n"                            ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f29, f1 (x13)\n"                            ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f24, f1 (x11)\n"                            ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f30, f1 (x12)\n"                            ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f2, f1 (x11)\n"                             ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f26, f1 (x12)\n"                            ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f14, f1 (x14)\n"                            ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f28, f1 (x11)\n"                            ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f13, f1 (x15)\n"                            ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f0, f1 (x12)\n"                             ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f17, f1 (x11)\n"                            ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f14, f1 (x13)\n"                            ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f28, f1 (x12)\n"                            ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f18, f1 (x14)\n"                            ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f15, f1 (x12)\n"                            ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f26, f1 (x12)\n"                            ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f19, f1 (x14)\n"                            ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f22, f1 (x11)\n"                            ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f5, f1 (x15)\n"                             ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f2, f1 (x15)\n"                             ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f1, f1 (x11)\n"                             ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f19, f1 (x13)\n"                            ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f1, f1 (x13)\n"                             ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f28, f1 (x13)\n"                            ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f2, f1 (x14)\n"                             ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f0, f1 (x11)\n"                             ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f4, f1 (x11)\n"                             ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f9, f1 (x14)\n"                             ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscbg.ps f0, f1 (x14)\n"                             ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
