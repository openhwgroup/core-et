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
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00000
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00004
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0000c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00010
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0001c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00020
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00024
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00028
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0002c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00030
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00034
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00038
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0003c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00040
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00044
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00048
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0004c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00050
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00054
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00058
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0005c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00060
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00064
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00068
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0006c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00070
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00074
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00078
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0007c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00084
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00088
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0008c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00090
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00094
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00098
			 0xff800000,                                                  // -inf                                        /// 0009c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 000a0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000a4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 000a8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 000ac
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000b0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b8
			 0x0e000001,                                                  // Trailing 1s:                                /// 000bc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000c8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 000cc
			 0xffc00001,                                                  // -signaling NaN                              /// 000d0
			 0xff800000,                                                  // -inf                                        /// 000d4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000d8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 000dc
			 0x0c600000,                                                  // Leading 1s:                                 /// 000e0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000e4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000e8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000ec
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0x80000000,                                                  // -zero                                       /// 000f4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 000f8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000fc
			 0xff800000,                                                  // -inf                                        /// 00100
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00104
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00108
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0010c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00114
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00118
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0011c
			 0xffc00001,                                                  // -signaling NaN                              /// 00120
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00124
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0012c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00130
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00134
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00138
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0013c
			 0xffc00001,                                                  // -signaling NaN                              /// 00140
			 0x7f800000,                                                  // inf                                         /// 00144
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00148
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0014c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00150
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00154
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00158
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0015c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00160
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00164
			 0x4b000000,                                                  // 8388608.0                                   /// 00168
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0016c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00170
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00174
			 0x0c400000,                                                  // Leading 1s:                                 /// 00178
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0017c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00184
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00188
			 0x00000000,                                                  // zero                                        /// 0018c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00190
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00194
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00198
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0019c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001a0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001a4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001a8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 001b0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001b4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 001b8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001bc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 001c4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001c8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001cc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001d8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001dc
			 0xbf028f5c,                                                  // -0.51                                       /// 001e0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001e4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 001e8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001ec
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001f0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 001f4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 001f8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 001fc
			 0x55555555,                                                  // 4 random values                             /// 00200
			 0x80000000,                                                  // -zero                                       /// 00204
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0x33333333,                                                  // 4 random values                             /// 00210
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00214
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00218
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0021c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00220
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00224
			 0x0e000001,                                                  // Trailing 1s:                                /// 00228
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0022c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00230
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00234
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00238
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0023c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00240
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00244
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00248
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0024c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00250
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00254
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00258
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00260
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00264
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00268
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0026c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00270
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00274
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0x0e000007,                                                  // Trailing 1s:                                /// 0027c
			 0x7f800000,                                                  // inf                                         /// 00280
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00284
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00288
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0028c
			 0x80000000,                                                  // -zero                                       /// 00290
			 0x33333333,                                                  // 4 random values                             /// 00294
			 0x0e000001,                                                  // Trailing 1s:                                /// 00298
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0029c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002a0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 002a4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002a8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0x0e000001,                                                  // Trailing 1s:                                /// 002b0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002b4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 002b8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002bc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 002c4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 002c8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002cc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002d0
			 0x4b000000,                                                  // 8388608.0                                   /// 002d4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002d8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 002dc
			 0xbf028f5c,                                                  // -0.51                                       /// 002e0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 002e4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 002ec
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002f0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 002f4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 002fc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00300
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00304
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00308
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0030c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00310
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00318
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0x7f800000,                                                  // inf                                         /// 00320
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00324
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00328
			 0xff800000,                                                  // -inf                                        /// 0032c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00330
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00334
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00338
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0033c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00340
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00344
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00348
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0034c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00350
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00354
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00358
			 0x80011111,                                                  // -9.7958E-41                                 /// 0035c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00360
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00364
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00368
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0036c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00370
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00374
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00378
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0037c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00380
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00384
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00388
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0038c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00390
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00394
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00398
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0039c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003a8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 003ac
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 003b4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003b8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003bc
			 0xff800000,                                                  // -inf                                        /// 003c0
			 0x7fc00001,                                                  // signaling NaN                               /// 003c4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 003c8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003d0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 003d4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 003dc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 003e4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 003e8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003ec
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003f0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003f4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 003f8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003fc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00400
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00404
			 0x00011111,                                                  // 9.7958E-41                                  /// 00408
			 0x7f800000,                                                  // inf                                         /// 0040c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00410
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00414
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00418
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0041c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00420
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00424
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00428
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00430
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00434
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0043c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00444
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00448
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0044c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00454
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00458
			 0xff800000,                                                  // -inf                                        /// 0045c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00460
			 0x00000000,                                                  // zero                                        /// 00464
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0x7fc00001,                                                  // signaling NaN                               /// 0046c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00470
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00474
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00478
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0047c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00480
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00484
			 0xbf028f5c,                                                  // -0.51                                       /// 00488
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0048c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00490
			 0x33333333,                                                  // 4 random values                             /// 00494
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00498
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 004a4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 004a8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ac
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004b0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 004b8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004bc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004c4
			 0x0c700000,                                                  // Leading 1s:                                 /// 004c8
			 0x00011111,                                                  // 9.7958E-41                                  /// 004cc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 004d0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 004d4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 004dc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004e0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 004e4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 004e8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 004ec
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004f0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004f4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004fc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00500
			 0x00000000,                                                  // zero                                        /// 00504
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00508
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0050c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00510
			 0x00011111,                                                  // 9.7958E-41                                  /// 00514
			 0xff800000,                                                  // -inf                                        /// 00518
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0051c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00520
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00524
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00528
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0052c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00530
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00534
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00538
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00540
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00548
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0054c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00550
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00558
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0055c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00560
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00564
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00568
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0056c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00570
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00574
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0x00011111,                                                  // 9.7958E-41                                  /// 0057c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00584
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00588
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0058c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00594
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00598
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0059c
			 0x00011111,                                                  // 9.7958E-41                                  /// 005a0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005a4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005a8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005ac
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 005b4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005b8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 005bc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005c0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005c8
			 0x33333333,                                                  // 4 random values                             /// 005cc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 005d0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 005d4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005d8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005dc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005e0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005e4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005e8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005f0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 005f4
			 0xbf028f5c,                                                  // -0.51                                       /// 005f8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005fc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00600
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00604
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00608
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0060c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00610
			 0x0e000003,                                                  // Trailing 1s:                                /// 00614
			 0xff800000,                                                  // -inf                                        /// 00618
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0061c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00620
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00624
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00628
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0062c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00630
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00634
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00638
			 0x0e000007,                                                  // Trailing 1s:                                /// 0063c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00644
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00648
			 0x4b000000,                                                  // 8388608.0                                   /// 0064c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00650
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00654
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00658
			 0x00000000,                                                  // zero                                        /// 0065c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00660
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00664
			 0x00011111,                                                  // 9.7958E-41                                  /// 00668
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0066c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00670
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00674
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00678
			 0x0c700000,                                                  // Leading 1s:                                 /// 0067c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00680
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00684
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00688
			 0x80000000,                                                  // -zero                                       /// 0068c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00690
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00694
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00698
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0069c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006a0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006a4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 006a8
			 0x7f800000,                                                  // inf                                         /// 006ac
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006b0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 006b4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006bc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006c0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006c4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006cc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006d0
			 0x4b000000,                                                  // 8388608.0                                   /// 006d4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006d8
			 0x00011111,                                                  // 9.7958E-41                                  /// 006dc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006ec
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 006f4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006fc
			 0xffc00001,                                                  // -signaling NaN                              /// 00700
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00704
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00708
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0070c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00710
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00714
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00718
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0071c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00720
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00724
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00728
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0072c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00730
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00734
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00738
			 0x0c600000,                                                  // Leading 1s:                                 /// 0073c
			 0x7f800000,                                                  // inf                                         /// 00740
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00744
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00748
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0x3f028f5c,                                                  // 0.51                                        /// 00750
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00754
			 0x00000000,                                                  // zero                                        /// 00758
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0075c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00760
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00764
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00768
			 0x00011111,                                                  // 9.7958E-41                                  /// 0076c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00774
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00778
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0077c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00780
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00784
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00788
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0078c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00790
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00794
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0079c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007a4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007a8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007ac
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007b0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 007bc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 007c0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 007c4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007c8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007cc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 007d0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007d4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007dc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007e0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007e8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 007ec
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 007f4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007f8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007fc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00800
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00804
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00808
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0080c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00810
			 0x80011111,                                                  // -9.7958E-41                                 /// 00814
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00818
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0081c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00820
			 0xbf028f5c,                                                  // -0.51                                       /// 00824
			 0x4b000000,                                                  // 8388608.0                                   /// 00828
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0082c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00830
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00834
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0083c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00840
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00848
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0084c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00850
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0085c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00864
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00868
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0086c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00870
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00874
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00878
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0087c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00880
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00884
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00888
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0088c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00890
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00894
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00898
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0089c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 008a0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 008a4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 008a8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008ac
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008b0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008b8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 008bc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008c0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 008c4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 008c8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008cc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008d4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008d8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e0
			 0xffc00001,                                                  // -signaling NaN                              /// 008e4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008e8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ec
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008f0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008f4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 008f8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008fc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00904
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00908
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0090c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00910
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00914
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00918
			 0x7fc00001,                                                  // signaling NaN                               /// 0091c
			 0x00000000,                                                  // zero                                        /// 00920
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00924
			 0x0c700000,                                                  // Leading 1s:                                 /// 00928
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0092c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00934
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00938
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0093c
			 0x4b000000,                                                  // 8388608.0                                   /// 00940
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00944
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00948
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0094c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00954
			 0xffc00001,                                                  // -signaling NaN                              /// 00958
			 0x0c780000,                                                  // Leading 1s:                                 /// 0095c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00960
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00964
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00968
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0096c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00970
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00978
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0097c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00980
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00988
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0098c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00990
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00994
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00998
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0099c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x0c700000,                                                  // Leading 1s:                                 /// 009a4
			 0x0c600000,                                                  // Leading 1s:                                 /// 009a8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009ac
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009b0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009b4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009b8
			 0x0c780000,                                                  // Leading 1s:                                 /// 009bc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009c0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009c4
			 0x0c400000,                                                  // Leading 1s:                                 /// 009c8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 009cc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009d0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 009d4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 009d8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009dc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 009e0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009e4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ec
			 0xff800000,                                                  // -inf                                        /// 009f0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 009f4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009f8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 009fc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a04
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a08
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a0c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a10
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a18
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a1c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a24
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a28
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a30
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a34
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a38
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a3c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a40
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a44
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a48
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a4c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a50
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a54
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a60
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a64
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a68
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a6c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a70
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a74
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a78
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a7c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a80
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a84
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a8c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a90
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a94
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a98
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a9c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00aa0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00aa4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00aa8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00aac
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ab0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ab4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ab8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ac0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ac4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ac8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00acc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ad0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ad8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0x7f800000,                                                  // inf                                         /// 00ae0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ae4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ae8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00aec
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00af0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00af8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00afc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b00
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b04
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b08
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b0c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b10
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b14
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b18
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b20
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b24
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b2c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b30
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b34
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b38
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b3c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b40
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b44
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b48
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b4c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b50
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b54
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b58
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b5c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b60
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b64
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b68
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b70
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b74
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b78
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b84
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b88
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b90
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b94
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b98
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b9c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ba0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ba4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ba8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00bb4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00bc0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00bc4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00bd0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00bd4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00bdc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00be0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00be8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00bec
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00bf4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00bfc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c00
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c04
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c08
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c0c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c14
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c18
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c1c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c20
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c24
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c28
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c30
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c34
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c38
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c3c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c40
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c44
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c48
			 0x7fc00001,                                                  // signaling NaN                               /// 00c4c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c50
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c54
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c58
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c5c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c60
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c64
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c68
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c6c
			 0xff800000,                                                  // -inf                                        /// 00c70
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c74
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c78
			 0x00000000,                                                  // zero                                        /// 00c7c
			 0xff800000,                                                  // -inf                                        /// 00c80
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c84
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c8c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c90
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c94
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c98
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c9c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ca0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ca4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ca8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00cac
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cb4
			 0xffc00001,                                                  // -signaling NaN                              /// 00cb8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00cbc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00cc0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00cc4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00cc8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ccc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00cd0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00cd4
			 0xffc00001,                                                  // -signaling NaN                              /// 00cd8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00cdc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ce0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ce4
			 0xffc00001,                                                  // -signaling NaN                              /// 00ce8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00cec
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00cf0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00cf8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00cfc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d00
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d04
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d08
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d0c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d14
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d18
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d1c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d20
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d24
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d28
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d2c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d30
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d34
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d38
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d3c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d40
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d44
			 0xffc00001,                                                  // -signaling NaN                              /// 00d48
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d4c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d50
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d58
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d5c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d60
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d64
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d68
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d6c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d70
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d74
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d78
			 0x3f028f5c,                                                  // 0.51                                        /// 00d7c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d80
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d84
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d88
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d8c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d90
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d94
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d98
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d9c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00da0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00dac
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00db0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00db4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00db8
			 0x7fc00001,                                                  // signaling NaN                               /// 00dbc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00dc0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00dc4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00dc8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00dcc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00dd0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00dd4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00dd8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ddc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00de4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00df0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00df8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00dfc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e00
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e04
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e08
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e0c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e10
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e14
			 0xff800000,                                                  // -inf                                        /// 00e18
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e1c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e28
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e2c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e30
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e34
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e38
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e3c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e44
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e4c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e50
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e54
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e58
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e5c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e60
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e64
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e68
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e6c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e74
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e78
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e7c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e80
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e84
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e88
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e8c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e90
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e94
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e98
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e9c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ea0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ea8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00eac
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00eb0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00eb4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00eb8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ebc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec0
			 0x7f800000,                                                  // inf                                         /// 00ec4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ecc
			 0x00000000,                                                  // zero                                        /// 00ed0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00edc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ee4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ee8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00eec
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ef4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ef8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00efc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f00
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f04
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f08
			 0xcb000000,                                                  // -8388608.0                                  /// 00f0c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f10
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f14
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f18
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f1c
			 0xffc00001,                                                  // -signaling NaN                              /// 00f20
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f24
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f28
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f2c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f30
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f34
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f38
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f3c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f40
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f44
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f48
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f4c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f50
			 0xffc00001,                                                  // -signaling NaN                              /// 00f54
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f58
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f5c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f60
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f68
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f6c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f70
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f74
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f78
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f7c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f80
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f84
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f8c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f90
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f94
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f98
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f9c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fa0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00fa4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00fac
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00fb0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb4
			 0x00000000,                                                  // zero                                        /// 00fb8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00fbc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00fc0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00fc4
			 0x7fc00001,                                                  // signaling NaN                               /// 00fc8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fcc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fd4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fd8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fdc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00fe4
			 0x80000000,                                                  // -zero                                       /// 00fe8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fec
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ff0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ff4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ff8
			 0x7fc00001 // QNan                                          // SP +ve numbers                              /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00000
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00004
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00008
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0000c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00010
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00014
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00018
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0001c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00020
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00024
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00028
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0002c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00030
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00034
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00038
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0003c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00040
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00044
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00048
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0004c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00050
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00054
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00058
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0005c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00060
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00068
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0006c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00070
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00074
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00078
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0007c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00080
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00084
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00088
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0008c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00090
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00094
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00098
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0009c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 000a4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000ac
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000b0
			 0xcb000000,                                                  // -8388608.0                                  /// 000b4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000b8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000c0
			 0xffc00001,                                                  // -signaling NaN                              /// 000c4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 000c8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000cc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000d0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 000d4
			 0x80011111,                                                  // -9.7958E-41                                 /// 000d8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 000e0
			 0x0e000003,                                                  // Trailing 1s:                                /// 000e4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 000e8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000f0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000f4
			 0xff800000,                                                  // -inf                                        /// 000f8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00104
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00108
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0010c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00110
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00114
			 0x0c400000,                                                  // Leading 1s:                                 /// 00118
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0011c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00120
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00124
			 0x55555555,                                                  // 4 random values                             /// 00128
			 0x55555555,                                                  // 4 random values                             /// 0012c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00130
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00134
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0013c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0x4b000000,                                                  // 8388608.0                                   /// 00144
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00148
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0014c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00158
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0015c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00160
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00164
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00168
			 0x0c600000,                                                  // Leading 1s:                                 /// 0016c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00170
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00174
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00178
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0017c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00180
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00184
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00188
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00190
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00194
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00198
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0019c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001a4
			 0xcb000000,                                                  // -8388608.0                                  /// 001a8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001ac
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 001b8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 001bc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001c0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c8
			 0x3f028f5c,                                                  // 0.51                                        /// 001cc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001d0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 001d8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001dc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001e0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 001e8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 001ec
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001f8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001fc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00208
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0020c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00210
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00214
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00218
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0021c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00220
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00224
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00228
			 0x0c780000,                                                  // Leading 1s:                                 /// 0022c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00230
			 0x7f800000,                                                  // inf                                         /// 00234
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00238
			 0x00011111,                                                  // 9.7958E-41                                  /// 0023c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00240
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00244
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00248
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00250
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00254
			 0x4b000000,                                                  // 8388608.0                                   /// 00258
			 0x80000000,                                                  // -zero                                       /// 0025c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00260
			 0x0e000007,                                                  // Trailing 1s:                                /// 00264
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00268
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0026c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00270
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00274
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00278
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0027c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00280
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00284
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00288
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0028c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00290
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00294
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0029c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002a0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002a4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002a8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002ac
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002b0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002b4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 002b8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 002bc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 002c0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 002c4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002cc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002d0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002d4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002d8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002dc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002e8
			 0x00000000,                                                  // zero                                        /// 002ec
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002f4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002f8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002fc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00300
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00304
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00308
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00310
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00314
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00318
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0031c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00320
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00324
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00328
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00330
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00334
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00338
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00340
			 0xffc00001,                                                  // -signaling NaN                              /// 00344
			 0x0e000007,                                                  // Trailing 1s:                                /// 00348
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0034c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00350
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00354
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00358
			 0x0c400000,                                                  // Leading 1s:                                 /// 0035c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00360
			 0x3f028f5c,                                                  // 0.51                                        /// 00364
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00368
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0036c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00378
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0037c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00380
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00384
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00388
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0038c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00390
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00394
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 003a0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003a4
			 0x33333333,                                                  // 4 random values                             /// 003a8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 003ac
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003b0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003bc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003c4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003c8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 003d0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 003d4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003dc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 003e0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003e8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ec
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 003f0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003f4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003fc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00400
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00404
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00408
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0040c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00414
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00418
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0041c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00420
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00424
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00428
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0042c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00430
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00434
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00438
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0043c
			 0xbf028f5c,                                                  // -0.51                                       /// 00440
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00444
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00448
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0044c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00450
			 0x0c400000,                                                  // Leading 1s:                                 /// 00454
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00458
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0045c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00460
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00464
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0046c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00470
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00474
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00478
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0047c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00480
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00484
			 0xff800000,                                                  // -inf                                        /// 00488
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0048c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00494
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0049c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004a0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004a4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 004ac
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 004b8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004c4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 004c8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004d0
			 0x80011111,                                                  // -9.7958E-41                                 /// 004d4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004d8
			 0x00000000,                                                  // zero                                        /// 004dc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 004e0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004f0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 004f8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 004fc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00500
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00504
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00508
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00510
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00514
			 0x80000000,                                                  // -zero                                       /// 00518
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00520
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00524
			 0xff800000,                                                  // -inf                                        /// 00528
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0052c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00530
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00534
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00538
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0053c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00544
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00548
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0054c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00550
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00554
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00558
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00560
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00564
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00568
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0056c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00570
			 0x55555555,                                                  // 4 random values                             /// 00574
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00578
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0057c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00580
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00584
			 0x0c600000,                                                  // Leading 1s:                                 /// 00588
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0058c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00590
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00594
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00598
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0059c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005a0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005a4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005a8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b0
			 0xbf028f5c,                                                  // -0.51                                       /// 005b4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 005b8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005bc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005c0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0x0e000003,                                                  // Trailing 1s:                                /// 005cc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005d0
			 0x0e000003,                                                  // Trailing 1s:                                /// 005d4
			 0xbf028f5c,                                                  // -0.51                                       /// 005d8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005ec
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005f0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 005f4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005fc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00600
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00604
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0060c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00610
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00614
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00618
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0061c
			 0xffc00001,                                                  // -signaling NaN                              /// 00620
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00624
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00628
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0062c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00630
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00634
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00638
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0063c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00640
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00644
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00648
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0064c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00650
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00654
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00658
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0065c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00660
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00664
			 0x3f028f5c,                                                  // 0.51                                        /// 00668
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0066c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00670
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00674
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0067c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0x33333333,                                                  // 4 random values                             /// 0068c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00694
			 0x00011111,                                                  // 9.7958E-41                                  /// 00698
			 0x0e000003,                                                  // Trailing 1s:                                /// 0069c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006a0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006a4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006a8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006ac
			 0xAAAAAAAA,                                                  // 4 random values                             /// 006b0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006b4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006bc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006c0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006c8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006cc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 006d0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 006d4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 006d8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006dc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006e8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006ec
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006f0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 006f4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x00000000,                                                  // zero                                        /// 006fc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00700
			 0x0c600000,                                                  // Leading 1s:                                 /// 00704
			 0x0c400000,                                                  // Leading 1s:                                 /// 00708
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0070c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00710
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00714
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0xbf028f5c,                                                  // -0.51                                       /// 0071c
			 0x33333333,                                                  // 4 random values                             /// 00720
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00724
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00728
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0072c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00730
			 0x80011111,                                                  // -9.7958E-41                                 /// 00734
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00738
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0073c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00740
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00744
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00748
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0074c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00750
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00754
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00758
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0075c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00764
			 0x0c400000,                                                  // Leading 1s:                                 /// 00768
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0076c
			 0xff800000,                                                  // -inf                                        /// 00770
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00774
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00778
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0077c
			 0x33333333,                                                  // 4 random values                             /// 00780
			 0x0c700000,                                                  // Leading 1s:                                 /// 00784
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00788
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0078c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00790
			 0x55555555,                                                  // 4 random values                             /// 00794
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0xff800000,                                                  // -inf                                        /// 0079c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007a0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 007a4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 007a8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ac
			 0xAAAAAAAA,                                                  // 4 random values                             /// 007b0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 007b4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 007bc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007cc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007d0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007d8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 007dc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 007e8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007ec
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007f0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007f8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007fc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00800
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00804
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00808
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0080c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00810
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00814
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00818
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0081c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00820
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00824
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00828
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0082c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00830
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00834
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00838
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00840
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00844
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00848
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00854
			 0x33333333,                                                  // 4 random values                             /// 00858
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0085c
			 0x55555555,                                                  // 4 random values                             /// 00860
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00864
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00868
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0086c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00870
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00874
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00878
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0x33333333,                                                  // 4 random values                             /// 00884
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00888
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0088c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0x55555555,                                                  // 4 random values                             /// 00894
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0x80000000,                                                  // -zero                                       /// 0089c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 008a0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008a4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 008a8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008ac
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008b0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008b4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 008b8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008bc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 008c0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 008c4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008c8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 008cc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008d0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 008d4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 008d8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008dc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 008e0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 008e4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008e8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ec
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008f0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 008f4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008f8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008fc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00900
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00908
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0090c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00910
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00914
			 0x00011111,                                                  // 9.7958E-41                                  /// 00918
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0091c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00920
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00924
			 0x33333333,                                                  // 4 random values                             /// 00928
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0092c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00930
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00934
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00938
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0093c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00940
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0x7fc00001,                                                  // signaling NaN                               /// 00948
			 0x0e000001,                                                  // Trailing 1s:                                /// 0094c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00950
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00954
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00958
			 0x0c700000,                                                  // Leading 1s:                                 /// 0095c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00960
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00964
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00968
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00970
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00978
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0097c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00980
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00984
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00988
			 0x0c400000,                                                  // Leading 1s:                                 /// 0098c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00990
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00998
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0099c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 009a0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009a8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 009ac
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009b4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009b8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009bc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009c0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 009c4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009c8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009d0
			 0x0c600000,                                                  // Leading 1s:                                 /// 009d4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009dc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009e0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 009e8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 009ec
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009f0
			 0x00011111,                                                  // 9.7958E-41                                  /// 009f4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009f8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 009fc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a00
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a04
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a08
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a0c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a10
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a14
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a18
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a1c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a24
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a28
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a30
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a34
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a38
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a3c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a40
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a44
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a48
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a4c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a50
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a54
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a58
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a5c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a60
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a64
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a68
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a6c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a70
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a74
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a78
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a7c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a80
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a84
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a88
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a8c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a90
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a94
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a98
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a9c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00aa0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00aa4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00aa8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00aac
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ab4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ab8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00abc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ac0
			 0x3f028f5c,                                                  // 0.51                                        /// 00ac4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00acc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ad0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x55555555,                                                  // 4 random values                             /// 00ad8
			 0x33333333,                                                  // 4 random values                             /// 00adc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ae0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0xff800000,                                                  // -inf                                        /// 00ae8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00aec
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00af0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00af4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00af8
			 0x55555555,                                                  // 4 random values                             /// 00afc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b00
			 0xff800000,                                                  // -inf                                        /// 00b04
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b08
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b0c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b10
			 0x7fc00001,                                                  // signaling NaN                               /// 00b14
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b1c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b20
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b28
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b2c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b30
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b34
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b38
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b3c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b40
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b44
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b48
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0x3f028f5c,                                                  // 0.51                                        /// 00b50
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b54
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b58
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b5c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b64
			 0x33333333,                                                  // 4 random values                             /// 00b68
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b6c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b70
			 0x33333333,                                                  // 4 random values                             /// 00b74
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b78
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b80
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b88
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b8c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b90
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b94
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b98
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b9c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ba0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ba8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bac
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00bb0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00bb4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00bb8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bbc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00bc0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00bc4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00bc8
			 0x55555555,                                                  // 4 random values                             /// 00bcc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00bd4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00bdc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00be0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00be4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00be8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bec
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00bf4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0x80000000,                                                  // -zero                                       /// 00bfc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c00
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c04
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c08
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c10
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c14
			 0x7fc00001,                                                  // signaling NaN                               /// 00c18
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c1c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c20
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c24
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c28
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c2c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c30
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c34
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c38
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c40
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c44
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c48
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c4c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c50
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c54
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c58
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c5c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c60
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c64
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c68
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c6c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c70
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c74
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c78
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c7c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c80
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c84
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c8c
			 0x4b000000,                                                  // 8388608.0                                   /// 00c90
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c94
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c98
			 0xff800000,                                                  // -inf                                        /// 00c9c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ca0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ca8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00cac
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00cb0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cbc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cc0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cc4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00cc8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ccc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00cd0
			 0x00000000,                                                  // zero                                        /// 00cd4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00cd8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cdc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ce8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00cf0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0xcb000000,                                                  // -8388608.0                                  /// 00cfc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d00
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d04
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d08
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d0c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d10
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d14
			 0xffc00001,                                                  // -signaling NaN                              /// 00d18
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d1c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d20
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d24
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d28
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d2c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d30
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d34
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d38
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d3c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d44
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d48
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d4c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d54
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d58
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d5c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d60
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d64
			 0xbf028f5c,                                                  // -0.51                                       /// 00d68
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d6c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d70
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d74
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d78
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d7c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d80
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d84
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d8c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d90
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d94
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d98
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d9c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00da0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00da4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00da8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00dac
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00db0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00db4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00db8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00dbc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00dc0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00dc4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00dc8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00dcc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00dd4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00dd8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ddc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00de0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00de8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00dec
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00df0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00df8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00dfc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e04
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e08
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e0c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e10
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e14
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e18
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e1c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e20
			 0x3f028f5c,                                                  // 0.51                                        /// 00e24
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e28
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e2c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e30
			 0x4b000000,                                                  // 8388608.0                                   /// 00e34
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e38
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e3c
			 0x80000000,                                                  // -zero                                       /// 00e40
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e44
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e48
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e4c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e50
			 0x7f800000,                                                  // inf                                         /// 00e54
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e5c
			 0x7fc00001,                                                  // signaling NaN                               /// 00e60
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e64
			 0x7f800000,                                                  // inf                                         /// 00e68
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e6c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e74
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e78
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e7c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e80
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e84
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e88
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e8c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e90
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e94
			 0xcb000000,                                                  // -8388608.0                                  /// 00e98
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e9c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ea0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ea4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ea8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00eac
			 0xcb000000,                                                  // -8388608.0                                  /// 00eb0
			 0x55555555,                                                  // 4 random values                             /// 00eb4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00eb8
			 0x3f028f5c,                                                  // 0.51                                        /// 00ebc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ec0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0x3f028f5c,                                                  // 0.51                                        /// 00ec8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ecc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ed0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ed4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ed8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00edc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ef0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ef8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00efc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f00
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f04
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f08
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f10
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f14
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f18
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f1c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f24
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f2c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f30
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f34
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f38
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f3c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f40
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f44
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f48
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f4c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f50
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f54
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f58
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f5c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f60
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f64
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f68
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f6c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f70
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f78
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f7c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f80
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f84
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f88
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f8c
			 0x3f028f5c,                                                  // 0.51                                        /// 00f90
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f94
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f9c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00fa0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00fa4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fa8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fac
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00fb0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00fb8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00fbc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fc0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00fc8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fcc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00fd0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd4
			 0x7f800000,                                                  // inf                                         /// 00fd8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00fdc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00fe4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fe8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ff0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ff8
			 0x7fc00001                                                  // signaling NaN                               /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x97eeaf78, /// 0x0
			 0xb480b996, /// 0x4
			 0x2a1ae9b2, /// 0x8
			 0x2cd81a57, /// 0xc
			 0xc4e95c1a, /// 0x10
			 0x594970d4, /// 0x14
			 0x118b004a, /// 0x18
			 0x47137d0c, /// 0x1c
			 0xe61203e5, /// 0x20
			 0x855d1bc5, /// 0x24
			 0x5e080ae7, /// 0x28
			 0x269bea38, /// 0x2c
			 0xf6768e2f, /// 0x30
			 0x9e99a180, /// 0x34
			 0xebe34094, /// 0x38
			 0xf1e43f80, /// 0x3c
			 0x320eac4d, /// 0x40
			 0xf9edd809, /// 0x44
			 0x70ed846c, /// 0x48
			 0x4626fa93, /// 0x4c
			 0xe918d410, /// 0x50
			 0x912d1ae5, /// 0x54
			 0xea2d97dd, /// 0x58
			 0x81010eb8, /// 0x5c
			 0x4f89e59b, /// 0x60
			 0x25c73a77, /// 0x64
			 0xa0b55f10, /// 0x68
			 0x4a8bbfff, /// 0x6c
			 0x44f3c78f, /// 0x70
			 0x0c11a8fe, /// 0x74
			 0x7ab06ce6, /// 0x78
			 0x28834d44, /// 0x7c
			 0xfdfe61b7, /// 0x80
			 0x2ad52b48, /// 0x84
			 0x3013d637, /// 0x88
			 0x858f88a1, /// 0x8c
			 0x70db8388, /// 0x90
			 0x14344661, /// 0x94
			 0x7e8ff55d, /// 0x98
			 0x1214a5c9, /// 0x9c
			 0x8a6d54d4, /// 0xa0
			 0xa4256ed9, /// 0xa4
			 0x8feb812a, /// 0xa8
			 0xba64e3db, /// 0xac
			 0xbdd46df9, /// 0xb0
			 0x48baff22, /// 0xb4
			 0xaa89006c, /// 0xb8
			 0xdc603275, /// 0xbc
			 0xeddd59e4, /// 0xc0
			 0xaf9c1021, /// 0xc4
			 0x824d4fef, /// 0xc8
			 0x174d0e35, /// 0xcc
			 0x6d459fa9, /// 0xd0
			 0x2992c08d, /// 0xd4
			 0x6958a917, /// 0xd8
			 0x9c3b9194, /// 0xdc
			 0x72fb7b2f, /// 0xe0
			 0xa4629297, /// 0xe4
			 0xe30cc0be, /// 0xe8
			 0xbfb2d369, /// 0xec
			 0x0df51deb, /// 0xf0
			 0x13ec1e20, /// 0xf4
			 0xc6ef8251, /// 0xf8
			 0xd9bd15b7, /// 0xfc
			 0x1b0636fb, /// 0x100
			 0xe9ef7705, /// 0x104
			 0xf4000c7f, /// 0x108
			 0xeb6b3268, /// 0x10c
			 0xe1f14d39, /// 0x110
			 0xe3af40fc, /// 0x114
			 0x2a9905f3, /// 0x118
			 0xea25a2a1, /// 0x11c
			 0xc00fecb9, /// 0x120
			 0x83ed6948, /// 0x124
			 0x69dde263, /// 0x128
			 0x2e770d04, /// 0x12c
			 0xaa33a3ea, /// 0x130
			 0x44f331af, /// 0x134
			 0xd4279889, /// 0x138
			 0xf739884c, /// 0x13c
			 0x3fa2bd5e, /// 0x140
			 0xc014fbb6, /// 0x144
			 0xa3ca3e72, /// 0x148
			 0x8e85a02f, /// 0x14c
			 0x31caaa20, /// 0x150
			 0x869b6775, /// 0x154
			 0x2ed6591e, /// 0x158
			 0xd1005502, /// 0x15c
			 0x9f443409, /// 0x160
			 0x17654e61, /// 0x164
			 0xb9cf8330, /// 0x168
			 0x0d4ed145, /// 0x16c
			 0x0124d047, /// 0x170
			 0x5da3791d, /// 0x174
			 0x2d8244a6, /// 0x178
			 0x993ba2a7, /// 0x17c
			 0x378813d9, /// 0x180
			 0x282e5789, /// 0x184
			 0x949cffec, /// 0x188
			 0x069c912a, /// 0x18c
			 0x8160007f, /// 0x190
			 0xa95c3878, /// 0x194
			 0xe61855d0, /// 0x198
			 0xf90ecc92, /// 0x19c
			 0xf7c09620, /// 0x1a0
			 0x7b225d9c, /// 0x1a4
			 0x68770cfa, /// 0x1a8
			 0x01dd2f25, /// 0x1ac
			 0x54ae7e03, /// 0x1b0
			 0x436e1cf4, /// 0x1b4
			 0x094bffaf, /// 0x1b8
			 0xa1d1ecfe, /// 0x1bc
			 0xdba083aa, /// 0x1c0
			 0xfb072413, /// 0x1c4
			 0x4010ee89, /// 0x1c8
			 0x318ed380, /// 0x1cc
			 0x141025d5, /// 0x1d0
			 0xb1dd023c, /// 0x1d4
			 0xb8d5ecca, /// 0x1d8
			 0x693d997c, /// 0x1dc
			 0x45873ab4, /// 0x1e0
			 0x47d58fc3, /// 0x1e4
			 0xb53f8d7c, /// 0x1e8
			 0x37fd1b21, /// 0x1ec
			 0x4563ca99, /// 0x1f0
			 0xda4b22bb, /// 0x1f4
			 0x9ba7de58, /// 0x1f8
			 0x86ec1686, /// 0x1fc
			 0xdf45ee32, /// 0x200
			 0xaf9a4799, /// 0x204
			 0xfc8dd007, /// 0x208
			 0x552bb6a0, /// 0x20c
			 0x715e3e99, /// 0x210
			 0x9fc6da2d, /// 0x214
			 0x89767be7, /// 0x218
			 0x4e74fdf9, /// 0x21c
			 0xec15c58a, /// 0x220
			 0x43c24907, /// 0x224
			 0xb58883a7, /// 0x228
			 0x14d62634, /// 0x22c
			 0x8ae67f0c, /// 0x230
			 0xc9b62d42, /// 0x234
			 0x32349b82, /// 0x238
			 0x6858303e, /// 0x23c
			 0x5d0e7f14, /// 0x240
			 0xe489b013, /// 0x244
			 0xbbd87c26, /// 0x248
			 0xf2a4f458, /// 0x24c
			 0xd9e76918, /// 0x250
			 0xc292efe8, /// 0x254
			 0x545f408a, /// 0x258
			 0xc3bd9eab, /// 0x25c
			 0xdf9d7225, /// 0x260
			 0xe1f802f7, /// 0x264
			 0x4b5b58b9, /// 0x268
			 0x67536e87, /// 0x26c
			 0xb47a1822, /// 0x270
			 0xaff79be1, /// 0x274
			 0xaf22c12e, /// 0x278
			 0xd5a74149, /// 0x27c
			 0x336264d0, /// 0x280
			 0xfc2fb76f, /// 0x284
			 0x348a0d53, /// 0x288
			 0x1a376656, /// 0x28c
			 0xa4c61d6e, /// 0x290
			 0x56cda533, /// 0x294
			 0x0afc4d72, /// 0x298
			 0xe7ef143a, /// 0x29c
			 0xa6eb6c93, /// 0x2a0
			 0x314eb194, /// 0x2a4
			 0x05cccf6a, /// 0x2a8
			 0x8659a603, /// 0x2ac
			 0x7cb85e6b, /// 0x2b0
			 0x36398893, /// 0x2b4
			 0xb438843b, /// 0x2b8
			 0x0cfa56c9, /// 0x2bc
			 0x41e47e2d, /// 0x2c0
			 0xd7e29012, /// 0x2c4
			 0x019fa204, /// 0x2c8
			 0x2991389f, /// 0x2cc
			 0x5a13a297, /// 0x2d0
			 0x4a479630, /// 0x2d4
			 0xa12ccc94, /// 0x2d8
			 0x58b4c887, /// 0x2dc
			 0x29e2165b, /// 0x2e0
			 0xbc912844, /// 0x2e4
			 0xe8ffb1f7, /// 0x2e8
			 0x1be7906e, /// 0x2ec
			 0x00a7304e, /// 0x2f0
			 0xd93366a8, /// 0x2f4
			 0x6b357866, /// 0x2f8
			 0xb846379c, /// 0x2fc
			 0xcaf42fa1, /// 0x300
			 0xa842cafb, /// 0x304
			 0xcb0b260b, /// 0x308
			 0x0e8b3822, /// 0x30c
			 0x319b864a, /// 0x310
			 0xf2ae88c4, /// 0x314
			 0x81ab2771, /// 0x318
			 0xc3398eaa, /// 0x31c
			 0x59b74f40, /// 0x320
			 0x760a577e, /// 0x324
			 0x5cd54afe, /// 0x328
			 0xfb19a6c3, /// 0x32c
			 0x3dffca22, /// 0x330
			 0x011acd5d, /// 0x334
			 0x863363f3, /// 0x338
			 0x7a3af4ab, /// 0x33c
			 0x862a7b28, /// 0x340
			 0x8cf27ce5, /// 0x344
			 0x7caf585c, /// 0x348
			 0x8d3a74bf, /// 0x34c
			 0x1a7c5788, /// 0x350
			 0xf3d97250, /// 0x354
			 0x82d486d7, /// 0x358
			 0x0220c86d, /// 0x35c
			 0xe20776f0, /// 0x360
			 0x5075b4e4, /// 0x364
			 0x1adf8867, /// 0x368
			 0xa8c2f071, /// 0x36c
			 0xa6092adf, /// 0x370
			 0x51672e4e, /// 0x374
			 0xab7e5d96, /// 0x378
			 0x61f04f58, /// 0x37c
			 0xb6ebb425, /// 0x380
			 0xb1309014, /// 0x384
			 0x7d93bbde, /// 0x388
			 0x56197a51, /// 0x38c
			 0x798eaa84, /// 0x390
			 0xe1668533, /// 0x394
			 0x2ed3804c, /// 0x398
			 0x574fc657, /// 0x39c
			 0x8e53c1b2, /// 0x3a0
			 0x753cd707, /// 0x3a4
			 0x066d8192, /// 0x3a8
			 0x24a25c32, /// 0x3ac
			 0xa846af6f, /// 0x3b0
			 0x2fd44394, /// 0x3b4
			 0x8d348e92, /// 0x3b8
			 0x3e50779d, /// 0x3bc
			 0x70ba5c70, /// 0x3c0
			 0xa2356b25, /// 0x3c4
			 0x74919668, /// 0x3c8
			 0x7f260b66, /// 0x3cc
			 0xe4447f9a, /// 0x3d0
			 0x5bebe6b8, /// 0x3d4
			 0x69dd0ae1, /// 0x3d8
			 0xdc1f879e, /// 0x3dc
			 0x5677865c, /// 0x3e0
			 0x02480bcf, /// 0x3e4
			 0xc36a0b64, /// 0x3e8
			 0xb65680fb, /// 0x3ec
			 0x6f767626, /// 0x3f0
			 0x6a72df29, /// 0x3f4
			 0x73d5c5ea, /// 0x3f8
			 0xde5804b5, /// 0x3fc
			 0x45036444, /// 0x400
			 0x84bb38b3, /// 0x404
			 0xf217b8d7, /// 0x408
			 0x1d143927, /// 0x40c
			 0x103cb426, /// 0x410
			 0x1fc25420, /// 0x414
			 0xb5383039, /// 0x418
			 0x42dbb679, /// 0x41c
			 0xc5a2a510, /// 0x420
			 0xa8e71c4d, /// 0x424
			 0x32994c9d, /// 0x428
			 0xb6191bcd, /// 0x42c
			 0xba488d64, /// 0x430
			 0x92206532, /// 0x434
			 0x4d8d43fe, /// 0x438
			 0x4c8f0dac, /// 0x43c
			 0x292b2a05, /// 0x440
			 0xa4f5a60b, /// 0x444
			 0x63014b1a, /// 0x448
			 0xffc461e2, /// 0x44c
			 0x789fbc61, /// 0x450
			 0x255e9f0b, /// 0x454
			 0xd4083a16, /// 0x458
			 0xa0366d74, /// 0x45c
			 0xfd5efb5e, /// 0x460
			 0xa5040a10, /// 0x464
			 0xb6d09cb5, /// 0x468
			 0x27a21185, /// 0x46c
			 0x89825cc5, /// 0x470
			 0xd3bbe3e1, /// 0x474
			 0x0d6dedf0, /// 0x478
			 0xfe9760a7, /// 0x47c
			 0x649cd0db, /// 0x480
			 0x0b273be8, /// 0x484
			 0x7563b68e, /// 0x488
			 0xdc1684e9, /// 0x48c
			 0x54886ecd, /// 0x490
			 0x6b54b078, /// 0x494
			 0x0f4637a6, /// 0x498
			 0x27544d7d, /// 0x49c
			 0xf16d448c, /// 0x4a0
			 0xb341c9b3, /// 0x4a4
			 0x91ceb73e, /// 0x4a8
			 0xbb610d63, /// 0x4ac
			 0x1b22ba9d, /// 0x4b0
			 0x6704aa4c, /// 0x4b4
			 0x2f8f4d44, /// 0x4b8
			 0xd3438526, /// 0x4bc
			 0xbb31144a, /// 0x4c0
			 0x2f4144aa, /// 0x4c4
			 0xc4ac86b7, /// 0x4c8
			 0x9a24f37f, /// 0x4cc
			 0x99c0f60e, /// 0x4d0
			 0x91a12275, /// 0x4d4
			 0x1a12bc9f, /// 0x4d8
			 0xc168b446, /// 0x4dc
			 0x5b4ae908, /// 0x4e0
			 0x202adea2, /// 0x4e4
			 0x5459103a, /// 0x4e8
			 0x5a605823, /// 0x4ec
			 0x978117b3, /// 0x4f0
			 0x1f522f4b, /// 0x4f4
			 0x4e55f23c, /// 0x4f8
			 0xc81d9972, /// 0x4fc
			 0xe0aa9244, /// 0x500
			 0x0cd14ac1, /// 0x504
			 0x6581d2d9, /// 0x508
			 0x7f3f81ff, /// 0x50c
			 0x22f9a870, /// 0x510
			 0xd1280e55, /// 0x514
			 0x4e1f3757, /// 0x518
			 0x1b643020, /// 0x51c
			 0xd2ce673a, /// 0x520
			 0xd1897e28, /// 0x524
			 0x97ff2a07, /// 0x528
			 0x29a3ac4c, /// 0x52c
			 0x4eda92f1, /// 0x530
			 0xecd4874a, /// 0x534
			 0x15c7135d, /// 0x538
			 0x0a18b393, /// 0x53c
			 0x93699f6d, /// 0x540
			 0x51cc6acc, /// 0x544
			 0x1d3b0835, /// 0x548
			 0xfad462f2, /// 0x54c
			 0xce7ab60f, /// 0x550
			 0x9618d24e, /// 0x554
			 0xe3a5a105, /// 0x558
			 0x6f0b05ac, /// 0x55c
			 0x95279b0d, /// 0x560
			 0xa9d87fa3, /// 0x564
			 0xc6e13f31, /// 0x568
			 0xe369e2c2, /// 0x56c
			 0x4d6d3cee, /// 0x570
			 0x18998668, /// 0x574
			 0xaac1d27a, /// 0x578
			 0x18315c81, /// 0x57c
			 0x2b387c2d, /// 0x580
			 0x8265c9fb, /// 0x584
			 0x5ccbbf5b, /// 0x588
			 0x67276593, /// 0x58c
			 0x9afed7e6, /// 0x590
			 0x29881c13, /// 0x594
			 0xa67091ad, /// 0x598
			 0x7a607517, /// 0x59c
			 0x3062dbee, /// 0x5a0
			 0xda0119d4, /// 0x5a4
			 0xbb9677fe, /// 0x5a8
			 0x119bf3a7, /// 0x5ac
			 0x14e018b6, /// 0x5b0
			 0xc784b10a, /// 0x5b4
			 0x43da3370, /// 0x5b8
			 0xe7a860fa, /// 0x5bc
			 0x2fc4f779, /// 0x5c0
			 0xe568d7af, /// 0x5c4
			 0x3daea5fd, /// 0x5c8
			 0x55a28c24, /// 0x5cc
			 0xbcf63347, /// 0x5d0
			 0x253f7a5d, /// 0x5d4
			 0xc7222c84, /// 0x5d8
			 0x1924b61f, /// 0x5dc
			 0x9faa2c72, /// 0x5e0
			 0xb54f282e, /// 0x5e4
			 0x043a665e, /// 0x5e8
			 0xc72b92d7, /// 0x5ec
			 0x940710cc, /// 0x5f0
			 0x4c2748d3, /// 0x5f4
			 0xf824305e, /// 0x5f8
			 0xc5fbd2b5, /// 0x5fc
			 0x16764d42, /// 0x600
			 0x3c6501c2, /// 0x604
			 0xb44cdb92, /// 0x608
			 0x1c0c43f1, /// 0x60c
			 0x5f9d8d10, /// 0x610
			 0x4a244955, /// 0x614
			 0x6cefdb48, /// 0x618
			 0x445bd2a8, /// 0x61c
			 0xaec07da3, /// 0x620
			 0x242db85a, /// 0x624
			 0x3eaddddc, /// 0x628
			 0x9a251ad1, /// 0x62c
			 0x402d0e04, /// 0x630
			 0x012a7e92, /// 0x634
			 0x9ce32ad0, /// 0x638
			 0xcb177d49, /// 0x63c
			 0x95aff32c, /// 0x640
			 0x25e3b344, /// 0x644
			 0xd06117e5, /// 0x648
			 0x09fce80a, /// 0x64c
			 0xbff2aaab, /// 0x650
			 0xb9fbca4a, /// 0x654
			 0xb82291fc, /// 0x658
			 0x2c226377, /// 0x65c
			 0xfec4ade0, /// 0x660
			 0x1ced073e, /// 0x664
			 0x116d2e24, /// 0x668
			 0x809cf81c, /// 0x66c
			 0xcec12b0c, /// 0x670
			 0xafd57be3, /// 0x674
			 0x3bbeb833, /// 0x678
			 0xf51a40db, /// 0x67c
			 0xda6836fa, /// 0x680
			 0xc850421a, /// 0x684
			 0x855f2176, /// 0x688
			 0xfec5b618, /// 0x68c
			 0x7192b25e, /// 0x690
			 0xe7c098b8, /// 0x694
			 0x096651e3, /// 0x698
			 0xab9ffc66, /// 0x69c
			 0x146bfb66, /// 0x6a0
			 0x1921ebc8, /// 0x6a4
			 0x65534d41, /// 0x6a8
			 0x35f0ab3f, /// 0x6ac
			 0x2c92ec7c, /// 0x6b0
			 0x7ea89a60, /// 0x6b4
			 0x055b6ce5, /// 0x6b8
			 0xde87a0d8, /// 0x6bc
			 0x8031ab20, /// 0x6c0
			 0x8bc6a982, /// 0x6c4
			 0xc51f0c4a, /// 0x6c8
			 0xeee95873, /// 0x6cc
			 0x1af0de2b, /// 0x6d0
			 0x8c3e497c, /// 0x6d4
			 0xccfc1aea, /// 0x6d8
			 0xf7909fa6, /// 0x6dc
			 0xf713c827, /// 0x6e0
			 0x2d2879d2, /// 0x6e4
			 0x435da3b5, /// 0x6e8
			 0xd819c65c, /// 0x6ec
			 0x37c3b0ce, /// 0x6f0
			 0xd2169367, /// 0x6f4
			 0x895da136, /// 0x6f8
			 0x1dcdc6c6, /// 0x6fc
			 0xc2cb64b5, /// 0x700
			 0x484f88fe, /// 0x704
			 0xa7810813, /// 0x708
			 0xea950334, /// 0x70c
			 0x15b15586, /// 0x710
			 0x592f2512, /// 0x714
			 0x20795e5d, /// 0x718
			 0x6df7f716, /// 0x71c
			 0x070a8115, /// 0x720
			 0xc706d660, /// 0x724
			 0xdd7e27ea, /// 0x728
			 0x0fae0d84, /// 0x72c
			 0x6b90cabf, /// 0x730
			 0x0e93fc72, /// 0x734
			 0x8d50af64, /// 0x738
			 0x8355060d, /// 0x73c
			 0x228a73ef, /// 0x740
			 0xf8cd2617, /// 0x744
			 0xb1214594, /// 0x748
			 0x3baf295a, /// 0x74c
			 0xe1398a25, /// 0x750
			 0x690aa123, /// 0x754
			 0xb7054413, /// 0x758
			 0x5c1cc4d8, /// 0x75c
			 0x203880c0, /// 0x760
			 0xdcf27c7d, /// 0x764
			 0xe94577f2, /// 0x768
			 0x148d65bf, /// 0x76c
			 0xd4814a3b, /// 0x770
			 0x5dc2dec7, /// 0x774
			 0x65ae669f, /// 0x778
			 0xb5883256, /// 0x77c
			 0xf907e308, /// 0x780
			 0x89d713fa, /// 0x784
			 0xe320c5cf, /// 0x788
			 0x4448327d, /// 0x78c
			 0x71584730, /// 0x790
			 0x1ceadd5d, /// 0x794
			 0xe9745585, /// 0x798
			 0x8b3f1263, /// 0x79c
			 0x3b4bbc07, /// 0x7a0
			 0x9954be98, /// 0x7a4
			 0xcc766906, /// 0x7a8
			 0x54c94fa7, /// 0x7ac
			 0x79b9ffa5, /// 0x7b0
			 0x112ddc71, /// 0x7b4
			 0xbf6b3dbb, /// 0x7b8
			 0x85a628d9, /// 0x7bc
			 0xb98fb202, /// 0x7c0
			 0xf8a6e3c0, /// 0x7c4
			 0x476d7d66, /// 0x7c8
			 0x92ece609, /// 0x7cc
			 0x9e2cbfc0, /// 0x7d0
			 0x72b1337c, /// 0x7d4
			 0x3ed8d697, /// 0x7d8
			 0xe2f5b2a7, /// 0x7dc
			 0xe7a6a16d, /// 0x7e0
			 0xdc3c6a7d, /// 0x7e4
			 0xa4b8d1cd, /// 0x7e8
			 0xe3eb3a65, /// 0x7ec
			 0xc09b47c8, /// 0x7f0
			 0x58478cf7, /// 0x7f4
			 0x849ef0b1, /// 0x7f8
			 0xff6da49c, /// 0x7fc
			 0x1189e367, /// 0x800
			 0xaafded4c, /// 0x804
			 0xa0e11956, /// 0x808
			 0x7892aba3, /// 0x80c
			 0x6ab35baa, /// 0x810
			 0x18d5c1c2, /// 0x814
			 0x5042843c, /// 0x818
			 0x1961d637, /// 0x81c
			 0xd90d9831, /// 0x820
			 0xf32edfeb, /// 0x824
			 0xcd8e0a30, /// 0x828
			 0x87bbd7c9, /// 0x82c
			 0xe0636f8a, /// 0x830
			 0x97ed1b5d, /// 0x834
			 0x0b83d89f, /// 0x838
			 0xd47295a8, /// 0x83c
			 0x256b9e8d, /// 0x840
			 0xe50d0506, /// 0x844
			 0x77faa77a, /// 0x848
			 0xeb2a66a3, /// 0x84c
			 0xa4c9b407, /// 0x850
			 0x9a221278, /// 0x854
			 0x05c41b7a, /// 0x858
			 0xe8b797bd, /// 0x85c
			 0xab0c7907, /// 0x860
			 0x958d442a, /// 0x864
			 0x797a0493, /// 0x868
			 0x4088376e, /// 0x86c
			 0x6db8bc8a, /// 0x870
			 0x78c14261, /// 0x874
			 0x6701953c, /// 0x878
			 0x90d4ad3e, /// 0x87c
			 0x20c44e11, /// 0x880
			 0x7a82da7c, /// 0x884
			 0xd28aa8c8, /// 0x888
			 0x00641e30, /// 0x88c
			 0xc5a62383, /// 0x890
			 0xb012ba97, /// 0x894
			 0xcdc049b6, /// 0x898
			 0xfe1219dd, /// 0x89c
			 0xc3ff503a, /// 0x8a0
			 0x516619bf, /// 0x8a4
			 0x8618bb15, /// 0x8a8
			 0xcca48e14, /// 0x8ac
			 0x3d461899, /// 0x8b0
			 0xbd10e3eb, /// 0x8b4
			 0xf2d46449, /// 0x8b8
			 0x3cb13c73, /// 0x8bc
			 0x1e5ba7a7, /// 0x8c0
			 0x7814152a, /// 0x8c4
			 0xcfe29608, /// 0x8c8
			 0x6f73ed98, /// 0x8cc
			 0x7980866e, /// 0x8d0
			 0x98ae02ef, /// 0x8d4
			 0x4875df38, /// 0x8d8
			 0x694ffbfd, /// 0x8dc
			 0x63f6ddbf, /// 0x8e0
			 0xf6fd7a09, /// 0x8e4
			 0xd8eb8121, /// 0x8e8
			 0x45a08332, /// 0x8ec
			 0x9806941a, /// 0x8f0
			 0x31539565, /// 0x8f4
			 0xd549195e, /// 0x8f8
			 0xfc6a4e82, /// 0x8fc
			 0x98063833, /// 0x900
			 0x5f0ea566, /// 0x904
			 0x77171477, /// 0x908
			 0x08a3b45f, /// 0x90c
			 0x0748dc35, /// 0x910
			 0x0fe1fe40, /// 0x914
			 0xe57f9e7a, /// 0x918
			 0x4830f94a, /// 0x91c
			 0x6772b56e, /// 0x920
			 0x657fa9ec, /// 0x924
			 0xbf5c7f7b, /// 0x928
			 0xd0191361, /// 0x92c
			 0x9df28444, /// 0x930
			 0x61fa3ebb, /// 0x934
			 0xecb4cc5a, /// 0x938
			 0xefc41e57, /// 0x93c
			 0xc8fbb3f7, /// 0x940
			 0xaee75a26, /// 0x944
			 0xb0196b1d, /// 0x948
			 0x32da2174, /// 0x94c
			 0xfb63aeb4, /// 0x950
			 0x61faf904, /// 0x954
			 0x191eb372, /// 0x958
			 0x1903c843, /// 0x95c
			 0xecbb34c8, /// 0x960
			 0x33577a43, /// 0x964
			 0x4d2b97a3, /// 0x968
			 0xf16c968c, /// 0x96c
			 0xa6eb1daf, /// 0x970
			 0x47b76300, /// 0x974
			 0xe282194c, /// 0x978
			 0x0356a38b, /// 0x97c
			 0x19430fbd, /// 0x980
			 0x356b1945, /// 0x984
			 0xdb7a93d3, /// 0x988
			 0x923dfba5, /// 0x98c
			 0x179bb6a1, /// 0x990
			 0xd937134a, /// 0x994
			 0x7ff5c00f, /// 0x998
			 0xb02884f5, /// 0x99c
			 0x774700e0, /// 0x9a0
			 0x27ade5f0, /// 0x9a4
			 0x44dd3904, /// 0x9a8
			 0xe9e7235b, /// 0x9ac
			 0x70aba3ae, /// 0x9b0
			 0x9fa66172, /// 0x9b4
			 0x1af0b201, /// 0x9b8
			 0x5327d7a5, /// 0x9bc
			 0x5bc9f166, /// 0x9c0
			 0xcc3f7496, /// 0x9c4
			 0x6a60f483, /// 0x9c8
			 0x3e73507d, /// 0x9cc
			 0x3d101ae6, /// 0x9d0
			 0xd4507129, /// 0x9d4
			 0xf31ef0f7, /// 0x9d8
			 0xa9231503, /// 0x9dc
			 0x61195d99, /// 0x9e0
			 0x293e7a70, /// 0x9e4
			 0x8aa857d1, /// 0x9e8
			 0x0875c877, /// 0x9ec
			 0x549be65b, /// 0x9f0
			 0x2b4c5213, /// 0x9f4
			 0x0eda1723, /// 0x9f8
			 0xf9b4bf8e, /// 0x9fc
			 0x15a4278f, /// 0xa00
			 0x87bf75ab, /// 0xa04
			 0xf8cef5d6, /// 0xa08
			 0x662cc39a, /// 0xa0c
			 0x7b5eb072, /// 0xa10
			 0x3ae39ffa, /// 0xa14
			 0xfe6a618c, /// 0xa18
			 0x46b69057, /// 0xa1c
			 0xc23ed2ba, /// 0xa20
			 0xe5d03f0a, /// 0xa24
			 0x6d2890fb, /// 0xa28
			 0xdf1f5810, /// 0xa2c
			 0x6a6ef20b, /// 0xa30
			 0x925117ba, /// 0xa34
			 0xf2cbeddc, /// 0xa38
			 0xb383ad6a, /// 0xa3c
			 0x86f58bc8, /// 0xa40
			 0x2380ec26, /// 0xa44
			 0xd5941dc0, /// 0xa48
			 0xd9d33101, /// 0xa4c
			 0x9a21eb5c, /// 0xa50
			 0x0e7293d1, /// 0xa54
			 0x571388a0, /// 0xa58
			 0x8e18633b, /// 0xa5c
			 0xdb5e61d4, /// 0xa60
			 0xf49e19b7, /// 0xa64
			 0x9b63cc05, /// 0xa68
			 0x2ad8c28d, /// 0xa6c
			 0x26d408e2, /// 0xa70
			 0xb95d7fe5, /// 0xa74
			 0x17851788, /// 0xa78
			 0x5f4c7409, /// 0xa7c
			 0x34ae18d0, /// 0xa80
			 0xe5aac0a1, /// 0xa84
			 0x0d48a820, /// 0xa88
			 0x657c41e5, /// 0xa8c
			 0x56c7849c, /// 0xa90
			 0x997a9373, /// 0xa94
			 0xb4cb9d74, /// 0xa98
			 0x6dfe4274, /// 0xa9c
			 0xbbef3e87, /// 0xaa0
			 0x5a6d10d3, /// 0xaa4
			 0x858e8c86, /// 0xaa8
			 0xc60522a1, /// 0xaac
			 0x6c4b4388, /// 0xab0
			 0x2b3ffd7d, /// 0xab4
			 0x737d05cf, /// 0xab8
			 0x9522b43c, /// 0xabc
			 0x5106bb7c, /// 0xac0
			 0x00d31b0c, /// 0xac4
			 0x669ca90b, /// 0xac8
			 0x20351c8b, /// 0xacc
			 0x64bba3c2, /// 0xad0
			 0x3df81d74, /// 0xad4
			 0xeaf330f7, /// 0xad8
			 0xcbeae17c, /// 0xadc
			 0xd7e61cc5, /// 0xae0
			 0x619cd5cc, /// 0xae4
			 0x605f2a7b, /// 0xae8
			 0x11137f29, /// 0xaec
			 0x5bf684a9, /// 0xaf0
			 0x9a3b147c, /// 0xaf4
			 0xa7cfb090, /// 0xaf8
			 0xafc8ddb8, /// 0xafc
			 0x5733ffb7, /// 0xb00
			 0xc542adc9, /// 0xb04
			 0x35f66578, /// 0xb08
			 0x778f006b, /// 0xb0c
			 0xfa8bc7cb, /// 0xb10
			 0x23f234eb, /// 0xb14
			 0x18502c83, /// 0xb18
			 0x63d9697b, /// 0xb1c
			 0xd0dba63d, /// 0xb20
			 0x011a5ac2, /// 0xb24
			 0xab8c0177, /// 0xb28
			 0xfbfdfe7e, /// 0xb2c
			 0x7cd85ffb, /// 0xb30
			 0xcfe84494, /// 0xb34
			 0x30f0e801, /// 0xb38
			 0x7c16f131, /// 0xb3c
			 0x250001df, /// 0xb40
			 0x62fd5bfa, /// 0xb44
			 0xc88b621c, /// 0xb48
			 0x2b7378ec, /// 0xb4c
			 0xa58065b3, /// 0xb50
			 0x4bf5b3b4, /// 0xb54
			 0x88f65d18, /// 0xb58
			 0xa5abb320, /// 0xb5c
			 0xa3d35d1b, /// 0xb60
			 0x0ba564be, /// 0xb64
			 0x06e408f1, /// 0xb68
			 0xf082d1d2, /// 0xb6c
			 0x397726f6, /// 0xb70
			 0xb38f34c1, /// 0xb74
			 0x1a4ab1f5, /// 0xb78
			 0x7cd8fe6b, /// 0xb7c
			 0xd080444d, /// 0xb80
			 0x0951c9ac, /// 0xb84
			 0xa189ba65, /// 0xb88
			 0x0326a463, /// 0xb8c
			 0xec098f0a, /// 0xb90
			 0x73d6a295, /// 0xb94
			 0xe768c591, /// 0xb98
			 0xde2431bc, /// 0xb9c
			 0x6d166e6b, /// 0xba0
			 0x70032b18, /// 0xba4
			 0xca8da98b, /// 0xba8
			 0xa575d3d2, /// 0xbac
			 0x3a677f7d, /// 0xbb0
			 0x00f986a5, /// 0xbb4
			 0x7c5e2e5e, /// 0xbb8
			 0xdb5a580d, /// 0xbbc
			 0xf2739589, /// 0xbc0
			 0x4547eab1, /// 0xbc4
			 0x6f752c15, /// 0xbc8
			 0x39a03819, /// 0xbcc
			 0x55a78a64, /// 0xbd0
			 0x2ea9ad2b, /// 0xbd4
			 0x00942127, /// 0xbd8
			 0xd9b31027, /// 0xbdc
			 0xaa63c7f4, /// 0xbe0
			 0x05340b61, /// 0xbe4
			 0x22df3ad1, /// 0xbe8
			 0xd560bda4, /// 0xbec
			 0x83c2f59f, /// 0xbf0
			 0x021c66dd, /// 0xbf4
			 0x193a6b9c, /// 0xbf8
			 0xaef06a87, /// 0xbfc
			 0x362f8a87, /// 0xc00
			 0x73f7894f, /// 0xc04
			 0x9513fb34, /// 0xc08
			 0x85a85e98, /// 0xc0c
			 0x103a3c06, /// 0xc10
			 0x55c68243, /// 0xc14
			 0xae555b23, /// 0xc18
			 0x2c27a8bb, /// 0xc1c
			 0xdde79335, /// 0xc20
			 0xd32802a4, /// 0xc24
			 0x9ed5494a, /// 0xc28
			 0x28d51baa, /// 0xc2c
			 0x88dc015b, /// 0xc30
			 0x62278f15, /// 0xc34
			 0x32841bbb, /// 0xc38
			 0x432d7521, /// 0xc3c
			 0x8a124ae2, /// 0xc40
			 0x07d3a47d, /// 0xc44
			 0xaa3ccf49, /// 0xc48
			 0x492ec245, /// 0xc4c
			 0x3f78648a, /// 0xc50
			 0xf949fcc4, /// 0xc54
			 0x8855a807, /// 0xc58
			 0x5d8a4636, /// 0xc5c
			 0xb4216efb, /// 0xc60
			 0xc9eb8259, /// 0xc64
			 0x3bee13c2, /// 0xc68
			 0x8e96e167, /// 0xc6c
			 0x6e0044a2, /// 0xc70
			 0xc9d4230e, /// 0xc74
			 0x2a66a5cd, /// 0xc78
			 0xbb8d9edf, /// 0xc7c
			 0x62eb9aa0, /// 0xc80
			 0x62045ae5, /// 0xc84
			 0x32c859ab, /// 0xc88
			 0x9e6ae984, /// 0xc8c
			 0xd14adb31, /// 0xc90
			 0xcf401b15, /// 0xc94
			 0x2ba7aeba, /// 0xc98
			 0x949ba391, /// 0xc9c
			 0x7c8f335f, /// 0xca0
			 0xecb410da, /// 0xca4
			 0x90ac1723, /// 0xca8
			 0xf98b4b40, /// 0xcac
			 0x9ab757b4, /// 0xcb0
			 0x9791a62d, /// 0xcb4
			 0xb3d73bf1, /// 0xcb8
			 0xd1f4c3f6, /// 0xcbc
			 0x8c84e4c9, /// 0xcc0
			 0x25c5c925, /// 0xcc4
			 0x2c18714e, /// 0xcc8
			 0x2995cdc4, /// 0xccc
			 0x78a36556, /// 0xcd0
			 0xa4ec5adc, /// 0xcd4
			 0x014d6b70, /// 0xcd8
			 0x5a80d716, /// 0xcdc
			 0x3e491ffa, /// 0xce0
			 0x2144068d, /// 0xce4
			 0x81ee8be7, /// 0xce8
			 0x08c4524b, /// 0xcec
			 0x349a7ee9, /// 0xcf0
			 0xf314486d, /// 0xcf4
			 0xf01b8112, /// 0xcf8
			 0x4cf2d2a4, /// 0xcfc
			 0x30d51626, /// 0xd00
			 0x5ac00c0c, /// 0xd04
			 0xdb1c8238, /// 0xd08
			 0x7eb0d089, /// 0xd0c
			 0x0dc686f8, /// 0xd10
			 0xe45b7008, /// 0xd14
			 0x0068ee85, /// 0xd18
			 0x291714ba, /// 0xd1c
			 0xb380e366, /// 0xd20
			 0xb7e5bec2, /// 0xd24
			 0xb7f72d60, /// 0xd28
			 0x6a668376, /// 0xd2c
			 0xc09e25fd, /// 0xd30
			 0x5abf64ee, /// 0xd34
			 0x3cd94110, /// 0xd38
			 0x7289bf7c, /// 0xd3c
			 0x4451b535, /// 0xd40
			 0x65102c7e, /// 0xd44
			 0x07e7f028, /// 0xd48
			 0xef56aad5, /// 0xd4c
			 0x807c4f05, /// 0xd50
			 0x31329471, /// 0xd54
			 0x3c4d4f7f, /// 0xd58
			 0xccb9addf, /// 0xd5c
			 0x2cd21d5c, /// 0xd60
			 0x221f40ce, /// 0xd64
			 0xfa2f58ff, /// 0xd68
			 0x758b4934, /// 0xd6c
			 0x848b8c61, /// 0xd70
			 0xfdbe1120, /// 0xd74
			 0xa988af45, /// 0xd78
			 0x1bf3cdf4, /// 0xd7c
			 0x08eb615b, /// 0xd80
			 0x1cefe8fe, /// 0xd84
			 0xbcbf6c00, /// 0xd88
			 0xb2fce607, /// 0xd8c
			 0x9dbc9a56, /// 0xd90
			 0x5fc3918f, /// 0xd94
			 0x15e19a5f, /// 0xd98
			 0xd168aabe, /// 0xd9c
			 0x201674b5, /// 0xda0
			 0xb898d756, /// 0xda4
			 0x7b1b520d, /// 0xda8
			 0x8f0c94a3, /// 0xdac
			 0xa2260858, /// 0xdb0
			 0xae618860, /// 0xdb4
			 0xd93fd202, /// 0xdb8
			 0x6c9f4395, /// 0xdbc
			 0xd6495e8a, /// 0xdc0
			 0xb9d58927, /// 0xdc4
			 0xe512c1c4, /// 0xdc8
			 0x5d458e99, /// 0xdcc
			 0xa2b18985, /// 0xdd0
			 0x55b35a03, /// 0xdd4
			 0x64615baf, /// 0xdd8
			 0xba3c1afc, /// 0xddc
			 0x9841983b, /// 0xde0
			 0xbf97079f, /// 0xde4
			 0x21c5397e, /// 0xde8
			 0xf933803a, /// 0xdec
			 0x065efb8a, /// 0xdf0
			 0xb14327f1, /// 0xdf4
			 0x613daa68, /// 0xdf8
			 0x16dbd851, /// 0xdfc
			 0x92fa82b6, /// 0xe00
			 0x87a7863a, /// 0xe04
			 0xcd5009da, /// 0xe08
			 0x75c9dcc9, /// 0xe0c
			 0xd4dfdadb, /// 0xe10
			 0xbb533fce, /// 0xe14
			 0x588ac20e, /// 0xe18
			 0x8e57f221, /// 0xe1c
			 0xe5e0428f, /// 0xe20
			 0xeb034d60, /// 0xe24
			 0xf6a4c913, /// 0xe28
			 0x7f7c188c, /// 0xe2c
			 0xe7c88392, /// 0xe30
			 0xa7a0cebc, /// 0xe34
			 0xc4998aca, /// 0xe38
			 0xc6a175d1, /// 0xe3c
			 0x993c043a, /// 0xe40
			 0x9eec75c8, /// 0xe44
			 0x62783646, /// 0xe48
			 0xe3330618, /// 0xe4c
			 0x3b75299d, /// 0xe50
			 0x58009c58, /// 0xe54
			 0xaf223d2f, /// 0xe58
			 0x7a6b5f16, /// 0xe5c
			 0x77409814, /// 0xe60
			 0x85415fd7, /// 0xe64
			 0x4b3a27d6, /// 0xe68
			 0x1f580963, /// 0xe6c
			 0x2ed5dc9b, /// 0xe70
			 0x384bab6e, /// 0xe74
			 0x7bf3d228, /// 0xe78
			 0x328989eb, /// 0xe7c
			 0xe49704b6, /// 0xe80
			 0x054358a5, /// 0xe84
			 0x05e95a3e, /// 0xe88
			 0xa9dc1074, /// 0xe8c
			 0x5959ace6, /// 0xe90
			 0x14069ca9, /// 0xe94
			 0x0d7e129b, /// 0xe98
			 0x26a14939, /// 0xe9c
			 0x12c3bd06, /// 0xea0
			 0x3af1ac4f, /// 0xea4
			 0x7097fdcd, /// 0xea8
			 0x3e99c536, /// 0xeac
			 0x018df9cf, /// 0xeb0
			 0xd714980e, /// 0xeb4
			 0xf61d1248, /// 0xeb8
			 0x02c7c1ed, /// 0xebc
			 0x885c6588, /// 0xec0
			 0xb549e766, /// 0xec4
			 0xade65a82, /// 0xec8
			 0xdb3eb062, /// 0xecc
			 0x9edcb010, /// 0xed0
			 0x68b8d83d, /// 0xed4
			 0x1be3e2c7, /// 0xed8
			 0xa958a661, /// 0xedc
			 0x7abeb410, /// 0xee0
			 0x765ec92c, /// 0xee4
			 0xbf61b96a, /// 0xee8
			 0x080c346c, /// 0xeec
			 0x4d324198, /// 0xef0
			 0x47f01f38, /// 0xef4
			 0xbb3206e6, /// 0xef8
			 0xb9fb113c, /// 0xefc
			 0xcddd4afc, /// 0xf00
			 0xb1d5657d, /// 0xf04
			 0xcc741b21, /// 0xf08
			 0xa80b49fe, /// 0xf0c
			 0x379b5b28, /// 0xf10
			 0x3bd43c6e, /// 0xf14
			 0xe4d8268e, /// 0xf18
			 0xa2ad97c8, /// 0xf1c
			 0x5063a6b0, /// 0xf20
			 0x60278665, /// 0xf24
			 0x2281cfc6, /// 0xf28
			 0x9b0cc53c, /// 0xf2c
			 0xe38ba5e8, /// 0xf30
			 0x8d490898, /// 0xf34
			 0x4f9987b8, /// 0xf38
			 0x351f8f52, /// 0xf3c
			 0xaf5ac163, /// 0xf40
			 0x10f8ebe6, /// 0xf44
			 0xbae2a045, /// 0xf48
			 0x5148052a, /// 0xf4c
			 0x3f27e4e4, /// 0xf50
			 0xf89f83df, /// 0xf54
			 0x0d527be5, /// 0xf58
			 0x19f5e3e1, /// 0xf5c
			 0xd5fa65e7, /// 0xf60
			 0x67632328, /// 0xf64
			 0x0ea31911, /// 0xf68
			 0xe59df22d, /// 0xf6c
			 0xf8de3143, /// 0xf70
			 0x308c892b, /// 0xf74
			 0x51ca2c54, /// 0xf78
			 0xc9312e1e, /// 0xf7c
			 0x6c3909c3, /// 0xf80
			 0x4d9c593d, /// 0xf84
			 0x1142cb56, /// 0xf88
			 0xdded793c, /// 0xf8c
			 0x5187bd44, /// 0xf90
			 0xf11a9189, /// 0xf94
			 0xde501262, /// 0xf98
			 0x6a0c5950, /// 0xf9c
			 0x110fd027, /// 0xfa0
			 0xf89d4cf3, /// 0xfa4
			 0xf1eb3e20, /// 0xfa8
			 0x4da4f3c5, /// 0xfac
			 0x8f3e340c, /// 0xfb0
			 0x3ed20767, /// 0xfb4
			 0xe893e83b, /// 0xfb8
			 0xfe971ba3, /// 0xfbc
			 0x23e01989, /// 0xfc0
			 0xba92d89c, /// 0xfc4
			 0x20f08bbe, /// 0xfc8
			 0xf9385bbc, /// 0xfcc
			 0xdce05178, /// 0xfd0
			 0xed3c031b, /// 0xfd4
			 0x1c9352bb, /// 0xfd8
			 0x2decf056, /// 0xfdc
			 0x480f82ba, /// 0xfe0
			 0x53cb16f1, /// 0xfe4
			 0xa1e6700a, /// 0xfe8
			 0x438622cc, /// 0xfec
			 0x3cafc038, /// 0xff0
			 0x529b2849, /// 0xff4
			 0x478d2460, /// 0xff8
			 0x7ec224aa /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00000
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00008
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0000c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00010
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00014
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0001c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00020
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00024
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00028
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0002c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00030
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00038
			 0x7fc00001,                                                  // signaling NaN                               /// 0003c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00040
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00044
			 0x0e000003,                                                  // Trailing 1s:                                /// 00048
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0004c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00054
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00058
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0005c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00060
			 0x00000000,                                                  // zero                                        /// 00064
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00068
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00070
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00074
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00078
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0007c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00080
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00084
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00088
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0008c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00090
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00098
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0009c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000a0
			 0xbf028f5c,                                                  // -0.51                                       /// 000a4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000a8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000ac
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0x0c600000,                                                  // Leading 1s:                                 /// 000b8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000bc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 000c0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000cc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 000d0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000d4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000d8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000dc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000e0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000e4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000e8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 000ec
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 000f4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000f8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000fc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00100
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00104
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00110
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00114
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00118
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0011c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00120
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00124
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00128
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0012c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00130
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00134
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00138
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0013c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00144
			 0x7f800000,                                                  // inf                                         /// 00148
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0014c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00158
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0015c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00160
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00164
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00168
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00170
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00174
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00178
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0017c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00180
			 0x0c600000,                                                  // Leading 1s:                                 /// 00184
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0018c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00190
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00194
			 0x7fc00001,                                                  // signaling NaN                               /// 00198
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0019c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001a0
			 0x4b000000,                                                  // 8388608.0                                   /// 001a4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 001a8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001ac
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001b0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001b4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001b8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001bc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001c8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 001cc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001d0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 001d4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001dc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001e0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001e4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001e8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001ec
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001f4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 001fc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00204
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00208
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0020c
			 0xbf028f5c,                                                  // -0.51                                       /// 00210
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00214
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00220
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00224
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00228
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0022c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00230
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00234
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00238
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0023c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00240
			 0x33333333,                                                  // 4 random values                             /// 00244
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00248
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0024c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00250
			 0xcb000000,                                                  // -8388608.0                                  /// 00254
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00258
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00260
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00264
			 0x80000000,                                                  // -zero                                       /// 00268
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0026c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00270
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00274
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00278
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0027c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00280
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00284
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00288
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00298
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0029c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 002a0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 002a4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 002a8
			 0xbf028f5c,                                                  // -0.51                                       /// 002ac
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 002b0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 002b4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002b8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002bc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0x0e000003,                                                  // Trailing 1s:                                /// 002c4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 002cc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002d0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002d8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 002dc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002e0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002e4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002e8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002ec
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002f0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002f8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 002fc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00300
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00304
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00308
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0030c
			 0xffc00001,                                                  // -signaling NaN                              /// 00310
			 0xffc00001,                                                  // -signaling NaN                              /// 00314
			 0x7f800000,                                                  // inf                                         /// 00318
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00320
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00324
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00328
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0032c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00330
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00334
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00338
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0033c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00340
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00344
			 0x4b000000,                                                  // 8388608.0                                   /// 00348
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0x0c780000,                                                  // Leading 1s:                                 /// 00354
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00358
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00360
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00364
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00370
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00374
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00378
			 0x0e000007,                                                  // Trailing 1s:                                /// 0037c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00388
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0038c
			 0x80000000,                                                  // -zero                                       /// 00390
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00394
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00398
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0039c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003a0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 003a8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003ac
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003b0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003b8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003bc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 003c0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003c8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 003cc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 003d0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 003d4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 003d8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003dc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003e0
			 0xffc00001,                                                  // -signaling NaN                              /// 003e4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 003e8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 003ec
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003f0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003f4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 003f8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 003fc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00404
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00408
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0040c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00410
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00414
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00418
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0041c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00420
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00424
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00428
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00430
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00434
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00438
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0043c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00440
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00444
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00448
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0044c
			 0xbf028f5c,                                                  // -0.51                                       /// 00450
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00454
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0045c
			 0xbf028f5c,                                                  // -0.51                                       /// 00460
			 0xff800000,                                                  // -inf                                        /// 00464
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00468
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00470
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00474
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00478
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00480
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00484
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00488
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0048c
			 0xcb000000,                                                  // -8388608.0                                  /// 00490
			 0x00000000,                                                  // zero                                        /// 00494
			 0x0c600000,                                                  // Leading 1s:                                 /// 00498
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0049c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004a0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004a4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004a8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 004ac
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 004b0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004b4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004b8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004c0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 004c4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 004c8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004cc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004d0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 004d4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004d8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004e8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 004f0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004f4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00504
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00508
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0050c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00510
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00514
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00518
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0051c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00520
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00524
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00528
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0052c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00530
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00534
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00538
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0053c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00540
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00548
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0054c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00550
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00554
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00558
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0055c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00560
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00564
			 0xbf028f5c,                                                  // -0.51                                       /// 00568
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0056c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00570
			 0x0c780000,                                                  // Leading 1s:                                 /// 00574
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00578
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0057c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0x55555555,                                                  // 4 random values                             /// 00584
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00588
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0058c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00590
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00594
			 0x0c600000,                                                  // Leading 1s:                                 /// 00598
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0059c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 005a0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 005a8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x0e000001,                                                  // Trailing 1s:                                /// 005b0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0x0e000007,                                                  // Trailing 1s:                                /// 005b8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005bc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 005c4
			 0x00011111,                                                  // 9.7958E-41                                  /// 005c8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005cc
			 0x80000000,                                                  // -zero                                       /// 005d0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005d4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 005d8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005dc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 005e4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005e8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005ec
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005f0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005f4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005f8
			 0x0e000001,                                                  // Trailing 1s:                                /// 005fc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00600
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00604
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00608
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0060c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00610
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00614
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00620
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00624
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00628
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00630
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00634
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00638
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0063c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00640
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00644
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00648
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0064c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00650
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00654
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00658
			 0x00000000,                                                  // zero                                        /// 0065c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00660
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00664
			 0x55555555,                                                  // 4 random values                             /// 00668
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00670
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00678
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0067c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00680
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00684
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00688
			 0x0e000003,                                                  // Trailing 1s:                                /// 0068c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00690
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00694
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00698
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0069c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006a0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006a4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006a8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006ac
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006b4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 006bc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006c0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0x55555555,                                                  // 4 random values                             /// 006c8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006cc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 006d0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 006d4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006d8
			 0x0e000003,                                                  // Trailing 1s:                                /// 006dc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006e0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006e4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 006e8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 006ec
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006f0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006f4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 006f8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 006fc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00700
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00704
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00708
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0070c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0x4b000000,                                                  // 8388608.0                                   /// 00714
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00718
			 0xff800000,                                                  // -inf                                        /// 0071c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00720
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00724
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00728
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0072c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00730
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00734
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00738
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0073c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00740
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00744
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00748
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0074c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00750
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00754
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00758
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0075c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00760
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00764
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00768
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0076c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00770
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00774
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00778
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0077c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00784
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00788
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0078c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00790
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00794
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00798
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007a0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007a4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007a8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ac
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b0
			 0x7fc00001,                                                  // signaling NaN                               /// 007b4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 007b8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 007bc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 007c0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 007c4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 007cc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007d0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007d4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 007dc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 007e0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007e4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007e8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007ec
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007f4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007fc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00800
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00804
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00808
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0080c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00810
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00814
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00818
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0081c
			 0xffc00001,                                                  // -signaling NaN                              /// 00820
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00824
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00828
			 0x00011111,                                                  // 9.7958E-41                                  /// 0082c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00830
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00834
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00838
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00840
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00844
			 0x80000000,                                                  // -zero                                       /// 00848
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0084c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00850
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0085c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00860
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00864
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00868
			 0x80000000,                                                  // -zero                                       /// 0086c
			 0x7f800000,                                                  // inf                                         /// 00870
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00874
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00878
			 0x80011111,                                                  // -9.7958E-41                                 /// 0087c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00880
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00884
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00888
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0088c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00890
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00894
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00898
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0089c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008a0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 008a8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 008ac
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 008b0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008b4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008b8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008bc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008c0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 008c8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008cc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008d8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008dc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 008e0
			 0x33333333,                                                  // 4 random values                             /// 008e4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 008e8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008ec
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 008f4
			 0x00011111,                                                  // 9.7958E-41                                  /// 008f8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 008fc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00900
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00904
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00908
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0xcb000000,                                                  // -8388608.0                                  /// 00910
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00914
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0091c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0x0e000003,                                                  // Trailing 1s:                                /// 00924
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00928
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0092c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00930
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00934
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00938
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0093c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00940
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00944
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00948
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0094c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00950
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00954
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0095c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00960
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00964
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00968
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0096c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00970
			 0x0e000007,                                                  // Trailing 1s:                                /// 00974
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00978
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00980
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0x0e000001,                                                  // Trailing 1s:                                /// 00988
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00990
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00998
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0099c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009a4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 009a8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 009b4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009b8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 009bc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009c0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009c4
			 0x7fc00001,                                                  // signaling NaN                               /// 009c8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 009d0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009dc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009e0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009e4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 009e8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009ec
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 009f0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009f4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009f8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009fc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a00
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a08
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a0c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a10
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a14
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a18
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a1c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a24
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a28
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a2c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a30
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a34
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a38
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a3c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a40
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a44
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a48
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a4c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a50
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a54
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a58
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a5c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a64
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a68
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a6c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a70
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a74
			 0xcb000000,                                                  // -8388608.0                                  /// 00a78
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a80
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a84
			 0x00000000,                                                  // zero                                        /// 00a88
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a8c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a90
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a94
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a98
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a9c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00aa0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00aa4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00aa8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aac
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ab0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ab4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ab8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00abc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0xbf028f5c,                                                  // -0.51                                       /// 00ac4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ac8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00acc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ad0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ad8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00adc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ae0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ae8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00aec
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00af4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00af8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00afc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b00
			 0x7f800000,                                                  // inf                                         /// 00b04
			 0x7fc00001,                                                  // signaling NaN                               /// 00b08
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b0c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b10
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b14
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b18
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b20
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b28
			 0xbf028f5c,                                                  // -0.51                                       /// 00b2c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b30
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b34
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b38
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b3c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b40
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b44
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b48
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b4c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b50
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b54
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b58
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b5c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b60
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b64
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b68
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b6c
			 0xffc00001,                                                  // -signaling NaN                              /// 00b70
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b74
			 0x80000000,                                                  // -zero                                       /// 00b78
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b7c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b8c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b90
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b94
			 0x00000000,                                                  // zero                                        /// 00b98
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b9c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ba4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ba8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00bac
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00bb0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00bb4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00bb8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bbc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00bc0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00bcc
			 0x7fc00001,                                                  // signaling NaN                               /// 00bd0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00bd4
			 0xcb000000,                                                  // -8388608.0                                  /// 00bd8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00bdc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00be0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bec
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00bf0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00bf8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c00
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c04
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c08
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c0c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c10
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c14
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c18
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c1c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c20
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c24
			 0xff800000,                                                  // -inf                                        /// 00c28
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c30
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c34
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c38
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c3c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c40
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c44
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c48
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c50
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c54
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c58
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c5c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c60
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c64
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c68
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c6c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c70
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c74
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c78
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c7c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c80
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c84
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c88
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c8c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c90
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c94
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c98
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c9c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ca4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00cac
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00cb0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00cb4
			 0x80000000,                                                  // -zero                                       /// 00cb8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cbc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00cc4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00cd0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00cd4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00cd8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cdc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ce0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cec
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cf0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00cf8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00cfc
			 0xffc00001,                                                  // -signaling NaN                              /// 00d00
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d04
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d08
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d0c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d10
			 0x7fc00001,                                                  // signaling NaN                               /// 00d14
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d18
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d1c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d20
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d24
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d28
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d2c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d30
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d34
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d38
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d3c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d40
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d44
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d48
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d4c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d50
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d54
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d58
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d5c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d60
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d64
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d68
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d6c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d70
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d78
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d7c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d80
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d84
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d88
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d8c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d90
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d94
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d98
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d9c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00da0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00da4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00da8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00db0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00db4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00dbc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00dc0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00dc4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00dc8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00dcc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00dd0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00dd4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00dd8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ddc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00de4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00dec
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00df4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00dfc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e00
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e04
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e08
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e0c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e10
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e14
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e18
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e1c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e20
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e28
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e2c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e30
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e34
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e38
			 0x4b000000,                                                  // 8388608.0                                   /// 00e3c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e40
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e44
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e48
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e4c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e50
			 0x80000000,                                                  // -zero                                       /// 00e54
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e58
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e5c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e64
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e68
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e6c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e70
			 0x7fc00001,                                                  // signaling NaN                               /// 00e74
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e78
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e7c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e80
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e84
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e88
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e8c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e90
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e94
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e98
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e9c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ea0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ea4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ea8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00eac
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00eb0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00eb4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb8
			 0x55555555,                                                  // 4 random values                             /// 00ebc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ec0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ec4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ec8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ecc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ed0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ed4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ed8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00edc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ee0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ee4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ee8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00eec
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ef0
			 0x80000000,                                                  // -zero                                       /// 00ef4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ef8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00efc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f00
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f04
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f08
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f0c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f14
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f18
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f1c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f20
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f24
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f28
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f2c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f30
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f34
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f38
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f40
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f44
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f48
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f4c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f50
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f54
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f58
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f5c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f60
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f64
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f68
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f6c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f70
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f74
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f78
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f7c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f80
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f84
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f88
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f8c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f94
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f98
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f9c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00fa4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fa8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00fac
			 0x3f028f5c,                                                  // 0.51                                        /// 00fb0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00fb4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00fb8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00fbc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00fc0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc8
			 0x80000000,                                                  // -zero                                       /// 00fcc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fd0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fd4
			 0xff800000,                                                  // -inf                                        /// 00fd8
			 0x4b000000,                                                  // 8388608.0                                   /// 00fdc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00fe0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fe4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fec
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ff0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ff8
			 0x4b000000                                                  // 8388608.0                                   /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00000
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00004
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00008
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0000c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00010
			 0x00000000,                                                  // zero                                        /// 00014
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00018
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0001c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00020
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00024
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00028
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0002c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00030
			 0xff800000,                                                  // -inf                                        /// 00034
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00038
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0003c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00044
			 0x0c600000,                                                  // Leading 1s:                                 /// 00048
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0004c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00050
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00054
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00058
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0005c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00060
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00064
			 0x55555555,                                                  // 4 random values                             /// 00068
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0006c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00070
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00074
			 0x80000000,                                                  // -zero                                       /// 00078
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0007c
			 0xff800000,                                                  // -inf                                        /// 00080
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00084
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00088
			 0x7f800000,                                                  // inf                                         /// 0008c
			 0xbf028f5c,                                                  // -0.51                                       /// 00090
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00094
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00098
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0009c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 000a0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 000a8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 000ac
			 0x7fc00001,                                                  // signaling NaN                               /// 000b0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000b4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000b8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 000bc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000c4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000cc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000d0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 000d4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000d8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000dc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000e0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000e4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 000e8
			 0x3f028f5c,                                                  // 0.51                                        /// 000ec
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000f0
			 0x0c400000,                                                  // Leading 1s:                                 /// 000f4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00100
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00104
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0010c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00110
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00114
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00118
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0011c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00120
			 0xcb000000,                                                  // -8388608.0                                  /// 00124
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00128
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0012c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00130
			 0x0c700000,                                                  // Leading 1s:                                 /// 00134
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00138
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0013c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00140
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00144
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00148
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0014c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00150
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00154
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00158
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0015c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00160
			 0x80011111,                                                  // -9.7958E-41                                 /// 00164
			 0x0e000007,                                                  // Trailing 1s:                                /// 00168
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0016c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00170
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00174
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00178
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0017c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00180
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00184
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00188
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00190
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00194
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00198
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0019c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001a0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001a8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 001ac
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 001b8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0xffc00001,                                                  // -signaling NaN                              /// 001c0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 001c4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 001c8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 001cc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 001d0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 001d8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001dc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001e0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001e4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001e8
			 0xff800000,                                                  // -inf                                        /// 001ec
			 0x00000000,                                                  // zero                                        /// 001f0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 001f4
			 0x7f800000,                                                  // inf                                         /// 001f8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 001fc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00204
			 0x0c700000,                                                  // Leading 1s:                                 /// 00208
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0020c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00210
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00214
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00218
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0021c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00220
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00224
			 0x00011111,                                                  // 9.7958E-41                                  /// 00228
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0x3f028f5c,                                                  // 0.51                                        /// 00230
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00234
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00238
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00240
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00244
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00248
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0024c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00250
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00254
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00258
			 0x0c700000,                                                  // Leading 1s:                                 /// 0025c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00260
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00264
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00268
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0026c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00270
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00274
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00278
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00280
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00284
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00288
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0028c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00290
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00294
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00298
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0029c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002a4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 002a8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 002ac
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002b0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 002b8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002bc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 002c0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 002c4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002c8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002cc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002d0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 002d4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002d8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002dc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002e0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002e4
			 0x0e000003,                                                  // Trailing 1s:                                /// 002e8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 002ec
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002f0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 002f4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 002f8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00304
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00308
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0030c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00310
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00318
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0031c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00320
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00328
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0032c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00330
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00334
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00338
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0033c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00340
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00344
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00348
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0034c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00350
			 0x0c600000,                                                  // Leading 1s:                                 /// 00354
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00358
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0035c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00360
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00364
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00368
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00370
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00374
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00378
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0037c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00380
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00384
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00388
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0038c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00390
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00394
			 0x3f028f5c,                                                  // 0.51                                        /// 00398
			 0x00000000,                                                  // zero                                        /// 0039c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003a0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003a4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003a8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003ac
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003b4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003bc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 003c0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003c4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 003c8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003cc
			 0x0c700000,                                                  // Leading 1s:                                 /// 003d0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003d4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003d8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 003dc
			 0x55555555,                                                  // 4 random values                             /// 003e0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 003e4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0x55555555,                                                  // 4 random values                             /// 003ec
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003f0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 003f4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003f8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00400
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00404
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00408
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00410
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00414
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00418
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0041c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00420
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00428
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0042c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00430
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00434
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00438
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0043c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00440
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00444
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0044c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00450
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00454
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00458
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0045c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00460
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00464
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00468
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0046c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00470
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00474
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00478
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0047c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00480
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00484
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00488
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0048c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00490
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00494
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00498
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0049c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ac
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004b0
			 0x0c400000,                                                  // Leading 1s:                                 /// 004b4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 004b8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004bc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004c0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c4
			 0xff800000,                                                  // -inf                                        /// 004c8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004cc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004d0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004d4
			 0x0c400000,                                                  // Leading 1s:                                 /// 004d8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 004dc
			 0xcb000000,                                                  // -8388608.0                                  /// 004e0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 004e4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004e8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 004ec
			 0x0c700000,                                                  // Leading 1s:                                 /// 004f0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 004f8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00500
			 0xff800000,                                                  // -inf                                        /// 00504
			 0xbf028f5c,                                                  // -0.51                                       /// 00508
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0050c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00510
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00514
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00518
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00520
			 0x0c700000,                                                  // Leading 1s:                                 /// 00524
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0052c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00538
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0053c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00540
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00544
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00548
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0054c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00550
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00554
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00558
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0055c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00560
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00564
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00568
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0056c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00570
			 0x4b000000,                                                  // 8388608.0                                   /// 00574
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0057c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00584
			 0x4b000000,                                                  // 8388608.0                                   /// 00588
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0058c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00590
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00594
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00598
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0059c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005a0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005a4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 005a8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 005ac
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 005b0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 005b8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005bc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005c0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 005c4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0x0e000001,                                                  // Trailing 1s:                                /// 005cc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 005d0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005d8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005e0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005e4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005e8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 005f4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005f8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00600
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00604
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00608
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0060c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00610
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00614
			 0x0e000007,                                                  // Trailing 1s:                                /// 00618
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0061c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00620
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00624
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00628
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0062c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00630
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00634
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00638
			 0xff800000,                                                  // -inf                                        /// 0063c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00640
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00644
			 0xbf028f5c,                                                  // -0.51                                       /// 00648
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0064c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00654
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00658
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0065c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00660
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00664
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00668
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0066c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00670
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00678
			 0x0c780000,                                                  // Leading 1s:                                 /// 0067c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00680
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00684
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00688
			 0x0e000001,                                                  // Trailing 1s:                                /// 0068c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00694
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00698
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0069c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006a0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006a4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 006ac
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 006b0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 006b4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006b8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 006bc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006c0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c4
			 0x7f800000,                                                  // inf                                         /// 006c8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 006cc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006d0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006d4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006d8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006dc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006e0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 006e8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ec
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006f0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006f4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006f8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006fc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00704
			 0x55555555,                                                  // 4 random values                             /// 00708
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0070c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00710
			 0x0c700000,                                                  // Leading 1s:                                 /// 00714
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00718
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0071c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00720
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00724
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00728
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0072c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00730
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00734
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00738
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00748
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0074c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00750
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00758
			 0x4b000000,                                                  // 8388608.0                                   /// 0075c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00764
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00768
			 0x00000000,                                                  // zero                                        /// 0076c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00770
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00778
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0077c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00780
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00788
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0078c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00790
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0079c
			 0x3f028f5c,                                                  // 0.51                                        /// 007a0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007a4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007a8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007ac
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007b0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007b8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007bc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 007c0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007c8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 007cc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007d8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007dc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007e0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 007e4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 007e8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007ec
			 0x0e001fff,                                                  // Trailing 1s:                                /// 007f0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007f4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007f8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00800
			 0x80011111,                                                  // -9.7958E-41                                 /// 00804
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00808
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0080c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00814
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00818
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0081c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00820
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00824
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00828
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0082c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00834
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0083c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00840
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00844
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00848
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00850
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00854
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0085c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00864
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00868
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0086c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00870
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00874
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00878
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0087c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00880
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00884
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00888
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0088c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00890
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00894
			 0x00011111,                                                  // 9.7958E-41                                  /// 00898
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0089c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 008a0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008a4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 008a8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008ac
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x0e000007,                                                  // Trailing 1s:                                /// 008b4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008bc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008c4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 008c8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 008cc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008d0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008d4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008dc
			 0x80000000,                                                  // -zero                                       /// 008e0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008e4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008ec
			 0x00800000, // min norm                                      // subnormals +ve                              /// 008f0
			 0xcb000000,                                                  // -8388608.0                                  /// 008f4
			 0x0c400000,                                                  // Leading 1s:                                 /// 008f8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008fc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00900
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00904
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00908
			 0x0c700000,                                                  // Leading 1s:                                 /// 0090c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00910
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00914
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00918
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0091c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00920
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00924
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00928
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0092c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00930
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00934
			 0x0e000003,                                                  // Trailing 1s:                                /// 00938
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0093c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00940
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00944
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00948
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0094c
			 0xffc00001,                                                  // -signaling NaN                              /// 00950
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00954
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00958
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0095c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0xffc00001,                                                  // -signaling NaN                              /// 00964
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00968
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0096c
			 0x00000000,                                                  // zero                                        /// 00970
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00974
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00978
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00980
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00984
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0098c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00990
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00998
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0099c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009a4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 009b0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 009b4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009bc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009c0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 009c4
			 0x7f800000,                                                  // inf                                         /// 009c8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 009cc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d4
			 0x00000000,                                                  // zero                                        /// 009d8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 009dc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009e4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 009e8
			 0x0c600000,                                                  // Leading 1s:                                 /// 009ec
			 0x55555555,                                                  // 4 random values                             /// 009f0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 009f4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009f8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a00
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a04
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a08
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a0c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a10
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a14
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a18
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a1c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a20
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a28
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a2c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a30
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a34
			 0x7f800000,                                                  // inf                                         /// 00a38
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a3c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a40
			 0x3f028f5c,                                                  // 0.51                                        /// 00a44
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a48
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a4c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a50
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a54
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a58
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a5c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a60
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a64
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a68
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a6c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a70
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a74
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a78
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a7c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a80
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a88
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a8c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a90
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a94
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a98
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a9c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00aa0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00aa4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00aa8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ab0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ab4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ab8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00abc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ac0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ac4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ac8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00acc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ad0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00adc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ae0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ae4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ae8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00aec
			 0x0c700000,                                                  // Leading 1s:                                 /// 00af0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00afc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b00
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b04
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b08
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b0c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b10
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b14
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b18
			 0x80000000,                                                  // -zero                                       /// 00b1c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b20
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b24
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b28
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b30
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b34
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b38
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b3c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b40
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b48
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b4c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b50
			 0xffc00001,                                                  // -signaling NaN                              /// 00b54
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b58
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b5c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b60
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b64
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b6c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b70
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b74
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b78
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b7c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b80
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b84
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b88
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b8c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b90
			 0xffc00001,                                                  // -signaling NaN                              /// 00b94
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b9c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ba0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ba4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00bac
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00bb4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00bb8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00bbc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00bc0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00bc4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00bc8
			 0x33333333,                                                  // 4 random values                             /// 00bcc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00bd4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00bdc
			 0x4b000000,                                                  // 8388608.0                                   /// 00be0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00be8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00bec
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf8
			 0x3f028f5c,                                                  // 0.51                                        /// 00bfc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c00
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c04
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c08
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c0c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c10
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c18
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c1c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c20
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c24
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c28
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c2c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c34
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c38
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c3c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c44
			 0x33333333,                                                  // 4 random values                             /// 00c48
			 0x55555555,                                                  // 4 random values                             /// 00c4c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c50
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c54
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c58
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c60
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c64
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c68
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c6c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c74
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c78
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c7c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c80
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c84
			 0xffc00001,                                                  // -signaling NaN                              /// 00c88
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c8c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c90
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c98
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c9c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ca0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ca4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ca8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cac
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00cb0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cb4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0xcb000000,                                                  // -8388608.0                                  /// 00cbc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00cc0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00cc4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00cc8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ccc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00cd0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cd4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00cd8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00cdc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ce0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ce4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ce8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00cec
			 0x80011111,                                                  // -9.7958E-41                                 /// 00cf0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00cf4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00cf8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00cfc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d00
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d04
			 0xff800000,                                                  // -inf                                        /// 00d08
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d0c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d10
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d14
			 0xff800000,                                                  // -inf                                        /// 00d18
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d1c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d20
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d24
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d28
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d2c
			 0xffc00001,                                                  // -signaling NaN                              /// 00d30
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d34
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d38
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d3c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d40
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d44
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d48
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d4c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d50
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d54
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d58
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d5c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d60
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d64
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d68
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d6c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d70
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d78
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d7c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d80
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d84
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d8c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d90
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d94
			 0xbf028f5c,                                                  // -0.51                                       /// 00d98
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d9c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00da0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00da4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00dac
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00db0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00db4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00db8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x80000000,                                                  // -zero                                       /// 00dc0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00dc4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00dc8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00dcc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00dd0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00dd4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ddc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00de0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00de4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00df0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00df4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00df8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00dfc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e04
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e08
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e0c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e10
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e14
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e18
			 0xcb000000,                                                  // -8388608.0                                  /// 00e1c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e20
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e24
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e28
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e30
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e34
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e3c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e40
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e44
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e48
			 0x3f028f5c,                                                  // 0.51                                        /// 00e4c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e50
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e54
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e58
			 0x3f028f5c,                                                  // 0.51                                        /// 00e5c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e64
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e68
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e6c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e74
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e78
			 0x80000000,                                                  // -zero                                       /// 00e7c
			 0x3f028f5c,                                                  // 0.51                                        /// 00e80
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e84
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e88
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e8c
			 0x33333333,                                                  // 4 random values                             /// 00e90
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e9c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ea0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ea4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ea8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00eb4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00eb8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ebc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ec0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ec4
			 0xffc00001,                                                  // -signaling NaN                              /// 00ec8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ecc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ed0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ed4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ee0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ee4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ee8
			 0x00000000,                                                  // zero                                        /// 00eec
			 0x7f800000,                                                  // inf                                         /// 00ef0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ef4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00efc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f00
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f08
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f0c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f14
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f18
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f1c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f20
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f24
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f2c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f30
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f34
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f38
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f3c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f40
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f44
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f48
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f4c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f50
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f58
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f60
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f68
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f6c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f70
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f7c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0x33333333,                                                  // 4 random values                             /// 00f84
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f88
			 0x33333333,                                                  // 4 random values                             /// 00f8c
			 0xffc00001,                                                  // -signaling NaN                              /// 00f90
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f94
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f98
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00fa0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00fa8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fac
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00fb0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00fb8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fbc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00fc0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00fc4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00fc8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00fcc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00fd4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00fd8
			 0x33333333,                                                  // 4 random values                             /// 00fdc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00fe0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00fe4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00fe8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00fec
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ff0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ff4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ff8
			 0x00800002 // min norm + 2ulp                               // subnormals +ve                              /// last
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
			 0x0000067c,
			 0x00000428,
			 0x000006e0,
			 0x00000280,
			 0x00000328,
			 0x00000018,
			 0x00000620,
			 0x00000644,

			 /// vpu register f2
			 0x41b00000,
			 0x41600000,
			 0x41000000,
			 0x40c00000,
			 0x41e00000,
			 0x41e00000,
			 0x42000000,
			 0x41f00000,

			 /// vpu register f3
			 0x40400000,
			 0x40a00000,
			 0x40000000,
			 0x41100000,
			 0x40000000,
			 0x41f80000,
			 0x41b00000,
			 0x41e80000,

			 /// vpu register f4
			 0x42000000,
			 0x40000000,
			 0x41c80000,
			 0x41c80000,
			 0x41880000,
			 0x41b00000,
			 0x40000000,
			 0x41400000,

			 /// vpu register f5
			 0x41c80000,
			 0x40e00000,
			 0x41900000,
			 0x41f80000,
			 0x41e80000,
			 0x40400000,
			 0x41400000,
			 0x41b00000,

			 /// vpu register f6
			 0x41500000,
			 0x41880000,
			 0x41700000,
			 0x41b00000,
			 0x41b80000,
			 0x41d00000,
			 0x41000000,
			 0x41d00000,

			 /// vpu register f7
			 0x41f80000,
			 0x41a80000,
			 0x40400000,
			 0x41c00000,
			 0x41200000,
			 0x41c00000,
			 0x41a80000,
			 0x40000000,

			 /// vpu register f8
			 0x41e80000,
			 0x41e00000,
			 0x41980000,
			 0x41a80000,
			 0x40e00000,
			 0x40000000,
			 0x41800000,
			 0x42000000,

			 /// vpu register f9
			 0x41c80000,
			 0x41000000,
			 0x40a00000,
			 0x41d80000,
			 0x41c80000,
			 0x41b00000,
			 0x41b80000,
			 0x42000000,

			 /// vpu register f10
			 0x42000000,
			 0x41000000,
			 0x40e00000,
			 0x40c00000,
			 0x41300000,
			 0x41b80000,
			 0x41a80000,
			 0x41800000,

			 /// vpu register f11
			 0x40400000,
			 0x41c80000,
			 0x41100000,
			 0x41800000,
			 0x40800000,
			 0x40e00000,
			 0x41200000,
			 0x41400000,

			 /// vpu register f12
			 0x41800000,
			 0x40c00000,
			 0x3f800000,
			 0x41b00000,
			 0x41a00000,
			 0x40800000,
			 0x41b80000,
			 0x41e80000,

			 /// vpu register f13
			 0x41200000,
			 0x41b00000,
			 0x40800000,
			 0x41b00000,
			 0x41900000,
			 0x41c00000,
			 0x41700000,
			 0x41e80000,

			 /// vpu register f14
			 0x40800000,
			 0x41000000,
			 0x41600000,
			 0x41b80000,
			 0x40000000,
			 0x41400000,
			 0x41600000,
			 0x40a00000,

			 /// vpu register f15
			 0x41700000,
			 0x41900000,
			 0x42000000,
			 0x41200000,
			 0x41e80000,
			 0x41b80000,
			 0x41b80000,
			 0x41c00000,

			 /// vpu register f16
			 0x40800000,
			 0x40a00000,
			 0x41900000,
			 0x41a00000,
			 0x40a00000,
			 0x40e00000,
			 0x41800000,
			 0x40a00000,

			 /// vpu register f17
			 0x41e00000,
			 0x41b00000,
			 0x41500000,
			 0x41e80000,
			 0x40c00000,
			 0x40c00000,
			 0x41800000,
			 0x41100000,

			 /// vpu register f18
			 0x41900000,
			 0x40c00000,
			 0x41b00000,
			 0x41e00000,
			 0x41a80000,
			 0x41400000,
			 0x41880000,
			 0x41500000,

			 /// vpu register f19
			 0x40800000,
			 0x41d00000,
			 0x40000000,
			 0x41900000,
			 0x41e80000,
			 0x41d00000,
			 0x41800000,
			 0x41e80000,

			 /// vpu register f20
			 0x41e00000,
			 0x41980000,
			 0x41c00000,
			 0x41b00000,
			 0x41e80000,
			 0x40e00000,
			 0x41a00000,
			 0x40400000,

			 /// vpu register f21
			 0x41800000,
			 0x41c80000,
			 0x40a00000,
			 0x41a80000,
			 0x41a80000,
			 0x41b00000,
			 0x41f80000,
			 0x41f80000,

			 /// vpu register f22
			 0x41a00000,
			 0x41880000,
			 0x40e00000,
			 0x41e80000,
			 0x41b80000,
			 0x41e00000,
			 0x41600000,
			 0x41e80000,

			 /// vpu register f23
			 0x40e00000,
			 0x41500000,
			 0x41300000,
			 0x41f00000,
			 0x40c00000,
			 0x40000000,
			 0x41f80000,
			 0x41a80000,

			 /// vpu register f24
			 0x41400000,
			 0x41f80000,
			 0x41000000,
			 0x41880000,
			 0x41400000,
			 0x40e00000,
			 0x40400000,
			 0x41200000,

			 /// vpu register f25
			 0x40e00000,
			 0x42000000,
			 0x40800000,
			 0x41800000,
			 0x41e00000,
			 0x41100000,
			 0x3f800000,
			 0x41d00000,

			 /// vpu register f26
			 0x41880000,
			 0x41f00000,
			 0x40000000,
			 0x41d80000,
			 0x41d80000,
			 0x41a80000,
			 0x41800000,
			 0x41880000,

			 /// vpu register f27
			 0x41b00000,
			 0x41000000,
			 0x41500000,
			 0x40a00000,
			 0x41200000,
			 0x41100000,
			 0x41100000,
			 0x41d80000,

			 /// vpu register f28
			 0x41200000,
			 0x41d00000,
			 0x41e00000,
			 0x41a80000,
			 0x41800000,
			 0x41700000,
			 0x40a00000,
			 0x41900000,

			 /// vpu register f29
			 0x41f80000,
			 0x41000000,
			 0x41300000,
			 0x41a00000,
			 0x42000000,
			 0x41d00000,
			 0x41b80000,
			 0x41a00000,

			 /// vpu register f30
			 0x41700000,
			 0x41f80000,
			 0x41e80000,
			 0x41c00000,
			 0x41000000,
			 0x40800000,
			 0x41f80000,
			 0x41d00000,

			 /// vpu register f31
			 0x3f800000,
			 0x41b00000,
			 0x41e80000,
			 0x41800000,
			 0x40a00000,
			 0x40c00000,
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
		"nop\n"
		"fscwl.ps f10, f1 (x13)\n"                            ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f24, f1 (x15)\n"                            ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f8, f1 (x13)\n"                             ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f4, f1 (x13)\n"                             ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f13, f1 (x13)\n"                            ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f11, f1 (x15)\n"                            ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f9, f1 (x15)\n"                             ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f29, f1 (x13)\n"                            ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f16, f1 (x14)\n"                            ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f0, f1 (x13)\n"                             ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f26, f1 (x14)\n"                            ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f29, f1 (x12)\n"                            ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f18, f1 (x14)\n"                            ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f12, f1 (x13)\n"                            ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f17, f1 (x15)\n"                            ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f16, f1 (x11)\n"                            ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f9, f1 (x12)\n"                             ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f23, f1 (x11)\n"                            ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f3, f1 (x12)\n"                             ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f6, f1 (x12)\n"                             ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f23, f1 (x14)\n"                            ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f18, f1 (x12)\n"                            ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f23, f1 (x15)\n"                            ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f7, f1 (x11)\n"                             ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f4, f1 (x11)\n"                             ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f4, f1 (x12)\n"                             ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f13, f1 (x14)\n"                            ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f1, f1 (x15)\n"                             ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f13, f1 (x15)\n"                            ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f8, f1 (x12)\n"                             ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f26, f1 (x11)\n"                            ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f20, f1 (x15)\n"                            ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f23, f1 (x15)\n"                            ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f10, f1 (x14)\n"                            ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f0, f1 (x12)\n"                             ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f6, f1 (x13)\n"                             ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f7, f1 (x11)\n"                             ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f17, f1 (x11)\n"                            ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f16, f1 (x11)\n"                            ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f12, f1 (x14)\n"                            ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f0, f1 (x12)\n"                             ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f15, f1 (x14)\n"                            ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f18, f1 (x11)\n"                            ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f11, f1 (x13)\n"                            ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f5, f1 (x14)\n"                             ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f28, f1 (x11)\n"                            ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f12, f1 (x12)\n"                            ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f23, f1 (x13)\n"                            ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f4, f1 (x14)\n"                             ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f2, f1 (x15)\n"                             ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f4, f1 (x13)\n"                             ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f17, f1 (x13)\n"                            ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f20, f1 (x11)\n"                            ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f21, f1 (x14)\n"                            ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f7, f1 (x15)\n"                             ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f24, f1 (x11)\n"                            ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f4, f1 (x11)\n"                             ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f4, f1 (x15)\n"                             ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f23, f1 (x11)\n"                            ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f13, f1 (x13)\n"                            ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f29, f1 (x11)\n"                            ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f23, f1 (x11)\n"                            ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f1, f1 (x14)\n"                             ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f1, f1 (x13)\n"                             ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f22, f1 (x14)\n"                            ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f0, f1 (x12)\n"                             ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f21, f1 (x14)\n"                            ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f24, f1 (x11)\n"                            ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f8, f1 (x14)\n"                             ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f5, f1 (x15)\n"                             ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f6, f1 (x12)\n"                             ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f23, f1 (x15)\n"                            ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f17, f1 (x11)\n"                            ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f19, f1 (x15)\n"                            ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f5, f1 (x12)\n"                             ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f15, f1 (x12)\n"                            ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f4, f1 (x13)\n"                             ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f12, f1 (x15)\n"                            ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f0, f1 (x12)\n"                             ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f5, f1 (x11)\n"                             ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f25, f1 (x12)\n"                            ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f25, f1 (x15)\n"                            ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f6, f1 (x12)\n"                             ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f16, f1 (x14)\n"                            ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f28, f1 (x13)\n"                            ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f20, f1 (x15)\n"                            ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f21, f1 (x15)\n"                            ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f23, f1 (x15)\n"                            ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f28, f1 (x11)\n"                            ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f12, f1 (x11)\n"                            ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f13, f1 (x12)\n"                            ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f21, f1 (x11)\n"                            ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f25, f1 (x12)\n"                            ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f14, f1 (x13)\n"                            ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f1, f1 (x12)\n"                             ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f20, f1 (x12)\n"                            ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f12, f1 (x12)\n"                            ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f5, f1 (x13)\n"                             ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fscwl.ps f1, f1 (x13)\n"                             ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
