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
			 0x00011111,                                                  // 9.7958E-41                                  /// 00004
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00008
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0000c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00010
			 0xff800000,                                                  // -inf                                        /// 00014
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00018
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0001c
			 0xbf028f5c,                                                  // -0.51                                       /// 00020
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00024
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00028
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0002c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00030
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00034
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0003c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0x0c780000,                                                  // Leading 1s:                                 /// 00044
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00048
			 0x0c700000,                                                  // Leading 1s:                                 /// 0004c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00050
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00054
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00058
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0005c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00060
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00064
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00068
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0006c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00070
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00074
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00078
			 0x4b000000,                                                  // 8388608.0                                   /// 0007c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00080
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00084
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00088
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0008c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00090
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00094
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00098
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0009c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000a0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000a4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ac
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000b0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000b8
			 0x80000000,                                                  // -zero                                       /// 000bc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000c4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000c8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000cc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000d0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000d4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000dc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000e0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000e4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000ec
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000f0
			 0xffc00001,                                                  // -signaling NaN                              /// 000f4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000fc
			 0xcb000000,                                                  // -8388608.0                                  /// 00100
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00104
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00108
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0010c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00110
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00114
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00118
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0011c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00120
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00124
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00128
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00130
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00134
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00138
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0013c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00140
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00144
			 0x7fc00001,                                                  // signaling NaN                               /// 00148
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0014c
			 0x80000000,                                                  // -zero                                       /// 00150
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00154
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0015c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00160
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00164
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00168
			 0x7f800000,                                                  // inf                                         /// 0016c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00170
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00174
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00178
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0017c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00180
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00184
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00188
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0018c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00190
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00194
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00198
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0019c
			 0x80000000,                                                  // -zero                                       /// 001a0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001ac
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001b0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 001b4
			 0xffc00001,                                                  // -signaling NaN                              /// 001b8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 001bc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001c4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0x0e000007,                                                  // Trailing 1s:                                /// 001d0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001d4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001e4
			 0x80011111,                                                  // -9.7958E-41                                 /// 001e8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001ec
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001f0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 001f4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 001f8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 001fc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00200
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00208
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0020c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00210
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00214
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00218
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0021c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00220
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00224
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00228
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0022c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00230
			 0x0c400000,                                                  // Leading 1s:                                 /// 00234
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00238
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0023c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00240
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00248
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0024c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00250
			 0x00011111,                                                  // 9.7958E-41                                  /// 00254
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00258
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0025c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00264
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0x00000000,                                                  // zero                                        /// 0026c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00270
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0xcb000000,                                                  // -8388608.0                                  /// 00278
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0027c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00280
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00288
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00290
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00294
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00298
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0029c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 002a0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002a8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002ac
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002b0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002b4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002bc
			 0x0c780000,                                                  // Leading 1s:                                 /// 002c0
			 0x00011111,                                                  // 9.7958E-41                                  /// 002c4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002c8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 002cc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002d0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002d8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002dc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002e0
			 0x00011111,                                                  // 9.7958E-41                                  /// 002e4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002e8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002ec
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002f0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002f4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002f8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0xcb000000,                                                  // -8388608.0                                  /// 00304
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00308
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0030c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00310
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00314
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00318
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00320
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00324
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00328
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0032c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00330
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00334
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00338
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00344
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00348
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0034c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00354
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00358
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0035c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00360
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00364
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00368
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0036c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00370
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00374
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00378
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0037c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00380
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00384
			 0x4b000000,                                                  // 8388608.0                                   /// 00388
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0038c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00390
			 0x7f800000,                                                  // inf                                         /// 00394
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00398
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0039c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003a0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003a4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003ac
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003b0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 003b8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x00000000,                                                  // zero                                        /// 003c0
			 0x0c400000,                                                  // Leading 1s:                                 /// 003c4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003c8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003d4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 003d8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 003dc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 003e4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003ec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 003f4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0x7f800000,                                                  // inf                                         /// 003fc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00400
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00404
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00408
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0040c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00414
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00418
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0041c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00420
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00424
			 0xff800000,                                                  // -inf                                        /// 00428
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0042c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00430
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00434
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00438
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0043c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00440
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00444
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0044c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00450
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00454
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0x0e000001,                                                  // Trailing 1s:                                /// 0045c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00464
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0046c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00470
			 0x0e000001,                                                  // Trailing 1s:                                /// 00474
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00478
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0047c
			 0x00000000,                                                  // zero                                        /// 00480
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00484
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00488
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0048c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00490
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00498
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0049c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004a4
			 0xcb000000,                                                  // -8388608.0                                  /// 004a8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004ac
			 0x0e000003,                                                  // Trailing 1s:                                /// 004b0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004b4
			 0x0c400000,                                                  // Leading 1s:                                 /// 004b8
			 0xbf028f5c,                                                  // -0.51                                       /// 004bc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004c0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c4
			 0x0c600000,                                                  // Leading 1s:                                 /// 004c8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 004cc
			 0x3f028f5c,                                                  // 0.51                                        /// 004d0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004dc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004e4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004ec
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004f0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004f4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004f8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004fc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00500
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00504
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00508
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0xcb000000,                                                  // -8388608.0                                  /// 00510
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00514
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00518
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0051c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00520
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00524
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00528
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0052c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00530
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00534
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00538
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0053c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00540
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00544
			 0xffc00001,                                                  // -signaling NaN                              /// 00548
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0054c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00550
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00558
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0055c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00560
			 0x0e000007,                                                  // Trailing 1s:                                /// 00564
			 0x7fc00001,                                                  // signaling NaN                               /// 00568
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0xff800000,                                                  // -inf                                        /// 00570
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0057c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00580
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00584
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00588
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00594
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00598
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0059c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005a0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005a4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 005ac
			 0x7fc00001,                                                  // signaling NaN                               /// 005b0
			 0x3f028f5c,                                                  // 0.51                                        /// 005b4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005b8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 005bc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005c4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 005c8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 005cc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005d0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005d4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005d8
			 0x00011111,                                                  // 9.7958E-41                                  /// 005dc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 005e4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 005e8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 005f0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 005f8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005fc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00600
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00604
			 0x0c700000,                                                  // Leading 1s:                                 /// 00608
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0060c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00610
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00614
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00618
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0061c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00620
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00624
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00628
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0062c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00630
			 0x7fc00001,                                                  // signaling NaN                               /// 00634
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00638
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0063c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00640
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00644
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00648
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0064c
			 0x4b000000,                                                  // 8388608.0                                   /// 00650
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00654
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00658
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00660
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00664
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0066c
			 0x33333333,                                                  // 4 random values                             /// 00670
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00674
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0067c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00680
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00684
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0068c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00690
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00694
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00698
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0069c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006a0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 006a4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006a8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006ac
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006b8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006bc
			 0x80000000,                                                  // -zero                                       /// 006c0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006c4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006c8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006cc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006d8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006dc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e0
			 0x3f028f5c,                                                  // 0.51                                        /// 006e4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006e8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 006ec
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006f0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 006f8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006fc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x00011111,                                                  // 9.7958E-41                                  /// 00704
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00708
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0070c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00710
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00714
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00718
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0071c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00720
			 0x7fc00001,                                                  // signaling NaN                               /// 00724
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00728
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00730
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00734
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00738
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0073c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00740
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00748
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0074c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00750
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00754
			 0x7fc00001,                                                  // signaling NaN                               /// 00758
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0075c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00760
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00764
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00768
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0076c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00774
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00778
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0077c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00780
			 0xffc00001,                                                  // -signaling NaN                              /// 00784
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00788
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0078c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00790
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00794
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00798
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0079c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 007a0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 007a4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 007a8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007b0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 007b4
			 0x0c780000,                                                  // Leading 1s:                                 /// 007b8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 007bc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 007c0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007c4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007c8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007cc
			 0x0c700000,                                                  // Leading 1s:                                 /// 007d0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007d8
			 0xcb000000,                                                  // -8388608.0                                  /// 007dc
			 0xffc00001,                                                  // -signaling NaN                              /// 007e0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 007e4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 007ec
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007f0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 007f4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007f8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00800
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00804
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00808
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0080c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00810
			 0x0c600000,                                                  // Leading 1s:                                 /// 00814
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00818
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0081c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00820
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00824
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00828
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00834
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00838
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0083c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00840
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00844
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00848
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0084c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00850
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00854
			 0xbf028f5c,                                                  // -0.51                                       /// 00858
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0085c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00860
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00864
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0086c
			 0x3f028f5c,                                                  // 0.51                                        /// 00870
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00874
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00878
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0087c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00880
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00884
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00888
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0088c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00890
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00894
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00898
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008a0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 008a4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 008a8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008ac
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008b0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 008b8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008bc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 008cc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 008d0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 008d8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008dc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e0
			 0xbf028f5c,                                                  // -0.51                                       /// 008e4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 008ec
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008f0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008f4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008f8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00900
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00904
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00908
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00918
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0091c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00920
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00924
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00928
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0092c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00930
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00934
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00938
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0093c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00940
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00944
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00948
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0094c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00954
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00958
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0095c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00964
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00968
			 0x7fc00001,                                                  // signaling NaN                               /// 0096c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00970
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00974
			 0xcb000000,                                                  // -8388608.0                                  /// 00978
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0097c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00980
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00984
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0x0c400000,                                                  // Leading 1s:                                 /// 0098c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00990
			 0xffc00001,                                                  // -signaling NaN                              /// 00994
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00998
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0099c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009a0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009a4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009a8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 009b8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 009bc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 009cc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 009d0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009d4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d8
			 0x0c780000,                                                  // Leading 1s:                                 /// 009dc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009e0
			 0xbf028f5c,                                                  // -0.51                                       /// 009e4
			 0x0e000007,                                                  // Trailing 1s:                                /// 009e8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ec
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009f4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009f8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 009fc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a00
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a08
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a0c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a10
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a18
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a1c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a20
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a24
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x7f800000,                                                  // inf                                         /// 00a30
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a34
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a38
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a3c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a40
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a44
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a48
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a4c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a50
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a54
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a5c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a60
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a64
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a68
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a70
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a74
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a78
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a7c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a80
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a84
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a88
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a8c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a90
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a94
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a98
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a9c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00aa0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00aa4
			 0x3f028f5c,                                                  // 0.51                                        /// 00aa8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00aac
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ab0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ab4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ab8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00abc
			 0x7fc00001,                                                  // signaling NaN                               /// 00ac0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ac4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ac8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x3f028f5c,                                                  // 0.51                                        /// 00ad0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ad4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00adc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ae0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ae4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ae8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00aec
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00af0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00af8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b00
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b04
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b08
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b0c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b10
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b14
			 0xff800000,                                                  // -inf                                        /// 00b18
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b20
			 0x3f028f5c,                                                  // 0.51                                        /// 00b24
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b28
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b2c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b30
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b34
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b38
			 0x80000000,                                                  // -zero                                       /// 00b3c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b40
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b44
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b48
			 0x33333333,                                                  // 4 random values                             /// 00b4c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b50
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b54
			 0x7f800000,                                                  // inf                                         /// 00b58
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b5c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b60
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b64
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b68
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b6c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b70
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b74
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b78
			 0x3f028f5c,                                                  // 0.51                                        /// 00b7c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b80
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b84
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b88
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b90
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b94
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b98
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ba4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ba8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bac
			 0x33333333,                                                  // 4 random values                             /// 00bb0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00bb4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00bb8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00bc0
			 0x55555555,                                                  // 4 random values                             /// 00bc4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00bc8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bcc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00bd0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00bd4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bdc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00be4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00bec
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00bf0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00bf8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00bfc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c00
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c08
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c0c
			 0xbf028f5c,                                                  // -0.51                                       /// 00c10
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c14
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c18
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c1c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c24
			 0x3f028f5c,                                                  // 0.51                                        /// 00c28
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c2c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c30
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c34
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c38
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c3c
			 0xffc00001,                                                  // -signaling NaN                              /// 00c40
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c48
			 0x80000000,                                                  // -zero                                       /// 00c4c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c50
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c54
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c58
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c5c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0x7fc00001,                                                  // signaling NaN                               /// 00c64
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c68
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c6c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c70
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c74
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c78
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c7c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c80
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c84
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c88
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c8c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c90
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c94
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c98
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c9c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ca0
			 0x4b000000,                                                  // 8388608.0                                   /// 00ca4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cac
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00cb0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00cb4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00cbc
			 0xffc00001,                                                  // -signaling NaN                              /// 00cc0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00cc8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ccc
			 0xffc00001,                                                  // -signaling NaN                              /// 00cd0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00cd4
			 0x3f028f5c,                                                  // 0.51                                        /// 00cd8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ce4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ce8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00cec
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00cf4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00cf8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00cfc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d00
			 0x7f800000,                                                  // inf                                         /// 00d04
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d08
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d0c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d10
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d14
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d18
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d1c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d20
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d24
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d28
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d2c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d30
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d34
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d38
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d3c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d44
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d48
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d4c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d50
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d58
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d5c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d60
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d64
			 0x7fc00001,                                                  // signaling NaN                               /// 00d68
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d6c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d70
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d74
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d7c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d80
			 0xff800000,                                                  // -inf                                        /// 00d84
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d88
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d8c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d90
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d94
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d98
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d9c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00da0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00da4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00da8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00dac
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00db4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00db8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00dbc
			 0xcb000000,                                                  // -8388608.0                                  /// 00dc0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00dc4
			 0xffc00001,                                                  // -signaling NaN                              /// 00dc8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00dcc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00dd4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00dd8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ddc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00de0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00de4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00de8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00dec
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00df0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00df8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00dfc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e00
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e04
			 0xcb000000,                                                  // -8388608.0                                  /// 00e08
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e0c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e10
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e14
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e18
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e1c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e24
			 0x80000000,                                                  // -zero                                       /// 00e28
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e2c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e30
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e34
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e38
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e3c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e40
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e44
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e48
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e4c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e50
			 0x00000000,                                                  // zero                                        /// 00e54
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e58
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e5c
			 0x7f800000,                                                  // inf                                         /// 00e60
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e64
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e68
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e6c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e74
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e78
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e7c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e80
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e84
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e88
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e8c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e90
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e98
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e9c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ea0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ea4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00eac
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x3f028f5c,                                                  // 0.51                                        /// 00eb8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ebc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ec4
			 0x7f800000,                                                  // inf                                         /// 00ec8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ed0
			 0x4b000000,                                                  // 8388608.0                                   /// 00ed4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ed8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00edc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ee0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ee4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ee8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00eec
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ef0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ef4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ef8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00efc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f00
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f04
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f08
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f0c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f10
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f14
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f18
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f1c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f20
			 0x4b000000,                                                  // 8388608.0                                   /// 00f24
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f28
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f2c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f30
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f34
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f38
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f3c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f40
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f44
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f48
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f4c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f50
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f54
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f58
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f5c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f60
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f64
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f68
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f6c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f70
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f74
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f7c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f80
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f84
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f88
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f8c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f90
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f94
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f98
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f9c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00fa0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fa8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00fac
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00fb0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00fb8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00fbc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00fc0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00fc4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00fc8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fcc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00fd8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fdc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fe0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00fe4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00fe8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ff0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ff4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff8
			 0x80000000 // 0                                             // SP - ve numbers                             /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0xf99e610f, /// 0x0
			 0x5f52d3f1, /// 0x4
			 0x8f88eff1, /// 0x8
			 0xd3b808f4, /// 0xc
			 0x9af481be, /// 0x10
			 0x152ba494, /// 0x14
			 0x7196fa7f, /// 0x18
			 0x76858edf, /// 0x1c
			 0x5e6f2ffc, /// 0x20
			 0x4f78cea3, /// 0x24
			 0xc1daf653, /// 0x28
			 0x90c4c63a, /// 0x2c
			 0x4b8ce891, /// 0x30
			 0x0f525d15, /// 0x34
			 0x6a439236, /// 0x38
			 0x1a53a870, /// 0x3c
			 0xf0d19920, /// 0x40
			 0xbab1ff82, /// 0x44
			 0xca03db29, /// 0x48
			 0x8a930094, /// 0x4c
			 0xa55c3a7f, /// 0x50
			 0x910b38b1, /// 0x54
			 0xba181846, /// 0x58
			 0xe3604886, /// 0x5c
			 0xa116eb2b, /// 0x60
			 0x2c499f76, /// 0x64
			 0x7f9dc1d7, /// 0x68
			 0x83ea862a, /// 0x6c
			 0x6c6304f3, /// 0x70
			 0xefd4542a, /// 0x74
			 0x6177468a, /// 0x78
			 0x17639ff5, /// 0x7c
			 0x1583399e, /// 0x80
			 0x26501bf9, /// 0x84
			 0x6f5d05a6, /// 0x88
			 0x4ec75b60, /// 0x8c
			 0xc7f0c7d8, /// 0x90
			 0x2b5f0ebe, /// 0x94
			 0xbb2dd71d, /// 0x98
			 0xe3c417b5, /// 0x9c
			 0x709c8062, /// 0xa0
			 0xae765a82, /// 0xa4
			 0xcb80b2e7, /// 0xa8
			 0x01b42c55, /// 0xac
			 0xa3712949, /// 0xb0
			 0x81abd786, /// 0xb4
			 0x1fc4ac0b, /// 0xb8
			 0x79b59289, /// 0xbc
			 0xa074c4fb, /// 0xc0
			 0xf8d3119a, /// 0xc4
			 0x44965da2, /// 0xc8
			 0x0139f3b3, /// 0xcc
			 0x20ee9f5c, /// 0xd0
			 0xfb1cc3b8, /// 0xd4
			 0xdf5f2d00, /// 0xd8
			 0xa637d2b2, /// 0xdc
			 0x5359ef30, /// 0xe0
			 0xc319df92, /// 0xe4
			 0x1abbb95a, /// 0xe8
			 0x9e4c6146, /// 0xec
			 0xd6de61a4, /// 0xf0
			 0xfe656bae, /// 0xf4
			 0xe4ec194b, /// 0xf8
			 0xce45ebe8, /// 0xfc
			 0xacd6dee5, /// 0x100
			 0xacfdfe0b, /// 0x104
			 0x27abb0bf, /// 0x108
			 0x3010336c, /// 0x10c
			 0x8e8b21a6, /// 0x110
			 0x0f1229e6, /// 0x114
			 0x88a2e54e, /// 0x118
			 0x70894e40, /// 0x11c
			 0x806c17e2, /// 0x120
			 0x2da4c7a0, /// 0x124
			 0xe92c0522, /// 0x128
			 0xc7e83612, /// 0x12c
			 0x4308cd19, /// 0x130
			 0xef4913a2, /// 0x134
			 0x0cd1324c, /// 0x138
			 0x384500bc, /// 0x13c
			 0x806dbb50, /// 0x140
			 0xb1f7c9b7, /// 0x144
			 0xa96a5c42, /// 0x148
			 0xe5a2681c, /// 0x14c
			 0xeb70bd7c, /// 0x150
			 0x2c8b763c, /// 0x154
			 0xeec905f3, /// 0x158
			 0x4fc8fec5, /// 0x15c
			 0xa4431ee1, /// 0x160
			 0x335ecdba, /// 0x164
			 0x59df2042, /// 0x168
			 0x42b13b30, /// 0x16c
			 0xbea8dcf3, /// 0x170
			 0x83b6cc43, /// 0x174
			 0xeeddf481, /// 0x178
			 0x1ab89e9c, /// 0x17c
			 0xd9142671, /// 0x180
			 0x77cc03ef, /// 0x184
			 0x1fe7acd1, /// 0x188
			 0x79fdc51c, /// 0x18c
			 0xd95e61e7, /// 0x190
			 0x625f1b8b, /// 0x194
			 0xda9bcbd8, /// 0x198
			 0xd7cfabd4, /// 0x19c
			 0xb7af92da, /// 0x1a0
			 0x5df7899b, /// 0x1a4
			 0xe4d96a5b, /// 0x1a8
			 0x706e7e73, /// 0x1ac
			 0x39e10717, /// 0x1b0
			 0xe4615098, /// 0x1b4
			 0xe600797e, /// 0x1b8
			 0xed4bfb88, /// 0x1bc
			 0x08f48bc9, /// 0x1c0
			 0x38aea155, /// 0x1c4
			 0x2526a9ff, /// 0x1c8
			 0xc544eed9, /// 0x1cc
			 0x92a91abc, /// 0x1d0
			 0x7b5a83fc, /// 0x1d4
			 0x08a4f15f, /// 0x1d8
			 0x3932684f, /// 0x1dc
			 0x6eac98de, /// 0x1e0
			 0xbf32d896, /// 0x1e4
			 0x189a29f1, /// 0x1e8
			 0x91de950e, /// 0x1ec
			 0x7b663d79, /// 0x1f0
			 0xfe37a65b, /// 0x1f4
			 0xa8273c33, /// 0x1f8
			 0x9daddca3, /// 0x1fc
			 0x83b1245c, /// 0x200
			 0x219f5a85, /// 0x204
			 0x778e8b35, /// 0x208
			 0xf7f2daf7, /// 0x20c
			 0x5afa18c1, /// 0x210
			 0x85da2a76, /// 0x214
			 0xd1591f4e, /// 0x218
			 0x5ff70448, /// 0x21c
			 0x66598538, /// 0x220
			 0x09135754, /// 0x224
			 0x2a237a10, /// 0x228
			 0x4020c91d, /// 0x22c
			 0xdfb8cedf, /// 0x230
			 0xb52848f4, /// 0x234
			 0x7fccdad9, /// 0x238
			 0xbd14eac6, /// 0x23c
			 0xf482d76f, /// 0x240
			 0x98e6f1cd, /// 0x244
			 0x82ddfcc0, /// 0x248
			 0xb944be85, /// 0x24c
			 0x1d8ed5e0, /// 0x250
			 0x37b9dca1, /// 0x254
			 0x48ebda98, /// 0x258
			 0x32910824, /// 0x25c
			 0x196030ea, /// 0x260
			 0x2ad70c16, /// 0x264
			 0x6575f1d0, /// 0x268
			 0x3b338b3f, /// 0x26c
			 0xc1c2d433, /// 0x270
			 0xf5cc1d55, /// 0x274
			 0x13883aa7, /// 0x278
			 0x65ba1c16, /// 0x27c
			 0x06e09dd9, /// 0x280
			 0x5844d77c, /// 0x284
			 0xb38aa492, /// 0x288
			 0x79c1d236, /// 0x28c
			 0xda67c3d1, /// 0x290
			 0xf4351861, /// 0x294
			 0x18697dcb, /// 0x298
			 0x94d667fd, /// 0x29c
			 0x4c362550, /// 0x2a0
			 0x24d284e0, /// 0x2a4
			 0x26fad221, /// 0x2a8
			 0xb9cd7192, /// 0x2ac
			 0x9b27e040, /// 0x2b0
			 0x0f889925, /// 0x2b4
			 0x8264be6a, /// 0x2b8
			 0x44eba703, /// 0x2bc
			 0xf7f331d6, /// 0x2c0
			 0x2ba50323, /// 0x2c4
			 0x1c72a26d, /// 0x2c8
			 0x3330dd46, /// 0x2cc
			 0x220ab6f3, /// 0x2d0
			 0x6e990093, /// 0x2d4
			 0x1d1c1505, /// 0x2d8
			 0x8bd7b8ea, /// 0x2dc
			 0x1b442e10, /// 0x2e0
			 0xb3e1b57a, /// 0x2e4
			 0xad106707, /// 0x2e8
			 0x564a51d2, /// 0x2ec
			 0xaef08a11, /// 0x2f0
			 0x7556bb23, /// 0x2f4
			 0x11537ca0, /// 0x2f8
			 0x8b29d572, /// 0x2fc
			 0x11216c05, /// 0x300
			 0x8a93abd3, /// 0x304
			 0x9918091e, /// 0x308
			 0xd99e993a, /// 0x30c
			 0xe842dd8b, /// 0x310
			 0xe1d4b729, /// 0x314
			 0x3ead7d40, /// 0x318
			 0x532af54c, /// 0x31c
			 0x22b27c05, /// 0x320
			 0x558f7b17, /// 0x324
			 0x64e34cd9, /// 0x328
			 0x19696a22, /// 0x32c
			 0x7e1852fd, /// 0x330
			 0x3fde8fcb, /// 0x334
			 0x3aa9429a, /// 0x338
			 0xddea70a1, /// 0x33c
			 0x6efd3b05, /// 0x340
			 0x2f7adf1d, /// 0x344
			 0x4eccc432, /// 0x348
			 0xe8721431, /// 0x34c
			 0x73605bbe, /// 0x350
			 0x4662e046, /// 0x354
			 0xed5ad208, /// 0x358
			 0x4ea5e8e7, /// 0x35c
			 0x3263dac6, /// 0x360
			 0xe1800169, /// 0x364
			 0x8e336536, /// 0x368
			 0xc7288573, /// 0x36c
			 0x735d4709, /// 0x370
			 0x609d67de, /// 0x374
			 0x818fe116, /// 0x378
			 0x23e30225, /// 0x37c
			 0xc7183151, /// 0x380
			 0x1d70c682, /// 0x384
			 0x7f9b1067, /// 0x388
			 0xf546d7f9, /// 0x38c
			 0x33a0fa3b, /// 0x390
			 0xd0c58b98, /// 0x394
			 0x788e9a25, /// 0x398
			 0x2cc8da4e, /// 0x39c
			 0xaf1d6ac4, /// 0x3a0
			 0x369f2ceb, /// 0x3a4
			 0x316ab7aa, /// 0x3a8
			 0x16ed7003, /// 0x3ac
			 0x8a38303f, /// 0x3b0
			 0xf92b3d1e, /// 0x3b4
			 0x91b6643f, /// 0x3b8
			 0xe16962ad, /// 0x3bc
			 0xa55e8f47, /// 0x3c0
			 0x31c3a398, /// 0x3c4
			 0x5b3564bb, /// 0x3c8
			 0xc3d94b69, /// 0x3cc
			 0x4f4ae4de, /// 0x3d0
			 0xe56818cd, /// 0x3d4
			 0x4c135415, /// 0x3d8
			 0xd15e303e, /// 0x3dc
			 0x7c7e1fe2, /// 0x3e0
			 0xd8afdf77, /// 0x3e4
			 0xa378149b, /// 0x3e8
			 0x1298476b, /// 0x3ec
			 0x64cda001, /// 0x3f0
			 0xb35af0d4, /// 0x3f4
			 0x536279c4, /// 0x3f8
			 0x6cc2cdad, /// 0x3fc
			 0x6513b434, /// 0x400
			 0x72fbc746, /// 0x404
			 0xf2282d2d, /// 0x408
			 0x4907c01e, /// 0x40c
			 0x981654a7, /// 0x410
			 0x0d2a8477, /// 0x414
			 0x3dd364f7, /// 0x418
			 0x011ee710, /// 0x41c
			 0x841e0d03, /// 0x420
			 0x05950467, /// 0x424
			 0x01701ccd, /// 0x428
			 0xeaf60a44, /// 0x42c
			 0xeed2677a, /// 0x430
			 0x60c8c0f7, /// 0x434
			 0xe28b776a, /// 0x438
			 0x207b0c3a, /// 0x43c
			 0x3f3d7da1, /// 0x440
			 0xb1521f7b, /// 0x444
			 0x99522196, /// 0x448
			 0xf55bfae1, /// 0x44c
			 0x364d056a, /// 0x450
			 0xdedff1b7, /// 0x454
			 0x3d6b34f9, /// 0x458
			 0x05f0cab8, /// 0x45c
			 0x940b4e49, /// 0x460
			 0x36f78e66, /// 0x464
			 0x6c2f783e, /// 0x468
			 0xd5aa8057, /// 0x46c
			 0x18bf5751, /// 0x470
			 0xd21b7bb1, /// 0x474
			 0xd1b05f56, /// 0x478
			 0x6cdfba71, /// 0x47c
			 0x42d50758, /// 0x480
			 0x93d2956f, /// 0x484
			 0xc43bc886, /// 0x488
			 0xb2c04ba5, /// 0x48c
			 0x0aea48fb, /// 0x490
			 0x819158e2, /// 0x494
			 0x0ed711bd, /// 0x498
			 0x8f10cae9, /// 0x49c
			 0x4a55d1be, /// 0x4a0
			 0xc44149f9, /// 0x4a4
			 0x1fa8f0b3, /// 0x4a8
			 0x88833267, /// 0x4ac
			 0x7ed2ad17, /// 0x4b0
			 0x5c97bb9e, /// 0x4b4
			 0x9712e80b, /// 0x4b8
			 0xdd49af1f, /// 0x4bc
			 0x9bb93c24, /// 0x4c0
			 0xed3bca00, /// 0x4c4
			 0xfd94b407, /// 0x4c8
			 0xdb72428c, /// 0x4cc
			 0xb0c8466e, /// 0x4d0
			 0xc17c9d60, /// 0x4d4
			 0xbda851be, /// 0x4d8
			 0xc1e2c6fa, /// 0x4dc
			 0x44b89b7c, /// 0x4e0
			 0x64c459be, /// 0x4e4
			 0x42ef45c5, /// 0x4e8
			 0xc8a90483, /// 0x4ec
			 0x6c6a578d, /// 0x4f0
			 0x4ef13961, /// 0x4f4
			 0xb6e7b8f9, /// 0x4f8
			 0x0334ff11, /// 0x4fc
			 0x6a9ab839, /// 0x500
			 0x0d31a95b, /// 0x504
			 0xf4cfc978, /// 0x508
			 0xfd1cf8e7, /// 0x50c
			 0xf4e16699, /// 0x510
			 0xacb6bef5, /// 0x514
			 0x8962ef5e, /// 0x518
			 0xcc4cbf23, /// 0x51c
			 0xa2584275, /// 0x520
			 0xfe2ba302, /// 0x524
			 0xf26a7fbd, /// 0x528
			 0x93b10dd6, /// 0x52c
			 0x3f4896fd, /// 0x530
			 0x5acaec33, /// 0x534
			 0x1a6ef72d, /// 0x538
			 0x9a1288ff, /// 0x53c
			 0xe3e5ed88, /// 0x540
			 0x63a7a824, /// 0x544
			 0xfb1086ab, /// 0x548
			 0x10f956d4, /// 0x54c
			 0xa9d8a776, /// 0x550
			 0xb5ee5ba0, /// 0x554
			 0xb4f6dfa4, /// 0x558
			 0xdc0e2633, /// 0x55c
			 0xea6b3f2a, /// 0x560
			 0x135a1b90, /// 0x564
			 0x4a352ebc, /// 0x568
			 0x32830857, /// 0x56c
			 0xd2d5fb2e, /// 0x570
			 0x392dd1e9, /// 0x574
			 0x09df8398, /// 0x578
			 0x936357e7, /// 0x57c
			 0xd680c246, /// 0x580
			 0x042fb60b, /// 0x584
			 0xf99bb3f2, /// 0x588
			 0x9784c220, /// 0x58c
			 0x94211aa4, /// 0x590
			 0xf3c638d4, /// 0x594
			 0x8d961d5b, /// 0x598
			 0x1d49a680, /// 0x59c
			 0x20df93ac, /// 0x5a0
			 0x924fe836, /// 0x5a4
			 0x1dd1906d, /// 0x5a8
			 0xbc20e630, /// 0x5ac
			 0x2e3dc7c3, /// 0x5b0
			 0x994d3534, /// 0x5b4
			 0x45972d07, /// 0x5b8
			 0xca9825f1, /// 0x5bc
			 0xfb11abc1, /// 0x5c0
			 0x7303571e, /// 0x5c4
			 0x57773c1c, /// 0x5c8
			 0x3aa906be, /// 0x5cc
			 0x36a339fe, /// 0x5d0
			 0x8adf9ff8, /// 0x5d4
			 0x033e1da1, /// 0x5d8
			 0x6493c53f, /// 0x5dc
			 0xd40bbcc9, /// 0x5e0
			 0x3459f915, /// 0x5e4
			 0x63a85f63, /// 0x5e8
			 0x4de217d4, /// 0x5ec
			 0x76a250cd, /// 0x5f0
			 0xd1b811bf, /// 0x5f4
			 0x6be24b7d, /// 0x5f8
			 0x04e7abc4, /// 0x5fc
			 0x06edec8c, /// 0x600
			 0x83d1bacc, /// 0x604
			 0x2ce0f9bf, /// 0x608
			 0xb2c63d68, /// 0x60c
			 0x383a8001, /// 0x610
			 0x3113f036, /// 0x614
			 0x3d7c99f0, /// 0x618
			 0x0c90d6e9, /// 0x61c
			 0x617ab0f0, /// 0x620
			 0x1bed85f0, /// 0x624
			 0x490f89a3, /// 0x628
			 0xc06298d6, /// 0x62c
			 0xada6430a, /// 0x630
			 0x44b93eda, /// 0x634
			 0x48e107ab, /// 0x638
			 0x4b32023d, /// 0x63c
			 0xcd8de6fd, /// 0x640
			 0x7e86aded, /// 0x644
			 0xefdb151a, /// 0x648
			 0x33ffadef, /// 0x64c
			 0x0ac27761, /// 0x650
			 0x0c479772, /// 0x654
			 0xccceac41, /// 0x658
			 0x5deb09ae, /// 0x65c
			 0x11384152, /// 0x660
			 0x38e088e3, /// 0x664
			 0xa6019841, /// 0x668
			 0x00cdfefb, /// 0x66c
			 0xb5a49b5e, /// 0x670
			 0x77d65db1, /// 0x674
			 0x5a96bee5, /// 0x678
			 0x36e28fa2, /// 0x67c
			 0xfd491cb7, /// 0x680
			 0x09241be4, /// 0x684
			 0xe0890b5e, /// 0x688
			 0x6980414f, /// 0x68c
			 0x938b806d, /// 0x690
			 0x90e73fd8, /// 0x694
			 0xf67c8958, /// 0x698
			 0x917293c8, /// 0x69c
			 0x8bfc6c26, /// 0x6a0
			 0xe741304a, /// 0x6a4
			 0x4ae4fd6c, /// 0x6a8
			 0x63aed040, /// 0x6ac
			 0x70d7a87e, /// 0x6b0
			 0x82095031, /// 0x6b4
			 0xae1c4404, /// 0x6b8
			 0x5812ffbe, /// 0x6bc
			 0x6bfba88e, /// 0x6c0
			 0x4a3672db, /// 0x6c4
			 0x6b0554d0, /// 0x6c8
			 0x816739df, /// 0x6cc
			 0x12e0ce20, /// 0x6d0
			 0x7acac3c6, /// 0x6d4
			 0xc930b4dc, /// 0x6d8
			 0xb44cd1d5, /// 0x6dc
			 0x528e59ef, /// 0x6e0
			 0xbacfcaee, /// 0x6e4
			 0x20e0c0d9, /// 0x6e8
			 0x8f2031a8, /// 0x6ec
			 0xb84d191c, /// 0x6f0
			 0x36c17a96, /// 0x6f4
			 0xb655b3d7, /// 0x6f8
			 0x6a757769, /// 0x6fc
			 0x599bf836, /// 0x700
			 0x47981e3e, /// 0x704
			 0x721dc5ee, /// 0x708
			 0x8ea7c5e0, /// 0x70c
			 0x860c0e6f, /// 0x710
			 0xc06ceb90, /// 0x714
			 0x1ea16f96, /// 0x718
			 0x0254d3d9, /// 0x71c
			 0x050b71a0, /// 0x720
			 0xa524f826, /// 0x724
			 0xa3c540ab, /// 0x728
			 0x4d529214, /// 0x72c
			 0x4d823450, /// 0x730
			 0x2920f5d9, /// 0x734
			 0x5e7d20e4, /// 0x738
			 0x8cafdff4, /// 0x73c
			 0x9d08e12b, /// 0x740
			 0x4667e010, /// 0x744
			 0x38bdbfa4, /// 0x748
			 0x4eb80abf, /// 0x74c
			 0x47d0f90a, /// 0x750
			 0x8fe90b9d, /// 0x754
			 0xa0503a33, /// 0x758
			 0x741f1519, /// 0x75c
			 0xeb94f752, /// 0x760
			 0x92e41bd5, /// 0x764
			 0x06476eaf, /// 0x768
			 0xfbce83c8, /// 0x76c
			 0xf939d5e4, /// 0x770
			 0x59692b56, /// 0x774
			 0x1edaf094, /// 0x778
			 0x51deb179, /// 0x77c
			 0x25d26270, /// 0x780
			 0x79f68457, /// 0x784
			 0x64571b2f, /// 0x788
			 0x900e72a5, /// 0x78c
			 0xf7aeea01, /// 0x790
			 0x3b5cacc6, /// 0x794
			 0x956ccfb6, /// 0x798
			 0x91d3d0a9, /// 0x79c
			 0x500e955b, /// 0x7a0
			 0x9f242b61, /// 0x7a4
			 0xd9c11663, /// 0x7a8
			 0xcad7b7ea, /// 0x7ac
			 0x2674649b, /// 0x7b0
			 0x08d71f90, /// 0x7b4
			 0xdda137b3, /// 0x7b8
			 0xa0c5dc70, /// 0x7bc
			 0xd9fa2a8c, /// 0x7c0
			 0x6e462ed5, /// 0x7c4
			 0x120710f4, /// 0x7c8
			 0x3d12e923, /// 0x7cc
			 0x2281b1fa, /// 0x7d0
			 0x2b735665, /// 0x7d4
			 0x0d7d586b, /// 0x7d8
			 0x6a35e241, /// 0x7dc
			 0x611e812b, /// 0x7e0
			 0x35ecd3c8, /// 0x7e4
			 0xb29a8be6, /// 0x7e8
			 0xc35c73ac, /// 0x7ec
			 0xa6fe7e26, /// 0x7f0
			 0x17d0f95a, /// 0x7f4
			 0xf58b68e0, /// 0x7f8
			 0xe1054db1, /// 0x7fc
			 0xcc46a165, /// 0x800
			 0x5c830096, /// 0x804
			 0x29a9b35a, /// 0x808
			 0x2365fa8b, /// 0x80c
			 0xe9235b93, /// 0x810
			 0x68af3a9b, /// 0x814
			 0x38287ac1, /// 0x818
			 0x2db0c03f, /// 0x81c
			 0x387656e9, /// 0x820
			 0xca798907, /// 0x824
			 0xf9aafcd3, /// 0x828
			 0x692e9e7b, /// 0x82c
			 0xb192c06c, /// 0x830
			 0x1074e649, /// 0x834
			 0x03ae5360, /// 0x838
			 0xc1316583, /// 0x83c
			 0xc5330ef6, /// 0x840
			 0xb139f672, /// 0x844
			 0xc8510775, /// 0x848
			 0xb5372499, /// 0x84c
			 0xc8f8623f, /// 0x850
			 0x7e4d56e8, /// 0x854
			 0x902d3752, /// 0x858
			 0x13c427bd, /// 0x85c
			 0x29a9c46d, /// 0x860
			 0x8f69f0de, /// 0x864
			 0x86a21780, /// 0x868
			 0x87a74050, /// 0x86c
			 0xa8ffa6c7, /// 0x870
			 0x43a80b0b, /// 0x874
			 0x835a4c5c, /// 0x878
			 0xef4a6d10, /// 0x87c
			 0x9eff8dcb, /// 0x880
			 0xce885e59, /// 0x884
			 0xe0f337f3, /// 0x888
			 0x84c28550, /// 0x88c
			 0x97f20cb5, /// 0x890
			 0x20b7156c, /// 0x894
			 0xce73dbf0, /// 0x898
			 0x4252a78c, /// 0x89c
			 0xae273021, /// 0x8a0
			 0x620a8721, /// 0x8a4
			 0xddbfc8a5, /// 0x8a8
			 0x61578918, /// 0x8ac
			 0xd680c1d3, /// 0x8b0
			 0x46d88acd, /// 0x8b4
			 0xa313f8b1, /// 0x8b8
			 0xc93b24b8, /// 0x8bc
			 0xf0e78b66, /// 0x8c0
			 0x99cfb175, /// 0x8c4
			 0xebfd08c0, /// 0x8c8
			 0x1cebee2e, /// 0x8cc
			 0xae2708d7, /// 0x8d0
			 0xc8bef400, /// 0x8d4
			 0x09ca3da4, /// 0x8d8
			 0x4093eda8, /// 0x8dc
			 0x8818bc4e, /// 0x8e0
			 0x5140adc7, /// 0x8e4
			 0x82c7b0bb, /// 0x8e8
			 0x2086668f, /// 0x8ec
			 0x17cb0f8b, /// 0x8f0
			 0xaa5463da, /// 0x8f4
			 0x51330283, /// 0x8f8
			 0x2bf07aab, /// 0x8fc
			 0x002de764, /// 0x900
			 0x9e6dba80, /// 0x904
			 0x5476cef3, /// 0x908
			 0x27a6ba2a, /// 0x90c
			 0xdaf5b482, /// 0x910
			 0xf60ce74e, /// 0x914
			 0x075f9d15, /// 0x918
			 0xe6d9ff0c, /// 0x91c
			 0x953dbfd0, /// 0x920
			 0xf4ee9960, /// 0x924
			 0x929b70dc, /// 0x928
			 0x9cea0a0e, /// 0x92c
			 0x587d185b, /// 0x930
			 0xd69d0957, /// 0x934
			 0x8feb4b4e, /// 0x938
			 0xa350a071, /// 0x93c
			 0xfee54073, /// 0x940
			 0xb39c09e5, /// 0x944
			 0x6ce15662, /// 0x948
			 0xacbcb6fe, /// 0x94c
			 0x9c5aa89c, /// 0x950
			 0xa1c44953, /// 0x954
			 0xe9db0195, /// 0x958
			 0x6d9b12ea, /// 0x95c
			 0x84f8c6a8, /// 0x960
			 0x2890d063, /// 0x964
			 0x32dfecee, /// 0x968
			 0xaf6dc3a9, /// 0x96c
			 0x731a8ea8, /// 0x970
			 0x34501776, /// 0x974
			 0x1fe1f562, /// 0x978
			 0x0c92ab02, /// 0x97c
			 0xc7837803, /// 0x980
			 0xdc0f1c51, /// 0x984
			 0x837edafc, /// 0x988
			 0xb9c8af59, /// 0x98c
			 0xf854cedd, /// 0x990
			 0xb118ccb0, /// 0x994
			 0xf470f5af, /// 0x998
			 0xaba21c5a, /// 0x99c
			 0x0e98c2b1, /// 0x9a0
			 0xb328ca43, /// 0x9a4
			 0x7b835457, /// 0x9a8
			 0xaa5f617b, /// 0x9ac
			 0xd31d0690, /// 0x9b0
			 0x53f98d83, /// 0x9b4
			 0x3981eef0, /// 0x9b8
			 0xdf5fe4b2, /// 0x9bc
			 0xda37b109, /// 0x9c0
			 0x29203354, /// 0x9c4
			 0x7b5a9e92, /// 0x9c8
			 0x890e64ee, /// 0x9cc
			 0xcdc9517e, /// 0x9d0
			 0x884fd517, /// 0x9d4
			 0xe5bd2ab3, /// 0x9d8
			 0x07a786a7, /// 0x9dc
			 0xae6a26ea, /// 0x9e0
			 0xdcbeb9e5, /// 0x9e4
			 0x6c161554, /// 0x9e8
			 0xd54ed345, /// 0x9ec
			 0x0337bdb0, /// 0x9f0
			 0x604e430b, /// 0x9f4
			 0x892d9060, /// 0x9f8
			 0x551c33db, /// 0x9fc
			 0xdca1bb85, /// 0xa00
			 0x7e3b64f8, /// 0xa04
			 0xe693f8cc, /// 0xa08
			 0x4cb6f7e6, /// 0xa0c
			 0x4fd8e0e1, /// 0xa10
			 0x0fd07551, /// 0xa14
			 0xb8b22774, /// 0xa18
			 0x3a48c785, /// 0xa1c
			 0x99f1ffa3, /// 0xa20
			 0xc8cf7c5d, /// 0xa24
			 0x01bebf54, /// 0xa28
			 0x73f02b2b, /// 0xa2c
			 0xcbb9ac87, /// 0xa30
			 0xedfa69be, /// 0xa34
			 0x1fce6437, /// 0xa38
			 0xec56d1d6, /// 0xa3c
			 0x5088dc7c, /// 0xa40
			 0x8398cc8b, /// 0xa44
			 0xc1ae8f2f, /// 0xa48
			 0x403e22b8, /// 0xa4c
			 0x23f9ff14, /// 0xa50
			 0x583b39a7, /// 0xa54
			 0x90f500a9, /// 0xa58
			 0x74fe45fe, /// 0xa5c
			 0xd5be06dc, /// 0xa60
			 0x13bc3e6c, /// 0xa64
			 0xfdb56e2c, /// 0xa68
			 0xc529b0fe, /// 0xa6c
			 0x229b9b6e, /// 0xa70
			 0x8b76040a, /// 0xa74
			 0x045567c5, /// 0xa78
			 0xa24c9a52, /// 0xa7c
			 0x6d6eb29e, /// 0xa80
			 0x62455425, /// 0xa84
			 0x85f0147b, /// 0xa88
			 0x86d71c9c, /// 0xa8c
			 0x37958fb0, /// 0xa90
			 0x98315759, /// 0xa94
			 0x05ef0ca6, /// 0xa98
			 0x3ee9383c, /// 0xa9c
			 0x76248786, /// 0xaa0
			 0x7a27c936, /// 0xaa4
			 0xde52dc85, /// 0xaa8
			 0x7556af76, /// 0xaac
			 0x51ee0f63, /// 0xab0
			 0x021be9ff, /// 0xab4
			 0xf33397db, /// 0xab8
			 0xe3aa149c, /// 0xabc
			 0xb0617472, /// 0xac0
			 0x6b654b03, /// 0xac4
			 0xce485032, /// 0xac8
			 0x22f46c0b, /// 0xacc
			 0x38292430, /// 0xad0
			 0x39bb2937, /// 0xad4
			 0xc72913fe, /// 0xad8
			 0xbe37efd3, /// 0xadc
			 0xf87ddc14, /// 0xae0
			 0x4e9b3252, /// 0xae4
			 0x40728e37, /// 0xae8
			 0x279a1048, /// 0xaec
			 0xe1ccc095, /// 0xaf0
			 0x7f3c7be1, /// 0xaf4
			 0xc6527df5, /// 0xaf8
			 0xef1115f2, /// 0xafc
			 0xcc66bcf3, /// 0xb00
			 0xc11298b0, /// 0xb04
			 0xf531bb73, /// 0xb08
			 0xa04ee9ea, /// 0xb0c
			 0xc176092b, /// 0xb10
			 0xe99f4c47, /// 0xb14
			 0x3d75b727, /// 0xb18
			 0xdbe8cbdf, /// 0xb1c
			 0xcebb732d, /// 0xb20
			 0xfa8f262d, /// 0xb24
			 0xfa7f17a0, /// 0xb28
			 0x2c3f0a78, /// 0xb2c
			 0xb0b9478a, /// 0xb30
			 0x5df5784d, /// 0xb34
			 0x4f8502a2, /// 0xb38
			 0x0dbd6fa6, /// 0xb3c
			 0x24cf8ad3, /// 0xb40
			 0xd4ff0c9c, /// 0xb44
			 0x36c701fa, /// 0xb48
			 0x8bc44be3, /// 0xb4c
			 0xe77cdb3e, /// 0xb50
			 0x0ad4bcfb, /// 0xb54
			 0x00fa3767, /// 0xb58
			 0xd4f2b2c2, /// 0xb5c
			 0x1f02a7ae, /// 0xb60
			 0xc82ed207, /// 0xb64
			 0x4cc0d986, /// 0xb68
			 0xe0b99a35, /// 0xb6c
			 0x6bb812ef, /// 0xb70
			 0x6cd5868b, /// 0xb74
			 0x99b3da87, /// 0xb78
			 0x366d442e, /// 0xb7c
			 0xf59d7ad3, /// 0xb80
			 0x8c5e8515, /// 0xb84
			 0x94d57f78, /// 0xb88
			 0xa9f014e7, /// 0xb8c
			 0x8eda17ac, /// 0xb90
			 0x615a68d3, /// 0xb94
			 0x86d16cd3, /// 0xb98
			 0x56895f3a, /// 0xb9c
			 0x459d16f7, /// 0xba0
			 0x4ef55356, /// 0xba4
			 0xa101e13c, /// 0xba8
			 0x717e65c2, /// 0xbac
			 0xab90740c, /// 0xbb0
			 0xa5e4bb04, /// 0xbb4
			 0xd0e86607, /// 0xbb8
			 0x6403f16c, /// 0xbbc
			 0xd0173d5c, /// 0xbc0
			 0x650c035f, /// 0xbc4
			 0x46475b80, /// 0xbc8
			 0x08789e6e, /// 0xbcc
			 0x9bf0c688, /// 0xbd0
			 0xc2334483, /// 0xbd4
			 0x907010d5, /// 0xbd8
			 0xd3722f4b, /// 0xbdc
			 0xb158c8b1, /// 0xbe0
			 0xfb7ad49c, /// 0xbe4
			 0x90530c78, /// 0xbe8
			 0x9df41603, /// 0xbec
			 0x882c8d66, /// 0xbf0
			 0x01c245e2, /// 0xbf4
			 0xccdb3a67, /// 0xbf8
			 0xcd1f5291, /// 0xbfc
			 0xe037bfbc, /// 0xc00
			 0xca605ebe, /// 0xc04
			 0xc082b1e5, /// 0xc08
			 0x4fd8f34d, /// 0xc0c
			 0x95be2ba2, /// 0xc10
			 0xafa38ac7, /// 0xc14
			 0x093aaf89, /// 0xc18
			 0xdf577a03, /// 0xc1c
			 0x8aa52983, /// 0xc20
			 0x64ec7ba8, /// 0xc24
			 0xeab4d42e, /// 0xc28
			 0x5a96821e, /// 0xc2c
			 0x1f54eaac, /// 0xc30
			 0x79511822, /// 0xc34
			 0x7f1889a8, /// 0xc38
			 0x953111df, /// 0xc3c
			 0xc486d5de, /// 0xc40
			 0x55696407, /// 0xc44
			 0x012ea961, /// 0xc48
			 0xc55a112b, /// 0xc4c
			 0xef0b8c60, /// 0xc50
			 0xc29e30d6, /// 0xc54
			 0x2cb97b83, /// 0xc58
			 0x6badead2, /// 0xc5c
			 0x7d8e29ad, /// 0xc60
			 0xd524e83b, /// 0xc64
			 0x315cb5cf, /// 0xc68
			 0x739b5fc7, /// 0xc6c
			 0xe18296f8, /// 0xc70
			 0xead338ff, /// 0xc74
			 0x0c6b785c, /// 0xc78
			 0xb9d6603f, /// 0xc7c
			 0x72a733ff, /// 0xc80
			 0xbba4ed10, /// 0xc84
			 0xd26a0c87, /// 0xc88
			 0xf08d03fa, /// 0xc8c
			 0x6bf6ba16, /// 0xc90
			 0x6790946c, /// 0xc94
			 0x525987af, /// 0xc98
			 0xe33558fc, /// 0xc9c
			 0x217d5bc8, /// 0xca0
			 0xe5f37f46, /// 0xca4
			 0xf0a5d93c, /// 0xca8
			 0xb3625d47, /// 0xcac
			 0xea6a57f7, /// 0xcb0
			 0xd4b6949a, /// 0xcb4
			 0x4e33c567, /// 0xcb8
			 0x0f12def9, /// 0xcbc
			 0xc603c9ba, /// 0xcc0
			 0x2e4c9d38, /// 0xcc4
			 0x1bac2d1c, /// 0xcc8
			 0x9ad322db, /// 0xccc
			 0x8d8fe5cc, /// 0xcd0
			 0xb944d9f4, /// 0xcd4
			 0xaf205edc, /// 0xcd8
			 0xaa0957c7, /// 0xcdc
			 0x07c0f0ee, /// 0xce0
			 0x1cf1cac3, /// 0xce4
			 0xab9ff253, /// 0xce8
			 0x6e3ff39f, /// 0xcec
			 0xbf47e241, /// 0xcf0
			 0x2d10e0f9, /// 0xcf4
			 0x3a3e07a1, /// 0xcf8
			 0xf7971636, /// 0xcfc
			 0x56b6cdde, /// 0xd00
			 0xda0a5cfc, /// 0xd04
			 0xb3e0ead7, /// 0xd08
			 0xeac6100f, /// 0xd0c
			 0xd151a90c, /// 0xd10
			 0xcea80f4d, /// 0xd14
			 0xf0596c9c, /// 0xd18
			 0x6a9b8596, /// 0xd1c
			 0x17729e6b, /// 0xd20
			 0x43ab9dda, /// 0xd24
			 0xd3074593, /// 0xd28
			 0x83d241ff, /// 0xd2c
			 0x14fac2e9, /// 0xd30
			 0x9a249736, /// 0xd34
			 0x635e30cb, /// 0xd38
			 0xa0de297c, /// 0xd3c
			 0x93d99d3c, /// 0xd40
			 0x8b8c7f27, /// 0xd44
			 0xb8468de9, /// 0xd48
			 0x97fa41de, /// 0xd4c
			 0x8e585222, /// 0xd50
			 0x96624195, /// 0xd54
			 0x94bbabd8, /// 0xd58
			 0x5bbf0af8, /// 0xd5c
			 0xdba8beb0, /// 0xd60
			 0xef803ac0, /// 0xd64
			 0x911b0c8e, /// 0xd68
			 0xce8616e6, /// 0xd6c
			 0xf120cf7b, /// 0xd70
			 0xb8202977, /// 0xd74
			 0xeac95a30, /// 0xd78
			 0xa6b63755, /// 0xd7c
			 0xb5ee2e69, /// 0xd80
			 0xa6228aeb, /// 0xd84
			 0xb98d3903, /// 0xd88
			 0x31236a49, /// 0xd8c
			 0x66163eb1, /// 0xd90
			 0x3578aafa, /// 0xd94
			 0x6242f59d, /// 0xd98
			 0xabf12e48, /// 0xd9c
			 0xf8f7599d, /// 0xda0
			 0x7f5b0223, /// 0xda4
			 0x835a7569, /// 0xda8
			 0x691ccff7, /// 0xdac
			 0x11823063, /// 0xdb0
			 0x5a440ef1, /// 0xdb4
			 0x4377d9fb, /// 0xdb8
			 0x515abd02, /// 0xdbc
			 0xe4c58af1, /// 0xdc0
			 0xbfc73796, /// 0xdc4
			 0x85cff3ef, /// 0xdc8
			 0x73e79369, /// 0xdcc
			 0x62afc6e8, /// 0xdd0
			 0x8cd98fe2, /// 0xdd4
			 0x65a3125c, /// 0xdd8
			 0x3b130029, /// 0xddc
			 0xa1c0fc31, /// 0xde0
			 0x0c1005f4, /// 0xde4
			 0xa469140c, /// 0xde8
			 0x4ac77d06, /// 0xdec
			 0x12c350db, /// 0xdf0
			 0x83873836, /// 0xdf4
			 0x1962a063, /// 0xdf8
			 0x99e4f428, /// 0xdfc
			 0xc29300eb, /// 0xe00
			 0xb551b300, /// 0xe04
			 0xc3535258, /// 0xe08
			 0x6716c6ab, /// 0xe0c
			 0xacf2c45d, /// 0xe10
			 0x7e984af8, /// 0xe14
			 0xd4ec2342, /// 0xe18
			 0xea2519c0, /// 0xe1c
			 0xb2cf86f6, /// 0xe20
			 0x198f3c48, /// 0xe24
			 0xa4813787, /// 0xe28
			 0x52277866, /// 0xe2c
			 0x31453e00, /// 0xe30
			 0xbe3abce7, /// 0xe34
			 0x0088c35b, /// 0xe38
			 0x5b7b5b49, /// 0xe3c
			 0x424091b2, /// 0xe40
			 0xe8f540a4, /// 0xe44
			 0x33dacd8a, /// 0xe48
			 0x3595aa6e, /// 0xe4c
			 0xbb1c7a3f, /// 0xe50
			 0x0b919eff, /// 0xe54
			 0x2a6c6c11, /// 0xe58
			 0x1ec563a6, /// 0xe5c
			 0xd6b2ce2e, /// 0xe60
			 0x84d89410, /// 0xe64
			 0x7983318e, /// 0xe68
			 0xf220e0e7, /// 0xe6c
			 0xbff8b03a, /// 0xe70
			 0x1b48121c, /// 0xe74
			 0x73af9848, /// 0xe78
			 0x1ead014c, /// 0xe7c
			 0x1516d89d, /// 0xe80
			 0x2de4acf3, /// 0xe84
			 0x9a9ff47b, /// 0xe88
			 0x27761b32, /// 0xe8c
			 0xb1cb1375, /// 0xe90
			 0xe5d75bc4, /// 0xe94
			 0x56dd05b7, /// 0xe98
			 0x56617732, /// 0xe9c
			 0xc49c0e71, /// 0xea0
			 0xb10778ea, /// 0xea4
			 0x6c9fb5cf, /// 0xea8
			 0x23f44a2c, /// 0xeac
			 0xc7821c7b, /// 0xeb0
			 0xe0cf1549, /// 0xeb4
			 0xd3a66b5a, /// 0xeb8
			 0x32e0fae9, /// 0xebc
			 0x3c1a5dda, /// 0xec0
			 0x0368610f, /// 0xec4
			 0xc7a9d7c6, /// 0xec8
			 0xf061e2e1, /// 0xecc
			 0xcd72866b, /// 0xed0
			 0x00c21304, /// 0xed4
			 0xb28da7f8, /// 0xed8
			 0x27a2a0d5, /// 0xedc
			 0x054a6cd7, /// 0xee0
			 0xfc44c496, /// 0xee4
			 0x57e644e8, /// 0xee8
			 0xba016849, /// 0xeec
			 0x860e4b38, /// 0xef0
			 0x736cc425, /// 0xef4
			 0x4ea2df50, /// 0xef8
			 0x93f42ba2, /// 0xefc
			 0x95092755, /// 0xf00
			 0x424af138, /// 0xf04
			 0x56e78229, /// 0xf08
			 0x79c82deb, /// 0xf0c
			 0x30da2562, /// 0xf10
			 0x4d7eb644, /// 0xf14
			 0xbad48e55, /// 0xf18
			 0xf9d28cd1, /// 0xf1c
			 0x671f8b0d, /// 0xf20
			 0xf8a6ec7d, /// 0xf24
			 0xb6405720, /// 0xf28
			 0xa482a338, /// 0xf2c
			 0x59811201, /// 0xf30
			 0x9a48a860, /// 0xf34
			 0xae0cc571, /// 0xf38
			 0xddf0e833, /// 0xf3c
			 0xa4625ba1, /// 0xf40
			 0x8c12156e, /// 0xf44
			 0x01afe08a, /// 0xf48
			 0x7507ec03, /// 0xf4c
			 0x04db565d, /// 0xf50
			 0x748ec1d7, /// 0xf54
			 0xedb7e902, /// 0xf58
			 0x1660b6d9, /// 0xf5c
			 0x3c750511, /// 0xf60
			 0x463562bb, /// 0xf64
			 0x0f88527c, /// 0xf68
			 0xeaeafefd, /// 0xf6c
			 0xc92b4fbd, /// 0xf70
			 0xcd3295db, /// 0xf74
			 0x44ab55c6, /// 0xf78
			 0x337c7e08, /// 0xf7c
			 0xe8a4b2f9, /// 0xf80
			 0x0ec7dcbe, /// 0xf84
			 0x8928cebc, /// 0xf88
			 0xe930fde1, /// 0xf8c
			 0x6ddd6a05, /// 0xf90
			 0xede599b5, /// 0xf94
			 0x20aa7876, /// 0xf98
			 0x80538bf1, /// 0xf9c
			 0xc5c31ff0, /// 0xfa0
			 0x819742d0, /// 0xfa4
			 0x032c91a0, /// 0xfa8
			 0x1ca19a2d, /// 0xfac
			 0xd0ae85bf, /// 0xfb0
			 0x12487194, /// 0xfb4
			 0xbb849de6, /// 0xfb8
			 0x357ad294, /// 0xfbc
			 0x8fba722f, /// 0xfc0
			 0x158f92d9, /// 0xfc4
			 0x848fa3a3, /// 0xfc8
			 0x4d56ca26, /// 0xfcc
			 0xccd9afa2, /// 0xfd0
			 0x653fa2c4, /// 0xfd4
			 0x34408d91, /// 0xfd8
			 0x7ef612bf, /// 0xfdc
			 0xa94db112, /// 0xfe0
			 0xef594fcf, /// 0xfe4
			 0x4fcc3e25, /// 0xfe8
			 0xfad49a15, /// 0xfec
			 0xe2bfef24, /// 0xff0
			 0x185466ca, /// 0xff4
			 0x88ee95a9, /// 0xff8
			 0x20817219 /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00000
			 0x0e000003,                                                  // Trailing 1s:                                /// 00004
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00008
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0000c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00010
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00014
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0001c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00020
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00024
			 0x55555555,                                                  // 4 random values                             /// 00028
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0002c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00030
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00038
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0003c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00044
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0004c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00050
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00054
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00058
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0005c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00060
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00064
			 0x55555555,                                                  // 4 random values                             /// 00068
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0006c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00070
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00078
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00080
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00084
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00088
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00090
			 0x80011111,                                                  // -9.7958E-41                                 /// 00094
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0009c
			 0x0c780000,                                                  // Leading 1s:                                 /// 000a0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000a8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000ac
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000b0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 000b4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000b8
			 0x0c400000,                                                  // Leading 1s:                                 /// 000bc
			 0x0e000001,                                                  // Trailing 1s:                                /// 000c0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000c4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000c8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000cc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000d0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 000d4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000dc
			 0x0c600000,                                                  // Leading 1s:                                 /// 000e0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000ec
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 000f0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 000f4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000f8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000fc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00100
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00104
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00108
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0010c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00110
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00114
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00118
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0011c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00120
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00124
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00128
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0012c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00130
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0x00000000,                                                  // zero                                        /// 0013c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00140
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00144
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00148
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0014c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0015c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00160
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00164
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00168
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00170
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00174
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00178
			 0x80000000,                                                  // -zero                                       /// 0017c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00180
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00184
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00188
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0018c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00190
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00194
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00198
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0019c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 001a0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001a4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 001a8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 001ac
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b0
			 0x80000000,                                                  // -zero                                       /// 001b4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001b8
			 0x00011111,                                                  // 9.7958E-41                                  /// 001bc
			 0x7fc00001,                                                  // signaling NaN                               /// 001c0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 001c4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001c8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001cc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001d0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001d4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001d8
			 0xffc00001,                                                  // -signaling NaN                              /// 001dc
			 0x4b000000,                                                  // 8388608.0                                   /// 001e0
			 0x0c400000,                                                  // Leading 1s:                                 /// 001e4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001e8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001ec
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001f4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 001f8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001fc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00200
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00204
			 0x00011111,                                                  // 9.7958E-41                                  /// 00208
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0020c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00210
			 0x80000000,                                                  // -zero                                       /// 00214
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0021c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00220
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00224
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00228
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00230
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00234
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00238
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0023c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00240
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00244
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00248
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0024c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00250
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00258
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0x7f800000,                                                  // inf                                         /// 00260
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00264
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00268
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0026c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00270
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00274
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0027c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00280
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00284
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00288
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0028c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x4b000000,                                                  // 8388608.0                                   /// 00294
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00298
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0029c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002a0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002a4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 002a8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ac
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002b0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 002b4
			 0x80000000,                                                  // -zero                                       /// 002b8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002bc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 002c0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 002c4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002c8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 002cc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002d0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002d4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 002d8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 002dc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002e0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002e8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002ec
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0x0e000001,                                                  // Trailing 1s:                                /// 002f4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 002f8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 002fc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00300
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00304
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00308
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0030c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00310
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00314
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00318
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0031c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00320
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x00011111,                                                  // 9.7958E-41                                  /// 00328
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0032c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00330
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00334
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00338
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0033c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00340
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00344
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00348
			 0x7fc00001,                                                  // signaling NaN                               /// 0034c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00350
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00354
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00358
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0035c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00360
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00364
			 0x0e000003,                                                  // Trailing 1s:                                /// 00368
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0036c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00370
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00374
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00378
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0037c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00384
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00388
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0038c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00390
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00394
			 0x80011111,                                                  // -9.7958E-41                                 /// 00398
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0039c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003a0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003a8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 003ac
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003b0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003b4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 003b8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003bc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003c0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003cc
			 0xcb000000,                                                  // -8388608.0                                  /// 003d0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003d4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 003d8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 003e8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003ec
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003f4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003f8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0x7f800000,                                                  // inf                                         /// 00400
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00404
			 0x0c700000,                                                  // Leading 1s:                                 /// 00408
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0040c
			 0x55555555,                                                  // 4 random values                             /// 00410
			 0xbf028f5c,                                                  // -0.51                                       /// 00414
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00418
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0041c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00420
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00428
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0x33333333,                                                  // 4 random values                             /// 00430
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00434
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00438
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00440
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00444
			 0xbf028f5c,                                                  // -0.51                                       /// 00448
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0044c
			 0xffc00001,                                                  // -signaling NaN                              /// 00450
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00454
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0045c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00460
			 0x0c400000,                                                  // Leading 1s:                                 /// 00464
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00468
			 0x00011111,                                                  // 9.7958E-41                                  /// 0046c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00470
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00474
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00478
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0047c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00480
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00484
			 0x0c780000,                                                  // Leading 1s:                                 /// 00488
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0048c
			 0xbf028f5c,                                                  // -0.51                                       /// 00490
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00494
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00498
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 004a0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a4
			 0x0e000001,                                                  // Trailing 1s:                                /// 004a8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004ac
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b0
			 0xffc00001,                                                  // -signaling NaN                              /// 004b4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004b8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 004bc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 004c0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004c4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004cc
			 0x0c700000,                                                  // Leading 1s:                                 /// 004d0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004d8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x4b000000,                                                  // 8388608.0                                   /// 004e0
			 0xcb000000,                                                  // -8388608.0                                  /// 004e4
			 0xff800000,                                                  // -inf                                        /// 004e8
			 0x7f800000,                                                  // inf                                         /// 004ec
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 004f4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 004f8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004fc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00500
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00504
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00508
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0050c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00510
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00514
			 0x80011111,                                                  // -9.7958E-41                                 /// 00518
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0051c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00520
			 0x80011111,                                                  // -9.7958E-41                                 /// 00524
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0052c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00530
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00538
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00540
			 0x0c780000,                                                  // Leading 1s:                                 /// 00544
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00548
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0054c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00550
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00558
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0xff800000,                                                  // -inf                                        /// 00560
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00564
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00568
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0056c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00570
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00574
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00578
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0057c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00584
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00588
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0058c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00590
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00594
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00598
			 0x3f028f5c,                                                  // 0.51                                        /// 0059c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 005a0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 005a4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005a8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005ac
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b0
			 0x0e000007,                                                  // Trailing 1s:                                /// 005b4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005b8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005bc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 005c0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 005c4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 005c8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 005cc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005d4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005d8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 005e0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005e8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005f0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 005f4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 005f8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005fc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00600
			 0x0c700000,                                                  // Leading 1s:                                 /// 00604
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0060c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00610
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00614
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00618
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0061c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00620
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00624
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00628
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0062c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00630
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00634
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00638
			 0x0c700000,                                                  // Leading 1s:                                 /// 0063c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00640
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00644
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00648
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0064c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00650
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00654
			 0xbf028f5c,                                                  // -0.51                                       /// 00658
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0065c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00660
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00664
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00668
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00670
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00674
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00678
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0067c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00680
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00688
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0068c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00690
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00694
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00698
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0069c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006a0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 006a4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006a8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006ac
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 006b0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006b4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006b8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006bc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 006c0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006c4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006c8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006cc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006d0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006d4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006dc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 006e0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006e4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006e8
			 0x00000000,                                                  // zero                                        /// 006ec
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006f0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006f4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006f8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006fc
			 0x7f800000,                                                  // inf                                         /// 00700
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00708
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0070c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00710
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00714
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00718
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0071c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00720
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00724
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00728
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0072c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00738
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0073c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00740
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00744
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00748
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00750
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00754
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00758
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0075c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00764
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00768
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0076c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00770
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00778
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0077c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00780
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00784
			 0x0c600000,                                                  // Leading 1s:                                 /// 00788
			 0x0e000007,                                                  // Trailing 1s:                                /// 0078c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00794
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00798
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0079c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007a4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 007ac
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007b0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007b4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 007b8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007bc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007c0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007c4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007c8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007cc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007d0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 007d8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 007dc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007e0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007e8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007f8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00800
			 0x80011111,                                                  // -9.7958E-41                                 /// 00804
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00808
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0080c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00810
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00818
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0081c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00820
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00824
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00828
			 0x0c780000,                                                  // Leading 1s:                                 /// 0082c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00830
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00834
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00838
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0083c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00840
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00844
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00848
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0084c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00850
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00854
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00858
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0085c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00860
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00864
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00868
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0086c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00874
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00878
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00880
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00884
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00888
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0088c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00894
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0089c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 008a0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008a4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008a8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008ac
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 008b0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008b8
			 0x0e000007,                                                  // Trailing 1s:                                /// 008bc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008c0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008cc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008d0
			 0x0e000007,                                                  // Trailing 1s:                                /// 008d4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008e4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008e8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 008f0
			 0xff800000,                                                  // -inf                                        /// 008f4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008fc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00900
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00904
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00908
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00910
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0xbf028f5c,                                                  // -0.51                                       /// 00918
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0091c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00920
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00924
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00928
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0092c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00930
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00934
			 0x3f028f5c,                                                  // 0.51                                        /// 00938
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0093c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00940
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00944
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00948
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0094c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00950
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00954
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00958
			 0x00011111,                                                  // 9.7958E-41                                  /// 0095c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00960
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00964
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00968
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0096c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00970
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00974
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00978
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00980
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00988
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0098c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00990
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00998
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0099c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009a0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009a4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009a8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009ac
			 0x0c600000,                                                  // Leading 1s:                                 /// 009b0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009b8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009bc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009c0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009cc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 009d0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009dc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009e0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009e4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009ec
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009f0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0xffc00001,                                                  // -signaling NaN                              /// 009f8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 009fc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a00
			 0xff800000,                                                  // -inf                                        /// 00a04
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a08
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a0c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a10
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a18
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a1c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a20
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a24
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0xcb000000,                                                  // -8388608.0                                  /// 00a34
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a3c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a44
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a48
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a4c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a50
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a54
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a58
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a5c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a64
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a68
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a6c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a70
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a74
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a78
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a7c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a80
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a8c
			 0x7f800000,                                                  // inf                                         /// 00a90
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a94
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a98
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a9c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00aa0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00aa8
			 0x3f028f5c,                                                  // 0.51                                        /// 00aac
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ab0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ab8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00abc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ac4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ac8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ad0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ad8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00adc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ae0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ae8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00aec
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00af4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b04
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b08
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b0c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b10
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b14
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b18
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b1c
			 0xbf028f5c,                                                  // -0.51                                       /// 00b20
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0xbf028f5c,                                                  // -0.51                                       /// 00b28
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b2c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b30
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b34
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b38
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b3c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b40
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b48
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b4c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b50
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b54
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b58
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b5c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b60
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b64
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b68
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b6c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b70
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b74
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b78
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b84
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b88
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b8c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b90
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b94
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b98
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b9c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ba4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ba8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00bac
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00bb0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00bb8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00bbc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00bc0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00bc4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00bc8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00bcc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00bd4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00bdc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00be0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00be8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bec
			 0x4b000000,                                                  // 8388608.0                                   /// 00bf0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00bf4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00bfc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c00
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c04
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c08
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c10
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c14
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c18
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c1c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c20
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c24
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c28
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c2c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c30
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c34
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c38
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c40
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c44
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c48
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c4c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c54
			 0x3f028f5c,                                                  // 0.51                                        /// 00c58
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c5c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c60
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c64
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c68
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c6c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c70
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c74
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c78
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c7c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c80
			 0xbf028f5c,                                                  // -0.51                                       /// 00c84
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c88
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c8c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c90
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c94
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0xcb000000,                                                  // -8388608.0                                  /// 00c9c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ca0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ca4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ca8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00cac
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00cb0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00cb4
			 0x55555555,                                                  // 4 random values                             /// 00cb8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00cbc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00cc0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00cc4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ccc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00cd4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00cdc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ce4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ce8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00cec
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00cf0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00cf4
			 0x00000000,                                                  // zero                                        /// 00cf8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00cfc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d00
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d04
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d08
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d0c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d10
			 0xff800000,                                                  // -inf                                        /// 00d14
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d18
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d1c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d20
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d2c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d30
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d34
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d38
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d3c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d40
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d48
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d4c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d50
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d54
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d58
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d5c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d60
			 0x55555555,                                                  // 4 random values                             /// 00d64
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d68
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d6c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d70
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d74
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x00000000,                                                  // zero                                        /// 00d7c
			 0x4b000000,                                                  // 8388608.0                                   /// 00d80
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d84
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d88
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d8c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d90
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d94
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d98
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d9c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00da0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00da8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00dac
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00db0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00db4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00db8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00dc0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00dc4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00dc8
			 0x00000000,                                                  // zero                                        /// 00dcc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00dd4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ddc
			 0x55555555,                                                  // 4 random values                             /// 00de0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00de4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00de8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00dec
			 0x4b000000,                                                  // 8388608.0                                   /// 00df0
			 0x7fc00001,                                                  // signaling NaN                               /// 00df4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00dfc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e00
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e04
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e08
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e0c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e10
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e14
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e18
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e1c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e20
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e24
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e28
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e2c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e34
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e38
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e3c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e40
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e44
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e48
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e4c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e50
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e58
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e5c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e64
			 0x00000000,                                                  // zero                                        /// 00e68
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e6c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e70
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e7c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e80
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e84
			 0xff800000,                                                  // -inf                                        /// 00e88
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e8c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e90
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e98
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e9c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea0
			 0x7f800000,                                                  // inf                                         /// 00ea4
			 0x80000000,                                                  // -zero                                       /// 00ea8
			 0xbf028f5c,                                                  // -0.51                                       /// 00eac
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00eb0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00eb4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ec0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ec4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ec8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ecc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ed0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ed4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ed8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00edc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ee0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ef0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ef4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ef8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00efc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f00
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f04
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f08
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f0c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f10
			 0x4b000000,                                                  // 8388608.0                                   /// 00f14
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f18
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f1c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f20
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f24
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f2c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f30
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f34
			 0xff800000,                                                  // -inf                                        /// 00f38
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f3c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f40
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f44
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f48
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f4c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0x7fc00001,                                                  // signaling NaN                               /// 00f54
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f58
			 0x55555555,                                                  // 4 random values                             /// 00f5c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f60
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f64
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f68
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f6c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f70
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f74
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f7c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f80
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f84
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f88
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f90
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f98
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fa0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fa4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00fac
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fb4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00fb8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00fbc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00fc0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00fc8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00fcc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00fd0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00fd4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00fd8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00fdc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fe0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fe4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00fe8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fec
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ff4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ff8
			 0x00000020                                                  // SP - 1 bit alone set in mantissa +ve        /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00004
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0000c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00010
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00014
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00018
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0001c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00020
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00028
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0002c
			 0xcb000000,                                                  // -8388608.0                                  /// 00030
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00034
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00038
			 0xbf028f5c,                                                  // -0.51                                       /// 0003c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00040
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00044
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00048
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0004c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00050
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00054
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00058
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0005c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00060
			 0x0e000003,                                                  // Trailing 1s:                                /// 00064
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00068
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0006c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00070
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00074
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00078
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0007c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00080
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00084
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x4b000000,                                                  // 8388608.0                                   /// 0008c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00090
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00094
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00098
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0009c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 000a4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000a8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000b0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000b4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 000b8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000bc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000c0
			 0x7f800000,                                                  // inf                                         /// 000c4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 000c8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000cc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0xff800000,                                                  // -inf                                        /// 000d4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000d8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 000e4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000e8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000ec
			 0x80011111,                                                  // -9.7958E-41                                 /// 000f0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000f8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00100
			 0x33333333,                                                  // 4 random values                             /// 00104
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00108
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00110
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00114
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00118
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0011c
			 0xbf028f5c,                                                  // -0.51                                       /// 00120
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00124
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0012c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00130
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00138
			 0x4b000000,                                                  // 8388608.0                                   /// 0013c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00140
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00144
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00148
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0014c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00150
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00154
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00158
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0015c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00160
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00164
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00168
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0016c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00174
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00178
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0017c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00180
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00184
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00188
			 0xffc00001,                                                  // -signaling NaN                              /// 0018c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00190
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00198
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0019c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001a0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 001a4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001a8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 001b0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 001b4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001b8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001bc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 001c4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 001c8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 001d4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 001d8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001dc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001e0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001e8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 001ec
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001f0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001f4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001f8
			 0xff800000,                                                  // -inf                                        /// 001fc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00200
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00204
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00208
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0020c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00210
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00214
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00218
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0021c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00220
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00224
			 0xcb000000,                                                  // -8388608.0                                  /// 00228
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0022c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00230
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00234
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00238
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00240
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00244
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00248
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0024c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00250
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00254
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00258
			 0xffc00001,                                                  // -signaling NaN                              /// 0025c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00260
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00264
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0026c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00270
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00274
			 0xffc00001,                                                  // -signaling NaN                              /// 00278
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0027c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00280
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00284
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00288
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0028c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00290
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00294
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00298
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0029c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002a0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 002a4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 002a8
			 0xbf028f5c,                                                  // -0.51                                       /// 002ac
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002b0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002b4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002bc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002c0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0x80011111,                                                  // -9.7958E-41                                 /// 002c8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 002cc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002d4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 002d8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002dc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002e0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 002e4
			 0x80000000,                                                  // -zero                                       /// 002e8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 002f0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002f4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002f8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 002fc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00300
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00304
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00308
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0030c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x0c400000,                                                  // Leading 1s:                                 /// 00314
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00318
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0031c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00320
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00324
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00328
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0032c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00330
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00334
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00338
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00344
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00348
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0034c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00350
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00354
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00358
			 0x7f800000,                                                  // inf                                         /// 0035c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00360
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00364
			 0x7fc00001,                                                  // signaling NaN                               /// 00368
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0036c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00370
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00374
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00378
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0037c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00380
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00384
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00388
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0038c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00390
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00398
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0039c
			 0xff800000,                                                  // -inf                                        /// 003a0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a4
			 0x7fc00001,                                                  // signaling NaN                               /// 003a8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003b0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003b8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003bc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 003c4
			 0xff800000,                                                  // -inf                                        /// 003c8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003cc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003d0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 003d4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003d8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003dc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003e0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003e4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 003e8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ec
			 0x0d000ff0,                                                  // Set of 1s                                   /// 003f0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003f8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003fc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00400
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00404
			 0x0c400000,                                                  // Leading 1s:                                 /// 00408
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0040c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00410
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00414
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00418
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0041c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00420
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00424
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00428
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0042c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00430
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00434
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00438
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0043c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00440
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00444
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00448
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0044c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00454
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00458
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0045c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00460
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00464
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0046c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00470
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00474
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00478
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0047c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x0e000007,                                                  // Trailing 1s:                                /// 00484
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00488
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0048c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00490
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00494
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00498
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0049c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004a0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004a4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 004a8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004ac
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004b0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 004b4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004b8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 004c0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004c4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 004cc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004d0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004d4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004d8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 004dc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e8
			 0x7fc00001,                                                  // signaling NaN                               /// 004ec
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004f0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004fc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00500
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00504
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00508
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00510
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00514
			 0xcb000000,                                                  // -8388608.0                                  /// 00518
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00520
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00524
			 0xffc00001,                                                  // -signaling NaN                              /// 00528
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00530
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00534
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00538
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0053c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00540
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00544
			 0xffc00001,                                                  // -signaling NaN                              /// 00548
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0054c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00550
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00554
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00558
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0055c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0x0c400000,                                                  // Leading 1s:                                 /// 00564
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00568
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0056c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00570
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00574
			 0xcb000000,                                                  // -8388608.0                                  /// 00578
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0057c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00580
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00584
			 0x80011111,                                                  // -9.7958E-41                                 /// 00588
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0058c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00590
			 0x80000000,                                                  // -zero                                       /// 00594
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00598
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0059c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 005a0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005a4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005a8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 005ac
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005b0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005c0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 005c4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 005c8
			 0x0c780000,                                                  // Leading 1s:                                 /// 005cc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005d4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005d8
			 0x80000000,                                                  // -zero                                       /// 005dc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005e0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005ec
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 005f0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 005fc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00600
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00604
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00608
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0060c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00610
			 0x80000000,                                                  // -zero                                       /// 00614
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00618
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0061c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00620
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00624
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00628
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00630
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00634
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00638
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0063c
			 0x3f028f5c,                                                  // 0.51                                        /// 00640
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00644
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00648
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0064c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00650
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00654
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00658
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0065c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00660
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00668
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0066c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00670
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00674
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00678
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0067c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00680
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00684
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0068c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00690
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00694
			 0x0c600000,                                                  // Leading 1s:                                 /// 00698
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0069c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 006a0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006a4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006a8
			 0x80011111,                                                  // -9.7958E-41                                 /// 006ac
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006b4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 006bc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 006c0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006c4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006c8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 006cc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d0
			 0xcb000000,                                                  // -8388608.0                                  /// 006d4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 006d8
			 0x0c780000,                                                  // Leading 1s:                                 /// 006dc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006e0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006e4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 006ec
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 006f0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 006f4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 006f8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006fc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00700
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00704
			 0x0e000003,                                                  // Trailing 1s:                                /// 00708
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0070c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00714
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00718
			 0x80011111,                                                  // -9.7958E-41                                 /// 0071c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00720
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00724
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00728
			 0xcb000000,                                                  // -8388608.0                                  /// 0072c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00730
			 0x00000000,                                                  // zero                                        /// 00734
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0073c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00740
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00744
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00748
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0074c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00750
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00754
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00758
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0075c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00760
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00764
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00768
			 0x33333333,                                                  // 4 random values                             /// 0076c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00770
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00774
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0077c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00784
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00788
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00794
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00798
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007a4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 007a8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 007ac
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 007b0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 007b4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 007b8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x0c700000,                                                  // Leading 1s:                                 /// 007c0
			 0x0c400000,                                                  // Leading 1s:                                 /// 007c4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 007c8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007cc
			 0x80000000,                                                  // -zero                                       /// 007d0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 007d4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 007d8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007e0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007e4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 007e8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007ec
			 0x0e001fff,                                                  // Trailing 1s:                                /// 007f0
			 0x0e000003,                                                  // Trailing 1s:                                /// 007f4
			 0x3f028f5c,                                                  // 0.51                                        /// 007f8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 007fc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00800
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00804
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00808
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0080c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00810
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00814
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00818
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0081c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00820
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00824
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00828
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0082c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00830
			 0x00000000,                                                  // zero                                        /// 00834
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00838
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0083c
			 0xff800000,                                                  // -inf                                        /// 00840
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00844
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00848
			 0xcb000000,                                                  // -8388608.0                                  /// 0084c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00850
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00854
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0085c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00860
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00864
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00868
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0086c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00874
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00878
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0087c
			 0x55555555,                                                  // 4 random values                             /// 00880
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00884
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00888
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00894
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00898
			 0x0c400000,                                                  // Leading 1s:                                 /// 0089c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 008a0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008a4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008a8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ac
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 008b0
			 0x4b000000,                                                  // 8388608.0                                   /// 008b4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008b8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 008bc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 008c0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008c4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008c8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008cc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 008d0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 008d4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008d8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008dc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008e0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 008e4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 008f0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 008f4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 008fc
			 0x3f028f5c,                                                  // 0.51                                        /// 00900
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00904
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00908
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0090c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00910
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00914
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00918
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0091c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00920
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00924
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00928
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0092c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00934
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00938
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0093c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00940
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00944
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00948
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0094c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00950
			 0xbf028f5c,                                                  // -0.51                                       /// 00954
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00958
			 0x00011111,                                                  // 9.7958E-41                                  /// 0095c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00960
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00968
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0096c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00970
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00974
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00978
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0097c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00980
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00984
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00988
			 0x00011111,                                                  // 9.7958E-41                                  /// 0098c
			 0xcb000000,                                                  // -8388608.0                                  /// 00990
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00994
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00998
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0099c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 009a0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 009a8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 009b0
			 0xff800000,                                                  // -inf                                        /// 009b4
			 0x80000000,                                                  // -zero                                       /// 009b8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009bc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009c0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009c4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 009c8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009cc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009d8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009dc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009e0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009e4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 009ec
			 0x3f028f5c,                                                  // 0.51                                        /// 009f0
			 0xcb000000,                                                  // -8388608.0                                  /// 009f4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009f8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 009fc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a00
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a04
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a08
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a0c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a10
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a14
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a18
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a1c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a20
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a28
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a2c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a30
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a34
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a3c
			 0xffc00001,                                                  // -signaling NaN                              /// 00a40
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a44
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a48
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a50
			 0xff800000,                                                  // -inf                                        /// 00a54
			 0xff800000,                                                  // -inf                                        /// 00a58
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a5c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a60
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a68
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a6c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a70
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a74
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a78
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a7c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a84
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a88
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a8c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a90
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a94
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a98
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a9c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00aa4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00aa8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00aac
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ab0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ab4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ab8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ac4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ac8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ad0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ad4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ad8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00adc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ae0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ae4
			 0xcb000000,                                                  // -8388608.0                                  /// 00ae8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00af0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00af8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00afc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b04
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b08
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b0c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b10
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b14
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b18
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b20
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b28
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b2c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b30
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b34
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b38
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b3c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b40
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b44
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b48
			 0x80000000,                                                  // -zero                                       /// 00b4c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b50
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b54
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b58
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b5c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b60
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b64
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b68
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b6c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b74
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b7c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b84
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b88
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b8c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b90
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b94
			 0xbf028f5c,                                                  // -0.51                                       /// 00b98
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b9c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ba0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ba4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ba8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00bac
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00bb4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00bbc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00bc0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00bc4
			 0xff800000,                                                  // -inf                                        /// 00bc8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00bcc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bd4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00bd8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00bdc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00be0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00be4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00bec
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00bf0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00bf8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00bfc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c00
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c04
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c08
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c0c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c10
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c1c
			 0xcb000000,                                                  // -8388608.0                                  /// 00c20
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c24
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c28
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c2c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c30
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c34
			 0xff800000,                                                  // -inf                                        /// 00c38
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c3c
			 0x80000000,                                                  // -zero                                       /// 00c40
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c48
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c4c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c54
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c58
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c5c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c60
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c68
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c6c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c70
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c74
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c78
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c7c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c80
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c84
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c8c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c90
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c94
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c98
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c9c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ca4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cac
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00cb0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cb4
			 0x00000000,                                                  // zero                                        /// 00cb8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00cbc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00cc0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00cc8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ccc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00cd0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00cd4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00cd8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00cdc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ce4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ce8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00cec
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00cf0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00cf4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00cf8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d00
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d04
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d08
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d0c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d10
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d14
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d18
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d20
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d24
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d28
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d2c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d30
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d34
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d38
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d40
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d44
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d48
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d4c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d50
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d54
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d58
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d5c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d60
			 0x4b000000,                                                  // 8388608.0                                   /// 00d64
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d6c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d70
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d74
			 0x00000000,                                                  // zero                                        /// 00d78
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d7c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d80
			 0xffc00001,                                                  // -signaling NaN                              /// 00d84
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d88
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d8c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d94
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d98
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d9c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00da0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00da4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00da8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00dac
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00db0
			 0x55555555,                                                  // 4 random values                             /// 00db4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00db8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00dc4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00dc8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00dcc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00dd0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00dd4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ddc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00de0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00de4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00de8
			 0x4b000000,                                                  // 8388608.0                                   /// 00dec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00df0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00df4
			 0x80000000,                                                  // -zero                                       /// 00df8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00dfc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e00
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e04
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e08
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e0c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e10
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e14
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e18
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e1c
			 0xbf028f5c,                                                  // -0.51                                       /// 00e20
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e28
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e30
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e34
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e38
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e3c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0x4b000000,                                                  // 8388608.0                                   /// 00e44
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e48
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e4c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e50
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e54
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e58
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e5c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e60
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e64
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e6c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e70
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e74
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e78
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e7c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e80
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e84
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e88
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e8c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e90
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e94
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e98
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e9c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ea0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ea4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00eac
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ebc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ec8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ecc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ed0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed4
			 0xcb000000,                                                  // -8388608.0                                  /// 00ed8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00edc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ee0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ee8
			 0x00000000,                                                  // zero                                        /// 00eec
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ef0
			 0x7f800000,                                                  // inf                                         /// 00ef4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ef8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00efc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f00
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f08
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f0c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f14
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f18
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f1c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f20
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f24
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f2c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f30
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f34
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f38
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f40
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f44
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f4c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f50
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f54
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f58
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f5c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f64
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f68
			 0xff800000,                                                  // -inf                                        /// 00f6c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f70
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f74
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f7c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f80
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f88
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f8c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f90
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f94
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f98
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f9c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fa4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fac
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00fb0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fb4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00fb8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00fbc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00fc0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fc4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00fc8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00fcc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fd4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00fd8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fe0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00fe4
			 0x55555555,                                                  // 4 random values                             /// 00fe8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00fec
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ff0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ff4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff8
			 0x00000004                                                  // SP - 1 bit alone set in mantissa +ve        /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x7cae0efa, /// 0x0
			 0xd4d2dbf8, /// 0x4
			 0xe662e7fe, /// 0x8
			 0xb23b18b9, /// 0xc
			 0x7fe30b17, /// 0x10
			 0x99c69470, /// 0x14
			 0x9f680785, /// 0x18
			 0x8eabdba3, /// 0x1c
			 0x483e3d02, /// 0x20
			 0x7ef25f23, /// 0x24
			 0xdf073d6b, /// 0x28
			 0x9e3ea377, /// 0x2c
			 0x3ca22683, /// 0x30
			 0x0a6868e2, /// 0x34
			 0x539d825e, /// 0x38
			 0xb2d39ee0, /// 0x3c
			 0x65235840, /// 0x40
			 0x4768d3f9, /// 0x44
			 0x4d1c59c0, /// 0x48
			 0xdea5347f, /// 0x4c
			 0x4a2eb91a, /// 0x50
			 0x22e1f360, /// 0x54
			 0x9eb5dcda, /// 0x58
			 0xe1704771, /// 0x5c
			 0xa1f0cf0b, /// 0x60
			 0xb0b506b5, /// 0x64
			 0xbcc5d4de, /// 0x68
			 0xc349087d, /// 0x6c
			 0x90a8b86e, /// 0x70
			 0x2d64509e, /// 0x74
			 0xd2e3659c, /// 0x78
			 0x5f82bc25, /// 0x7c
			 0x5f1fce9c, /// 0x80
			 0xa3759926, /// 0x84
			 0xd9f34c8a, /// 0x88
			 0xc3cd2679, /// 0x8c
			 0x6094183a, /// 0x90
			 0xaf4e82db, /// 0x94
			 0x8f556adc, /// 0x98
			 0xeee48feb, /// 0x9c
			 0x2d444c0d, /// 0xa0
			 0xea210089, /// 0xa4
			 0xce6a31d4, /// 0xa8
			 0x95e8805b, /// 0xac
			 0xb376ab25, /// 0xb0
			 0xcd2b4561, /// 0xb4
			 0x135ea3f7, /// 0xb8
			 0xaf1b67bf, /// 0xbc
			 0x22078aef, /// 0xc0
			 0xca29ce87, /// 0xc4
			 0xbc1e12f1, /// 0xc8
			 0x4f6372f7, /// 0xcc
			 0x6d9bf3fe, /// 0xd0
			 0x1209df1c, /// 0xd4
			 0x616b8123, /// 0xd8
			 0xd71bbe5b, /// 0xdc
			 0x066918b3, /// 0xe0
			 0x2a1417b9, /// 0xe4
			 0xc9e65001, /// 0xe8
			 0x96cb699f, /// 0xec
			 0xbcafb24f, /// 0xf0
			 0xa1ce2ef3, /// 0xf4
			 0x29e88284, /// 0xf8
			 0xf8239a69, /// 0xfc
			 0x7e0b70e4, /// 0x100
			 0x15c3362b, /// 0x104
			 0x696c514f, /// 0x108
			 0x4822fbaf, /// 0x10c
			 0x9b8d2065, /// 0x110
			 0x0cd2ceec, /// 0x114
			 0x77e9576c, /// 0x118
			 0x70d8d60d, /// 0x11c
			 0x5ff987c6, /// 0x120
			 0x039219f1, /// 0x124
			 0xca5ae787, /// 0x128
			 0xf7c8da30, /// 0x12c
			 0x6839dfc7, /// 0x130
			 0x5092a04a, /// 0x134
			 0xccef8bfd, /// 0x138
			 0x52d5420d, /// 0x13c
			 0x1dbfee5f, /// 0x140
			 0x737298da, /// 0x144
			 0x7fa857ab, /// 0x148
			 0x720c8bbe, /// 0x14c
			 0x671c64f9, /// 0x150
			 0x60fec759, /// 0x154
			 0xa63e9770, /// 0x158
			 0xde09e140, /// 0x15c
			 0x1d17260f, /// 0x160
			 0x0e9a6f95, /// 0x164
			 0xd5806081, /// 0x168
			 0x5b22a2ab, /// 0x16c
			 0xc8e3bab8, /// 0x170
			 0x6f506fc1, /// 0x174
			 0x882e566c, /// 0x178
			 0x56ea2327, /// 0x17c
			 0xaa725e94, /// 0x180
			 0x784b553e, /// 0x184
			 0xe6a2158c, /// 0x188
			 0x2a70276c, /// 0x18c
			 0x515a0521, /// 0x190
			 0x69ea2780, /// 0x194
			 0x9b49996d, /// 0x198
			 0xe01c1c1b, /// 0x19c
			 0x82b61f42, /// 0x1a0
			 0xcc74fbcf, /// 0x1a4
			 0x12245242, /// 0x1a8
			 0x48ad6184, /// 0x1ac
			 0xd9eee770, /// 0x1b0
			 0xf07d6c32, /// 0x1b4
			 0x14a2cce9, /// 0x1b8
			 0x8efbcf32, /// 0x1bc
			 0xa85a98a6, /// 0x1c0
			 0xc777740e, /// 0x1c4
			 0xc5e15f6d, /// 0x1c8
			 0xf9710d88, /// 0x1cc
			 0x5ef72069, /// 0x1d0
			 0x9586f520, /// 0x1d4
			 0x124218fd, /// 0x1d8
			 0xb730356d, /// 0x1dc
			 0x59f54a9d, /// 0x1e0
			 0xe9ab5206, /// 0x1e4
			 0x4be5a705, /// 0x1e8
			 0x2c5d83f9, /// 0x1ec
			 0xae2396c3, /// 0x1f0
			 0x5dbb5238, /// 0x1f4
			 0xf65147c2, /// 0x1f8
			 0x13bef66a, /// 0x1fc
			 0x68892f59, /// 0x200
			 0xd8b1620c, /// 0x204
			 0xc0058edf, /// 0x208
			 0xa30a40e3, /// 0x20c
			 0x0cab1b43, /// 0x210
			 0x38cc4615, /// 0x214
			 0x34fe23d0, /// 0x218
			 0x29b34d24, /// 0x21c
			 0x68e3cd96, /// 0x220
			 0x7f2ab125, /// 0x224
			 0x1988e76d, /// 0x228
			 0x1de6deb5, /// 0x22c
			 0x8e3d8fa6, /// 0x230
			 0x5e3859e6, /// 0x234
			 0x19e20324, /// 0x238
			 0xce5ff839, /// 0x23c
			 0xa8b7a488, /// 0x240
			 0x6c69574f, /// 0x244
			 0xca90dc0c, /// 0x248
			 0x2f0105ee, /// 0x24c
			 0x5e245fcd, /// 0x250
			 0xf6e952a5, /// 0x254
			 0xf3036399, /// 0x258
			 0xf8d6bff4, /// 0x25c
			 0x6fbc1637, /// 0x260
			 0x83aba441, /// 0x264
			 0x047f9343, /// 0x268
			 0x094a94eb, /// 0x26c
			 0x1842cbd6, /// 0x270
			 0xf62cacdd, /// 0x274
			 0xaaf4db49, /// 0x278
			 0x64db0628, /// 0x27c
			 0x1c3d1fc6, /// 0x280
			 0x39965b1e, /// 0x284
			 0x138a85f0, /// 0x288
			 0x529d57fb, /// 0x28c
			 0x53d86771, /// 0x290
			 0x09973cff, /// 0x294
			 0xe519774e, /// 0x298
			 0x2a99033b, /// 0x29c
			 0xedbd7254, /// 0x2a0
			 0x69f3b2e5, /// 0x2a4
			 0x8c55f3ba, /// 0x2a8
			 0x4a4e4080, /// 0x2ac
			 0xba832cbb, /// 0x2b0
			 0x354cca8e, /// 0x2b4
			 0xfb5cb8bd, /// 0x2b8
			 0x19a89633, /// 0x2bc
			 0xdc35f0a6, /// 0x2c0
			 0xd4dd1a20, /// 0x2c4
			 0x70c5c044, /// 0x2c8
			 0xd8a6e219, /// 0x2cc
			 0x49f4ca16, /// 0x2d0
			 0x1c0a8074, /// 0x2d4
			 0x1f9f57f1, /// 0x2d8
			 0x9f9e653a, /// 0x2dc
			 0x1abc1d18, /// 0x2e0
			 0x6d7b5292, /// 0x2e4
			 0xe1599ebf, /// 0x2e8
			 0x191fbfc0, /// 0x2ec
			 0x7008385b, /// 0x2f0
			 0xdc7d3517, /// 0x2f4
			 0x42a53f53, /// 0x2f8
			 0x79286a87, /// 0x2fc
			 0x15985bf6, /// 0x300
			 0x9dc69f15, /// 0x304
			 0x3f0c92df, /// 0x308
			 0x8b28e55a, /// 0x30c
			 0x8283c8c7, /// 0x310
			 0xff51b991, /// 0x314
			 0x7475186c, /// 0x318
			 0x5c09efe3, /// 0x31c
			 0x9e069432, /// 0x320
			 0xe53e3a5d, /// 0x324
			 0xc30fe4ff, /// 0x328
			 0x963afa71, /// 0x32c
			 0xf5a6fcee, /// 0x330
			 0xd6c4e32a, /// 0x334
			 0x0a146226, /// 0x338
			 0x8844a360, /// 0x33c
			 0x7457e4e1, /// 0x340
			 0xebce9fa4, /// 0x344
			 0x9f489f9b, /// 0x348
			 0xa38bfeef, /// 0x34c
			 0xb939c11e, /// 0x350
			 0xcea650d7, /// 0x354
			 0xb49df777, /// 0x358
			 0x647c5163, /// 0x35c
			 0x7c38d20e, /// 0x360
			 0x84ed5063, /// 0x364
			 0x4e8e733e, /// 0x368
			 0x9a9d4fcc, /// 0x36c
			 0x7ff354ef, /// 0x370
			 0x6b96338d, /// 0x374
			 0x25edb3ea, /// 0x378
			 0x61e64e8f, /// 0x37c
			 0x7e1a1a04, /// 0x380
			 0x6b9d7b05, /// 0x384
			 0x66b7e137, /// 0x388
			 0x03ef9fa2, /// 0x38c
			 0x1e809432, /// 0x390
			 0x3ef25da2, /// 0x394
			 0x7e670dd4, /// 0x398
			 0x78d8c26b, /// 0x39c
			 0xd5e8cdbe, /// 0x3a0
			 0xd9282d57, /// 0x3a4
			 0x69545f8f, /// 0x3a8
			 0x38255763, /// 0x3ac
			 0x150faec7, /// 0x3b0
			 0x3f99ac71, /// 0x3b4
			 0xcc96f291, /// 0x3b8
			 0x4874e2b4, /// 0x3bc
			 0xde8c686f, /// 0x3c0
			 0x88072148, /// 0x3c4
			 0x011765f6, /// 0x3c8
			 0x49e3b1c3, /// 0x3cc
			 0xa6e64352, /// 0x3d0
			 0x6c1c02ec, /// 0x3d4
			 0xc19a9416, /// 0x3d8
			 0xa09df946, /// 0x3dc
			 0x88937cda, /// 0x3e0
			 0x3391ca7a, /// 0x3e4
			 0x6e686263, /// 0x3e8
			 0x11f64a3a, /// 0x3ec
			 0x95f8fa74, /// 0x3f0
			 0x54b7626b, /// 0x3f4
			 0x1e3af1e2, /// 0x3f8
			 0x45405d64, /// 0x3fc
			 0xd3941047, /// 0x400
			 0x2ce33b03, /// 0x404
			 0x5a2a4a04, /// 0x408
			 0x08b4c80f, /// 0x40c
			 0x05213418, /// 0x410
			 0x5402d164, /// 0x414
			 0x6f51e652, /// 0x418
			 0x30278206, /// 0x41c
			 0x75084a3c, /// 0x420
			 0xcb755df9, /// 0x424
			 0x24caded2, /// 0x428
			 0x625364e8, /// 0x42c
			 0x2fde7812, /// 0x430
			 0x36483c0e, /// 0x434
			 0x2e94d289, /// 0x438
			 0xf67802e7, /// 0x43c
			 0xa6ea626d, /// 0x440
			 0x6c6f02c5, /// 0x444
			 0x7dc099a9, /// 0x448
			 0x0b3ea827, /// 0x44c
			 0xad9e4ea6, /// 0x450
			 0x72fdc752, /// 0x454
			 0xb931aade, /// 0x458
			 0x0a566af8, /// 0x45c
			 0x44502fb1, /// 0x460
			 0xe7b48253, /// 0x464
			 0x9e9beb0e, /// 0x468
			 0x6d592bea, /// 0x46c
			 0x6d076e00, /// 0x470
			 0x2f1778e5, /// 0x474
			 0x0bfbfdae, /// 0x478
			 0xc4500b77, /// 0x47c
			 0x774501bd, /// 0x480
			 0xfdbf7309, /// 0x484
			 0xd52e48c2, /// 0x488
			 0xfed6c2b0, /// 0x48c
			 0x9115ef21, /// 0x490
			 0x6deca617, /// 0x494
			 0x12c9984a, /// 0x498
			 0x89096901, /// 0x49c
			 0x34b4ff50, /// 0x4a0
			 0xc839c9bd, /// 0x4a4
			 0x5df1dd3c, /// 0x4a8
			 0xf2c46443, /// 0x4ac
			 0xd8c0ef50, /// 0x4b0
			 0xb12dec28, /// 0x4b4
			 0x26826930, /// 0x4b8
			 0xa7d85dc2, /// 0x4bc
			 0x131428a4, /// 0x4c0
			 0x82455bae, /// 0x4c4
			 0xfb4b7872, /// 0x4c8
			 0xd1eecd54, /// 0x4cc
			 0x7bb76888, /// 0x4d0
			 0x036350e3, /// 0x4d4
			 0xc447e8d2, /// 0x4d8
			 0x43f9a3d1, /// 0x4dc
			 0x0265e906, /// 0x4e0
			 0x8ef54e00, /// 0x4e4
			 0x5498c79e, /// 0x4e8
			 0xe283e30a, /// 0x4ec
			 0x2865c96d, /// 0x4f0
			 0x2aff06cb, /// 0x4f4
			 0xa4413d65, /// 0x4f8
			 0x1464da48, /// 0x4fc
			 0xc5eaffa5, /// 0x500
			 0x62b79f70, /// 0x504
			 0x2a5e9ede, /// 0x508
			 0x953c87b5, /// 0x50c
			 0x68fb1d53, /// 0x510
			 0x4ca7c437, /// 0x514
			 0xf3996bf5, /// 0x518
			 0x0e0f17e7, /// 0x51c
			 0x928554a4, /// 0x520
			 0xde9f81c0, /// 0x524
			 0x0d3b312d, /// 0x528
			 0xb3bc63ad, /// 0x52c
			 0x317d3723, /// 0x530
			 0x081c7d23, /// 0x534
			 0xfd3b5f7f, /// 0x538
			 0x0c1a66eb, /// 0x53c
			 0xe97952bf, /// 0x540
			 0x25f4e256, /// 0x544
			 0xbaebec20, /// 0x548
			 0x76ff6abf, /// 0x54c
			 0x3d532fc6, /// 0x550
			 0x8fdc9648, /// 0x554
			 0x09f04cec, /// 0x558
			 0xdfb83bd2, /// 0x55c
			 0x314a62f7, /// 0x560
			 0xd7b4f94e, /// 0x564
			 0x7717e394, /// 0x568
			 0xe9bc47d4, /// 0x56c
			 0x6634ff5d, /// 0x570
			 0x6090ce09, /// 0x574
			 0x4d1a6305, /// 0x578
			 0x02f5936f, /// 0x57c
			 0x1a7e936f, /// 0x580
			 0xe4553b59, /// 0x584
			 0xfe5d8506, /// 0x588
			 0x82d4d693, /// 0x58c
			 0x03e08663, /// 0x590
			 0xeca8d596, /// 0x594
			 0x20d83d06, /// 0x598
			 0x701788bb, /// 0x59c
			 0xbdf5cf84, /// 0x5a0
			 0x9195cbff, /// 0x5a4
			 0x0a052044, /// 0x5a8
			 0xbb97d960, /// 0x5ac
			 0xf95acf93, /// 0x5b0
			 0xb9e26b9e, /// 0x5b4
			 0xc36f994d, /// 0x5b8
			 0xec52fc0f, /// 0x5bc
			 0xed295e63, /// 0x5c0
			 0x471ddc06, /// 0x5c4
			 0x9295c65e, /// 0x5c8
			 0xd00b3622, /// 0x5cc
			 0xb6db5ba9, /// 0x5d0
			 0xae9ebc48, /// 0x5d4
			 0x689d52f0, /// 0x5d8
			 0xff158e89, /// 0x5dc
			 0xb91ce1e9, /// 0x5e0
			 0xb018d35f, /// 0x5e4
			 0xa5a5eaca, /// 0x5e8
			 0xc288b14a, /// 0x5ec
			 0xad8593a7, /// 0x5f0
			 0x168dc1a4, /// 0x5f4
			 0x5068de2d, /// 0x5f8
			 0x50c04037, /// 0x5fc
			 0x19bce2c4, /// 0x600
			 0x8f2d0faf, /// 0x604
			 0x390401db, /// 0x608
			 0x1e5490ec, /// 0x60c
			 0x0ad655c8, /// 0x610
			 0xeabb33b3, /// 0x614
			 0x98267f51, /// 0x618
			 0x6b646186, /// 0x61c
			 0xcda62010, /// 0x620
			 0xe05b94a1, /// 0x624
			 0xe91137b3, /// 0x628
			 0x9aac8dd2, /// 0x62c
			 0x3d0e7bd5, /// 0x630
			 0xf4d8c75f, /// 0x634
			 0x7d9ad35f, /// 0x638
			 0x7d0d65e0, /// 0x63c
			 0x31505ba8, /// 0x640
			 0x00fcdf71, /// 0x644
			 0x4874995d, /// 0x648
			 0x2bac5900, /// 0x64c
			 0x941058c9, /// 0x650
			 0x376e0d6a, /// 0x654
			 0xbcbe17d4, /// 0x658
			 0x8f9ecfcd, /// 0x65c
			 0x06803a13, /// 0x660
			 0xd2922db5, /// 0x664
			 0xb67aeb4b, /// 0x668
			 0x83fadc76, /// 0x66c
			 0xb74a739e, /// 0x670
			 0xe497730b, /// 0x674
			 0xb0c96afe, /// 0x678
			 0xa45ea3ec, /// 0x67c
			 0xebdd2f1b, /// 0x680
			 0x2a9f7a78, /// 0x684
			 0x24352d02, /// 0x688
			 0xa68db893, /// 0x68c
			 0x0cebca32, /// 0x690
			 0x603e7fb3, /// 0x694
			 0x06bb493a, /// 0x698
			 0x1222f886, /// 0x69c
			 0x2762a297, /// 0x6a0
			 0xb8263b2b, /// 0x6a4
			 0xdbfd4709, /// 0x6a8
			 0x9f8f5e2e, /// 0x6ac
			 0x2e2b9252, /// 0x6b0
			 0xd89135f3, /// 0x6b4
			 0xa43050df, /// 0x6b8
			 0xde1e690e, /// 0x6bc
			 0xac3e5afa, /// 0x6c0
			 0x3385de0b, /// 0x6c4
			 0x090faa39, /// 0x6c8
			 0x8f9dc425, /// 0x6cc
			 0x02bc49a5, /// 0x6d0
			 0xcfdf03cd, /// 0x6d4
			 0x264e8d5f, /// 0x6d8
			 0x1cc9cf7c, /// 0x6dc
			 0xe9e137b6, /// 0x6e0
			 0xe0b8a17a, /// 0x6e4
			 0xd943682e, /// 0x6e8
			 0xbe1fd271, /// 0x6ec
			 0xacd24ca7, /// 0x6f0
			 0x5c7a08aa, /// 0x6f4
			 0xb34a036d, /// 0x6f8
			 0x0191eb02, /// 0x6fc
			 0xf19cb10b, /// 0x700
			 0xd5c1da72, /// 0x704
			 0x1b2c5aa4, /// 0x708
			 0x339bb1ce, /// 0x70c
			 0x7ceb9d38, /// 0x710
			 0xc84fcac9, /// 0x714
			 0xc88e31b7, /// 0x718
			 0xa8cc17f1, /// 0x71c
			 0xec9824bc, /// 0x720
			 0x8b11c69a, /// 0x724
			 0xf7e368d3, /// 0x728
			 0xf7ddc682, /// 0x72c
			 0x569254f1, /// 0x730
			 0xa943dfe9, /// 0x734
			 0x72aae0b6, /// 0x738
			 0x5213cfe9, /// 0x73c
			 0x63da50be, /// 0x740
			 0x5de3663a, /// 0x744
			 0x2ae3bd44, /// 0x748
			 0xf1fd8428, /// 0x74c
			 0x8ed4db07, /// 0x750
			 0x0fae1ee0, /// 0x754
			 0x1d8276c7, /// 0x758
			 0xe9d7e3e9, /// 0x75c
			 0x3055cd16, /// 0x760
			 0x1d4d597c, /// 0x764
			 0x7c16bf19, /// 0x768
			 0x9762008a, /// 0x76c
			 0x1d9378c2, /// 0x770
			 0xeab7ee21, /// 0x774
			 0xdeb761eb, /// 0x778
			 0x764b3d8d, /// 0x77c
			 0x9bc20973, /// 0x780
			 0x23dc4fc9, /// 0x784
			 0xf028b46c, /// 0x788
			 0xc51f19c5, /// 0x78c
			 0x98c82a34, /// 0x790
			 0x91caefeb, /// 0x794
			 0x5abb4478, /// 0x798
			 0x4c1634c4, /// 0x79c
			 0xe4e8124f, /// 0x7a0
			 0xbd4b03b7, /// 0x7a4
			 0x99d30dac, /// 0x7a8
			 0x2d0f0831, /// 0x7ac
			 0xf5dce613, /// 0x7b0
			 0x967bc465, /// 0x7b4
			 0x4c9527a4, /// 0x7b8
			 0x96ff8720, /// 0x7bc
			 0xbdfea85d, /// 0x7c0
			 0xdcb3bfd3, /// 0x7c4
			 0xe6ebb1b1, /// 0x7c8
			 0xa128b05c, /// 0x7cc
			 0xfec7c3df, /// 0x7d0
			 0x10009ac1, /// 0x7d4
			 0x63c72705, /// 0x7d8
			 0x37fcee88, /// 0x7dc
			 0xc2a0af94, /// 0x7e0
			 0x77aa5c9b, /// 0x7e4
			 0xd2373f45, /// 0x7e8
			 0xcbf1969d, /// 0x7ec
			 0xb3e6a61f, /// 0x7f0
			 0x9846a4af, /// 0x7f4
			 0xf6e3d8f7, /// 0x7f8
			 0x6bf5b992, /// 0x7fc
			 0x0f0290a7, /// 0x800
			 0x68979a15, /// 0x804
			 0xde83b1b9, /// 0x808
			 0xce00b720, /// 0x80c
			 0xf0781599, /// 0x810
			 0x61be8254, /// 0x814
			 0x71b47f4d, /// 0x818
			 0x95fbdc5a, /// 0x81c
			 0xae4a32a7, /// 0x820
			 0xb1373960, /// 0x824
			 0x7ddfaf81, /// 0x828
			 0x6dae575f, /// 0x82c
			 0x843251ff, /// 0x830
			 0xe8435687, /// 0x834
			 0x0513593e, /// 0x838
			 0x063fd513, /// 0x83c
			 0xb4230c27, /// 0x840
			 0x511b7e84, /// 0x844
			 0x7663241e, /// 0x848
			 0x62aa6367, /// 0x84c
			 0x24b035e6, /// 0x850
			 0x5ad8734b, /// 0x854
			 0x96ce970a, /// 0x858
			 0x5b41bf7b, /// 0x85c
			 0xf294dede, /// 0x860
			 0x72c88c06, /// 0x864
			 0x45431c9a, /// 0x868
			 0xfe969c25, /// 0x86c
			 0xb9a5089e, /// 0x870
			 0xc1395f60, /// 0x874
			 0x885cb7d4, /// 0x878
			 0x948e0c97, /// 0x87c
			 0xc62ae938, /// 0x880
			 0x069c6d28, /// 0x884
			 0xd47e048e, /// 0x888
			 0x6b36d9ff, /// 0x88c
			 0x7fd04d8d, /// 0x890
			 0x022e1c1a, /// 0x894
			 0x3c66507e, /// 0x898
			 0xd31caef3, /// 0x89c
			 0x89d4ad42, /// 0x8a0
			 0x162d39c1, /// 0x8a4
			 0x8429ed42, /// 0x8a8
			 0x309aba88, /// 0x8ac
			 0x2aa1481b, /// 0x8b0
			 0x00ed784d, /// 0x8b4
			 0x38e47a3f, /// 0x8b8
			 0xede028b7, /// 0x8bc
			 0x6cfccc18, /// 0x8c0
			 0x49dff927, /// 0x8c4
			 0x674d47bf, /// 0x8c8
			 0x45826d48, /// 0x8cc
			 0x4c32d4f7, /// 0x8d0
			 0x301b63d0, /// 0x8d4
			 0xa6057f65, /// 0x8d8
			 0xdbfc3015, /// 0x8dc
			 0x3f55a26d, /// 0x8e0
			 0x7fe5d64a, /// 0x8e4
			 0x9ab8a734, /// 0x8e8
			 0xe7cb54c0, /// 0x8ec
			 0x7e818dbc, /// 0x8f0
			 0x21b26702, /// 0x8f4
			 0x1468b89a, /// 0x8f8
			 0x3937ad11, /// 0x8fc
			 0xf4f1120f, /// 0x900
			 0x1b53880a, /// 0x904
			 0xaa25be94, /// 0x908
			 0xb2cd2ed4, /// 0x90c
			 0x1f94e544, /// 0x910
			 0x4b103d8a, /// 0x914
			 0xbee717b5, /// 0x918
			 0xa279058d, /// 0x91c
			 0x871d71af, /// 0x920
			 0xd13b4c38, /// 0x924
			 0x6a69d7fd, /// 0x928
			 0x3bb1e502, /// 0x92c
			 0x6162a0c6, /// 0x930
			 0xdf156b72, /// 0x934
			 0x25bcc73b, /// 0x938
			 0x444f01ae, /// 0x93c
			 0xb77326db, /// 0x940
			 0xf9bee2bc, /// 0x944
			 0xd1956322, /// 0x948
			 0x073e9e33, /// 0x94c
			 0x90fa7ea4, /// 0x950
			 0x03ab3d0e, /// 0x954
			 0xae93df79, /// 0x958
			 0xf8252386, /// 0x95c
			 0xb31d1955, /// 0x960
			 0x4c9063fc, /// 0x964
			 0xfbbf39d8, /// 0x968
			 0xb928e909, /// 0x96c
			 0xff75f763, /// 0x970
			 0x93ab804b, /// 0x974
			 0xe78e2d39, /// 0x978
			 0x2dbd421a, /// 0x97c
			 0xce604844, /// 0x980
			 0x9d08d35f, /// 0x984
			 0x0081ae42, /// 0x988
			 0xec73c568, /// 0x98c
			 0xd4631162, /// 0x990
			 0xa9bd4d7f, /// 0x994
			 0x0554f2e4, /// 0x998
			 0x3bcac238, /// 0x99c
			 0x04d9b0dd, /// 0x9a0
			 0x49eb8c1d, /// 0x9a4
			 0x40b0607f, /// 0x9a8
			 0xc43d9367, /// 0x9ac
			 0x3e375b78, /// 0x9b0
			 0x7f8b3e1b, /// 0x9b4
			 0xb6c10a6c, /// 0x9b8
			 0x249d2810, /// 0x9bc
			 0x56031069, /// 0x9c0
			 0x55a2eb11, /// 0x9c4
			 0x89b728f3, /// 0x9c8
			 0xee40bd61, /// 0x9cc
			 0x056c6762, /// 0x9d0
			 0xcf49cca4, /// 0x9d4
			 0x11d3bbf0, /// 0x9d8
			 0xab0d8d7f, /// 0x9dc
			 0x15cf9866, /// 0x9e0
			 0xa86d5c1e, /// 0x9e4
			 0x4aab75aa, /// 0x9e8
			 0x39f8e6f0, /// 0x9ec
			 0x726d50a1, /// 0x9f0
			 0xf8c49872, /// 0x9f4
			 0x7f70ff18, /// 0x9f8
			 0x12fe48af, /// 0x9fc
			 0x0527d418, /// 0xa00
			 0xbd2855b7, /// 0xa04
			 0x131309ee, /// 0xa08
			 0x22d2dc97, /// 0xa0c
			 0xdf816205, /// 0xa10
			 0xee035569, /// 0xa14
			 0xfe2f16f7, /// 0xa18
			 0x338c30c6, /// 0xa1c
			 0xa564b8d1, /// 0xa20
			 0x3c858d87, /// 0xa24
			 0x94501150, /// 0xa28
			 0x721f21d3, /// 0xa2c
			 0xcbbbf6bb, /// 0xa30
			 0x07912840, /// 0xa34
			 0xf3b05301, /// 0xa38
			 0x0f3df5d0, /// 0xa3c
			 0x07ba66b7, /// 0xa40
			 0x872f083f, /// 0xa44
			 0xaeb31287, /// 0xa48
			 0x3e35a3d6, /// 0xa4c
			 0xb254def6, /// 0xa50
			 0xf58f30bd, /// 0xa54
			 0xdc997d6d, /// 0xa58
			 0x1c835b4a, /// 0xa5c
			 0x53b5ac45, /// 0xa60
			 0xbb433320, /// 0xa64
			 0x19692da0, /// 0xa68
			 0x0cc9005d, /// 0xa6c
			 0x3ddf248b, /// 0xa70
			 0x06e37db0, /// 0xa74
			 0xf4b344d7, /// 0xa78
			 0x92522ffd, /// 0xa7c
			 0x68d18805, /// 0xa80
			 0x38263210, /// 0xa84
			 0x3cf152e7, /// 0xa88
			 0xfc833005, /// 0xa8c
			 0xc3d45be9, /// 0xa90
			 0x9e3cfdea, /// 0xa94
			 0x8879e40e, /// 0xa98
			 0xf8dd72f2, /// 0xa9c
			 0x28b8e3ed, /// 0xaa0
			 0x54ab4672, /// 0xaa4
			 0xfb1c1a9b, /// 0xaa8
			 0x602aa919, /// 0xaac
			 0x4882e150, /// 0xab0
			 0x0eb93073, /// 0xab4
			 0xf7de122c, /// 0xab8
			 0x144d44d1, /// 0xabc
			 0x2be7d4ec, /// 0xac0
			 0x392f37b0, /// 0xac4
			 0x4867927c, /// 0xac8
			 0xb670bd9c, /// 0xacc
			 0x41d038db, /// 0xad0
			 0x9737ee7e, /// 0xad4
			 0x09d79225, /// 0xad8
			 0x5a11de6c, /// 0xadc
			 0x71e3485e, /// 0xae0
			 0x8991b103, /// 0xae4
			 0x03d5df48, /// 0xae8
			 0xaa7e5dca, /// 0xaec
			 0x32c1159a, /// 0xaf0
			 0x364e9437, /// 0xaf4
			 0x9c6d19e1, /// 0xaf8
			 0x54115138, /// 0xafc
			 0x9992c676, /// 0xb00
			 0xf1e402b9, /// 0xb04
			 0x0459a86e, /// 0xb08
			 0xe4b58dd9, /// 0xb0c
			 0xa99153b3, /// 0xb10
			 0xc765d8a3, /// 0xb14
			 0xc48760f4, /// 0xb18
			 0x9f89fe9c, /// 0xb1c
			 0xce3675df, /// 0xb20
			 0x6af39d23, /// 0xb24
			 0x9dd35795, /// 0xb28
			 0x2aef1454, /// 0xb2c
			 0x234490f5, /// 0xb30
			 0x26c126d8, /// 0xb34
			 0xbd2dd90a, /// 0xb38
			 0x0962d5c6, /// 0xb3c
			 0x9b209ca6, /// 0xb40
			 0x65f52141, /// 0xb44
			 0x02fca81d, /// 0xb48
			 0x88a44c6f, /// 0xb4c
			 0xf2b1e39e, /// 0xb50
			 0x24dceecf, /// 0xb54
			 0xfa1e735b, /// 0xb58
			 0x5b1a9e58, /// 0xb5c
			 0x4bb934c8, /// 0xb60
			 0xb412f255, /// 0xb64
			 0xc0157424, /// 0xb68
			 0x11d0a488, /// 0xb6c
			 0x5824285f, /// 0xb70
			 0x7ea4b96f, /// 0xb74
			 0x828e79f1, /// 0xb78
			 0x80978aba, /// 0xb7c
			 0x168028c2, /// 0xb80
			 0xd6cc460b, /// 0xb84
			 0xaf9e8613, /// 0xb88
			 0x3be2fa6f, /// 0xb8c
			 0xbb4ab4d4, /// 0xb90
			 0x416dc27a, /// 0xb94
			 0xb679a82e, /// 0xb98
			 0x20c3f6e7, /// 0xb9c
			 0x0dc89903, /// 0xba0
			 0xcfad102d, /// 0xba4
			 0x963e604c, /// 0xba8
			 0x3ac0934a, /// 0xbac
			 0x414d27c8, /// 0xbb0
			 0x9228b3ab, /// 0xbb4
			 0xcfb3221a, /// 0xbb8
			 0xc6f1e353, /// 0xbbc
			 0x7f6f7067, /// 0xbc0
			 0x8aba37fe, /// 0xbc4
			 0xa6910542, /// 0xbc8
			 0x31472c3e, /// 0xbcc
			 0x51393c61, /// 0xbd0
			 0xb8cd928f, /// 0xbd4
			 0xa44c8ae9, /// 0xbd8
			 0x11ffdc16, /// 0xbdc
			 0x3f5b3b38, /// 0xbe0
			 0xfe1d2924, /// 0xbe4
			 0xda8a7f85, /// 0xbe8
			 0xa1cd04a6, /// 0xbec
			 0xcf0753f2, /// 0xbf0
			 0x086efa59, /// 0xbf4
			 0xb8a355b2, /// 0xbf8
			 0xc82ac42b, /// 0xbfc
			 0x9e9901db, /// 0xc00
			 0xde0a2c99, /// 0xc04
			 0x228fcea3, /// 0xc08
			 0x8554e351, /// 0xc0c
			 0xb94b71b4, /// 0xc10
			 0x4c729a99, /// 0xc14
			 0x07352a9d, /// 0xc18
			 0x559c6a2d, /// 0xc1c
			 0x6c11e2f1, /// 0xc20
			 0x596507f9, /// 0xc24
			 0xab59a967, /// 0xc28
			 0xa58f3f62, /// 0xc2c
			 0xc905781a, /// 0xc30
			 0x88e0ae00, /// 0xc34
			 0x8ff6c504, /// 0xc38
			 0x2a82e1e5, /// 0xc3c
			 0xa25b7d4c, /// 0xc40
			 0x17e58085, /// 0xc44
			 0xe4e71ac9, /// 0xc48
			 0x1355e307, /// 0xc4c
			 0x7f5998cc, /// 0xc50
			 0xd0f02ce9, /// 0xc54
			 0xff44c6e8, /// 0xc58
			 0xa909e9e5, /// 0xc5c
			 0x616610b7, /// 0xc60
			 0xf65729c7, /// 0xc64
			 0xd4573097, /// 0xc68
			 0x3e45549d, /// 0xc6c
			 0xce96ddd8, /// 0xc70
			 0xeb0b70ed, /// 0xc74
			 0xafbd0a92, /// 0xc78
			 0x6e99a210, /// 0xc7c
			 0xdf000487, /// 0xc80
			 0xd9cd19c5, /// 0xc84
			 0x2d708fa3, /// 0xc88
			 0x8f43cc95, /// 0xc8c
			 0xf417fd56, /// 0xc90
			 0x1186ad80, /// 0xc94
			 0x8060c612, /// 0xc98
			 0x12a0a5f9, /// 0xc9c
			 0x14be88e2, /// 0xca0
			 0x52e600ac, /// 0xca4
			 0x401b2143, /// 0xca8
			 0x5b2667da, /// 0xcac
			 0x4873a812, /// 0xcb0
			 0x5741b262, /// 0xcb4
			 0x769f8904, /// 0xcb8
			 0x2cca87be, /// 0xcbc
			 0x7ef8c925, /// 0xcc0
			 0x56f73dd2, /// 0xcc4
			 0x160e4fa7, /// 0xcc8
			 0xf62ba9e9, /// 0xccc
			 0x7687c688, /// 0xcd0
			 0xd5fd2b6c, /// 0xcd4
			 0x851651b2, /// 0xcd8
			 0x23e0024b, /// 0xcdc
			 0xb82cd57c, /// 0xce0
			 0x86d01ddd, /// 0xce4
			 0xd91ccc42, /// 0xce8
			 0x22c3487e, /// 0xcec
			 0xa752cb0c, /// 0xcf0
			 0x40a17107, /// 0xcf4
			 0x0d00d4cf, /// 0xcf8
			 0x522507f0, /// 0xcfc
			 0xc39fb956, /// 0xd00
			 0xb7f30ef3, /// 0xd04
			 0x8bc3b994, /// 0xd08
			 0x8c354c9b, /// 0xd0c
			 0x8a6a06b0, /// 0xd10
			 0x43df25f6, /// 0xd14
			 0xf488d7e6, /// 0xd18
			 0x0f925b5e, /// 0xd1c
			 0x65c979d2, /// 0xd20
			 0x5afefd25, /// 0xd24
			 0xe7811793, /// 0xd28
			 0x37bf74ff, /// 0xd2c
			 0x52097843, /// 0xd30
			 0xe0b4b07b, /// 0xd34
			 0x1b674baa, /// 0xd38
			 0x8d2895d1, /// 0xd3c
			 0xd2541521, /// 0xd40
			 0x3ed4b7e8, /// 0xd44
			 0x614632e0, /// 0xd48
			 0xefce86fc, /// 0xd4c
			 0x02a21212, /// 0xd50
			 0x6fb709eb, /// 0xd54
			 0xa08a094e, /// 0xd58
			 0xe1bfb15c, /// 0xd5c
			 0xec16c0fb, /// 0xd60
			 0x4b92b4f5, /// 0xd64
			 0xfff243fc, /// 0xd68
			 0x8f9c23d0, /// 0xd6c
			 0x30367750, /// 0xd70
			 0xaefda7c7, /// 0xd74
			 0x9eb40a75, /// 0xd78
			 0xe64e10a8, /// 0xd7c
			 0xaea7050c, /// 0xd80
			 0x1e65a04a, /// 0xd84
			 0xef9d70f2, /// 0xd88
			 0xa66204f8, /// 0xd8c
			 0x787a3248, /// 0xd90
			 0x9db409f0, /// 0xd94
			 0x3b64e544, /// 0xd98
			 0xe6b9f1b1, /// 0xd9c
			 0x6e49df6a, /// 0xda0
			 0x65647cc0, /// 0xda4
			 0xda14d4dd, /// 0xda8
			 0xbb5ecb1f, /// 0xdac
			 0xb8fc024e, /// 0xdb0
			 0xa4b70d27, /// 0xdb4
			 0xf88a9413, /// 0xdb8
			 0x87bfd335, /// 0xdbc
			 0x8c4a48ab, /// 0xdc0
			 0x7b15c6ef, /// 0xdc4
			 0x8378fa86, /// 0xdc8
			 0xed918b02, /// 0xdcc
			 0x6238cbaf, /// 0xdd0
			 0xb73776a1, /// 0xdd4
			 0x9d5f9fae, /// 0xdd8
			 0x73c5fd26, /// 0xddc
			 0x65719151, /// 0xde0
			 0x78f4434f, /// 0xde4
			 0x021ac964, /// 0xde8
			 0xa625cf9e, /// 0xdec
			 0x5db6f61f, /// 0xdf0
			 0x385fb6f3, /// 0xdf4
			 0x04fd9dad, /// 0xdf8
			 0x882e332c, /// 0xdfc
			 0xcccd2112, /// 0xe00
			 0x66851b90, /// 0xe04
			 0x6a2742d2, /// 0xe08
			 0x6b350d37, /// 0xe0c
			 0xa1644b84, /// 0xe10
			 0xde2085f8, /// 0xe14
			 0x51aa528f, /// 0xe18
			 0x4ca4e982, /// 0xe1c
			 0x72ff3b57, /// 0xe20
			 0x7bc5b13a, /// 0xe24
			 0xd0e1ed8e, /// 0xe28
			 0x944a2db1, /// 0xe2c
			 0xf47e48dd, /// 0xe30
			 0x2e43d0cf, /// 0xe34
			 0xdfe1fb45, /// 0xe38
			 0x6cf4c1f3, /// 0xe3c
			 0x4ffe8b78, /// 0xe40
			 0x69e871b5, /// 0xe44
			 0xfe5e5256, /// 0xe48
			 0x55ac2e14, /// 0xe4c
			 0x81f76149, /// 0xe50
			 0x78acd636, /// 0xe54
			 0xa4689ca8, /// 0xe58
			 0xb1b4997b, /// 0xe5c
			 0xc24ea987, /// 0xe60
			 0xadeabd79, /// 0xe64
			 0x824e51af, /// 0xe68
			 0x9432357a, /// 0xe6c
			 0x30361709, /// 0xe70
			 0x1311730f, /// 0xe74
			 0x457500fe, /// 0xe78
			 0xc1ee6ef8, /// 0xe7c
			 0xfe9a0610, /// 0xe80
			 0x20b73a3f, /// 0xe84
			 0x4bdfd47f, /// 0xe88
			 0x567cf98e, /// 0xe8c
			 0x757d99cc, /// 0xe90
			 0x80bca006, /// 0xe94
			 0xbab31530, /// 0xe98
			 0x3f7aee1b, /// 0xe9c
			 0xddeade99, /// 0xea0
			 0x7cf0a897, /// 0xea4
			 0x819fc88f, /// 0xea8
			 0x4ec93ae9, /// 0xeac
			 0xa99b4afa, /// 0xeb0
			 0x50cc0011, /// 0xeb4
			 0x06083a9e, /// 0xeb8
			 0x8a66382d, /// 0xebc
			 0x2e47c123, /// 0xec0
			 0x4dd307d0, /// 0xec4
			 0x86874521, /// 0xec8
			 0xb6dae43a, /// 0xecc
			 0xff0fccff, /// 0xed0
			 0x7623b05c, /// 0xed4
			 0xbf20d8dc, /// 0xed8
			 0xd63323e7, /// 0xedc
			 0xe8eef442, /// 0xee0
			 0xcf448462, /// 0xee4
			 0x771fae98, /// 0xee8
			 0xa44c883a, /// 0xeec
			 0x14f62404, /// 0xef0
			 0xd68e5754, /// 0xef4
			 0x7916b3bd, /// 0xef8
			 0x7406b682, /// 0xefc
			 0x47d95571, /// 0xf00
			 0x9aca5fc3, /// 0xf04
			 0xe68c35a0, /// 0xf08
			 0x42615409, /// 0xf0c
			 0x37140a11, /// 0xf10
			 0x90d2bc83, /// 0xf14
			 0x0ec23bfa, /// 0xf18
			 0x91e93f7e, /// 0xf1c
			 0x41ed10d6, /// 0xf20
			 0x9cfb9942, /// 0xf24
			 0x4ab5b822, /// 0xf28
			 0xbbf06cd0, /// 0xf2c
			 0x35b1a59c, /// 0xf30
			 0xbb2ef765, /// 0xf34
			 0x45f8059b, /// 0xf38
			 0xecd0b8f2, /// 0xf3c
			 0x5a36c4ff, /// 0xf40
			 0x561c5be6, /// 0xf44
			 0xe462e3ae, /// 0xf48
			 0xecb16c80, /// 0xf4c
			 0x957911be, /// 0xf50
			 0xc35a53f5, /// 0xf54
			 0xfabef22c, /// 0xf58
			 0x2299dd87, /// 0xf5c
			 0x5810f197, /// 0xf60
			 0x9ad0e1ac, /// 0xf64
			 0x8d8d1f33, /// 0xf68
			 0x1ccc261c, /// 0xf6c
			 0x3127da88, /// 0xf70
			 0x29708bc5, /// 0xf74
			 0xf889382f, /// 0xf78
			 0xf33688ea, /// 0xf7c
			 0x46058ab0, /// 0xf80
			 0x0d5b5090, /// 0xf84
			 0x461c225a, /// 0xf88
			 0x538d4616, /// 0xf8c
			 0x124a756d, /// 0xf90
			 0xeefd55d6, /// 0xf94
			 0xc843e45b, /// 0xf98
			 0xaf29ea27, /// 0xf9c
			 0x9a4cc159, /// 0xfa0
			 0xbc17bb98, /// 0xfa4
			 0x3f147a90, /// 0xfa8
			 0xf0f533d5, /// 0xfac
			 0x3159f9cc, /// 0xfb0
			 0xd490cbe0, /// 0xfb4
			 0x2e7137b1, /// 0xfb8
			 0x09aabe0a, /// 0xfbc
			 0x5e57c4c0, /// 0xfc0
			 0xf6677a14, /// 0xfc4
			 0x973d996e, /// 0xfc8
			 0x4bcc1b39, /// 0xfcc
			 0x5de600ea, /// 0xfd0
			 0x6546923d, /// 0xfd4
			 0x9d34f995, /// 0xfd8
			 0x6304ac8a, /// 0xfdc
			 0xff358557, /// 0xfe0
			 0x588a3f89, /// 0xfe4
			 0x7b6a316e, /// 0xfe8
			 0xa9582469, /// 0xfec
			 0x1cebe4eb, /// 0xff0
			 0xd1a0efdc, /// 0xff4
			 0xb608d9b1, /// 0xff8
			 0xc7a69a72 /// last
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
			 0x00000444,
			 0x00000404,
			 0x000006a4,
			 0x00000704,
			 0x00000644,
			 0x00000628,
			 0x00000364,
			 0x0000049c,

			 /// vpu register f2
			 0x41880000,
			 0x41c80000,
			 0x41000000,
			 0x41700000,
			 0x40a00000,
			 0x41e00000,
			 0x41880000,
			 0x41000000,

			 /// vpu register f3
			 0x40e00000,
			 0x41f00000,
			 0x41c00000,
			 0x41880000,
			 0x41c80000,
			 0x40800000,
			 0x41f00000,
			 0x41300000,

			 /// vpu register f4
			 0x41c00000,
			 0x3f800000,
			 0x41d80000,
			 0x40000000,
			 0x40c00000,
			 0x41500000,
			 0x41500000,
			 0x41000000,

			 /// vpu register f5
			 0x41d00000,
			 0x41d80000,
			 0x41700000,
			 0x41e80000,
			 0x41e00000,
			 0x41e80000,
			 0x41700000,
			 0x41800000,

			 /// vpu register f6
			 0x41c00000,
			 0x41400000,
			 0x41a80000,
			 0x40400000,
			 0x41b00000,
			 0x41b80000,
			 0x41d80000,
			 0x41800000,

			 /// vpu register f7
			 0x41900000,
			 0x40000000,
			 0x40c00000,
			 0x41c80000,
			 0x41600000,
			 0x41c00000,
			 0x41c00000,
			 0x41000000,

			 /// vpu register f8
			 0x40000000,
			 0x3f800000,
			 0x41300000,
			 0x41800000,
			 0x41d80000,
			 0x41000000,
			 0x41900000,
			 0x40000000,

			 /// vpu register f9
			 0x42000000,
			 0x41e00000,
			 0x41100000,
			 0x41600000,
			 0x41f00000,
			 0x40800000,
			 0x41b80000,
			 0x41700000,

			 /// vpu register f10
			 0x42000000,
			 0x41000000,
			 0x40c00000,
			 0x41c80000,
			 0x41c00000,
			 0x40e00000,
			 0x41900000,
			 0x41700000,

			 /// vpu register f11
			 0x41d80000,
			 0x41300000,
			 0x41200000,
			 0x41980000,
			 0x41500000,
			 0x41700000,
			 0x40e00000,
			 0x41f00000,

			 /// vpu register f12
			 0x3f800000,
			 0x41b00000,
			 0x41800000,
			 0x41e00000,
			 0x41d80000,
			 0x41e00000,
			 0x41900000,
			 0x41b80000,

			 /// vpu register f13
			 0x40000000,
			 0x41b80000,
			 0x41700000,
			 0x40e00000,
			 0x40800000,
			 0x41880000,
			 0x42000000,
			 0x41900000,

			 /// vpu register f14
			 0x40400000,
			 0x41c80000,
			 0x41d80000,
			 0x41a00000,
			 0x41e80000,
			 0x41100000,
			 0x41800000,
			 0x41b00000,

			 /// vpu register f15
			 0x41d00000,
			 0x41e80000,
			 0x41700000,
			 0x42000000,
			 0x41f00000,
			 0x40c00000,
			 0x41d00000,
			 0x41900000,

			 /// vpu register f16
			 0x3f800000,
			 0x41d80000,
			 0x41800000,
			 0x41100000,
			 0x41100000,
			 0x40c00000,
			 0x40c00000,
			 0x41980000,

			 /// vpu register f17
			 0x41100000,
			 0x41000000,
			 0x41b80000,
			 0x41500000,
			 0x41700000,
			 0x41d80000,
			 0x41880000,
			 0x41b00000,

			 /// vpu register f18
			 0x41a80000,
			 0x41d00000,
			 0x42000000,
			 0x41700000,
			 0x40800000,
			 0x41f80000,
			 0x41500000,
			 0x3f800000,

			 /// vpu register f19
			 0x41b80000,
			 0x3f800000,
			 0x41e80000,
			 0x41d80000,
			 0x40e00000,
			 0x41700000,
			 0x40e00000,
			 0x41100000,

			 /// vpu register f20
			 0x41c80000,
			 0x41700000,
			 0x41600000,
			 0x41b00000,
			 0x41a00000,
			 0x41900000,
			 0x41980000,
			 0x40e00000,

			 /// vpu register f21
			 0x41800000,
			 0x41d80000,
			 0x41f80000,
			 0x41980000,
			 0x41600000,
			 0x41400000,
			 0x41e80000,
			 0x41d80000,

			 /// vpu register f22
			 0x41600000,
			 0x41d80000,
			 0x41c80000,
			 0x41600000,
			 0x41200000,
			 0x41500000,
			 0x41d80000,
			 0x40e00000,

			 /// vpu register f23
			 0x3f800000,
			 0x41400000,
			 0x41980000,
			 0x41500000,
			 0x41700000,
			 0x41000000,
			 0x41880000,
			 0x40000000,

			 /// vpu register f24
			 0x41400000,
			 0x41b00000,
			 0x40000000,
			 0x41f00000,
			 0x41e00000,
			 0x41200000,
			 0x41300000,
			 0x41a80000,

			 /// vpu register f25
			 0x41b00000,
			 0x41e00000,
			 0x41a00000,
			 0x41980000,
			 0x41c00000,
			 0x3f800000,
			 0x41980000,
			 0x42000000,

			 /// vpu register f26
			 0x40e00000,
			 0x40400000,
			 0x41c80000,
			 0x41600000,
			 0x41e80000,
			 0x41c00000,
			 0x41900000,
			 0x41800000,

			 /// vpu register f27
			 0x40a00000,
			 0x41200000,
			 0x40400000,
			 0x41600000,
			 0x40800000,
			 0x41300000,
			 0x40400000,
			 0x40a00000,

			 /// vpu register f28
			 0x41b00000,
			 0x41a80000,
			 0x41b00000,
			 0x41800000,
			 0x40e00000,
			 0x41f80000,
			 0x41d80000,
			 0x41500000,

			 /// vpu register f29
			 0x41b00000,
			 0x41a00000,
			 0x41200000,
			 0x40a00000,
			 0x40800000,
			 0x41a80000,
			 0x41e80000,
			 0x41f00000,

			 /// vpu register f30
			 0x40400000,
			 0x41200000,
			 0x41a80000,
			 0x42000000,
			 0x40400000,
			 0x41700000,
			 0x41800000,
			 0x41c00000,

			 /// vpu register f31
			 0x41880000,
			 0x41b00000,
			 0x41b80000,
			 0x3f800000,
			 0x41500000,
			 0x41980000,
			 0x41e80000,
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
		"fswizz.ps f8, f7, 89\n"                              ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f4, f18, 86\n"                             ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f17, f9, 41\n"                             ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f16, f13, 78\n"                            ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f11, f28, 64\n"                            ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f26, f13, 113\n"                           ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f11, f16, 9\n"                             ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f4, f6, 54\n"                              ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f10, f18, 88\n"                            ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f24, f11, 47\n"                            ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f12, f6, 19\n"                             ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f6, f30, 86\n"                             ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f14, f5, 75\n"                             ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f26, f6, 102\n"                            ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f19, f17, 1\n"                             ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f11, f6, 45\n"                             ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f10, f2, 117\n"                            ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f5, f16, 0\n"                              ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f30, f12, 70\n"                            ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f10, f28, 76\n"                            ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f11, f10, 32\n"                            ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f16, f6, 121\n"                            ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f8, f7, 125\n"                             ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f20, f5, 85\n"                             ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f8, f3, 113\n"                             ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f23, f17, 0\n"                             ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f26, f7, 40\n"                             ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f25, f26, 37\n"                            ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f20, f2, 27\n"                             ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f25, f14, 76\n"                            ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f8, f26, 81\n"                             ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f2, f10, 93\n"                             ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f17, f5, 108\n"                            ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f26, f28, 8\n"                             ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f24, f30, 92\n"                            ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f23, f30, 80\n"                            ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f23, f9, 80\n"                             ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f15, f8, 67\n"                             ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f25, f24, 125\n"                           ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f7, f0, 7\n"                               ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f5, f12, 31\n"                             ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f8, f30, 77\n"                             ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f10, f7, 17\n"                             ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f29, f21, 116\n"                           ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f24, f26, 112\n"                           ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f27, f11, 32\n"                            ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f16, f0, 69\n"                             ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f6, f8, 36\n"                              ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f16, f23, 17\n"                            ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f3, f4, 106\n"                             ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f30, f18, 118\n"                           ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f3, f0, 17\n"                              ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f22, f27, 77\n"                            ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f8, f28, 92\n"                             ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f19, f21, 63\n"                            ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f27, f19, 126\n"                           ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f27, f8, 9\n"                              ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f22, f4, 90\n"                             ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f11, f15, 63\n"                            ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f9, f1, 94\n"                              ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f29, f0, 81\n"                             ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f24, f26, 97\n"                            ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f12, f15, 54\n"                            ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f19, f18, 18\n"                            ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f5, f10, 42\n"                             ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f25, f21, 19\n"                            ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f30, f25, 48\n"                            ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f2, f28, 124\n"                            ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f18, f27, 37\n"                            ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f12, f15, 37\n"                            ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f27, f5, 108\n"                            ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f29, f3, 90\n"                             ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f11, f27, 83\n"                            ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f4, f14, 3\n"                              ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f12, f10, 4\n"                             ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f16, f3, 52\n"                             ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f12, f27, 107\n"                           ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f25, f3, 33\n"                             ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f6, f22, 107\n"                            ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f23, f25, 108\n"                           ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f2, f10, 26\n"                             ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f15, f21, 60\n"                            ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f15, f5, 65\n"                             ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f8, f13, 60\n"                             ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f9, f20, 53\n"                             ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f20, f17, 63\n"                            ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f29, f6, 87\n"                             ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f3, f28, 103\n"                            ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f14, f23, 86\n"                            ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f21, f25, 103\n"                           ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f26, f13, 86\n"                            ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f27, f19, 33\n"                            ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f15, f28, 60\n"                            ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f2, f13, 71\n"                             ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f9, f23, 122\n"                            ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f14, f16, 69\n"                            ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f3, f0, 77\n"                              ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f27, f14, 11\n"                            ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fswizz.ps f21, f4, 73\n"                             ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
