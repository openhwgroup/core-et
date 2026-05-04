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
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00000
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00008
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0000c
			 0xbf028f5c,                                                  // -0.51                                       /// 00010
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00014
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0001c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00020
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00024
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x0c600000,                                                  // Leading 1s:                                 /// 0002c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00030
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00034
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00038
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0003c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00040
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00044
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0004c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00050
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00054
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00058
			 0xbf028f5c,                                                  // -0.51                                       /// 0005c
			 0x80000000,                                                  // -zero                                       /// 00060
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00064
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00068
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00070
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00074
			 0xff800000,                                                  // -inf                                        /// 00078
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0007c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00080
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00084
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00088
			 0x0e000001,                                                  // Trailing 1s:                                /// 0008c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00090
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00098
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0009c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000a0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0x3f028f5c,                                                  // 0.51                                        /// 000a8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000ac
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 000b4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0x80011111,                                                  // -9.7958E-41                                 /// 000c0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000c4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 000c8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000cc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000d4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 000d8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000dc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000e4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 000e8
			 0x0c700000,                                                  // Leading 1s:                                 /// 000ec
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000f0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000f4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000f8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00100
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00104
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00108
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0010c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00110
			 0x0e000007,                                                  // Trailing 1s:                                /// 00114
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00118
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0011c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00120
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00124
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00128
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0012c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00130
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00134
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00138
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0013c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00140
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00144
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00148
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0014c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00150
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00154
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00158
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0015c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00160
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00164
			 0x0c780000,                                                  // Leading 1s:                                 /// 00168
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00170
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00174
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00178
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0017c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00180
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00184
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00188
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0018c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00190
			 0x00000000,                                                  // zero                                        /// 00194
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00198
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0019c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001a0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 001a4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001a8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001ac
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001b4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001b8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001bc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 001c0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 001c8
			 0x0e000007,                                                  // Trailing 1s:                                /// 001cc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001d0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d4
			 0x4b000000,                                                  // 8388608.0                                   /// 001d8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001dc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 001e0
			 0x00000000,                                                  // zero                                        /// 001e4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001e8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 001ec
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 001f0
			 0x0c400000,                                                  // Leading 1s:                                 /// 001f4
			 0xff800000,                                                  // -inf                                        /// 001f8
			 0x0e000001,                                                  // Trailing 1s:                                /// 001fc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0x0c400000,                                                  // Leading 1s:                                 /// 00204
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0020c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00220
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00224
			 0x7f800000,                                                  // inf                                         /// 00228
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0022c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00230
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00234
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00238
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0023c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00240
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00244
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00248
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0024c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00250
			 0x0c780000,                                                  // Leading 1s:                                 /// 00254
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00258
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0025c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00260
			 0x4b000000,                                                  // 8388608.0                                   /// 00264
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00268
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0026c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00270
			 0xcb000000,                                                  // -8388608.0                                  /// 00274
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0027c
			 0x4b000000,                                                  // 8388608.0                                   /// 00280
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00284
			 0x00000000,                                                  // zero                                        /// 00288
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0028c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00290
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00294
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00298
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0029c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002a8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002ac
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 002b0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002b4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b8
			 0x7fc00001,                                                  // signaling NaN                               /// 002bc
			 0xffc00001,                                                  // -signaling NaN                              /// 002c0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002c8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 002cc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 002d0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002d8
			 0x00011111,                                                  // 9.7958E-41                                  /// 002dc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002e0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 002e4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002e8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 002ec
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 002f4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002f8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00300
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00304
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00308
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0030c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00314
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00318
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0031c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00320
			 0x4b000000,                                                  // 8388608.0                                   /// 00324
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00328
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0032c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00330
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00334
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00340
			 0x0c400000,                                                  // Leading 1s:                                 /// 00344
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00348
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0034c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00350
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00354
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00358
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0035c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0x33333333,                                                  // 4 random values                             /// 00364
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0xbf028f5c,                                                  // -0.51                                       /// 0036c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00370
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00378
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0037c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00380
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00388
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0038c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00390
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00394
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00398
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0039c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003a0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003ac
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 003b8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003bc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 003c0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003c4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003c8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003cc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003d0
			 0x00011111,                                                  // 9.7958E-41                                  /// 003d4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003d8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003e0
			 0x80011111,                                                  // -9.7958E-41                                 /// 003e4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 003ec
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0x33333333,                                                  // 4 random values                             /// 003f4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003f8
			 0x00011111,                                                  // 9.7958E-41                                  /// 003fc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0x55555555,                                                  // 4 random values                             /// 00404
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00408
			 0x0e000007,                                                  // Trailing 1s:                                /// 0040c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00410
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00414
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00418
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0041c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00420
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00424
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00428
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00430
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00434
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0043c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00440
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00448
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0044c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00450
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00454
			 0x55555555,                                                  // 4 random values                             /// 00458
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0045c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00460
			 0x7fc00001,                                                  // signaling NaN                               /// 00464
			 0xbf028f5c,                                                  // -0.51                                       /// 00468
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0046c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00470
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00474
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00478
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0047c
			 0x7fc00001,                                                  // signaling NaN                               /// 00480
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00484
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00488
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0048c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00490
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00494
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0049c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004a4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004ac
			 0xffc00001,                                                  // -signaling NaN                              /// 004b0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 004b8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004bc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004c0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004c4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004cc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 004d0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004d8
			 0x0e000001,                                                  // Trailing 1s:                                /// 004dc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 004e0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004e4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004e8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004ec
			 0xcb000000,                                                  // -8388608.0                                  /// 004f0
			 0x4b000000,                                                  // 8388608.0                                   /// 004f4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0x7f800000,                                                  // inf                                         /// 00500
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00504
			 0x7f800000,                                                  // inf                                         /// 00508
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0050c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00510
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00518
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0051c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00520
			 0x0c700000,                                                  // Leading 1s:                                 /// 00524
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00528
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0052c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00530
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00534
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00538
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0053c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00540
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00544
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00548
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0054c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00550
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00554
			 0x0e000003,                                                  // Trailing 1s:                                /// 00558
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0055c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00564
			 0xff800000,                                                  // -inf                                        /// 00568
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0056c
			 0x00000000,                                                  // zero                                        /// 00570
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00574
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0057c
			 0xffc00001,                                                  // -signaling NaN                              /// 00580
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00584
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00588
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00590
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00594
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00598
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0059c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005a0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a4
			 0x7f800000,                                                  // inf                                         /// 005a8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005b0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005b4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005b8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005bc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 005c4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005c8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 005cc
			 0x0c400000,                                                  // Leading 1s:                                 /// 005d0
			 0x0e000003,                                                  // Trailing 1s:                                /// 005d4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 005d8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 005dc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005e0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 005e8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 005f0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005f8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005fc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00600
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00604
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00608
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0060c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00610
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00614
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00618
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0061c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00620
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00624
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00628
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0062c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00630
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00634
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00638
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0063c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00640
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00644
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00648
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00650
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00654
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00658
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00660
			 0xffc00001,                                                  // -signaling NaN                              /// 00664
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00668
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0066c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00670
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00674
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00678
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0067c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00680
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00684
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0068c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00690
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00694
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00698
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0069c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006a0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006a4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006a8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ac
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 006bc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 006c0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006d0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006d4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 006d8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006dc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 006e0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006ec
			 0x7fc00001,                                                  // signaling NaN                               /// 006f0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006f4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 006fc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00700
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00704
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0070c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0x3f028f5c,                                                  // 0.51                                        /// 00714
			 0x33333333,                                                  // 4 random values                             /// 00718
			 0xffc00001,                                                  // -signaling NaN                              /// 0071c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00720
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00724
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00728
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0072c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00730
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00734
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0073c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00740
			 0x0e000003,                                                  // Trailing 1s:                                /// 00744
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00748
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0074c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00750
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0xcb000000,                                                  // -8388608.0                                  /// 00758
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0075c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0x7f800000,                                                  // inf                                         /// 00764
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00768
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0076c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00770
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00774
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00778
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x00000000,                                                  // zero                                        /// 00780
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00784
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00788
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0078c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00794
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00798
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007a4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a8
			 0x0c600000,                                                  // Leading 1s:                                 /// 007ac
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 007b0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007b4
			 0x7fc00001,                                                  // signaling NaN                               /// 007b8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007bc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 007c0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 007cc
			 0x0e000003,                                                  // Trailing 1s:                                /// 007d0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 007d4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0x4b000000,                                                  // 8388608.0                                   /// 007dc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 007e0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007e4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007e8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007ec
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007fc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00800
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00804
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00808
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0080c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00810
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00818
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0081c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00820
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00824
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00828
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0082c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00830
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00834
			 0xcb000000,                                                  // -8388608.0                                  /// 00838
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00840
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00844
			 0x3f028f5c,                                                  // 0.51                                        /// 00848
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00850
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00854
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00858
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0085c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00860
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00864
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0086c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00870
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00874
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00878
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0087c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00884
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00888
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0088c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00894
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00898
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0089c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 008a0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008a4
			 0x0c400000,                                                  // Leading 1s:                                 /// 008a8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ac
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008b0
			 0x0c780000,                                                  // Leading 1s:                                 /// 008b4
			 0xcb000000,                                                  // -8388608.0                                  /// 008b8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 008bc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008cc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008d0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008d4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 008d8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008dc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 008e0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008e8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 008f4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008f8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008fc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00900
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00908
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0090c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00910
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00914
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00918
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0091c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00920
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00924
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00928
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0092c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00930
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00934
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00938
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0093c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00940
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00944
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00948
			 0xffc00001,                                                  // -signaling NaN                              /// 0094c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00954
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00958
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0095c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00960
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00964
			 0x33333333,                                                  // 4 random values                             /// 00968
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0096c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00970
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00974
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00978
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0097c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00980
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00984
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00988
			 0x80000000,                                                  // -zero                                       /// 0098c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00990
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00994
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00998
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0099c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 009a0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 009a4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 009a8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009b0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009b4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 009b8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009bc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009c4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009c8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009cc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 009d0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009dc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 009e0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 009e4
			 0xbf028f5c,                                                  // -0.51                                       /// 009e8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 009ec
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009f4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f8
			 0xbf028f5c,                                                  // -0.51                                       /// 009fc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a04
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a08
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a0c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a10
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a18
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a1c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a20
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a24
			 0x80000000,                                                  // -zero                                       /// 00a28
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a2c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a30
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a34
			 0xffc00001,                                                  // -signaling NaN                              /// 00a38
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a3c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a40
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a44
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a48
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a4c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a54
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a58
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a5c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a60
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a64
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a68
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a6c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a70
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a74
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a78
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a7c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a84
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a8c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a90
			 0xff800000,                                                  // -inf                                        /// 00a94
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a98
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a9c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00aa4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00aa8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00aac
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ab0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ab4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00abc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ac4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ac8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00acc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ad0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ad4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ad8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00adc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ae4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ae8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00aec
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00af0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00af4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00af8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00afc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b04
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b08
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b0c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b10
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b14
			 0x55555555,                                                  // 4 random values                             /// 00b18
			 0x7f800000,                                                  // inf                                         /// 00b1c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b20
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b24
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b28
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b2c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b30
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b34
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b38
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b3c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b40
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b44
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b48
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b50
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b58
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b5c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b60
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b64
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b6c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b70
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b74
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b78
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b7c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b84
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b88
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b8c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b90
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b94
			 0xbf028f5c,                                                  // -0.51                                       /// 00b98
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b9c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ba0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba4
			 0xbf028f5c,                                                  // -0.51                                       /// 00ba8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00bac
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00bb0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00bb4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00bb8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00bc4
			 0xff800000,                                                  // -inf                                        /// 00bc8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00bcc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00bd0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00bd8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00bdc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00be0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00be8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bec
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00bf4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00bf8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00bfc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c00
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c04
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c08
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c0c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c18
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c1c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c20
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c24
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c28
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c2c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c30
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c34
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c38
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c44
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c48
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c50
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c54
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c58
			 0x3f028f5c,                                                  // 0.51                                        /// 00c5c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c60
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c64
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c6c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c70
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c74
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c78
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c7c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c84
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c88
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c8c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c90
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c98
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c9c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ca4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ca8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00cac
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0xcb000000,                                                  // -8388608.0                                  /// 00cb4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00cb8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00cbc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00cc0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00cc4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00cc8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ccc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00cd0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00cd4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00cd8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00cdc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ce4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cec
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00cf4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00cf8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00cfc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d00
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d04
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d08
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d0c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d14
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d18
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d1c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d20
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d28
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d2c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d30
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d34
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d38
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d3c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d40
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d48
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d4c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d50
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d54
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d58
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d5c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d64
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d68
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d6c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d70
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d74
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d78
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d7c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d80
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d84
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d88
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d8c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d94
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d98
			 0x55555555,                                                  // 4 random values                             /// 00d9c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00da0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00da4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00da8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00dac
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00db4
			 0xff800000,                                                  // -inf                                        /// 00db8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00dbc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00dc0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00dc4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00dc8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00dcc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00dd4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00dd8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ddc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00de4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00de8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00dec
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00df0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dfc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e00
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e04
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e08
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e0c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e10
			 0x55555555,                                                  // 4 random values                             /// 00e14
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e18
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e1c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e20
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e24
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e28
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e2c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e30
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e34
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e38
			 0xff800000,                                                  // -inf                                        /// 00e3c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e40
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e44
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e48
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e4c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e50
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e54
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e58
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e5c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e60
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e64
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e68
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e6c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e70
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e74
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e78
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e7c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e80
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e84
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e88
			 0xbf028f5c,                                                  // -0.51                                       /// 00e8c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e90
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e94
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e98
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e9c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ea0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ea4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ea8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00eb0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00eb8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ebc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ec0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ecc
			 0x55555555,                                                  // 4 random values                             /// 00ed0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ed4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ed8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00edc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ee0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ee4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ee8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00eec
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ef4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ef8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00efc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f00
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f04
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0xbf028f5c,                                                  // -0.51                                       /// 00f0c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f10
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f14
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f18
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f1c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f20
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f24
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f28
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f2c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f30
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f34
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f38
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f44
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f48
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f4c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f50
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f54
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f58
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f5c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f60
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f64
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f68
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f6c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f74
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f78
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f7c
			 0x33333333,                                                  // 4 random values                             /// 00f80
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f84
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f88
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f8c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f90
			 0x7f800000,                                                  // inf                                         /// 00f94
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f98
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f9c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00fa0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00fa4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00fa8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fac
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00fb0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fb4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00fb8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00fbc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc4
			 0x7fc00001,                                                  // signaling NaN                               /// 00fc8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fcc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00fd0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00fd8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00fdc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fe0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00fe4
			 0x80000000,                                                  // -zero                                       /// 00fe8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ff0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ff8
			 0x0e0fffff                                                  // Trailing 1s:                                /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00000
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00004
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00008
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0000c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00010
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00014
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00018
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0001c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00020
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00024
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00028
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0002c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00030
			 0x0c780000,                                                  // Leading 1s:                                 /// 00034
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00038
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0003c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00040
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00044
			 0x0c400000,                                                  // Leading 1s:                                 /// 00048
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0004c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00050
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00054
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00058
			 0x0e000007,                                                  // Trailing 1s:                                /// 0005c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00060
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00064
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00068
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0006c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00070
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00074
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00078
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0007c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00080
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00084
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00088
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0008c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00090
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00094
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00098
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0009c
			 0x0c780000,                                                  // Leading 1s:                                 /// 000a0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000a4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000a8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ac
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000b0
			 0x7f800000,                                                  // inf                                         /// 000b4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000c0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 000c8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 000cc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000d0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 000d4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 000d8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000dc
			 0x55555555,                                                  // 4 random values                             /// 000e0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000e4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000ec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000f0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000f8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00104
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00108
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0010c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00110
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00114
			 0x80011111,                                                  // -9.7958E-41                                 /// 00118
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0011c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00120
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00124
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00128
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0012c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00130
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00134
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00138
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0013c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00140
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00144
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0xbf028f5c,                                                  // -0.51                                       /// 0014c
			 0x33333333,                                                  // 4 random values                             /// 00150
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00154
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0015c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00160
			 0x0c600000,                                                  // Leading 1s:                                 /// 00164
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00168
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00170
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00174
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00178
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00184
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0018c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00190
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00194
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00198
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0019c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001a4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 001b0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001b4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 001b8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 001c4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c8
			 0x7f800000,                                                  // inf                                         /// 001cc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 001d0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001d4
			 0x0e000003,                                                  // Trailing 1s:                                /// 001d8
			 0xcb000000,                                                  // -8388608.0                                  /// 001dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001e4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001e8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 001ec
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001f0
			 0x0e000001,                                                  // Trailing 1s:                                /// 001f4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001f8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 001fc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00200
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00208
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0020c
			 0x3f028f5c,                                                  // 0.51                                        /// 00210
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00214
			 0x00011111,                                                  // 9.7958E-41                                  /// 00218
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0021c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00220
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00224
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00228
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0022c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00230
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00234
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00238
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0023c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00240
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00244
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00248
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00250
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00254
			 0x0c780000,                                                  // Leading 1s:                                 /// 00258
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0025c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00260
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00264
			 0x0e000007,                                                  // Trailing 1s:                                /// 00268
			 0x33333333,                                                  // 4 random values                             /// 0026c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00270
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00274
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0027c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00280
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00284
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00288
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00290
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00294
			 0xff800000,                                                  // -inf                                        /// 00298
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0029c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 002a0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002ac
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002b0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 002b4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002bc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002c0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002c4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002c8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002cc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002d4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 002d8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002dc
			 0x33333333,                                                  // 4 random values                             /// 002e0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002f0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002f4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002f8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 002fc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00300
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00304
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00308
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0030c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00310
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00318
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0031c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00320
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00324
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00328
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0032c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00330
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00338
			 0x55555555,                                                  // 4 random values                             /// 0033c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00340
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00344
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00348
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0034c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00350
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00354
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00358
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0035c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00360
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00364
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00368
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0036c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00370
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00374
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0037c
			 0xff800000,                                                  // -inf                                        /// 00380
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00384
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00388
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0038c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00390
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00394
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00398
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0039c
			 0xcb000000,                                                  // -8388608.0                                  /// 003a0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003a4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003a8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003ac
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003b4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003b8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 003bc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003c0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003c4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003c8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003cc
			 0x55555555,                                                  // 4 random values                             /// 003d0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003dc
			 0x0e000001,                                                  // Trailing 1s:                                /// 003e0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003e4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 003ec
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 003f0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 003f4
			 0x0e000003,                                                  // Trailing 1s:                                /// 003f8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00404
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00408
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0040c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00414
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00418
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0041c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00420
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00424
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0042c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00430
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00434
			 0x80011111,                                                  // -9.7958E-41                                 /// 00438
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0043c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00440
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00444
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00448
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0044c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00454
			 0x0e000001,                                                  // Trailing 1s:                                /// 00458
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00460
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0046c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00470
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00474
			 0xff800000,                                                  // -inf                                        /// 00478
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0047c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00480
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00488
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0048c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00490
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00494
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0049c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 004a0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004a4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 004a8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004ac
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 004b0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 004b4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004b8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 004bc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 004c0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 004c4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004c8
			 0xffc00001,                                                  // -signaling NaN                              /// 004cc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 004d0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 004dc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 004e4
			 0x0c600000,                                                  // Leading 1s:                                 /// 004e8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004ec
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004f0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004f4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004f8
			 0xbf028f5c,                                                  // -0.51                                       /// 004fc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00500
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00504
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0050c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00510
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00518
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0051c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00520
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00524
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00528
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0052c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00530
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00534
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00538
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0053c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00540
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x0c600000,                                                  // Leading 1s:                                 /// 00548
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0054c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00550
			 0xffc00001,                                                  // -signaling NaN                              /// 00554
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00558
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0055c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00560
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00564
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00568
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00570
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00574
			 0x00000000,                                                  // zero                                        /// 00578
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0057c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00580
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00584
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00588
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00590
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00594
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00598
			 0x00011111,                                                  // 9.7958E-41                                  /// 0059c
			 0x0c400000,                                                  // Leading 1s:                                 /// 005a0
			 0x4b000000,                                                  // 8388608.0                                   /// 005a4
			 0x0c700000,                                                  // Leading 1s:                                 /// 005a8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 005ac
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 005b0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005b4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 005b8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 005bc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005c4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 005cc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 005d0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005d4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005d8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 005dc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005e0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 005e4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005e8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 005ec
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 005f0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 005f4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 005f8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005fc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00600
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00608
			 0x7fc00001,                                                  // signaling NaN                               /// 0060c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00610
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00614
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00618
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0061c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00620
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00624
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00628
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0062c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00630
			 0x3f028f5c,                                                  // 0.51                                        /// 00634
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00638
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0063c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00640
			 0x00011111,                                                  // 9.7958E-41                                  /// 00644
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0064c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00650
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00654
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00658
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0065c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00660
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00664
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0066c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00670
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00674
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0067c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00680
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00684
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00688
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0068c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00690
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00694
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00698
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 006a0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006a4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006ac
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006b0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006b4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006bc
			 0x4b000000,                                                  // 8388608.0                                   /// 006c0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006c4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006c8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 006cc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006d0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006dc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0x80011111,                                                  // -9.7958E-41                                 /// 006e4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006e8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006ec
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006f0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006f8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 006fc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00700
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00704
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00708
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0070c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00714
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00718
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0071c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00720
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00724
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00728
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0072c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00734
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00738
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0073c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00740
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00744
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00748
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0074c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00750
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00754
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00758
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0075c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00760
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00764
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00768
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0076c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00770
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00774
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00778
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0077c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00780
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00784
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0078c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00790
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00794
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00798
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0079c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 007a0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007a4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007a8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007ac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007b0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 007b4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 007c0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007c4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 007c8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007cc
			 0x0c780000,                                                  // Leading 1s:                                 /// 007d0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007d4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007d8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007dc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007e0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0x55555555,                                                  // 4 random values                             /// 007f0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007f8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007fc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00800
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00804
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00808
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0080c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00810
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00814
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0081c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00820
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00824
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00828
			 0xbf028f5c,                                                  // -0.51                                       /// 0082c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00834
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00838
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0083c
			 0x7fc00001,                                                  // signaling NaN                               /// 00840
			 0xbf028f5c,                                                  // -0.51                                       /// 00844
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00848
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0084c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00850
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00854
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0085c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00864
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00868
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00870
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00874
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0087c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00880
			 0x0c780000,                                                  // Leading 1s:                                 /// 00884
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00888
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00890
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00894
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00898
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0089c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 008a0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008a4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008a8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008ac
			 0x4b000000,                                                  // 8388608.0                                   /// 008b0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 008b8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 008bc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 008c0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008c4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008c8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008cc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008d0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008d4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 008d8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008dc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 008e4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 008e8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008f0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008f4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00900
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00904
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00908
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0090c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00910
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00918
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0091c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00920
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00924
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00928
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0092c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00930
			 0x80000000,                                                  // -zero                                       /// 00934
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00938
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0093c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00940
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00944
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00948
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0094c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00950
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00954
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00958
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0095c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00964
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00968
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0096c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00970
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00974
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00978
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0097c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00980
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00984
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0098c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00990
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00994
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00998
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0099c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 009a0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009a4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009ac
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009b4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 009bc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009c4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009cc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 009d0
			 0xbf028f5c,                                                  // -0.51                                       /// 009d4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009d8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009dc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 009e0
			 0x4b000000,                                                  // 8388608.0                                   /// 009e4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009e8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009ec
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 009f0
			 0xff800000,                                                  // -inf                                        /// 009f4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009f8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 009fc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a00
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0x55555555,                                                  // 4 random values                             /// 00a08
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a0c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a10
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a14
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a18
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a1c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a20
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a24
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a28
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a34
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a38
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a3c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a40
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a44
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a48
			 0xcb000000,                                                  // -8388608.0                                  /// 00a4c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a50
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a54
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a58
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a5c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a60
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a64
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a68
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a6c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a70
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a74
			 0xff800000,                                                  // -inf                                        /// 00a78
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a84
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0xbf028f5c,                                                  // -0.51                                       /// 00a8c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a90
			 0x00000000,                                                  // zero                                        /// 00a94
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a98
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a9c
			 0x7f800000,                                                  // inf                                         /// 00aa0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00aa4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00aa8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00aac
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ab0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ab4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ab8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00abc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ac0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0xffc00001,                                                  // -signaling NaN                              /// 00ac8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ad4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0x3f028f5c,                                                  // 0.51                                        /// 00adc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ae0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ae4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ae8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00af0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00af4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00af8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b00
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b04
			 0x3f028f5c,                                                  // 0.51                                        /// 00b08
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b0c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b10
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b14
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b18
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b1c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b20
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b24
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b28
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b2c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b30
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b34
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b38
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b40
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b44
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b48
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b4c
			 0x7fc00001,                                                  // signaling NaN                               /// 00b50
			 0x3f028f5c,                                                  // 0.51                                        /// 00b54
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b58
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b5c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b64
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b68
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b70
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b74
			 0x80000000,                                                  // -zero                                       /// 00b78
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b84
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b88
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b8c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b90
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b94
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b98
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b9c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ba4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ba8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00bb0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00bb8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00bbc
			 0xbf028f5c,                                                  // -0.51                                       /// 00bc0
			 0x00000000,                                                  // zero                                        /// 00bc4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00bc8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00bcc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00bd0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00bd4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00bd8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bdc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00be0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be8
			 0x4b000000,                                                  // 8388608.0                                   /// 00bec
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00bf0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00bf8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00bfc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c00
			 0x3f028f5c,                                                  // 0.51                                        /// 00c04
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c08
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c0c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c10
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c18
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c1c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c20
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c24
			 0x55555555,                                                  // 4 random values                             /// 00c28
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c2c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c30
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c34
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c38
			 0x33333333,                                                  // 4 random values                             /// 00c3c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c44
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c48
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c4c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c50
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c54
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c58
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c5c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c60
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c6c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c70
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c74
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c78
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c7c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c80
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c84
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c88
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c8c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c90
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c94
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c98
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c9c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ca0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ca4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ca8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cac
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cbc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00cc4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ccc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00cd0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00cd8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00cdc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ce0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ce4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ce8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00cec
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00cf0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00cf4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00cf8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00cfc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d00
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d04
			 0xff800000,                                                  // -inf                                        /// 00d08
			 0xcb000000,                                                  // -8388608.0                                  /// 00d0c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d18
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d20
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d24
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d28
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d2c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d30
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d34
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d38
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d40
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d44
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d48
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d4c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d50
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d54
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d58
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d5c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d60
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d64
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d68
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d6c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d70
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d74
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d78
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d7c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d80
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d84
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d88
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d8c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d94
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d98
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d9c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00da0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00da4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00db0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00db4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00db8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dbc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00dc4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00dc8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00dcc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00dd0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00dd8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ddc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00de0
			 0x4b000000,                                                  // 8388608.0                                   /// 00de4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00de8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00dec
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00df0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00df8
			 0xbf028f5c,                                                  // -0.51                                       /// 00dfc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e00
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e0c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e10
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e14
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e18
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e1c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0xff800000,                                                  // -inf                                        /// 00e24
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e28
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e2c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e30
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e34
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e38
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e3c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e40
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e44
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e48
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e4c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e50
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e54
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e58
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e5c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e64
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e68
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e6c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e70
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e78
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e7c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e80
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e84
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e88
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e8c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e90
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e94
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e98
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e9c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ea4
			 0x55555555,                                                  // 4 random values                             /// 00ea8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00eb0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00eb4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ebc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ec0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ecc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ed4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00edc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ee0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ee4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ee8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00eec
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ef0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ef4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ef8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00efc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f00
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f04
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f08
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f0c
			 0x3f028f5c,                                                  // 0.51                                        /// 00f10
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f14
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f18
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f1c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f20
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f24
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f28
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f2c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f30
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f34
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f38
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f3c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f40
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f48
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f4c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f50
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f54
			 0x00000000,                                                  // zero                                        /// 00f58
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f5c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f60
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f64
			 0x33333333,                                                  // 4 random values                             /// 00f68
			 0x4b000000,                                                  // 8388608.0                                   /// 00f6c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f70
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f74
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f78
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f7c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f80
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f84
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f88
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f8c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f90
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f98
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f9c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00fa4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fac
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb0
			 0x80000000,                                                  // -zero                                       /// 00fb4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00fbc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00fc4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00fc8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00fcc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00fd0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fd8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0xbf028f5c,                                                  // -0.51                                       /// 00ff0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ff4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ff8
			 0x00080000                                                  // SP - 1 bit alone set in mantissa +ve        /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x17cacdd4, /// 0x0
			 0x19ee56ff, /// 0x4
			 0x840f3a22, /// 0x8
			 0x05d7a1d7, /// 0xc
			 0xaed43053, /// 0x10
			 0xbfbf5ef6, /// 0x14
			 0xfd7b0e13, /// 0x18
			 0x45b7cd82, /// 0x1c
			 0xe4240c3f, /// 0x20
			 0x6fbb7e91, /// 0x24
			 0x8d61b72b, /// 0x28
			 0xe6a738d1, /// 0x2c
			 0xdc1b04c5, /// 0x30
			 0xb7d1b275, /// 0x34
			 0x319deca8, /// 0x38
			 0xa842ee78, /// 0x3c
			 0x4ba618ce, /// 0x40
			 0xf4c6a444, /// 0x44
			 0xcdd4a77d, /// 0x48
			 0x3cc1a7e4, /// 0x4c
			 0x799e4903, /// 0x50
			 0x24c36a3c, /// 0x54
			 0xc29dc992, /// 0x58
			 0x14f4815a, /// 0x5c
			 0xdc9a6623, /// 0x60
			 0x14bd2709, /// 0x64
			 0xed4bfe14, /// 0x68
			 0x4a443705, /// 0x6c
			 0xc3cb9733, /// 0x70
			 0x361eaf8d, /// 0x74
			 0xf130c782, /// 0x78
			 0xb6246508, /// 0x7c
			 0x01a74c3c, /// 0x80
			 0x0cefc80b, /// 0x84
			 0x2fa37d08, /// 0x88
			 0x2a2b3d22, /// 0x8c
			 0x203c7f31, /// 0x90
			 0x6248cba8, /// 0x94
			 0xd6f454ba, /// 0x98
			 0xae92f152, /// 0x9c
			 0x200ee86f, /// 0xa0
			 0x8d115500, /// 0xa4
			 0xf8cfb6f5, /// 0xa8
			 0xa6691e6a, /// 0xac
			 0x275bc9fe, /// 0xb0
			 0x5e280d94, /// 0xb4
			 0x6201ed68, /// 0xb8
			 0xd58ca425, /// 0xbc
			 0x4aab3458, /// 0xc0
			 0xd3c9e2db, /// 0xc4
			 0xe82f51f2, /// 0xc8
			 0xf5d68b1e, /// 0xcc
			 0x3ef41cc0, /// 0xd0
			 0xa3dac8a7, /// 0xd4
			 0xd3e8a842, /// 0xd8
			 0x5ea96760, /// 0xdc
			 0x9377f73b, /// 0xe0
			 0x70a54ef2, /// 0xe4
			 0xa33aa11e, /// 0xe8
			 0x667310a0, /// 0xec
			 0x3a6e7620, /// 0xf0
			 0x7346e992, /// 0xf4
			 0x439b0846, /// 0xf8
			 0xfcf051ab, /// 0xfc
			 0x305eac06, /// 0x100
			 0x9fbd3118, /// 0x104
			 0xbdb41448, /// 0x108
			 0xda684cd6, /// 0x10c
			 0xa691bcba, /// 0x110
			 0x49ac66b4, /// 0x114
			 0xcecbe72c, /// 0x118
			 0x30ab87d7, /// 0x11c
			 0xfc4ee0d1, /// 0x120
			 0x4400ad85, /// 0x124
			 0x1a7f8b1f, /// 0x128
			 0x07c3d3de, /// 0x12c
			 0xbd3a7982, /// 0x130
			 0x2e4c1cfd, /// 0x134
			 0xfe8e5e7c, /// 0x138
			 0xc287281a, /// 0x13c
			 0x4fddb569, /// 0x140
			 0x562ee65b, /// 0x144
			 0xa679f317, /// 0x148
			 0x322bd37e, /// 0x14c
			 0x094cd043, /// 0x150
			 0x16f562ab, /// 0x154
			 0xcb224734, /// 0x158
			 0x3cd5627d, /// 0x15c
			 0x5390b096, /// 0x160
			 0xdbb1da92, /// 0x164
			 0xc5829aeb, /// 0x168
			 0xc368f652, /// 0x16c
			 0x3357409a, /// 0x170
			 0xb66216d9, /// 0x174
			 0x50aee248, /// 0x178
			 0xc523d0e5, /// 0x17c
			 0xde5b0a30, /// 0x180
			 0x0425799c, /// 0x184
			 0xbb9bc47f, /// 0x188
			 0xd43fff2c, /// 0x18c
			 0xcd6609e8, /// 0x190
			 0x1079b4b6, /// 0x194
			 0xee52b843, /// 0x198
			 0xa24a1949, /// 0x19c
			 0x0721926e, /// 0x1a0
			 0xdb2f22e1, /// 0x1a4
			 0x3ea5223e, /// 0x1a8
			 0x1a1109f4, /// 0x1ac
			 0x0763a4f6, /// 0x1b0
			 0x0b696a4a, /// 0x1b4
			 0xafec9f90, /// 0x1b8
			 0x92c0647c, /// 0x1bc
			 0x286c5e40, /// 0x1c0
			 0xb849b96e, /// 0x1c4
			 0x00b900a4, /// 0x1c8
			 0x61505f3a, /// 0x1cc
			 0x99daea04, /// 0x1d0
			 0x4a5db440, /// 0x1d4
			 0xe1003d9b, /// 0x1d8
			 0x705cc073, /// 0x1dc
			 0xfca60452, /// 0x1e0
			 0x4446625a, /// 0x1e4
			 0x1f93fdd0, /// 0x1e8
			 0xdd1ddc0a, /// 0x1ec
			 0xb93ee72f, /// 0x1f0
			 0x4600cfa9, /// 0x1f4
			 0xf5a4d970, /// 0x1f8
			 0xed870a22, /// 0x1fc
			 0x72da67b3, /// 0x200
			 0xb66e96dc, /// 0x204
			 0x460c8920, /// 0x208
			 0x2ccff069, /// 0x20c
			 0xbd458cd0, /// 0x210
			 0xe3fcd678, /// 0x214
			 0x4b1aa18c, /// 0x218
			 0x7f68771a, /// 0x21c
			 0x153861f9, /// 0x220
			 0x0c6a0e15, /// 0x224
			 0x8f262e99, /// 0x228
			 0x3842bbad, /// 0x22c
			 0xedf7a13c, /// 0x230
			 0x0c1a559c, /// 0x234
			 0x508b6ca5, /// 0x238
			 0x1683265a, /// 0x23c
			 0xc6f7c69d, /// 0x240
			 0xee675039, /// 0x244
			 0xbbe32086, /// 0x248
			 0xce81e57e, /// 0x24c
			 0xba3c6b66, /// 0x250
			 0x7e1dd4ba, /// 0x254
			 0xc5d83054, /// 0x258
			 0xb91d1e46, /// 0x25c
			 0x92014ce9, /// 0x260
			 0x297cf9ac, /// 0x264
			 0x3bd6eb24, /// 0x268
			 0x58adccf5, /// 0x26c
			 0xca2a47a0, /// 0x270
			 0x0acf2766, /// 0x274
			 0x2fbdbd64, /// 0x278
			 0x91b100bc, /// 0x27c
			 0x5b224e6e, /// 0x280
			 0xf1e7e7c9, /// 0x284
			 0x36679d0e, /// 0x288
			 0x7610ca27, /// 0x28c
			 0xa11289da, /// 0x290
			 0x82bd3516, /// 0x294
			 0x45feebb8, /// 0x298
			 0x99d8e53d, /// 0x29c
			 0x74d88fea, /// 0x2a0
			 0x3445ca23, /// 0x2a4
			 0x259beae5, /// 0x2a8
			 0xf4092ef7, /// 0x2ac
			 0x15416635, /// 0x2b0
			 0x5222367a, /// 0x2b4
			 0xaba08e07, /// 0x2b8
			 0x7e891297, /// 0x2bc
			 0xcf228cef, /// 0x2c0
			 0x12c3d628, /// 0x2c4
			 0xca392193, /// 0x2c8
			 0xd2d06344, /// 0x2cc
			 0x3b88e351, /// 0x2d0
			 0x9f5565c2, /// 0x2d4
			 0x5be1d2c5, /// 0x2d8
			 0x77504765, /// 0x2dc
			 0x975b3794, /// 0x2e0
			 0x22f800eb, /// 0x2e4
			 0xe07027e9, /// 0x2e8
			 0x3a4ec016, /// 0x2ec
			 0xbfb06b68, /// 0x2f0
			 0xd433f18b, /// 0x2f4
			 0x62ba0784, /// 0x2f8
			 0x70cc1e0f, /// 0x2fc
			 0x5f32917a, /// 0x300
			 0x5433b77d, /// 0x304
			 0xd5cbb900, /// 0x308
			 0xf9cf6842, /// 0x30c
			 0xa708e21f, /// 0x310
			 0x2201ace8, /// 0x314
			 0x34ce2406, /// 0x318
			 0x6af4c5b0, /// 0x31c
			 0x9f108a93, /// 0x320
			 0x38265453, /// 0x324
			 0xf457f987, /// 0x328
			 0x4aa57a8b, /// 0x32c
			 0x6b00dc3f, /// 0x330
			 0x2f920c84, /// 0x334
			 0xa3104150, /// 0x338
			 0x6b26a9b7, /// 0x33c
			 0xa197189b, /// 0x340
			 0xa4361818, /// 0x344
			 0x71126eaa, /// 0x348
			 0x4b74051e, /// 0x34c
			 0xa95dd2d1, /// 0x350
			 0xe357b42d, /// 0x354
			 0x503fe132, /// 0x358
			 0xddcd5a4f, /// 0x35c
			 0x92e3259f, /// 0x360
			 0xe9b44981, /// 0x364
			 0x7b32798b, /// 0x368
			 0xddd4311c, /// 0x36c
			 0x7277a41a, /// 0x370
			 0x28a99c67, /// 0x374
			 0x39e46d3a, /// 0x378
			 0x57bc7065, /// 0x37c
			 0xd4fdb5fe, /// 0x380
			 0xc2a8151b, /// 0x384
			 0xbe0a1040, /// 0x388
			 0xb2fd6020, /// 0x38c
			 0x20e92a3f, /// 0x390
			 0xf3d6dc09, /// 0x394
			 0x40d5c20d, /// 0x398
			 0x0773ffaf, /// 0x39c
			 0x8bb5e51a, /// 0x3a0
			 0xbeadf4c9, /// 0x3a4
			 0xfa304361, /// 0x3a8
			 0xf8dbf398, /// 0x3ac
			 0x69b8a14f, /// 0x3b0
			 0xa66c4274, /// 0x3b4
			 0xee976fee, /// 0x3b8
			 0x7e097264, /// 0x3bc
			 0xd776cb6a, /// 0x3c0
			 0x1632adc7, /// 0x3c4
			 0x8d13e6b9, /// 0x3c8
			 0x3be927af, /// 0x3cc
			 0xb85007e7, /// 0x3d0
			 0xf8255c0d, /// 0x3d4
			 0x54403a0f, /// 0x3d8
			 0xad4cb3fa, /// 0x3dc
			 0xdbe98ae9, /// 0x3e0
			 0x69d60229, /// 0x3e4
			 0xcbe40bda, /// 0x3e8
			 0xf7f5328c, /// 0x3ec
			 0xf3386428, /// 0x3f0
			 0x2e8abb62, /// 0x3f4
			 0x3743c628, /// 0x3f8
			 0x001b361b, /// 0x3fc
			 0x5f1462c7, /// 0x400
			 0x99980677, /// 0x404
			 0xabd27a0d, /// 0x408
			 0x5859e672, /// 0x40c
			 0x8cf601f0, /// 0x410
			 0x833f7101, /// 0x414
			 0x1a11a73e, /// 0x418
			 0xfe8344e9, /// 0x41c
			 0xeed9876b, /// 0x420
			 0x97793bd1, /// 0x424
			 0xc23ab98b, /// 0x428
			 0x691cbca0, /// 0x42c
			 0xe2b35d07, /// 0x430
			 0xff8ba4f3, /// 0x434
			 0x7f8b6823, /// 0x438
			 0x4876b524, /// 0x43c
			 0x2bc8c06c, /// 0x440
			 0x0edcb404, /// 0x444
			 0x97a67d16, /// 0x448
			 0x7916e3fc, /// 0x44c
			 0x1d7882c5, /// 0x450
			 0x5b435be7, /// 0x454
			 0x0c70e169, /// 0x458
			 0xda92f6b6, /// 0x45c
			 0x9810f032, /// 0x460
			 0x95c9b189, /// 0x464
			 0x00eff87f, /// 0x468
			 0x9a47e270, /// 0x46c
			 0x9ff9fbf4, /// 0x470
			 0x93bea477, /// 0x474
			 0xe554803f, /// 0x478
			 0x53d93ba8, /// 0x47c
			 0x7c941139, /// 0x480
			 0x08126bf1, /// 0x484
			 0x0eeff1cd, /// 0x488
			 0xa1a8ca83, /// 0x48c
			 0xebcc153e, /// 0x490
			 0x7c183dfd, /// 0x494
			 0x44675abf, /// 0x498
			 0xdd61c015, /// 0x49c
			 0xa39c9840, /// 0x4a0
			 0x00045fad, /// 0x4a4
			 0xe934ccda, /// 0x4a8
			 0x58f20455, /// 0x4ac
			 0xbc5af9f6, /// 0x4b0
			 0xa891a13a, /// 0x4b4
			 0x6246c312, /// 0x4b8
			 0xcbb35643, /// 0x4bc
			 0x634e4c92, /// 0x4c0
			 0x86c85a64, /// 0x4c4
			 0x8c66b880, /// 0x4c8
			 0x4b63ea7d, /// 0x4cc
			 0x9fe7f7f8, /// 0x4d0
			 0x68145819, /// 0x4d4
			 0x2e3779bc, /// 0x4d8
			 0xa44b1e89, /// 0x4dc
			 0x2ebe898d, /// 0x4e0
			 0x0a7edd70, /// 0x4e4
			 0x08ac00f0, /// 0x4e8
			 0x6617f2f3, /// 0x4ec
			 0xb6b5bbce, /// 0x4f0
			 0xcf9c139e, /// 0x4f4
			 0xea81a1aa, /// 0x4f8
			 0xcbba9868, /// 0x4fc
			 0x4f1d6fff, /// 0x500
			 0xbb54a6eb, /// 0x504
			 0xb104ca6a, /// 0x508
			 0xa84bb95f, /// 0x50c
			 0x9477c741, /// 0x510
			 0x38fcd57f, /// 0x514
			 0x04eda960, /// 0x518
			 0x8167f769, /// 0x51c
			 0x7686e756, /// 0x520
			 0xce6e8417, /// 0x524
			 0x9f8e890d, /// 0x528
			 0x96b1666c, /// 0x52c
			 0xc3a9cf04, /// 0x530
			 0xfe35b02e, /// 0x534
			 0x391b29a8, /// 0x538
			 0x5d4560b0, /// 0x53c
			 0x3d82bda2, /// 0x540
			 0xe7f12b4d, /// 0x544
			 0x505fc3ba, /// 0x548
			 0xb5ddfeab, /// 0x54c
			 0xde0761ae, /// 0x550
			 0xea71a563, /// 0x554
			 0x4545c19b, /// 0x558
			 0x34036e7c, /// 0x55c
			 0x652e8341, /// 0x560
			 0xf2d0a263, /// 0x564
			 0x9020ce1c, /// 0x568
			 0xb4881e0f, /// 0x56c
			 0xe6706a3b, /// 0x570
			 0x66e53db0, /// 0x574
			 0xfcb1bcad, /// 0x578
			 0x65d811d3, /// 0x57c
			 0x92ca806f, /// 0x580
			 0x2712b198, /// 0x584
			 0x49d94fb0, /// 0x588
			 0x97bcf406, /// 0x58c
			 0xc03c8066, /// 0x590
			 0x552fb6a0, /// 0x594
			 0x77d8d00d, /// 0x598
			 0xabb395e9, /// 0x59c
			 0x43c0dc35, /// 0x5a0
			 0x4140a92e, /// 0x5a4
			 0xf1fda010, /// 0x5a8
			 0x69529d0c, /// 0x5ac
			 0x386a84f2, /// 0x5b0
			 0x84004469, /// 0x5b4
			 0xb2cf9b4b, /// 0x5b8
			 0x92eb27f8, /// 0x5bc
			 0xce60bb8b, /// 0x5c0
			 0xb177ef5b, /// 0x5c4
			 0xb6a7272a, /// 0x5c8
			 0x77eb6baa, /// 0x5cc
			 0x6409c717, /// 0x5d0
			 0xde4719db, /// 0x5d4
			 0x50056b2a, /// 0x5d8
			 0x2afef4c8, /// 0x5dc
			 0x71438eff, /// 0x5e0
			 0x1285fb5d, /// 0x5e4
			 0x458e8758, /// 0x5e8
			 0x82922f8d, /// 0x5ec
			 0x0b81797c, /// 0x5f0
			 0x23d12df4, /// 0x5f4
			 0xfcfcf4d4, /// 0x5f8
			 0x303168ba, /// 0x5fc
			 0xf2eb76d1, /// 0x600
			 0xd4ab13f9, /// 0x604
			 0x930f290c, /// 0x608
			 0xcd14ee89, /// 0x60c
			 0xb15971ef, /// 0x610
			 0x4eec43d7, /// 0x614
			 0x56be9170, /// 0x618
			 0xba586127, /// 0x61c
			 0x3b12771d, /// 0x620
			 0x8d2f78f1, /// 0x624
			 0xf39aacef, /// 0x628
			 0x23b1c17e, /// 0x62c
			 0x709a8f12, /// 0x630
			 0x007c1b4e, /// 0x634
			 0xf439889c, /// 0x638
			 0x49e50573, /// 0x63c
			 0x25eb7a3e, /// 0x640
			 0x4be39830, /// 0x644
			 0xb09efff8, /// 0x648
			 0x4b1bd7d2, /// 0x64c
			 0xe68b7a48, /// 0x650
			 0xeee170ac, /// 0x654
			 0x928cdc4b, /// 0x658
			 0xff4cd04f, /// 0x65c
			 0x86167b88, /// 0x660
			 0x4e891ce1, /// 0x664
			 0x6cd12821, /// 0x668
			 0xf35f9dde, /// 0x66c
			 0x581586fa, /// 0x670
			 0x36910a7c, /// 0x674
			 0xe33af34e, /// 0x678
			 0x75a4a685, /// 0x67c
			 0xb04de334, /// 0x680
			 0x36fd75ee, /// 0x684
			 0xe029cfa7, /// 0x688
			 0x4be3149a, /// 0x68c
			 0xf62a301a, /// 0x690
			 0x4c0377b1, /// 0x694
			 0xe7e76ead, /// 0x698
			 0x49d1eb45, /// 0x69c
			 0x06ac0444, /// 0x6a0
			 0x15e45370, /// 0x6a4
			 0xd7cd489c, /// 0x6a8
			 0x0873c981, /// 0x6ac
			 0x9f4969f6, /// 0x6b0
			 0x2e5b0e01, /// 0x6b4
			 0x2682cffc, /// 0x6b8
			 0x91182183, /// 0x6bc
			 0xbc7b6aba, /// 0x6c0
			 0x0d4b2af6, /// 0x6c4
			 0x417eea81, /// 0x6c8
			 0xc9570aa7, /// 0x6cc
			 0x42cee58b, /// 0x6d0
			 0xc2d247c5, /// 0x6d4
			 0xab4f54ee, /// 0x6d8
			 0xfe752cf7, /// 0x6dc
			 0x2f875128, /// 0x6e0
			 0xd1358a4e, /// 0x6e4
			 0x52ad3fe2, /// 0x6e8
			 0xe6816f80, /// 0x6ec
			 0x3a2a4845, /// 0x6f0
			 0x21a8f3f2, /// 0x6f4
			 0x1567ca53, /// 0x6f8
			 0xf3b465e8, /// 0x6fc
			 0xff9b54af, /// 0x700
			 0xdf2a899f, /// 0x704
			 0xaf152d25, /// 0x708
			 0x31a43f39, /// 0x70c
			 0x2b03b9ad, /// 0x710
			 0x5af05911, /// 0x714
			 0xa8e6a554, /// 0x718
			 0x50a45cc4, /// 0x71c
			 0xd4b28c8f, /// 0x720
			 0x57579891, /// 0x724
			 0x4bb22b67, /// 0x728
			 0x16ad40a0, /// 0x72c
			 0xf497bd26, /// 0x730
			 0xb57d9161, /// 0x734
			 0x69bc6827, /// 0x738
			 0xa97b2950, /// 0x73c
			 0xdf93a3db, /// 0x740
			 0x132f78dc, /// 0x744
			 0x4ad2030b, /// 0x748
			 0xd53a4526, /// 0x74c
			 0xaa3e2420, /// 0x750
			 0xd382b055, /// 0x754
			 0x44a88c2e, /// 0x758
			 0x0006def4, /// 0x75c
			 0x93287fdf, /// 0x760
			 0x655e96ec, /// 0x764
			 0x46f97962, /// 0x768
			 0x2fc359cb, /// 0x76c
			 0xa438bc99, /// 0x770
			 0x8c0648c5, /// 0x774
			 0x386b0432, /// 0x778
			 0x1c82c59a, /// 0x77c
			 0xc659c628, /// 0x780
			 0x6d5508b1, /// 0x784
			 0xe6f3b044, /// 0x788
			 0x5ddc7e13, /// 0x78c
			 0x955db1e4, /// 0x790
			 0xd846a303, /// 0x794
			 0xdcc28663, /// 0x798
			 0xddc9dcc9, /// 0x79c
			 0x2cc99b08, /// 0x7a0
			 0x062609ab, /// 0x7a4
			 0x930140cb, /// 0x7a8
			 0x8c29ae68, /// 0x7ac
			 0x64902897, /// 0x7b0
			 0xff6aeb13, /// 0x7b4
			 0xed8eae56, /// 0x7b8
			 0xab385bdb, /// 0x7bc
			 0x5bf129cc, /// 0x7c0
			 0xcb08b347, /// 0x7c4
			 0x8453393b, /// 0x7c8
			 0x872373e6, /// 0x7cc
			 0x7325d3cd, /// 0x7d0
			 0x4b3686ca, /// 0x7d4
			 0xc9897d38, /// 0x7d8
			 0xa0033dda, /// 0x7dc
			 0xab09f644, /// 0x7e0
			 0xf0f7cd44, /// 0x7e4
			 0x4877c7aa, /// 0x7e8
			 0xf2a5e0b0, /// 0x7ec
			 0x43cc8599, /// 0x7f0
			 0x92b0be7b, /// 0x7f4
			 0x5b4923cc, /// 0x7f8
			 0x7c771079, /// 0x7fc
			 0x02c8a6d4, /// 0x800
			 0xb339d00b, /// 0x804
			 0x72aa6efd, /// 0x808
			 0xc10cc8c3, /// 0x80c
			 0x008d4ed9, /// 0x810
			 0x5fc2057a, /// 0x814
			 0xc1095bde, /// 0x818
			 0x926c8a31, /// 0x81c
			 0xf34ef944, /// 0x820
			 0xb48e1d56, /// 0x824
			 0xdcee63c1, /// 0x828
			 0x00ba2d73, /// 0x82c
			 0x9d6f87fa, /// 0x830
			 0xc78922fe, /// 0x834
			 0x7fe1a576, /// 0x838
			 0xe86c5baf, /// 0x83c
			 0x12e3a3f3, /// 0x840
			 0x5b68dabd, /// 0x844
			 0x2bfdaea2, /// 0x848
			 0xc33028be, /// 0x84c
			 0x0db8b014, /// 0x850
			 0x44c8d475, /// 0x854
			 0x528408b2, /// 0x858
			 0xfd6b38f0, /// 0x85c
			 0x8aaab122, /// 0x860
			 0x9700dd85, /// 0x864
			 0xb6b41eee, /// 0x868
			 0x0b3603d2, /// 0x86c
			 0x7eb84929, /// 0x870
			 0xfdb9f2e8, /// 0x874
			 0xa1ece363, /// 0x878
			 0x0fe3626e, /// 0x87c
			 0x1ed2c509, /// 0x880
			 0xee46b13a, /// 0x884
			 0x9c25c4db, /// 0x888
			 0x1aed72e5, /// 0x88c
			 0x45f44a82, /// 0x890
			 0x42b2612f, /// 0x894
			 0xd28eaf96, /// 0x898
			 0x451ce9f9, /// 0x89c
			 0x496ca79d, /// 0x8a0
			 0xcc466acf, /// 0x8a4
			 0xdd168cb5, /// 0x8a8
			 0x74d91bde, /// 0x8ac
			 0xaaf9f123, /// 0x8b0
			 0x22c0c6f9, /// 0x8b4
			 0xe5b31597, /// 0x8b8
			 0x5871bd0f, /// 0x8bc
			 0x6208de35, /// 0x8c0
			 0xf49cd610, /// 0x8c4
			 0xc7634893, /// 0x8c8
			 0xad648bf4, /// 0x8cc
			 0x24a231f0, /// 0x8d0
			 0x582e6e37, /// 0x8d4
			 0xb10d40f4, /// 0x8d8
			 0xea3e1d9e, /// 0x8dc
			 0x9c4b47b4, /// 0x8e0
			 0xa8f3c1a7, /// 0x8e4
			 0x0fafd805, /// 0x8e8
			 0x1faac5d9, /// 0x8ec
			 0xd1a49171, /// 0x8f0
			 0x75a060b7, /// 0x8f4
			 0xd28f5c80, /// 0x8f8
			 0x7fe3df72, /// 0x8fc
			 0x504781f3, /// 0x900
			 0xa9b6713f, /// 0x904
			 0x833cf359, /// 0x908
			 0x8d4e0ec0, /// 0x90c
			 0xf10ee1c5, /// 0x910
			 0xc6cfbcfb, /// 0x914
			 0x3ca63037, /// 0x918
			 0xb12bc027, /// 0x91c
			 0x8f9a8606, /// 0x920
			 0x63e8461a, /// 0x924
			 0xd59212c4, /// 0x928
			 0xcaba5cc8, /// 0x92c
			 0x2d530678, /// 0x930
			 0xcbf67d3e, /// 0x934
			 0x9f9185da, /// 0x938
			 0x4ea35107, /// 0x93c
			 0x8fe301e0, /// 0x940
			 0x20871afb, /// 0x944
			 0x03fcebb6, /// 0x948
			 0xc382b4e3, /// 0x94c
			 0x1212b3af, /// 0x950
			 0xf0aa8530, /// 0x954
			 0x598045e6, /// 0x958
			 0x6ef27502, /// 0x95c
			 0x04e7bbc2, /// 0x960
			 0xbae81fe4, /// 0x964
			 0x3f4e24bd, /// 0x968
			 0x179b8b74, /// 0x96c
			 0xa9bdc15c, /// 0x970
			 0x936d20bf, /// 0x974
			 0xe847c34f, /// 0x978
			 0x3edeb6b7, /// 0x97c
			 0x28d1d557, /// 0x980
			 0x83187650, /// 0x984
			 0xe9c8b1cf, /// 0x988
			 0x3f83e79e, /// 0x98c
			 0xbc1e0e5a, /// 0x990
			 0x18a32265, /// 0x994
			 0xdf43bf7b, /// 0x998
			 0x81d2a721, /// 0x99c
			 0xecf123dd, /// 0x9a0
			 0x5af5307f, /// 0x9a4
			 0x743eeb68, /// 0x9a8
			 0xf20e9ca9, /// 0x9ac
			 0x4508dd83, /// 0x9b0
			 0x20aac99e, /// 0x9b4
			 0x1c002dca, /// 0x9b8
			 0xc81e26c6, /// 0x9bc
			 0x5a6e5cc2, /// 0x9c0
			 0x4b7cd6f5, /// 0x9c4
			 0x9f267f1e, /// 0x9c8
			 0xe222fb86, /// 0x9cc
			 0x020700d9, /// 0x9d0
			 0xca3472af, /// 0x9d4
			 0x3ad33edc, /// 0x9d8
			 0xe579d745, /// 0x9dc
			 0x0a5b7889, /// 0x9e0
			 0xfe250f26, /// 0x9e4
			 0x250545a5, /// 0x9e8
			 0x241e0967, /// 0x9ec
			 0xb0fcb6ea, /// 0x9f0
			 0xc8775ff2, /// 0x9f4
			 0x140907ad, /// 0x9f8
			 0xbc483a29, /// 0x9fc
			 0x5dd47ae8, /// 0xa00
			 0x5c6136d1, /// 0xa04
			 0xf686316c, /// 0xa08
			 0x630470c4, /// 0xa0c
			 0x115ccf43, /// 0xa10
			 0xfd884aea, /// 0xa14
			 0x32fc8168, /// 0xa18
			 0xbf0aae95, /// 0xa1c
			 0xcd5ae5d3, /// 0xa20
			 0xb5f19cf2, /// 0xa24
			 0xc4d1b491, /// 0xa28
			 0x50c4642c, /// 0xa2c
			 0xcc285d98, /// 0xa30
			 0xd9e53fd3, /// 0xa34
			 0xe7ee79e2, /// 0xa38
			 0x2d120b04, /// 0xa3c
			 0xdb5dc0d7, /// 0xa40
			 0x63592af7, /// 0xa44
			 0x4a3886f7, /// 0xa48
			 0x0841777d, /// 0xa4c
			 0xb0390890, /// 0xa50
			 0x266380db, /// 0xa54
			 0xf2ff7288, /// 0xa58
			 0x786344e6, /// 0xa5c
			 0x4f121b56, /// 0xa60
			 0xa0400c93, /// 0xa64
			 0x192ab1ad, /// 0xa68
			 0xe50d67e0, /// 0xa6c
			 0xb2df1cd8, /// 0xa70
			 0xb62956f7, /// 0xa74
			 0xf062fb12, /// 0xa78
			 0x9c8311fb, /// 0xa7c
			 0xff6f94b9, /// 0xa80
			 0xaee072d8, /// 0xa84
			 0x87d242de, /// 0xa88
			 0xb7e602b9, /// 0xa8c
			 0x3b6d8bb7, /// 0xa90
			 0x19fb9122, /// 0xa94
			 0x8603439d, /// 0xa98
			 0x488cb44a, /// 0xa9c
			 0x8f1cbaaa, /// 0xaa0
			 0xe644d2b3, /// 0xaa4
			 0xd90ea396, /// 0xaa8
			 0xff9dc1a3, /// 0xaac
			 0x5a8ac94f, /// 0xab0
			 0x3ed7e003, /// 0xab4
			 0x8a5b311d, /// 0xab8
			 0x8ab22675, /// 0xabc
			 0x98cc4445, /// 0xac0
			 0x8a75efdb, /// 0xac4
			 0x2204b6e9, /// 0xac8
			 0x461bac88, /// 0xacc
			 0x78e36228, /// 0xad0
			 0x6d58dd4f, /// 0xad4
			 0xe54caf54, /// 0xad8
			 0x2bf8f51f, /// 0xadc
			 0xcd0c8964, /// 0xae0
			 0x153e7206, /// 0xae4
			 0x657ef0b6, /// 0xae8
			 0x31cda005, /// 0xaec
			 0x7f13d0cf, /// 0xaf0
			 0xf2c5bcff, /// 0xaf4
			 0x2c177700, /// 0xaf8
			 0x4528db98, /// 0xafc
			 0xb2af92e9, /// 0xb00
			 0xc48f08ef, /// 0xb04
			 0x9de5178b, /// 0xb08
			 0x99980dec, /// 0xb0c
			 0x204908b5, /// 0xb10
			 0x8613dfcc, /// 0xb14
			 0x21c5d6c9, /// 0xb18
			 0xa60e1e34, /// 0xb1c
			 0xdfd1dcde, /// 0xb20
			 0x1438252c, /// 0xb24
			 0xdd03759a, /// 0xb28
			 0x9fe66ad0, /// 0xb2c
			 0x243ec927, /// 0xb30
			 0x799094f4, /// 0xb34
			 0xfd614f93, /// 0xb38
			 0x8c6b3233, /// 0xb3c
			 0x26b73b54, /// 0xb40
			 0x017d6414, /// 0xb44
			 0xedc6c045, /// 0xb48
			 0x19992781, /// 0xb4c
			 0x6d22f836, /// 0xb50
			 0x84f15682, /// 0xb54
			 0xb74f773a, /// 0xb58
			 0x433d2bf7, /// 0xb5c
			 0x68c7a043, /// 0xb60
			 0xbb4ee0b2, /// 0xb64
			 0x104d4b6a, /// 0xb68
			 0x7570e211, /// 0xb6c
			 0x21ea7e7a, /// 0xb70
			 0xf202cc73, /// 0xb74
			 0xbe8ecca8, /// 0xb78
			 0xa4e8d8a4, /// 0xb7c
			 0x1af4c7c7, /// 0xb80
			 0x331153bf, /// 0xb84
			 0x3b4a3372, /// 0xb88
			 0x7d8da164, /// 0xb8c
			 0x26a4a273, /// 0xb90
			 0xd6c428f3, /// 0xb94
			 0x702eef37, /// 0xb98
			 0x0b20ce30, /// 0xb9c
			 0xd62b8356, /// 0xba0
			 0x4e67de59, /// 0xba4
			 0x46fbca00, /// 0xba8
			 0x93827e1b, /// 0xbac
			 0xa7daf483, /// 0xbb0
			 0xcf6908ee, /// 0xbb4
			 0xbb8ba7f0, /// 0xbb8
			 0x91b1cb18, /// 0xbbc
			 0x58245fa9, /// 0xbc0
			 0x4ed95a9a, /// 0xbc4
			 0xf2dd39f1, /// 0xbc8
			 0x0070e361, /// 0xbcc
			 0x9711acd4, /// 0xbd0
			 0x97a4078b, /// 0xbd4
			 0xea33e66c, /// 0xbd8
			 0x79dafb5c, /// 0xbdc
			 0x296e1f24, /// 0xbe0
			 0xc5e638d5, /// 0xbe4
			 0x669e5c30, /// 0xbe8
			 0xd413e04b, /// 0xbec
			 0x3d5a8247, /// 0xbf0
			 0x2781cdc4, /// 0xbf4
			 0xe6d86062, /// 0xbf8
			 0x7d46458e, /// 0xbfc
			 0x786bd450, /// 0xc00
			 0x4b6b5f58, /// 0xc04
			 0x8d100950, /// 0xc08
			 0xfc47488d, /// 0xc0c
			 0x3727bfa6, /// 0xc10
			 0xf837eb08, /// 0xc14
			 0xd4224b22, /// 0xc18
			 0x34a4e776, /// 0xc1c
			 0x652ba345, /// 0xc20
			 0x134b211e, /// 0xc24
			 0x7a974471, /// 0xc28
			 0x410afde6, /// 0xc2c
			 0x8769193c, /// 0xc30
			 0x4382228e, /// 0xc34
			 0xde82f121, /// 0xc38
			 0xf04ac0ff, /// 0xc3c
			 0xd538a6a4, /// 0xc40
			 0xc6f5fb7e, /// 0xc44
			 0xfede5ee9, /// 0xc48
			 0xd0691a9c, /// 0xc4c
			 0xcd711118, /// 0xc50
			 0x3e9974a7, /// 0xc54
			 0x4666fe6a, /// 0xc58
			 0x823110e4, /// 0xc5c
			 0x8739e09d, /// 0xc60
			 0x9ac3d650, /// 0xc64
			 0x4308ff23, /// 0xc68
			 0xb4310643, /// 0xc6c
			 0x265e5c22, /// 0xc70
			 0x44105ac7, /// 0xc74
			 0xd99ce7af, /// 0xc78
			 0x943335e6, /// 0xc7c
			 0x6764484a, /// 0xc80
			 0x61d8d900, /// 0xc84
			 0xb7a954f0, /// 0xc88
			 0x1f5fb647, /// 0xc8c
			 0x50bd3c78, /// 0xc90
			 0xf72793a2, /// 0xc94
			 0x83facf1c, /// 0xc98
			 0x1dd1aaa8, /// 0xc9c
			 0x50b2fc77, /// 0xca0
			 0x6d412a49, /// 0xca4
			 0xbaa1e1c7, /// 0xca8
			 0x762c81f4, /// 0xcac
			 0x8c9fa927, /// 0xcb0
			 0x28f2dbcf, /// 0xcb4
			 0x25025a5d, /// 0xcb8
			 0x5d4848ad, /// 0xcbc
			 0xf5953b66, /// 0xcc0
			 0x62a26ae0, /// 0xcc4
			 0xce1a5883, /// 0xcc8
			 0x04b2bc4e, /// 0xccc
			 0xc93a4cf0, /// 0xcd0
			 0x035764a0, /// 0xcd4
			 0xcc37a17d, /// 0xcd8
			 0x6ccc6bf1, /// 0xcdc
			 0x48c35370, /// 0xce0
			 0xc708bf7d, /// 0xce4
			 0xd1797d89, /// 0xce8
			 0x01b3017b, /// 0xcec
			 0x5beab2cb, /// 0xcf0
			 0x24e2bd00, /// 0xcf4
			 0xc28c09ab, /// 0xcf8
			 0x5d6b96e1, /// 0xcfc
			 0x56dbcd27, /// 0xd00
			 0xc9c1b19a, /// 0xd04
			 0xc58caca4, /// 0xd08
			 0xc3578b95, /// 0xd0c
			 0x208eeadc, /// 0xd10
			 0x641f7861, /// 0xd14
			 0x3f3c5784, /// 0xd18
			 0xb21462cc, /// 0xd1c
			 0x0a9f5b47, /// 0xd20
			 0xa3b41eb7, /// 0xd24
			 0xbe9a710b, /// 0xd28
			 0x3762534c, /// 0xd2c
			 0x25dbc198, /// 0xd30
			 0xc8b838d0, /// 0xd34
			 0x0dda31a8, /// 0xd38
			 0x51ff0ef5, /// 0xd3c
			 0x079f0b54, /// 0xd40
			 0xf7cea05d, /// 0xd44
			 0x388158e5, /// 0xd48
			 0x242c34bb, /// 0xd4c
			 0x9185ebe3, /// 0xd50
			 0x8b09c58d, /// 0xd54
			 0xbd3f0b2a, /// 0xd58
			 0x8870853f, /// 0xd5c
			 0xc6a32f24, /// 0xd60
			 0xe7e3db63, /// 0xd64
			 0xceb395e3, /// 0xd68
			 0xab7bf499, /// 0xd6c
			 0x94aa7b18, /// 0xd70
			 0x94a73508, /// 0xd74
			 0x974d2144, /// 0xd78
			 0xb5ffae76, /// 0xd7c
			 0xbb7317d5, /// 0xd80
			 0xa47426be, /// 0xd84
			 0xf708c6ae, /// 0xd88
			 0xb7325d8b, /// 0xd8c
			 0xe8c4fc35, /// 0xd90
			 0x46d99b92, /// 0xd94
			 0xdc22e8ba, /// 0xd98
			 0xb9e851ad, /// 0xd9c
			 0x5729ec33, /// 0xda0
			 0xf79fbc68, /// 0xda4
			 0x62689199, /// 0xda8
			 0x93d2be45, /// 0xdac
			 0x952e04b6, /// 0xdb0
			 0xd59d6848, /// 0xdb4
			 0x40b86042, /// 0xdb8
			 0x3cf1682f, /// 0xdbc
			 0x47644fa6, /// 0xdc0
			 0x09640000, /// 0xdc4
			 0x1aca1ce7, /// 0xdc8
			 0x2447679c, /// 0xdcc
			 0x31e451ae, /// 0xdd0
			 0x1cf609c7, /// 0xdd4
			 0xf375fdf3, /// 0xdd8
			 0xb3272742, /// 0xddc
			 0xd7da0bed, /// 0xde0
			 0x79cefd6e, /// 0xde4
			 0x56389175, /// 0xde8
			 0x763051b4, /// 0xdec
			 0xb2def2ab, /// 0xdf0
			 0xbd1b9d0f, /// 0xdf4
			 0x81e7bfe1, /// 0xdf8
			 0x8e0cd1d0, /// 0xdfc
			 0xde096f74, /// 0xe00
			 0x16b52664, /// 0xe04
			 0x57f7b8d8, /// 0xe08
			 0xe1addc67, /// 0xe0c
			 0x7c63121d, /// 0xe10
			 0x7f0c8815, /// 0xe14
			 0x48606ffc, /// 0xe18
			 0x0c91c342, /// 0xe1c
			 0x07a5f78b, /// 0xe20
			 0x9cd1f51c, /// 0xe24
			 0x3a8c7c60, /// 0xe28
			 0x9c26fe35, /// 0xe2c
			 0x2a4bda70, /// 0xe30
			 0xb40b7b7d, /// 0xe34
			 0x6b17584e, /// 0xe38
			 0x16e073c2, /// 0xe3c
			 0x82b1e5a9, /// 0xe40
			 0xe39796ec, /// 0xe44
			 0xbd596927, /// 0xe48
			 0xd40fa4c4, /// 0xe4c
			 0x25e8aa63, /// 0xe50
			 0xcf9c8c3a, /// 0xe54
			 0x0e5602b1, /// 0xe58
			 0x2f4c902c, /// 0xe5c
			 0xc450142a, /// 0xe60
			 0xb498ff4f, /// 0xe64
			 0x219570da, /// 0xe68
			 0xa147c485, /// 0xe6c
			 0x205d4458, /// 0xe70
			 0xf7cf11c6, /// 0xe74
			 0x0640774d, /// 0xe78
			 0xf71c826a, /// 0xe7c
			 0x244216e7, /// 0xe80
			 0x8f9764aa, /// 0xe84
			 0x3d84c06c, /// 0xe88
			 0xe79db1a3, /// 0xe8c
			 0xf08b3d9e, /// 0xe90
			 0x26ce833c, /// 0xe94
			 0xf4a3eae7, /// 0xe98
			 0xa8476c56, /// 0xe9c
			 0xecd45480, /// 0xea0
			 0xe47d5bf7, /// 0xea4
			 0xf55e9a6b, /// 0xea8
			 0xe738c364, /// 0xeac
			 0xe28d717a, /// 0xeb0
			 0x7043bc9b, /// 0xeb4
			 0x57e1c832, /// 0xeb8
			 0x8b28e4b7, /// 0xebc
			 0x41892575, /// 0xec0
			 0x25155f38, /// 0xec4
			 0x4c959100, /// 0xec8
			 0x87df5bc2, /// 0xecc
			 0xccad4f9b, /// 0xed0
			 0x55b436f8, /// 0xed4
			 0xb587cbd4, /// 0xed8
			 0xb8bde6e8, /// 0xedc
			 0xd521ae3d, /// 0xee0
			 0x50e83f6b, /// 0xee4
			 0x8a71bd59, /// 0xee8
			 0x873e9371, /// 0xeec
			 0xe43f9466, /// 0xef0
			 0xf5d2609d, /// 0xef4
			 0x222c5532, /// 0xef8
			 0x6073d25d, /// 0xefc
			 0x1b5a50d1, /// 0xf00
			 0x34706cd9, /// 0xf04
			 0x1d5c2aab, /// 0xf08
			 0x4ba3c7ee, /// 0xf0c
			 0x497c5fd6, /// 0xf10
			 0xc4fc2f43, /// 0xf14
			 0x54e27995, /// 0xf18
			 0xf9163e58, /// 0xf1c
			 0x4e3b06a4, /// 0xf20
			 0xb4dbf258, /// 0xf24
			 0x433b4f37, /// 0xf28
			 0xe3a36b34, /// 0xf2c
			 0x6a71c78d, /// 0xf30
			 0xcafff276, /// 0xf34
			 0xf5c9cfe2, /// 0xf38
			 0x7272d55c, /// 0xf3c
			 0xb902627b, /// 0xf40
			 0xb8bee1ea, /// 0xf44
			 0x83656b44, /// 0xf48
			 0x37b5db90, /// 0xf4c
			 0x74ef3d07, /// 0xf50
			 0x2faedfc2, /// 0xf54
			 0xdff62879, /// 0xf58
			 0x753f073f, /// 0xf5c
			 0x608a1b6e, /// 0xf60
			 0xe1303c63, /// 0xf64
			 0xc3643fa0, /// 0xf68
			 0x2a99b090, /// 0xf6c
			 0x6a4a74fd, /// 0xf70
			 0x0eb25b8b, /// 0xf74
			 0xb3cc5ab4, /// 0xf78
			 0xb6cf11e7, /// 0xf7c
			 0x506a9ce6, /// 0xf80
			 0x9a88d28c, /// 0xf84
			 0xc3d042e1, /// 0xf88
			 0x8d28af01, /// 0xf8c
			 0xd1dce164, /// 0xf90
			 0x858acb8e, /// 0xf94
			 0xccb0b9b7, /// 0xf98
			 0x06f8cb24, /// 0xf9c
			 0x60502872, /// 0xfa0
			 0xb9e0a1c5, /// 0xfa4
			 0xc435bbe7, /// 0xfa8
			 0x34021ebf, /// 0xfac
			 0x55e92c89, /// 0xfb0
			 0xc2a1c230, /// 0xfb4
			 0x251f338f, /// 0xfb8
			 0x4ea19535, /// 0xfbc
			 0x0bc2e536, /// 0xfc0
			 0x06767d89, /// 0xfc4
			 0x4f9cbba1, /// 0xfc8
			 0xcbbe0fab, /// 0xfcc
			 0xc0f3f4e3, /// 0xfd0
			 0x3e6432d9, /// 0xfd4
			 0xa5d620c7, /// 0xfd8
			 0x296a2d5b, /// 0xfdc
			 0x125b2e83, /// 0xfe0
			 0xdbd5bbe4, /// 0xfe4
			 0xd0719776, /// 0xfe8
			 0xbf445a1d, /// 0xfec
			 0xa22c1e15, /// 0xff0
			 0x5a1e09d1, /// 0xff4
			 0x2b421645, /// 0xff8
			 0xd135e6be /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0xe9ddd785, /// 0x0
			 0xfd7c9356, /// 0x4
			 0x4c132f8c, /// 0x8
			 0xc062d98b, /// 0xc
			 0x02929cac, /// 0x10
			 0x3a39d114, /// 0x14
			 0xba91fb8d, /// 0x18
			 0x3b604475, /// 0x1c
			 0xf2a9bcfb, /// 0x20
			 0xe1b05e6b, /// 0x24
			 0x1ad817fc, /// 0x28
			 0xb3394d46, /// 0x2c
			 0xd7dfe953, /// 0x30
			 0x5347e88d, /// 0x34
			 0xdbf11fac, /// 0x38
			 0x93859a57, /// 0x3c
			 0x08bb59d0, /// 0x40
			 0xff3688df, /// 0x44
			 0x4b119c6b, /// 0x48
			 0xc921cfcd, /// 0x4c
			 0xb3384f0c, /// 0x50
			 0xbe5d98b3, /// 0x54
			 0x51f3349e, /// 0x58
			 0x9ccb7af4, /// 0x5c
			 0xd9181dd1, /// 0x60
			 0x83f0df65, /// 0x64
			 0xb3c82ad5, /// 0x68
			 0xd8ac9265, /// 0x6c
			 0xacb858e9, /// 0x70
			 0x0af221dd, /// 0x74
			 0x9f55fd32, /// 0x78
			 0x5cc0f855, /// 0x7c
			 0xf4498c5b, /// 0x80
			 0x51974ca0, /// 0x84
			 0xf14b1172, /// 0x88
			 0xfe4f4a7f, /// 0x8c
			 0x8b653a6c, /// 0x90
			 0x6626c0d6, /// 0x94
			 0xf841374e, /// 0x98
			 0x56cc5bde, /// 0x9c
			 0x189f4b75, /// 0xa0
			 0xbf36bcd0, /// 0xa4
			 0x2ab8d503, /// 0xa8
			 0xba5115cf, /// 0xac
			 0x29981d30, /// 0xb0
			 0x28ca3703, /// 0xb4
			 0x1bc616bc, /// 0xb8
			 0x6e4feebc, /// 0xbc
			 0x7be5f405, /// 0xc0
			 0x6f980794, /// 0xc4
			 0x51fdce3d, /// 0xc8
			 0x65073874, /// 0xcc
			 0x4262c3cf, /// 0xd0
			 0x9ef887eb, /// 0xd4
			 0x5ff00fe3, /// 0xd8
			 0x2b5735f7, /// 0xdc
			 0xb20bad54, /// 0xe0
			 0x93bc35d1, /// 0xe4
			 0xc270ba5f, /// 0xe8
			 0xe013c21e, /// 0xec
			 0x92bf164c, /// 0xf0
			 0xe8d4c002, /// 0xf4
			 0x14f7572c, /// 0xf8
			 0x8184f462, /// 0xfc
			 0x87bd60af, /// 0x100
			 0x37bc7177, /// 0x104
			 0x8fc9244b, /// 0x108
			 0x8763e7f5, /// 0x10c
			 0x20d7ad60, /// 0x110
			 0x119d3d5a, /// 0x114
			 0xfdb9b7c1, /// 0x118
			 0xaed7df14, /// 0x11c
			 0x849a7047, /// 0x120
			 0x42d252a0, /// 0x124
			 0x4675ad6b, /// 0x128
			 0x3de7b961, /// 0x12c
			 0xece5e56b, /// 0x130
			 0xaf930c96, /// 0x134
			 0xd6e38da7, /// 0x138
			 0x85195352, /// 0x13c
			 0xa309a2d5, /// 0x140
			 0x5de63886, /// 0x144
			 0xc3cdb48e, /// 0x148
			 0x77245b99, /// 0x14c
			 0xdaaac6f1, /// 0x150
			 0x49ea88fe, /// 0x154
			 0x6115a997, /// 0x158
			 0x9b96b7c0, /// 0x15c
			 0xd72ae062, /// 0x160
			 0x23045925, /// 0x164
			 0x285e3e39, /// 0x168
			 0x17d214e7, /// 0x16c
			 0xcfeccfd7, /// 0x170
			 0xe587dc84, /// 0x174
			 0x50b91c97, /// 0x178
			 0xab802610, /// 0x17c
			 0x600994a9, /// 0x180
			 0x2f78b4b1, /// 0x184
			 0xb68ea3db, /// 0x188
			 0x2e2d2e6a, /// 0x18c
			 0xa2fe42f1, /// 0x190
			 0x0a79ddfa, /// 0x194
			 0x96d51058, /// 0x198
			 0x792bea59, /// 0x19c
			 0xd98bbd74, /// 0x1a0
			 0x1f4c3792, /// 0x1a4
			 0xe8411c9f, /// 0x1a8
			 0x4faa45e0, /// 0x1ac
			 0xcfdd6222, /// 0x1b0
			 0x83d5ea29, /// 0x1b4
			 0xdec07ac6, /// 0x1b8
			 0x0ce56501, /// 0x1bc
			 0x573a5fa7, /// 0x1c0
			 0x6d243f92, /// 0x1c4
			 0x14758a2e, /// 0x1c8
			 0x25c6e1bc, /// 0x1cc
			 0xc81b357d, /// 0x1d0
			 0x7df6b13a, /// 0x1d4
			 0x9107c642, /// 0x1d8
			 0x48341486, /// 0x1dc
			 0x25f6c6af, /// 0x1e0
			 0x306d5e8a, /// 0x1e4
			 0xf0111e33, /// 0x1e8
			 0xa86f7f2c, /// 0x1ec
			 0xb6d67308, /// 0x1f0
			 0x9602c313, /// 0x1f4
			 0x479b4b21, /// 0x1f8
			 0xf0375048, /// 0x1fc
			 0xd295a21b, /// 0x200
			 0x09e504ef, /// 0x204
			 0x99de9b99, /// 0x208
			 0x2127fd7f, /// 0x20c
			 0xa7dec22b, /// 0x210
			 0x3d56db32, /// 0x214
			 0x4bc43220, /// 0x218
			 0xe1847fbd, /// 0x21c
			 0x20548290, /// 0x220
			 0x7af9a590, /// 0x224
			 0xa43f00b5, /// 0x228
			 0xcae0a21a, /// 0x22c
			 0xdd4ca766, /// 0x230
			 0xc23658cb, /// 0x234
			 0xd95a5e9c, /// 0x238
			 0xd8c823cb, /// 0x23c
			 0xfc1b5b84, /// 0x240
			 0x1fd3597b, /// 0x244
			 0xa3ca1f8f, /// 0x248
			 0x3b81a100, /// 0x24c
			 0xb7e99e5c, /// 0x250
			 0x042047fe, /// 0x254
			 0x2b47ce84, /// 0x258
			 0x4023cff5, /// 0x25c
			 0xa03d4cb1, /// 0x260
			 0x8be14f84, /// 0x264
			 0x488b3b41, /// 0x268
			 0xb461bbfb, /// 0x26c
			 0xf9ba9a4f, /// 0x270
			 0xe6fc93c4, /// 0x274
			 0x34f9a7a4, /// 0x278
			 0x568118a9, /// 0x27c
			 0xa0851dd7, /// 0x280
			 0x74e42f23, /// 0x284
			 0xd95fdcf5, /// 0x288
			 0xbf83b8f6, /// 0x28c
			 0xc267d2a7, /// 0x290
			 0xbccf7fdd, /// 0x294
			 0x703f507e, /// 0x298
			 0x13426ae2, /// 0x29c
			 0x3c8d602e, /// 0x2a0
			 0x402f4493, /// 0x2a4
			 0x1adf8e00, /// 0x2a8
			 0xa2852f48, /// 0x2ac
			 0xa0d3eff0, /// 0x2b0
			 0x11315a37, /// 0x2b4
			 0xb9f60f9a, /// 0x2b8
			 0xe2d50af0, /// 0x2bc
			 0x7d072722, /// 0x2c0
			 0xc510d4e3, /// 0x2c4
			 0x704d7bb7, /// 0x2c8
			 0xa9d896a5, /// 0x2cc
			 0xedf9afea, /// 0x2d0
			 0x7c5c5691, /// 0x2d4
			 0x2a7c95c8, /// 0x2d8
			 0xbbc8c835, /// 0x2dc
			 0xa6a3384d, /// 0x2e0
			 0xf0291f09, /// 0x2e4
			 0x6eaf38c7, /// 0x2e8
			 0xd7250342, /// 0x2ec
			 0x3da8f76d, /// 0x2f0
			 0xa5df8a00, /// 0x2f4
			 0x24834702, /// 0x2f8
			 0x7d17b93e, /// 0x2fc
			 0x29e0fdd7, /// 0x300
			 0xbc2601a0, /// 0x304
			 0xba594f60, /// 0x308
			 0xfe9628cb, /// 0x30c
			 0x788a7b22, /// 0x310
			 0xd13e1720, /// 0x314
			 0x5e375569, /// 0x318
			 0x8f78821a, /// 0x31c
			 0x6bdbb747, /// 0x320
			 0xd79bf167, /// 0x324
			 0x93e87093, /// 0x328
			 0x27947b78, /// 0x32c
			 0xa41ad2f4, /// 0x330
			 0xd1856c6c, /// 0x334
			 0xd12efd0a, /// 0x338
			 0x2f8cb2fc, /// 0x33c
			 0x1f0a4c7f, /// 0x340
			 0x0d212591, /// 0x344
			 0x871b70ef, /// 0x348
			 0x1af99608, /// 0x34c
			 0x88ba4c36, /// 0x350
			 0x0f1135eb, /// 0x354
			 0xa2b7ffe3, /// 0x358
			 0x5aa7d029, /// 0x35c
			 0x97ef93ff, /// 0x360
			 0x2e152aab, /// 0x364
			 0xd12d2076, /// 0x368
			 0x6a999b04, /// 0x36c
			 0x7c6ca31a, /// 0x370
			 0xef3fefba, /// 0x374
			 0xe86a9923, /// 0x378
			 0x88c904f1, /// 0x37c
			 0x03e5cc17, /// 0x380
			 0x74ce9ddd, /// 0x384
			 0xe26e2a7d, /// 0x388
			 0x9d3af891, /// 0x38c
			 0x820dc76d, /// 0x390
			 0x21cef6b6, /// 0x394
			 0xae8ecafb, /// 0x398
			 0xbf291dd7, /// 0x39c
			 0x57722b7a, /// 0x3a0
			 0xd59afa9b, /// 0x3a4
			 0x358cd0dd, /// 0x3a8
			 0x0d864d1a, /// 0x3ac
			 0x72a677c1, /// 0x3b0
			 0x0dd8e24a, /// 0x3b4
			 0xa314ef4c, /// 0x3b8
			 0x7d4d49d0, /// 0x3bc
			 0xbd5431cc, /// 0x3c0
			 0x21fb645b, /// 0x3c4
			 0x0603c53f, /// 0x3c8
			 0xdcb4f703, /// 0x3cc
			 0x37cdb49b, /// 0x3d0
			 0xf5e509ff, /// 0x3d4
			 0x57ddddf5, /// 0x3d8
			 0x5bfbd189, /// 0x3dc
			 0x9a3c3d57, /// 0x3e0
			 0x5cbb4f4c, /// 0x3e4
			 0x784dcc75, /// 0x3e8
			 0x0ca411a2, /// 0x3ec
			 0xb92f778b, /// 0x3f0
			 0xee0ccd68, /// 0x3f4
			 0x3ac338b6, /// 0x3f8
			 0xaa1d8440, /// 0x3fc
			 0x57bf2af8, /// 0x400
			 0x6b54bee3, /// 0x404
			 0x271e0e04, /// 0x408
			 0xaed22a1c, /// 0x40c
			 0x753651ea, /// 0x410
			 0x77ff716b, /// 0x414
			 0x26c88556, /// 0x418
			 0xfe4f440c, /// 0x41c
			 0xb6fbab09, /// 0x420
			 0x8eb29c2b, /// 0x424
			 0x1ec4de2f, /// 0x428
			 0xdb35651a, /// 0x42c
			 0x571b03d0, /// 0x430
			 0x38690ab5, /// 0x434
			 0x08d99122, /// 0x438
			 0x5c317f6a, /// 0x43c
			 0x83629e6f, /// 0x440
			 0xee1f67a2, /// 0x444
			 0xe41d73a1, /// 0x448
			 0x3f0a679f, /// 0x44c
			 0x1eec5ba5, /// 0x450
			 0x38fb4254, /// 0x454
			 0x93637d9e, /// 0x458
			 0x8c7c7ac2, /// 0x45c
			 0x136e9620, /// 0x460
			 0x4d23fbcb, /// 0x464
			 0xb039e3ac, /// 0x468
			 0xf3bec3f3, /// 0x46c
			 0x2540bdb4, /// 0x470
			 0xda913d53, /// 0x474
			 0xbb72bd5c, /// 0x478
			 0xac7564fc, /// 0x47c
			 0xcb2ba13e, /// 0x480
			 0x332fdecb, /// 0x484
			 0x826442f3, /// 0x488
			 0xcdd61331, /// 0x48c
			 0x6fd11029, /// 0x490
			 0x78ba66f4, /// 0x494
			 0x09019dba, /// 0x498
			 0x96069002, /// 0x49c
			 0xd0f7c9f2, /// 0x4a0
			 0x01362f35, /// 0x4a4
			 0x805404c7, /// 0x4a8
			 0x5143f4c5, /// 0x4ac
			 0xdee20ea6, /// 0x4b0
			 0x524e9794, /// 0x4b4
			 0x99bae825, /// 0x4b8
			 0xb629d2d6, /// 0x4bc
			 0xc0e0bb5f, /// 0x4c0
			 0x2ce3163d, /// 0x4c4
			 0x9e3a7a89, /// 0x4c8
			 0x9519871b, /// 0x4cc
			 0x3003edc0, /// 0x4d0
			 0x3490260e, /// 0x4d4
			 0x5849c563, /// 0x4d8
			 0x020c8e96, /// 0x4dc
			 0x7078dba0, /// 0x4e0
			 0xf7b7d65b, /// 0x4e4
			 0xa1830ee2, /// 0x4e8
			 0x40f86d92, /// 0x4ec
			 0x7a37da2b, /// 0x4f0
			 0xd098f79a, /// 0x4f4
			 0x320fdb7c, /// 0x4f8
			 0x739a4d5b, /// 0x4fc
			 0x9996ab52, /// 0x500
			 0x6ca5f88c, /// 0x504
			 0x64a919ac, /// 0x508
			 0x9d9ea9e6, /// 0x50c
			 0x5f242919, /// 0x510
			 0x349c0531, /// 0x514
			 0xcdab7051, /// 0x518
			 0x63fb8331, /// 0x51c
			 0xfecaea2a, /// 0x520
			 0xdb15cde0, /// 0x524
			 0x385bdadf, /// 0x528
			 0xd367b990, /// 0x52c
			 0x501811b2, /// 0x530
			 0xbc1f558f, /// 0x534
			 0xa37ba0d3, /// 0x538
			 0xb428cb75, /// 0x53c
			 0x984eb4fe, /// 0x540
			 0xc0976e2a, /// 0x544
			 0xef7b64b4, /// 0x548
			 0x421e2a40, /// 0x54c
			 0x99311eae, /// 0x550
			 0x818e1909, /// 0x554
			 0x3c3c0a26, /// 0x558
			 0xf06abd4f, /// 0x55c
			 0x1d87a765, /// 0x560
			 0xbb41c9fa, /// 0x564
			 0x694ff4f2, /// 0x568
			 0xc8cc845f, /// 0x56c
			 0xb48b50aa, /// 0x570
			 0xd50961a8, /// 0x574
			 0xd1e564a7, /// 0x578
			 0x6a09794a, /// 0x57c
			 0xb7853d5b, /// 0x580
			 0x2de01577, /// 0x584
			 0x1592d0f5, /// 0x588
			 0xaa132119, /// 0x58c
			 0x51d9a673, /// 0x590
			 0x9cc2b8f4, /// 0x594
			 0x99b4995d, /// 0x598
			 0x8b6426f0, /// 0x59c
			 0x7d51e11e, /// 0x5a0
			 0x5a66a60a, /// 0x5a4
			 0x2eb4c0c4, /// 0x5a8
			 0x51a4c2fe, /// 0x5ac
			 0xa570206d, /// 0x5b0
			 0xd004922a, /// 0x5b4
			 0xc4ee7c67, /// 0x5b8
			 0x32b78b23, /// 0x5bc
			 0x97aa9e4f, /// 0x5c0
			 0xc0135e31, /// 0x5c4
			 0x888ae019, /// 0x5c8
			 0x1a71fa60, /// 0x5cc
			 0x5209f386, /// 0x5d0
			 0x11f81d13, /// 0x5d4
			 0x997d665e, /// 0x5d8
			 0x92d6cca5, /// 0x5dc
			 0x71b22d63, /// 0x5e0
			 0x281e6b41, /// 0x5e4
			 0xa7879fa8, /// 0x5e8
			 0x462ec341, /// 0x5ec
			 0x61a99f9a, /// 0x5f0
			 0x746401bb, /// 0x5f4
			 0x19fb0ded, /// 0x5f8
			 0x210e72c6, /// 0x5fc
			 0xb3f90277, /// 0x600
			 0x0f0efaf6, /// 0x604
			 0xe705b181, /// 0x608
			 0x1e1d7833, /// 0x60c
			 0x3c547fcc, /// 0x610
			 0x21d1a51e, /// 0x614
			 0x5dd6b0a3, /// 0x618
			 0xe534f203, /// 0x61c
			 0x995f2de3, /// 0x620
			 0xe3e5d259, /// 0x624
			 0xee5e8734, /// 0x628
			 0x8074f50d, /// 0x62c
			 0x5da5f321, /// 0x630
			 0x3f1433e0, /// 0x634
			 0x2a57c071, /// 0x638
			 0x751fbd7c, /// 0x63c
			 0xe445f1de, /// 0x640
			 0x35d4f03f, /// 0x644
			 0x946fa451, /// 0x648
			 0xc8024931, /// 0x64c
			 0x99b2c2d4, /// 0x650
			 0xa28dc9b3, /// 0x654
			 0xa99774a9, /// 0x658
			 0xab4616fb, /// 0x65c
			 0xcda53716, /// 0x660
			 0x3a39a41f, /// 0x664
			 0x8384232a, /// 0x668
			 0xf49678a2, /// 0x66c
			 0xed2c520a, /// 0x670
			 0x46902b89, /// 0x674
			 0xcdc93e29, /// 0x678
			 0xfe3c61bf, /// 0x67c
			 0xd491a67c, /// 0x680
			 0xf2f4c4be, /// 0x684
			 0x43777d41, /// 0x688
			 0x8b72d982, /// 0x68c
			 0x8e99f024, /// 0x690
			 0x501f7f42, /// 0x694
			 0x3902ec5f, /// 0x698
			 0xa81357fe, /// 0x69c
			 0xa1f6de72, /// 0x6a0
			 0x914e1a2d, /// 0x6a4
			 0x5158cdde, /// 0x6a8
			 0xdcf2bced, /// 0x6ac
			 0x977d5538, /// 0x6b0
			 0x90652f96, /// 0x6b4
			 0x7a4fa2ba, /// 0x6b8
			 0xcfb7ab06, /// 0x6bc
			 0x80c9022a, /// 0x6c0
			 0xfcd0605e, /// 0x6c4
			 0xa5dce024, /// 0x6c8
			 0x3d59df75, /// 0x6cc
			 0x497c8d58, /// 0x6d0
			 0x64e447ca, /// 0x6d4
			 0x2b3f9f9e, /// 0x6d8
			 0x8e64b094, /// 0x6dc
			 0x9b2ac07a, /// 0x6e0
			 0x505ed632, /// 0x6e4
			 0x20c965e9, /// 0x6e8
			 0x8d1ff39a, /// 0x6ec
			 0xfcc897d6, /// 0x6f0
			 0x5b57d6e7, /// 0x6f4
			 0xb67cb6a1, /// 0x6f8
			 0xd2948108, /// 0x6fc
			 0x6d4e352a, /// 0x700
			 0x1205e456, /// 0x704
			 0xf020ea22, /// 0x708
			 0x0d99891d, /// 0x70c
			 0x6816f105, /// 0x710
			 0xc0af7c8d, /// 0x714
			 0x9c496d37, /// 0x718
			 0xbd117d83, /// 0x71c
			 0x6b74a1c9, /// 0x720
			 0x2bb984db, /// 0x724
			 0x96dba74f, /// 0x728
			 0x66e6b7fe, /// 0x72c
			 0x10647261, /// 0x730
			 0x4e91cbde, /// 0x734
			 0xc425c8cb, /// 0x738
			 0x98ff55bc, /// 0x73c
			 0xd46ac67e, /// 0x740
			 0xd743d90f, /// 0x744
			 0xbb53b584, /// 0x748
			 0x4734fdb4, /// 0x74c
			 0x4637f20c, /// 0x750
			 0x0cce22b9, /// 0x754
			 0x8855b5b3, /// 0x758
			 0xf65a7156, /// 0x75c
			 0xd3a10c2d, /// 0x760
			 0x247508c1, /// 0x764
			 0x322433fe, /// 0x768
			 0x3e2a0cfe, /// 0x76c
			 0xc0a6d3ad, /// 0x770
			 0x5c2a6d7f, /// 0x774
			 0xc8eb562f, /// 0x778
			 0x1c80ea1f, /// 0x77c
			 0xb2ff38e2, /// 0x780
			 0x992c0237, /// 0x784
			 0x17bc017c, /// 0x788
			 0x4030b0d6, /// 0x78c
			 0x47d2df5d, /// 0x790
			 0x56a5871a, /// 0x794
			 0x443ef2f5, /// 0x798
			 0xad28f40c, /// 0x79c
			 0x48e4e9c5, /// 0x7a0
			 0x83d3961f, /// 0x7a4
			 0x1f42b4cd, /// 0x7a8
			 0x3db49801, /// 0x7ac
			 0x531eae16, /// 0x7b0
			 0x271cc6ad, /// 0x7b4
			 0x7b93f561, /// 0x7b8
			 0x1cafb3d5, /// 0x7bc
			 0xe82a2351, /// 0x7c0
			 0x4439926c, /// 0x7c4
			 0xe26686a7, /// 0x7c8
			 0xfddc683e, /// 0x7cc
			 0xa3148a54, /// 0x7d0
			 0xafb6b630, /// 0x7d4
			 0x02dca711, /// 0x7d8
			 0x48887fe9, /// 0x7dc
			 0x43847060, /// 0x7e0
			 0x09cc61e9, /// 0x7e4
			 0xdb57c82e, /// 0x7e8
			 0x16cd48f2, /// 0x7ec
			 0x5814a601, /// 0x7f0
			 0x977e101e, /// 0x7f4
			 0x50040369, /// 0x7f8
			 0x1f9baad5, /// 0x7fc
			 0x114d20fc, /// 0x800
			 0x16f48ae0, /// 0x804
			 0x413cfc07, /// 0x808
			 0xfdac3244, /// 0x80c
			 0x3309fa5b, /// 0x810
			 0x0d18a919, /// 0x814
			 0x15ce5e5c, /// 0x818
			 0xb8d53e0f, /// 0x81c
			 0xe4a2cb8e, /// 0x820
			 0x3011f6f5, /// 0x824
			 0x9d53f79e, /// 0x828
			 0x1c9c038e, /// 0x82c
			 0x9b64379a, /// 0x830
			 0xce4bb3a0, /// 0x834
			 0x35a92913, /// 0x838
			 0x270df1f8, /// 0x83c
			 0x14d666c0, /// 0x840
			 0xbdf7bb19, /// 0x844
			 0x09cf7e53, /// 0x848
			 0x3a88915d, /// 0x84c
			 0x79b1fd1b, /// 0x850
			 0x8d6b0e3c, /// 0x854
			 0x18fed9a9, /// 0x858
			 0x36dbdaad, /// 0x85c
			 0x77bcb28f, /// 0x860
			 0x62172ff0, /// 0x864
			 0x816eccec, /// 0x868
			 0x0e6f3389, /// 0x86c
			 0x69c651fa, /// 0x870
			 0x091e6347, /// 0x874
			 0xd2825286, /// 0x878
			 0x95fa73b3, /// 0x87c
			 0xda78436c, /// 0x880
			 0x80554476, /// 0x884
			 0x5ef4cf98, /// 0x888
			 0xce02b4bd, /// 0x88c
			 0xd251d23f, /// 0x890
			 0xce24b905, /// 0x894
			 0x74977df6, /// 0x898
			 0x6f2ca83d, /// 0x89c
			 0x6f2f58ef, /// 0x8a0
			 0x6b3fe7ab, /// 0x8a4
			 0xdf3792da, /// 0x8a8
			 0x903bbf23, /// 0x8ac
			 0xde2ded59, /// 0x8b0
			 0xa0dbdc9a, /// 0x8b4
			 0x2c133277, /// 0x8b8
			 0xfc8e535f, /// 0x8bc
			 0x867e53e8, /// 0x8c0
			 0x0d396f91, /// 0x8c4
			 0xa67bfdc3, /// 0x8c8
			 0xcceb4a3c, /// 0x8cc
			 0x1cc613ff, /// 0x8d0
			 0x7fb79cb6, /// 0x8d4
			 0x0449281b, /// 0x8d8
			 0x00bfcdca, /// 0x8dc
			 0x6fd17a9a, /// 0x8e0
			 0xd0e5252e, /// 0x8e4
			 0x7644f312, /// 0x8e8
			 0x940a628b, /// 0x8ec
			 0xf18f231d, /// 0x8f0
			 0xfa218c0c, /// 0x8f4
			 0xa2ae346e, /// 0x8f8
			 0x09725ea1, /// 0x8fc
			 0x39acb501, /// 0x900
			 0xf412cd83, /// 0x904
			 0xc631bf9b, /// 0x908
			 0xcf1a9281, /// 0x90c
			 0x710cecc2, /// 0x910
			 0x0145d5d7, /// 0x914
			 0x19101618, /// 0x918
			 0xbee57031, /// 0x91c
			 0x95534dd6, /// 0x920
			 0x6141a4e0, /// 0x924
			 0xc18faf73, /// 0x928
			 0xcc35af2f, /// 0x92c
			 0x63d569e6, /// 0x930
			 0xf9b410c1, /// 0x934
			 0x14f19f49, /// 0x938
			 0x9476f4f4, /// 0x93c
			 0x5de27019, /// 0x940
			 0xd0968d18, /// 0x944
			 0xe567bc73, /// 0x948
			 0x70980fc7, /// 0x94c
			 0x099ae20c, /// 0x950
			 0x65d47767, /// 0x954
			 0xe0dba15f, /// 0x958
			 0x5b7f4077, /// 0x95c
			 0xe0034ba6, /// 0x960
			 0xbca34bd4, /// 0x964
			 0xa974bf72, /// 0x968
			 0x1df584b3, /// 0x96c
			 0xc1eacb2d, /// 0x970
			 0xd160a188, /// 0x974
			 0x33cb238a, /// 0x978
			 0x34c2c370, /// 0x97c
			 0x34ded00e, /// 0x980
			 0xc49ceeed, /// 0x984
			 0x704c0df6, /// 0x988
			 0xd9b5d798, /// 0x98c
			 0xfe88b603, /// 0x990
			 0xd342da0d, /// 0x994
			 0xacd1b368, /// 0x998
			 0x3bc18cb3, /// 0x99c
			 0xe445e809, /// 0x9a0
			 0x22808eb2, /// 0x9a4
			 0xe78ef9d8, /// 0x9a8
			 0xfa9e085a, /// 0x9ac
			 0xdf269b13, /// 0x9b0
			 0x0336b880, /// 0x9b4
			 0x03905c62, /// 0x9b8
			 0x93ea6886, /// 0x9bc
			 0xc3ba6e89, /// 0x9c0
			 0xd80c6315, /// 0x9c4
			 0xbadfc2e3, /// 0x9c8
			 0xa566f6a9, /// 0x9cc
			 0x3fa522a5, /// 0x9d0
			 0x1d6444fe, /// 0x9d4
			 0x62669332, /// 0x9d8
			 0x5a6f419b, /// 0x9dc
			 0xc655650b, /// 0x9e0
			 0xe1fc1217, /// 0x9e4
			 0x880f89de, /// 0x9e8
			 0xcae3f801, /// 0x9ec
			 0xdd5672db, /// 0x9f0
			 0x09336361, /// 0x9f4
			 0x022ac373, /// 0x9f8
			 0xbab89096, /// 0x9fc
			 0x2605ed1f, /// 0xa00
			 0x09c33e92, /// 0xa04
			 0x12c6de60, /// 0xa08
			 0x434a1e58, /// 0xa0c
			 0x9813e0b8, /// 0xa10
			 0x280e3464, /// 0xa14
			 0x10ff59f5, /// 0xa18
			 0xa22e5ecb, /// 0xa1c
			 0x67bb4ba8, /// 0xa20
			 0xf3c608fd, /// 0xa24
			 0x64836b3a, /// 0xa28
			 0xeb797afe, /// 0xa2c
			 0xea618294, /// 0xa30
			 0x6f88f7b0, /// 0xa34
			 0x0b6f49be, /// 0xa38
			 0x1acbb3cb, /// 0xa3c
			 0x75e652ea, /// 0xa40
			 0xb391caee, /// 0xa44
			 0xcad06006, /// 0xa48
			 0x9dc79304, /// 0xa4c
			 0x3ebdecd8, /// 0xa50
			 0x0a7cb0e2, /// 0xa54
			 0xae3533ae, /// 0xa58
			 0x62420124, /// 0xa5c
			 0x87687a7d, /// 0xa60
			 0x225af73f, /// 0xa64
			 0x8ef6c129, /// 0xa68
			 0x3eea4872, /// 0xa6c
			 0x59dbb62b, /// 0xa70
			 0x65712e72, /// 0xa74
			 0x50e266af, /// 0xa78
			 0xc9d62482, /// 0xa7c
			 0xcd97e826, /// 0xa80
			 0x838854a0, /// 0xa84
			 0x9fb3c73d, /// 0xa88
			 0xe6458279, /// 0xa8c
			 0xca176e17, /// 0xa90
			 0x3904d1ac, /// 0xa94
			 0xd42d5ead, /// 0xa98
			 0xea34e178, /// 0xa9c
			 0x4b3c51c1, /// 0xaa0
			 0x42c7d546, /// 0xaa4
			 0xc4739a8f, /// 0xaa8
			 0xea311697, /// 0xaac
			 0x1fb72eee, /// 0xab0
			 0x67ac0e99, /// 0xab4
			 0xd7db4ffa, /// 0xab8
			 0x12743561, /// 0xabc
			 0xb2dc1c43, /// 0xac0
			 0xe1a8e4cb, /// 0xac4
			 0x894d93f0, /// 0xac8
			 0x97595c7b, /// 0xacc
			 0xb0a8c4f5, /// 0xad0
			 0x4208128c, /// 0xad4
			 0x5dea592e, /// 0xad8
			 0xc82d1523, /// 0xadc
			 0x5b6e42b5, /// 0xae0
			 0x8a0b826f, /// 0xae4
			 0x6a153867, /// 0xae8
			 0x8df11404, /// 0xaec
			 0x6b090a4c, /// 0xaf0
			 0x9f775961, /// 0xaf4
			 0xb82dc9e8, /// 0xaf8
			 0x864c8144, /// 0xafc
			 0xc48e8613, /// 0xb00
			 0x25527d38, /// 0xb04
			 0x221c4643, /// 0xb08
			 0xff3f3ddf, /// 0xb0c
			 0xf361c5f1, /// 0xb10
			 0x3d32e3dc, /// 0xb14
			 0xcfa23ee1, /// 0xb18
			 0xa0e7342f, /// 0xb1c
			 0xc3295340, /// 0xb20
			 0xdde2a390, /// 0xb24
			 0x631c5ed0, /// 0xb28
			 0x7f775973, /// 0xb2c
			 0xc6235a60, /// 0xb30
			 0x820b6fc4, /// 0xb34
			 0x7708639e, /// 0xb38
			 0xbd6d0982, /// 0xb3c
			 0x7f2136ab, /// 0xb40
			 0x5f98a8b3, /// 0xb44
			 0xeaa4f0a6, /// 0xb48
			 0x65c76539, /// 0xb4c
			 0x29cd637f, /// 0xb50
			 0x64211bf7, /// 0xb54
			 0xf4a56f9c, /// 0xb58
			 0x10ebc579, /// 0xb5c
			 0x15b23775, /// 0xb60
			 0x09c6f336, /// 0xb64
			 0xac6d4eb2, /// 0xb68
			 0x6354036a, /// 0xb6c
			 0x980656d6, /// 0xb70
			 0x130f150f, /// 0xb74
			 0xc0602bdf, /// 0xb78
			 0x3c743035, /// 0xb7c
			 0xf7cd49f8, /// 0xb80
			 0x420fbe3a, /// 0xb84
			 0x8b299e43, /// 0xb88
			 0xe3d17a12, /// 0xb8c
			 0x211786a2, /// 0xb90
			 0x9c67024c, /// 0xb94
			 0x69cfa654, /// 0xb98
			 0xd0382937, /// 0xb9c
			 0xce541e80, /// 0xba0
			 0x66b61e6d, /// 0xba4
			 0xe476c0e8, /// 0xba8
			 0x4a24b228, /// 0xbac
			 0xb5b078cd, /// 0xbb0
			 0x56a8d85a, /// 0xbb4
			 0x175ddac8, /// 0xbb8
			 0x914a96c2, /// 0xbbc
			 0x10cce2e4, /// 0xbc0
			 0xdd0c2892, /// 0xbc4
			 0xa53ab082, /// 0xbc8
			 0x03e164bf, /// 0xbcc
			 0x56541ba2, /// 0xbd0
			 0xd65ac1d5, /// 0xbd4
			 0x99afb8ab, /// 0xbd8
			 0xc0c941ba, /// 0xbdc
			 0x458f5f97, /// 0xbe0
			 0x51d2d372, /// 0xbe4
			 0x004954b0, /// 0xbe8
			 0x02c7ba9b, /// 0xbec
			 0x04805dc1, /// 0xbf0
			 0x7f21de15, /// 0xbf4
			 0xa4f49955, /// 0xbf8
			 0x9f48eda6, /// 0xbfc
			 0x655c57cb, /// 0xc00
			 0x590d03c4, /// 0xc04
			 0xbacece33, /// 0xc08
			 0xaa2a8f44, /// 0xc0c
			 0x434aa8d5, /// 0xc10
			 0x3170999f, /// 0xc14
			 0x65d8260c, /// 0xc18
			 0xbbbb365e, /// 0xc1c
			 0x293a003a, /// 0xc20
			 0x52cd36e0, /// 0xc24
			 0x8f667b4a, /// 0xc28
			 0x64e9fa8b, /// 0xc2c
			 0x92082afd, /// 0xc30
			 0x8c81c118, /// 0xc34
			 0x7c794894, /// 0xc38
			 0x7b3caab6, /// 0xc3c
			 0x083bab4e, /// 0xc40
			 0x21cb493c, /// 0xc44
			 0xe28386ef, /// 0xc48
			 0x3df2b61e, /// 0xc4c
			 0xba694787, /// 0xc50
			 0xfef4891b, /// 0xc54
			 0x52af61e6, /// 0xc58
			 0x4a75a81c, /// 0xc5c
			 0x2386627f, /// 0xc60
			 0x390513bc, /// 0xc64
			 0xbc260bf4, /// 0xc68
			 0x42d09334, /// 0xc6c
			 0xaf7120fe, /// 0xc70
			 0x8edb1dff, /// 0xc74
			 0x638a5037, /// 0xc78
			 0x2665298b, /// 0xc7c
			 0x2a27fc07, /// 0xc80
			 0xd1c6bec4, /// 0xc84
			 0xd824222d, /// 0xc88
			 0xf95c856b, /// 0xc8c
			 0x3b15b162, /// 0xc90
			 0x560cabf8, /// 0xc94
			 0x4bc5b656, /// 0xc98
			 0x1d604b7c, /// 0xc9c
			 0x2e270ad6, /// 0xca0
			 0x5ee72f96, /// 0xca4
			 0x3e248f54, /// 0xca8
			 0x020e2532, /// 0xcac
			 0x10b64e08, /// 0xcb0
			 0xa49c0368, /// 0xcb4
			 0x82e9969b, /// 0xcb8
			 0x85c6e913, /// 0xcbc
			 0x77c10d55, /// 0xcc0
			 0x672a639f, /// 0xcc4
			 0x31c15af8, /// 0xcc8
			 0xf443c455, /// 0xccc
			 0x4047f6ab, /// 0xcd0
			 0x785318ca, /// 0xcd4
			 0xd5878602, /// 0xcd8
			 0x0b71e94c, /// 0xcdc
			 0xd65abe0c, /// 0xce0
			 0x2f2c589b, /// 0xce4
			 0xc9ffed9f, /// 0xce8
			 0xbb792f0e, /// 0xcec
			 0xd78a9091, /// 0xcf0
			 0xd48062e8, /// 0xcf4
			 0x64f7fa5a, /// 0xcf8
			 0xa0a48615, /// 0xcfc
			 0x34e19302, /// 0xd00
			 0xb6f437e0, /// 0xd04
			 0xd30bdd82, /// 0xd08
			 0x48cd1d5c, /// 0xd0c
			 0xb1e558a7, /// 0xd10
			 0xad940255, /// 0xd14
			 0xc72baa1d, /// 0xd18
			 0x16982419, /// 0xd1c
			 0xa3f2b004, /// 0xd20
			 0xfd6c3857, /// 0xd24
			 0x19fcb608, /// 0xd28
			 0x42fe266b, /// 0xd2c
			 0x3308b65a, /// 0xd30
			 0xf8122218, /// 0xd34
			 0x0581a3f2, /// 0xd38
			 0xbacb0881, /// 0xd3c
			 0x2338c85a, /// 0xd40
			 0x5fae3149, /// 0xd44
			 0xb8f61d5a, /// 0xd48
			 0x70a680be, /// 0xd4c
			 0x662b41df, /// 0xd50
			 0x2cb00eea, /// 0xd54
			 0x23a9e4f1, /// 0xd58
			 0x08cc6498, /// 0xd5c
			 0xccdcbdf5, /// 0xd60
			 0x0d8f7a70, /// 0xd64
			 0x698de775, /// 0xd68
			 0xc9b154e2, /// 0xd6c
			 0x450c6c53, /// 0xd70
			 0xc9be0e76, /// 0xd74
			 0xffad888a, /// 0xd78
			 0x3c84dfd8, /// 0xd7c
			 0x42205695, /// 0xd80
			 0x4510ac3e, /// 0xd84
			 0xc4d1ccf3, /// 0xd88
			 0x1575d58f, /// 0xd8c
			 0xcfddef32, /// 0xd90
			 0x0baf46d0, /// 0xd94
			 0xa61348f6, /// 0xd98
			 0x351f5905, /// 0xd9c
			 0x218e60b3, /// 0xda0
			 0xce8b23b6, /// 0xda4
			 0x7479f9ba, /// 0xda8
			 0xbb80b899, /// 0xdac
			 0x1e26699b, /// 0xdb0
			 0x17a2eefd, /// 0xdb4
			 0x7d44c5bd, /// 0xdb8
			 0xb4d11f23, /// 0xdbc
			 0xf6dc14b4, /// 0xdc0
			 0x5841735a, /// 0xdc4
			 0x49aea926, /// 0xdc8
			 0x721703b6, /// 0xdcc
			 0x4b430cfc, /// 0xdd0
			 0x9a60a67b, /// 0xdd4
			 0x39bd15e1, /// 0xdd8
			 0x949628d6, /// 0xddc
			 0x955e02c9, /// 0xde0
			 0xb2057fc2, /// 0xde4
			 0x3f82ee64, /// 0xde8
			 0x174d2931, /// 0xdec
			 0x99c29e8c, /// 0xdf0
			 0x15b67057, /// 0xdf4
			 0xb031414f, /// 0xdf8
			 0x17c5671b, /// 0xdfc
			 0x89a365cf, /// 0xe00
			 0x5d045565, /// 0xe04
			 0x6533bd56, /// 0xe08
			 0x5b205576, /// 0xe0c
			 0x5ab1365d, /// 0xe10
			 0x3840f9aa, /// 0xe14
			 0x8751f36f, /// 0xe18
			 0xcfde1649, /// 0xe1c
			 0x605358e4, /// 0xe20
			 0x89595ded, /// 0xe24
			 0x18a346e6, /// 0xe28
			 0x80054b91, /// 0xe2c
			 0x9a520d5a, /// 0xe30
			 0xc15437ec, /// 0xe34
			 0x3e781c64, /// 0xe38
			 0xe57cfb76, /// 0xe3c
			 0x2e638aba, /// 0xe40
			 0x70f52b70, /// 0xe44
			 0x439fbcef, /// 0xe48
			 0x2d771a1d, /// 0xe4c
			 0x97b002a9, /// 0xe50
			 0x4b6f6a1a, /// 0xe54
			 0xf94355e9, /// 0xe58
			 0xbdbcd5f1, /// 0xe5c
			 0x353ca877, /// 0xe60
			 0xa779f780, /// 0xe64
			 0x53d62c77, /// 0xe68
			 0x983dfd19, /// 0xe6c
			 0xa03e268c, /// 0xe70
			 0xdf1af380, /// 0xe74
			 0x4bb350a9, /// 0xe78
			 0xf16892f3, /// 0xe7c
			 0xd90af8e0, /// 0xe80
			 0x0d3115e2, /// 0xe84
			 0x39b00d13, /// 0xe88
			 0x2c77cc9d, /// 0xe8c
			 0x6338d8c8, /// 0xe90
			 0x2a3bd54c, /// 0xe94
			 0x36862dc4, /// 0xe98
			 0x9621852c, /// 0xe9c
			 0x4bb067ea, /// 0xea0
			 0xe087063a, /// 0xea4
			 0x21b6a9d7, /// 0xea8
			 0x9ab646c2, /// 0xeac
			 0x6079841c, /// 0xeb0
			 0x4cecb86e, /// 0xeb4
			 0x62fdb4bd, /// 0xeb8
			 0xb1848485, /// 0xebc
			 0xc2ff2372, /// 0xec0
			 0xdb1b3c53, /// 0xec4
			 0x2a81012b, /// 0xec8
			 0xec922c08, /// 0xecc
			 0x39696145, /// 0xed0
			 0x520785e9, /// 0xed4
			 0xba820471, /// 0xed8
			 0xcb86abf3, /// 0xedc
			 0x47ce6906, /// 0xee0
			 0xb1ef4780, /// 0xee4
			 0xe11d9489, /// 0xee8
			 0x60fe7007, /// 0xeec
			 0xb3fed5ec, /// 0xef0
			 0xbbed35d9, /// 0xef4
			 0xdca9436e, /// 0xef8
			 0x7a1c26ca, /// 0xefc
			 0x445e57b8, /// 0xf00
			 0x91e794c4, /// 0xf04
			 0x7f3c7124, /// 0xf08
			 0x0b6c12d3, /// 0xf0c
			 0x3488ad9d, /// 0xf10
			 0x28658f3f, /// 0xf14
			 0x509fc69d, /// 0xf18
			 0x196a079d, /// 0xf1c
			 0x6e14cbf9, /// 0xf20
			 0xd7ecdfc6, /// 0xf24
			 0xced487a4, /// 0xf28
			 0x8c2a7b75, /// 0xf2c
			 0x0e29963b, /// 0xf30
			 0xc7d1db4e, /// 0xf34
			 0x9ae68b62, /// 0xf38
			 0xd206b847, /// 0xf3c
			 0x8eb36cee, /// 0xf40
			 0x2da21261, /// 0xf44
			 0xdef645f4, /// 0xf48
			 0xd338fc5c, /// 0xf4c
			 0xdc3eb794, /// 0xf50
			 0x3cc0f769, /// 0xf54
			 0x580925c3, /// 0xf58
			 0x6868ef47, /// 0xf5c
			 0x1d9c1063, /// 0xf60
			 0x7ff6f77d, /// 0xf64
			 0x5224000e, /// 0xf68
			 0x0211014c, /// 0xf6c
			 0xa32ab031, /// 0xf70
			 0x51f9ee89, /// 0xf74
			 0xa069ee44, /// 0xf78
			 0x9992864c, /// 0xf7c
			 0xc37a5d15, /// 0xf80
			 0xdcdd0f99, /// 0xf84
			 0x132708fd, /// 0xf88
			 0xef7f0284, /// 0xf8c
			 0xa6fed524, /// 0xf90
			 0x10ba1915, /// 0xf94
			 0xb9ef097d, /// 0xf98
			 0xadcaf6cd, /// 0xf9c
			 0x78054493, /// 0xfa0
			 0xf8168825, /// 0xfa4
			 0xba939834, /// 0xfa8
			 0xa40f78c1, /// 0xfac
			 0x2668d4b7, /// 0xfb0
			 0x82ba7f77, /// 0xfb4
			 0x944bc2c8, /// 0xfb8
			 0x774385ce, /// 0xfbc
			 0x50c8c987, /// 0xfc0
			 0x2da4ff8f, /// 0xfc4
			 0x00ed072a, /// 0xfc8
			 0x8e828347, /// 0xfcc
			 0x0d022a67, /// 0xfd0
			 0xb4cf75d3, /// 0xfd4
			 0x883d8837, /// 0xfd8
			 0x999dfd70, /// 0xfdc
			 0x82fd947f, /// 0xfe0
			 0x6322e4de, /// 0xfe4
			 0x08f1ddfd, /// 0xfe8
			 0x12428310, /// 0xfec
			 0x707cbcb6, /// 0xff0
			 0x70e54d54, /// 0xff4
			 0xafc467d3, /// 0xff8
			 0xb48aae66 /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00004
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00008
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0000c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00010
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00014
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0001c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00020
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00024
			 0x3f028f5c,                                                  // 0.51                                        /// 00028
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0002c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00030
			 0x33333333,                                                  // 4 random values                             /// 00034
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00038
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0003c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00040
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00044
			 0x0c780000,                                                  // Leading 1s:                                 /// 00048
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0004c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00050
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00054
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00058
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0005c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00060
			 0xcb000000,                                                  // -8388608.0                                  /// 00064
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00068
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0006c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00070
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00074
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00078
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0007c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00080
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00084
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00088
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0008c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00090
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00094
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00098
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0009c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 000a4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000a8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000b0
			 0xcb000000,                                                  // -8388608.0                                  /// 000b4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 000b8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 000bc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000c0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000c4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000cc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000d0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 000d4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000dc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e0
			 0x0c780000,                                                  // Leading 1s:                                 /// 000e4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 000e8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000f0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000f4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00100
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00104
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0010c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00110
			 0x3f028f5c,                                                  // 0.51                                        /// 00114
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0011c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00120
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00124
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00128
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0012c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00130
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00138
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0013c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00144
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0014c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00150
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0x0c400000,                                                  // Leading 1s:                                 /// 00158
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0015c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00160
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00164
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00168
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0016c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00170
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00174
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00178
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00180
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00184
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00188
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0018c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00190
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00198
			 0xcb000000,                                                  // -8388608.0                                  /// 0019c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001a0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001a4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ac
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001b0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001b4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001bc
			 0x00011111,                                                  // 9.7958E-41                                  /// 001c0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 001c4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001c8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001cc
			 0x7fc00001,                                                  // signaling NaN                               /// 001d0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001d4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001d8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 001dc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 001e0
			 0xff800000,                                                  // -inf                                        /// 001e4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001e8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ec
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001f0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001f4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 001f8
			 0x0c600000,                                                  // Leading 1s:                                 /// 001fc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00200
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00204
			 0x0c400000,                                                  // Leading 1s:                                 /// 00208
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0020c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00214
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00218
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0021c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00220
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00224
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00228
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0022c
			 0x33333333,                                                  // 4 random values                             /// 00230
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00234
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00238
			 0x55555555,                                                  // 4 random values                             /// 0023c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00240
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00244
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00248
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0024c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00250
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00254
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00258
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00260
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00264
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00270
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00274
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00278
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0027c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00280
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00284
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00288
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0028c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00290
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00294
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00298
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0029c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002a0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 002a4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002a8
			 0x0e000003,                                                  // Trailing 1s:                                /// 002ac
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002b0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0xffc00001,                                                  // -signaling NaN                              /// 002b8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002bc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002c4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002cc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002d4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002dc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002e0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002e8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ec
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 002f0
			 0x00011111,                                                  // 9.7958E-41                                  /// 002f4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 002fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0x0e000001,                                                  // Trailing 1s:                                /// 00304
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00308
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0xbf028f5c,                                                  // -0.51                                       /// 00310
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00314
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00318
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00320
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00328
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00330
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00334
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00338
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0033c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00344
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00348
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0034c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00354
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00358
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0035c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00364
			 0x3f028f5c,                                                  // 0.51                                        /// 00368
			 0x80011111,                                                  // -9.7958E-41                                 /// 0036c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00370
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00374
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00378
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00384
			 0x80011111,                                                  // -9.7958E-41                                 /// 00388
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0038c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00390
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00394
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00398
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003a0
			 0xff800000,                                                  // -inf                                        /// 003a4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003b0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 003b4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 003b8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003bc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0x0e000007,                                                  // Trailing 1s:                                /// 003c4
			 0x33333333,                                                  // 4 random values                             /// 003c8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003cc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003d0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003d4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003d8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003dc
			 0xcb000000,                                                  // -8388608.0                                  /// 003e0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 003e4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003e8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003ec
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003f0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f4
			 0x3f028f5c,                                                  // 0.51                                        /// 003f8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003fc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00400
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00404
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00408
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0040c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00410
			 0x0c780000,                                                  // Leading 1s:                                 /// 00414
			 0xff800000,                                                  // -inf                                        /// 00418
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0041c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00420
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00428
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0042c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00430
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00434
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0043c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00440
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00444
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0044c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00450
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00454
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00458
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0045c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00460
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00464
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00468
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0046c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00470
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00474
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00478
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0047c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00480
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00488
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0048c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00490
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00498
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 004a0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004a4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004ac
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004b4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004b8
			 0x0e000001,                                                  // Trailing 1s:                                /// 004bc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 004c0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 004c4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 004d0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 004d8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004dc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 004e0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 004e4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004e8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 004ec
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 004f0
			 0xff800000,                                                  // -inf                                        /// 004f4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 004f8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004fc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00500
			 0x7f800000,                                                  // inf                                         /// 00504
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00508
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0050c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00510
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00514
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00518
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0051c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00524
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00528
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0052c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00530
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00534
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00538
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00540
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00544
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00548
			 0x7fc00001,                                                  // signaling NaN                               /// 0054c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00550
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00558
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0055c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00560
			 0x00011111,                                                  // 9.7958E-41                                  /// 00564
			 0x0c700000,                                                  // Leading 1s:                                 /// 00568
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0056c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00570
			 0x0c780000,                                                  // Leading 1s:                                 /// 00574
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0057c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00580
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00584
			 0x0c700000,                                                  // Leading 1s:                                 /// 00588
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00590
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00594
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00598
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0059c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 005a0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 005a4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 005a8
			 0xffc00001,                                                  // -signaling NaN                              /// 005ac
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005b0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 005b4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005b8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005bc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005c4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005c8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005cc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005d0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0x00011111,                                                  // 9.7958E-41                                  /// 005d8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 005dc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005e0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005e4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005e8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005f0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005f4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005f8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005fc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00600
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00608
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0060c
			 0xff800000,                                                  // -inf                                        /// 00610
			 0x0e000007,                                                  // Trailing 1s:                                /// 00614
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00618
			 0x0c400000,                                                  // Leading 1s:                                 /// 0061c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00620
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00624
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00628
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0062c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00634
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00638
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0063c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00644
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00648
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0064c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00650
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00654
			 0x0c600000,                                                  // Leading 1s:                                 /// 00658
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0x00000000,                                                  // zero                                        /// 00660
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00664
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00668
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0066c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00670
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00674
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00678
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0067c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00680
			 0x0c400000,                                                  // Leading 1s:                                 /// 00684
			 0x55555555,                                                  // 4 random values                             /// 00688
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0068c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00694
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00698
			 0x4b000000,                                                  // 8388608.0                                   /// 0069c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 006a4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006a8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006ac
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006b4
			 0x7f800000,                                                  // inf                                         /// 006b8
			 0x33333333,                                                  // 4 random values                             /// 006bc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006c0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006c4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006c8
			 0x0c400000,                                                  // Leading 1s:                                 /// 006cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 006dc
			 0x33333333,                                                  // 4 random values                             /// 006e0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006e4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006e8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006ec
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006f0
			 0x3f028f5c,                                                  // 0.51                                        /// 006f4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006f8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006fc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00704
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0070c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00714
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00718
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0071c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00720
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00724
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00728
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0072c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00730
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00734
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0073c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00740
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00748
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0074c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00750
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00758
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0075c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00760
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00764
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00768
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0076c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00774
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0077c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00780
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00784
			 0x0e000007,                                                  // Trailing 1s:                                /// 00788
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00790
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00794
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00798
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0079c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007a0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0x0e000001,                                                  // Trailing 1s:                                /// 007a8
			 0x00011111,                                                  // 9.7958E-41                                  /// 007ac
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007b0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007b4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 007b8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007c0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007c4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007c8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 007cc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 007d0
			 0x80000000,                                                  // -zero                                       /// 007d4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007d8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 007dc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007e0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007e4
			 0x00000000,                                                  // zero                                        /// 007e8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ec
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007f0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 007f4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007f8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 007fc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00804
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00808
			 0x0e000001,                                                  // Trailing 1s:                                /// 0080c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00810
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00814
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00818
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0081c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00820
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00824
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00828
			 0x3f028f5c,                                                  // 0.51                                        /// 0082c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00834
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00838
			 0x0e000001,                                                  // Trailing 1s:                                /// 0083c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00840
			 0x00011111,                                                  // 9.7958E-41                                  /// 00844
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00848
			 0xff800000,                                                  // -inf                                        /// 0084c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00850
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00854
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00858
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0085c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00860
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00864
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00868
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00870
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00874
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00878
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0087c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00880
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00884
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00888
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00894
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00898
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0089c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 008a0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008a4
			 0x00011111,                                                  // 9.7958E-41                                  /// 008a8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 008ac
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008b0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008b4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008b8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008bc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 008c0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 008c4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008c8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008cc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 008d0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 008d4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008d8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008dc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008e0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008e4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ec
			 0x0c700000,                                                  // Leading 1s:                                 /// 008f0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 008f4
			 0x0e000001,                                                  // Trailing 1s:                                /// 008f8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008fc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00900
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00904
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00908
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0090c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x33333333,                                                  // 4 random values                             /// 00918
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00920
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00924
			 0x0e000007,                                                  // Trailing 1s:                                /// 00928
			 0x0c780000,                                                  // Leading 1s:                                 /// 0092c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00930
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00934
			 0x0c400000,                                                  // Leading 1s:                                 /// 00938
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0093c
			 0x4b000000,                                                  // 8388608.0                                   /// 00940
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00944
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00948
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0094c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00954
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00958
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0095c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00960
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00964
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00968
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0096c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00970
			 0x7f800000,                                                  // inf                                         /// 00974
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00980
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00984
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0098c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00990
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0x4b000000,                                                  // 8388608.0                                   /// 00998
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0099c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009a8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009ac
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009b0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009b4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 009b8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009bc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009c0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009cc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 009d0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009d4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009d8
			 0x0e000003,                                                  // Trailing 1s:                                /// 009dc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009e4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 009e8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009ec
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009f8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 009fc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a00
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a08
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a0c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a10
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a14
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a18
			 0x7f800000,                                                  // inf                                         /// 00a1c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a20
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a24
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a28
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x80000000,                                                  // -zero                                       /// 00a30
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a34
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a38
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a3c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a40
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a44
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a48
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a4c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a50
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a54
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a58
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a5c
			 0x7fc00001,                                                  // signaling NaN                               /// 00a60
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a64
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a68
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a6c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a70
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a74
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a78
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a7c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a80
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a88
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a8c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a90
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a94
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a98
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a9c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00aa0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00aa8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00aac
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ab0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ab4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ab8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00abc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ac0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ac4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ac8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00acc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ad4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00adc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00aec
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00af0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00af4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00afc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b00
			 0x7fc00001,                                                  // signaling NaN                               /// 00b04
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b08
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b0c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b10
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b14
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b20
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b28
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b30
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b34
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b38
			 0xff800000,                                                  // -inf                                        /// 00b3c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b40
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b44
			 0x33333333,                                                  // 4 random values                             /// 00b48
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b4c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b50
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b54
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b58
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b5c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b64
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b68
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b6c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b74
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b78
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b7c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b88
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b8c
			 0x3f028f5c,                                                  // 0.51                                        /// 00b90
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b94
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b98
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b9c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ba4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00bac
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00bb4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00bb8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00bbc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00bc0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00bc4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00bc8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bcc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bd4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bdc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00be0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00be4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00be8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bec
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00bf4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00bf8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00bfc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c00
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c04
			 0x4b000000,                                                  // 8388608.0                                   /// 00c08
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c0c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c1c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c20
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c28
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c2c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c30
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c34
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c38
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c3c
			 0x80000000,                                                  // -zero                                       /// 00c40
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c48
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c4c
			 0x7fc00001,                                                  // signaling NaN                               /// 00c50
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c54
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c58
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c60
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c64
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c6c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c70
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c74
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c78
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c7c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c80
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c84
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c8c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c90
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c94
			 0x55555555,                                                  // 4 random values                             /// 00c98
			 0x7f800000,                                                  // inf                                         /// 00c9c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00cac
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00cb8
			 0xffc00001,                                                  // -signaling NaN                              /// 00cbc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00cc0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00cc8
			 0x80000000,                                                  // -zero                                       /// 00ccc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00cd0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00cd4
			 0xffc00001,                                                  // -signaling NaN                              /// 00cd8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00cdc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ce0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ce4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00cec
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00cf0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00cf8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cfc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d00
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d04
			 0xcb000000,                                                  // -8388608.0                                  /// 00d08
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d0c
			 0xffc00001,                                                  // -signaling NaN                              /// 00d10
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d14
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d18
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d20
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d28
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d2c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d30
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d34
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d38
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d3c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d40
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d44
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d48
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d4c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d50
			 0x00000000,                                                  // zero                                        /// 00d54
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d58
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d5c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d60
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d64
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d68
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d70
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d74
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d78
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d7c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d80
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d84
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d88
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d94
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d98
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d9c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00da0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00da8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00dac
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00db4
			 0x80000000,                                                  // -zero                                       /// 00db8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00dbc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00dc0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00dc4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00dc8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dcc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00dd0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00dd4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00dd8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ddc
			 0xff800000,                                                  // -inf                                        /// 00de0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00de8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00dec
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00df4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00df8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00dfc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e00
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e08
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e0c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e10
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e14
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e18
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e1c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e24
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e28
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e2c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e30
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e34
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e38
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e3c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e40
			 0x3f028f5c,                                                  // 0.51                                        /// 00e44
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e48
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e4c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e50
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e54
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e5c
			 0x4b000000,                                                  // 8388608.0                                   /// 00e60
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e64
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e68
			 0xcb000000,                                                  // -8388608.0                                  /// 00e6c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e70
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e74
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e78
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e7c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e80
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e84
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e88
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e8c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e90
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e94
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e98
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e9c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ea0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00eac
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00eb8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ebc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ec0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ec4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ec8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ecc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ed0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ed8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00edc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ee4
			 0x7f800000,                                                  // inf                                         /// 00ee8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00eec
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ef0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ef8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00efc
			 0x80000000,                                                  // -zero                                       /// 00f00
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f04
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f08
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f0c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f10
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f14
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f18
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f1c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f20
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f24
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f28
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f2c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f30
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f34
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f38
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f3c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f40
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f44
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f48
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f4c
			 0x4b000000,                                                  // 8388608.0                                   /// 00f50
			 0x7f800000,                                                  // inf                                         /// 00f54
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f58
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f5c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f60
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f68
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f6c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f70
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f74
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f78
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f7c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f80
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f84
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f8c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f90
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f94
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f9c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fa0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x7f800000,                                                  // inf                                         /// 00fa8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fac
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00fb0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00fb8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fc4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fc8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00fcc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00fd4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fd8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00fdc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00fe0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00fe4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00fe8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00fec
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ff0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ff4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff8
			 0x00010000                                                  // SP - 1 bit alone set in mantissa +ve        /// last
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
			 0x00000118,
			 0x0000075c,
			 0x000003a0,
			 0x00000510,
			 0x00000524,
			 0x000000a0,
			 0x00000108,
			 0x00000130,

			 /// vpu register f2
			 0x41400000,
			 0x41400000,
			 0x41100000,
			 0x41c80000,
			 0x3f800000,
			 0x41b00000,
			 0x41600000,
			 0x3f800000,

			 /// vpu register f3
			 0x41a00000,
			 0x41100000,
			 0x40c00000,
			 0x41700000,
			 0x41500000,
			 0x41d00000,
			 0x41d80000,
			 0x41c00000,

			 /// vpu register f4
			 0x41000000,
			 0x41c00000,
			 0x41600000,
			 0x40c00000,
			 0x41d80000,
			 0x41500000,
			 0x41200000,
			 0x40a00000,

			 /// vpu register f5
			 0x41300000,
			 0x41600000,
			 0x41d80000,
			 0x41f80000,
			 0x41e00000,
			 0x41c80000,
			 0x41600000,
			 0x41d00000,

			 /// vpu register f6
			 0x41e00000,
			 0x41f80000,
			 0x42000000,
			 0x41c00000,
			 0x41e00000,
			 0x41500000,
			 0x41c00000,
			 0x41900000,

			 /// vpu register f7
			 0x40e00000,
			 0x3f800000,
			 0x41c80000,
			 0x41400000,
			 0x40a00000,
			 0x41b80000,
			 0x40000000,
			 0x41d00000,

			 /// vpu register f8
			 0x40a00000,
			 0x41500000,
			 0x41d80000,
			 0x3f800000,
			 0x40000000,
			 0x42000000,
			 0x41e00000,
			 0x40000000,

			 /// vpu register f9
			 0x41000000,
			 0x40c00000,
			 0x41400000,
			 0x41000000,
			 0x41400000,
			 0x40800000,
			 0x40800000,
			 0x41e00000,

			 /// vpu register f10
			 0x41e00000,
			 0x41f00000,
			 0x41b80000,
			 0x40e00000,
			 0x41100000,
			 0x41f80000,
			 0x41300000,
			 0x3f800000,

			 /// vpu register f11
			 0x41800000,
			 0x41e80000,
			 0x40400000,
			 0x41800000,
			 0x41800000,
			 0x41a00000,
			 0x41a80000,
			 0x41200000,

			 /// vpu register f12
			 0x41f00000,
			 0x41000000,
			 0x41300000,
			 0x41c80000,
			 0x41e80000,
			 0x41c00000,
			 0x42000000,
			 0x40000000,

			 /// vpu register f13
			 0x41900000,
			 0x40c00000,
			 0x41400000,
			 0x41000000,
			 0x41900000,
			 0x41c80000,
			 0x40800000,
			 0x40e00000,

			 /// vpu register f14
			 0x41880000,
			 0x41d00000,
			 0x41a00000,
			 0x40000000,
			 0x41900000,
			 0x41e00000,
			 0x41d80000,
			 0x41400000,

			 /// vpu register f15
			 0x41800000,
			 0x41b00000,
			 0x41c00000,
			 0x41e80000,
			 0x41700000,
			 0x42000000,
			 0x40400000,
			 0x42000000,

			 /// vpu register f16
			 0x41c80000,
			 0x41e00000,
			 0x41100000,
			 0x3f800000,
			 0x41800000,
			 0x41e80000,
			 0x41d00000,
			 0x41980000,

			 /// vpu register f17
			 0x40800000,
			 0x40a00000,
			 0x41a00000,
			 0x40a00000,
			 0x41100000,
			 0x41e00000,
			 0x41200000,
			 0x41f80000,

			 /// vpu register f18
			 0x41f00000,
			 0x40800000,
			 0x41d80000,
			 0x40000000,
			 0x40400000,
			 0x41900000,
			 0x41a00000,
			 0x41200000,

			 /// vpu register f19
			 0x40a00000,
			 0x41a00000,
			 0x41d00000,
			 0x41700000,
			 0x41c80000,
			 0x40c00000,
			 0x41e80000,
			 0x41600000,

			 /// vpu register f20
			 0x41100000,
			 0x41300000,
			 0x41500000,
			 0x41c00000,
			 0x41980000,
			 0x42000000,
			 0x41800000,
			 0x41980000,

			 /// vpu register f21
			 0x41d80000,
			 0x41c80000,
			 0x40000000,
			 0x41000000,
			 0x41980000,
			 0x41d00000,
			 0x41980000,
			 0x41a80000,

			 /// vpu register f22
			 0x41a00000,
			 0x41500000,
			 0x41b80000,
			 0x41700000,
			 0x41980000,
			 0x41700000,
			 0x41e80000,
			 0x41b80000,

			 /// vpu register f23
			 0x41a00000,
			 0x41c00000,
			 0x41d00000,
			 0x41600000,
			 0x41700000,
			 0x41b00000,
			 0x41800000,
			 0x40a00000,

			 /// vpu register f24
			 0x41900000,
			 0x41a00000,
			 0x40c00000,
			 0x40000000,
			 0x41100000,
			 0x41980000,
			 0x41600000,
			 0x41e00000,

			 /// vpu register f25
			 0x41980000,
			 0x40800000,
			 0x42000000,
			 0x41700000,
			 0x40000000,
			 0x41e00000,
			 0x41d80000,
			 0x42000000,

			 /// vpu register f26
			 0x41a00000,
			 0x41d80000,
			 0x41c80000,
			 0x40800000,
			 0x40400000,
			 0x41300000,
			 0x41c80000,
			 0x41f00000,

			 /// vpu register f27
			 0x40400000,
			 0x41800000,
			 0x41a80000,
			 0x41500000,
			 0x41300000,
			 0x41000000,
			 0x41d80000,
			 0x41600000,

			 /// vpu register f28
			 0x41a00000,
			 0x41f00000,
			 0x41e80000,
			 0x41d00000,
			 0x41a00000,
			 0x41800000,
			 0x40c00000,
			 0x41700000,

			 /// vpu register f29
			 0x41a80000,
			 0x41400000,
			 0x40800000,
			 0x41a80000,
			 0x41c00000,
			 0x41f00000,
			 0x41200000,
			 0x41300000,

			 /// vpu register f30
			 0x41c80000,
			 0x41f00000,
			 0x41880000,
			 0x40800000,
			 0x41400000,
			 0x41f80000,
			 0x41880000,
			 0x41f80000,

			 /// vpu register f31
			 0x41d80000,
			 0x41100000,
			 0x41200000,
			 0x40000000,
			 0x40e00000,
			 0x40a00000,
			 0x41b00000,
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
		"fmul.pi f30, f19, f26\n"                             ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f12, f30, f1\n"                              ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f27, f16, f25\n"                             ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f3, f10, f2\n"                               ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f18, f25, f20\n"                             ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f14, f14, f21\n"                             ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f22, f20, f2\n"                              ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f22, f27, f3\n"                              ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f20, f27, f1\n"                              ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f8, f25, f9\n"                               ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f5, f21, f19\n"                              ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f18, f14, f9\n"                              ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f22, f3, f11\n"                              ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f16, f15, f19\n"                             ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f28, f5, f25\n"                              ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f23, f28, f17\n"                             ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f23, f0, f25\n"                              ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f20, f17, f7\n"                              ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f8, f8, f23\n"                               ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f6, f18, f27\n"                              ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f16, f5, f12\n"                              ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f29, f14, f15\n"                             ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f24, f11, f4\n"                              ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f8, f25, f29\n"                              ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f16, f6, f17\n"                              ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f22, f27, f19\n"                             ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f26, f24, f2\n"                              ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f10, f28, f3\n"                              ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f19, f23, f1\n"                              ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f10, f7, f10\n"                              ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f24, f17, f25\n"                             ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f24, f18, f28\n"                             ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f18, f12, f21\n"                             ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f28, f30, f24\n"                             ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f21, f25, f25\n"                             ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f16, f17, f4\n"                              ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f25, f1, f7\n"                               ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f9, f8, f16\n"                               ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f28, f19, f25\n"                             ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f28, f24, f10\n"                             ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f28, f2, f5\n"                               ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f7, f14, f27\n"                              ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f2, f22, f19\n"                              ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f21, f13, f9\n"                              ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f19, f17, f27\n"                             ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f24, f27, f6\n"                              ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f8, f15, f15\n"                              ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f9, f0, f0\n"                                ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f9, f24, f30\n"                              ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f14, f6, f9\n"                               ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f8, f5, f8\n"                                ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f8, f25, f10\n"                              ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f23, f1, f4\n"                               ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f20, f23, f23\n"                             ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f12, f11, f6\n"                              ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f17, f20, f21\n"                             ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f12, f22, f19\n"                             ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f3, f8, f24\n"                               ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f4, f22, f9\n"                               ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f29, f10, f18\n"                             ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f10, f30, f20\n"                             ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f11, f16, f12\n"                             ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f13, f2, f9\n"                               ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f21, f6, f28\n"                              ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f16, f17, f28\n"                             ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f10, f25, f0\n"                              ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f20, f15, f11\n"                             ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f10, f2, f15\n"                              ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f30, f27, f25\n"                             ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f5, f17, f1\n"                               ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f18, f15, f24\n"                             ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f2, f20, f27\n"                              ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f3, f23, f8\n"                               ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f9, f12, f1\n"                               ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f2, f10, f18\n"                              ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f8, f18, f16\n"                              ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f14, f1, f8\n"                               ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f28, f24, f1\n"                              ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f12, f5, f27\n"                              ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f8, f18, f11\n"                              ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f27, f0, f14\n"                              ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f16, f9, f8\n"                               ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f8, f5, f7\n"                                ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f27, f9, f23\n"                              ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f29, f23, f25\n"                             ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f23, f19, f21\n"                             ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f15, f3, f24\n"                              ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f6, f8, f9\n"                                ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f9, f17, f29\n"                              ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f9, f13, f9\n"                               ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f26, f17, f26\n"                             ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f17, f0, f0\n"                               ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f20, f17, f13\n"                             ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f9, f30, f13\n"                              ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f6, f28, f20\n"                              ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f24, f1, f0\n"                               ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f13, f26, f13\n"                             ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f5, f20, f12\n"                              ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmul.pi f13, f20, f14\n"                             ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
