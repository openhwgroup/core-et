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
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00000
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00004
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00008
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0000c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00010
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00014
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00018
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0001c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00020
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00024
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00028
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0002c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00030
			 0x3f028f5c,                                                  // 0.51                                        /// 00034
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00038
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0003c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00040
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00044
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00048
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0004c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00054
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00058
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0005c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00060
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00064
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00068
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0006c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00070
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00074
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00078
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0007c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00080
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00084
			 0x55555555,                                                  // 4 random values                             /// 00088
			 0x4b000000,                                                  // 8388608.0                                   /// 0008c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00090
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00098
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0009c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000a0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 000a4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 000ac
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000b0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000bc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000c4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 000c8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000cc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000d0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 000d4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0x33333333,                                                  // 4 random values                             /// 000e0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000e4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000ec
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000f0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000f4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0xbf028f5c,                                                  // -0.51                                       /// 00100
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00104
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00110
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00114
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00118
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0011c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00120
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00124
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00128
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00130
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00134
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0013c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00140
			 0xbf028f5c,                                                  // -0.51                                       /// 00144
			 0x0e000007,                                                  // Trailing 1s:                                /// 00148
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0014c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00150
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0xcb000000,                                                  // -8388608.0                                  /// 00158
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0015c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00160
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00164
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00168
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0016c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00170
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00174
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00178
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0017c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00184
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00188
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0018c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00190
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00194
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00198
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0019c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001a0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 001a4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001a8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001ac
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001b0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 001b4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 001b8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 001bc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 001c4
			 0x80011111,                                                  // -9.7958E-41                                 /// 001c8
			 0x80000000,                                                  // -zero                                       /// 001cc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 001d0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001dc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001e0
			 0x80000000,                                                  // -zero                                       /// 001e4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 001e8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 001ec
			 0x0e007fff,                                                  // Trailing 1s:                                /// 001f0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001f4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001f8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001fc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00200
			 0x80000000,                                                  // -zero                                       /// 00204
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00208
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0020c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00210
			 0x0c780000,                                                  // Leading 1s:                                 /// 00214
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00218
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0021c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00220
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00224
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00228
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0022c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00230
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00234
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00238
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0023c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00240
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0024c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00250
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00254
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00258
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0025c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00260
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00264
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00268
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0026c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00270
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00274
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00280
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00284
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00288
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0028c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00290
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00294
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0029c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 002a0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002a4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ac
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002b0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002b4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b8
			 0xcb000000,                                                  // -8388608.0                                  /// 002bc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002c0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 002c8
			 0xcb000000,                                                  // -8388608.0                                  /// 002cc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x33333333,                                                  // 4 random values                             /// 002d4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002dc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002e4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 002e8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 002f0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002f4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002f8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00300
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00304
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00308
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0030c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00310
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00318
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0031c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00320
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00324
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00328
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0032c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00334
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00338
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0033c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00340
			 0x0c780000,                                                  // Leading 1s:                                 /// 00344
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00348
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0034c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00354
			 0x0e000003,                                                  // Trailing 1s:                                /// 00358
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0035c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00360
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00364
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00368
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0036c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00374
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00378
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0037c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00380
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00384
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00388
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0038c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00390
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00398
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0039c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003a0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003a8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 003ac
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003b0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003b4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 003b8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003bc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003c0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003c4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 003c8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003cc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003d0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 003d4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 003d8
			 0xffc00001,                                                  // -signaling NaN                              /// 003dc
			 0x3f028f5c,                                                  // 0.51                                        /// 003e0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003e4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0x0e000001,                                                  // Trailing 1s:                                /// 003ec
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 003f4
			 0x00000000,                                                  // zero                                        /// 003f8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003fc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00400
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00404
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00408
			 0x0c600000,                                                  // Leading 1s:                                 /// 0040c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00410
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00414
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00418
			 0x0c400000,                                                  // Leading 1s:                                 /// 0041c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00420
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00428
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0042c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00430
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00438
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0043c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00440
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00444
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00448
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0044c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00454
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00458
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0045c
			 0x3f028f5c,                                                  // 0.51                                        /// 00460
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00464
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0046c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00470
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00474
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00478
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0047c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00484
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00488
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0048c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00498
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0049c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004a0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 004a4
			 0x33333333,                                                  // 4 random values                             /// 004a8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 004ac
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b0
			 0xff800000,                                                  // -inf                                        /// 004b4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004bc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 004c0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004c4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004c8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 004cc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004d0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004d4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 004d8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004dc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004e0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 004e4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 004e8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004ec
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 004f0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 004f4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004f8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004fc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00500
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00504
			 0x0c780000,                                                  // Leading 1s:                                 /// 00508
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0050c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00510
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00514
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00518
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0051c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00520
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00524
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00528
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0052c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00530
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00534
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00538
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0053c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00540
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00544
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00548
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0054c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00550
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00554
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00558
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0055c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00564
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00568
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0056c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00570
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00578
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0057c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00580
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00584
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00588
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0058c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00590
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00594
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00598
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0059c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005a0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005a8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0x80000000,                                                  // -zero                                       /// 005b0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005b8
			 0x7f800000,                                                  // inf                                         /// 005bc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005c0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005c4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 005c8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005cc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 005d0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 005dc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 005ec
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f0
			 0xffc00001,                                                  // -signaling NaN                              /// 005f4
			 0x55555555,                                                  // 4 random values                             /// 005f8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00600
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00604
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00608
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0060c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00610
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00614
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00618
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0061c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00620
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00624
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00628
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0062c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00630
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00638
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0063c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00640
			 0x80011111,                                                  // -9.7958E-41                                 /// 00644
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00648
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0064c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00650
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00654
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00658
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00660
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00664
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00668
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0066c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x3f028f5c,                                                  // 0.51                                        /// 00678
			 0x80000000,                                                  // -zero                                       /// 0067c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00680
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00684
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00688
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0068c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00690
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00694
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00698
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006a0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 006a4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006a8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 006ac
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006b0
			 0x0e000001,                                                  // Trailing 1s:                                /// 006b4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006b8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006bc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006c0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006c4
			 0xffc00001,                                                  // -signaling NaN                              /// 006c8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006cc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 006d0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006d8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 006dc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006e4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e8
			 0xbf028f5c,                                                  // -0.51                                       /// 006ec
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006f0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 006f4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 006f8
			 0x00011111,                                                  // 9.7958E-41                                  /// 006fc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00700
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00704
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0070c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00710
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00714
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00718
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0071c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00720
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00724
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00728
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0072c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00730
			 0xff800000,                                                  // -inf                                        /// 00734
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00738
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00748
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00750
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00754
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00758
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0075c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00764
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00768
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0076c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00770
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00774
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00778
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0077c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00780
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00784
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00788
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00790
			 0x0c700000,                                                  // Leading 1s:                                 /// 00794
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00798
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 007a0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007a4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0x55555555,                                                  // 4 random values                             /// 007ac
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007b0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007b4
			 0x80000000,                                                  // -zero                                       /// 007b8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 007bc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007c0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 007c4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 007c8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 007cc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007d0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 007d8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007dc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007e0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007e4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 007ec
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 007f4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007f8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007fc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00800
			 0x4b000000,                                                  // 8388608.0                                   /// 00804
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00808
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0080c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00810
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00814
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00818
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0081c
			 0x7fc00001,                                                  // signaling NaN                               /// 00820
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00824
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00828
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0082c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00830
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00834
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00838
			 0x0e000001,                                                  // Trailing 1s:                                /// 0083c
			 0xcb000000,                                                  // -8388608.0                                  /// 00840
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00848
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00850
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00858
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0085c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00860
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00864
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00868
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0086c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00874
			 0x00000000,                                                  // zero                                        /// 00878
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00880
			 0x0c780000,                                                  // Leading 1s:                                 /// 00884
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00888
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0088c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00890
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00894
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00898
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0089c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 008a0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008a4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008a8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008ac
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 008b0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008b8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 008bc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 008c0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008c8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 008cc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008d0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 008d8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008e0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 008e4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008e8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008ec
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008f0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008f8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00900
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00904
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00908
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00910
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00918
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0091c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00924
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00928
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0092c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00930
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00934
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00938
			 0x0c400000,                                                  // Leading 1s:                                 /// 0093c
			 0x33333333,                                                  // 4 random values                             /// 00940
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00944
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00948
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0094c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00950
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00954
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00958
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0095c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00960
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00964
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00968
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0096c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00970
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00974
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0097c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00980
			 0x4b000000,                                                  // 8388608.0                                   /// 00984
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00988
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0098c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00990
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0099c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009a8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 009b4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009b8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009bc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009c8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 009cc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009d0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009d4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009d8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009dc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009e4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009e8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ec
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 009f0
			 0x7fc00001,                                                  // signaling NaN                               /// 009f4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 009fc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a00
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a04
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a08
			 0x4b000000,                                                  // 8388608.0                                   /// 00a0c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a10
			 0xbf028f5c,                                                  // -0.51                                       /// 00a14
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a18
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a1c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a20
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a24
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a28
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0x55555555,                                                  // 4 random values                             /// 00a38
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a3c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a40
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a44
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a48
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a4c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a50
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a54
			 0x4b000000,                                                  // 8388608.0                                   /// 00a58
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a5c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a60
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a64
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a68
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a6c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a70
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a74
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a78
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a7c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a80
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a84
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a88
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a8c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a90
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a94
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a98
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a9c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00aa0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00aa4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00aa8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aac
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ab4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ab8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00abc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ac4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ac8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00acc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ad0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ad4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00adc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ae0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ae8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00aec
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00af0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00afc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b00
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b04
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b08
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b0c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b10
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b14
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b18
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b1c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b20
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b24
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b28
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b2c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b30
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b38
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b3c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b40
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b44
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b48
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b4c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b50
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b54
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b58
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b5c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b64
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b68
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b6c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b74
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b78
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b7c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b88
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b8c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b90
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b94
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ba0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ba4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ba8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00bac
			 0x0e000003,                                                  // Trailing 1s:                                /// 00bb0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00bb8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00bc0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00bc4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00bc8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00bcc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00bd0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00bd8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bdc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00be0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00be4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00be8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00bec
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bf4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00bfc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c00
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c04
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c08
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c0c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c10
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c1c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c20
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c24
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c28
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c2c
			 0x55555555,                                                  // 4 random values                             /// 00c30
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c34
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c38
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c3c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c40
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c44
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c48
			 0x7f800000,                                                  // inf                                         /// 00c4c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c50
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c54
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c58
			 0x55555555,                                                  // 4 random values                             /// 00c5c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c60
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c64
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c6c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c70
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c74
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c78
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c7c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c84
			 0xcb000000,                                                  // -8388608.0                                  /// 00c88
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c8c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c90
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c94
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c9c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ca0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ca4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ca8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00cac
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00cb0
			 0x4b000000,                                                  // 8388608.0                                   /// 00cb4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00cb8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cbc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00cc0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00cc4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00cc8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ccc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00cd0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cd4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00cdc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ce0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce4
			 0xffc00001,                                                  // -signaling NaN                              /// 00ce8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00cf8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00cfc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d00
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d04
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d08
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d0c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d10
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d14
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d1c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d24
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d28
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d2c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d30
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d34
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d38
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d3c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d40
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d44
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d48
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d4c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d50
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d58
			 0xffc00001,                                                  // -signaling NaN                              /// 00d5c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d60
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d64
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d68
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d6c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d74
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d7c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d80
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d84
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d88
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d8c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d90
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d94
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d98
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d9c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00da4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00da8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00dac
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00db0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00db8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00dbc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00dc0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00dc4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00dc8
			 0x7fc00001,                                                  // signaling NaN                               /// 00dcc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00dd0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00dd4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ddc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00de0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00de4
			 0xbf028f5c,                                                  // -0.51                                       /// 00de8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00df0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00df4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00df8
			 0x55555555,                                                  // 4 random values                             /// 00dfc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e00
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e04
			 0x4b000000,                                                  // 8388608.0                                   /// 00e08
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e0c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e10
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e14
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e18
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e1c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e20
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e24
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e28
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e2c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e30
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e34
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e38
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e3c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e40
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e44
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0xcb000000,                                                  // -8388608.0                                  /// 00e4c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e50
			 0x33333333,                                                  // 4 random values                             /// 00e54
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e5c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e64
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e6c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e70
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e74
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e78
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e80
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e84
			 0x55555555,                                                  // 4 random values                             /// 00e88
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e8c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e90
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e94
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e9c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ea0
			 0x80000000,                                                  // -zero                                       /// 00ea4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ea8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00eb0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00eb4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00eb8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ebc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ec0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ec4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ec8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ed0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ed4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ed8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00edc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ee0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ee4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ee8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eec
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ef0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ef4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ef8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00efc
			 0x4b000000,                                                  // 8388608.0                                   /// 00f00
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f04
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f08
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f0c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f10
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f14
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f20
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f24
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f2c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f30
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f40
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f44
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f48
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f4c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f50
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f54
			 0x80000000,                                                  // -zero                                       /// 00f58
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f5c
			 0x33333333,                                                  // 4 random values                             /// 00f60
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f68
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f6c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f74
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f78
			 0x80000000,                                                  // -zero                                       /// 00f7c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f80
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f84
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f88
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f8c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f90
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f94
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f98
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f9c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00fa0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00fa4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00fa8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00fac
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fb0
			 0x80000000,                                                  // -zero                                       /// 00fb4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00fbc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00fc0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00fcc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00fd0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00fd4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00fdc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fe0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fe4
			 0x00000000,                                                  // zero                                        /// 00fe8
			 0x3f028f5c,                                                  // 0.51                                        /// 00fec
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ff0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ff4
			 0x55555555,                                                  // 4 random values                             /// 00ff8
			 0x807fffff // max subnorm                                   // SP - ve numbers                             /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x6475f11c, /// 0x0
			 0x2030a540, /// 0x4
			 0xd991ab96, /// 0x8
			 0x6cafb496, /// 0xc
			 0x8556a0a0, /// 0x10
			 0xcc12a6fb, /// 0x14
			 0x55d6eec4, /// 0x18
			 0x97b3f574, /// 0x1c
			 0x4fea4456, /// 0x20
			 0xcc07e14b, /// 0x24
			 0x9936b14c, /// 0x28
			 0xe463a0d2, /// 0x2c
			 0x280bc9ae, /// 0x30
			 0x2172f69e, /// 0x34
			 0xb123bc93, /// 0x38
			 0xd81b92e6, /// 0x3c
			 0x2780db05, /// 0x40
			 0xeacd0457, /// 0x44
			 0x67c016aa, /// 0x48
			 0x58abcc91, /// 0x4c
			 0xc65779a4, /// 0x50
			 0x15441101, /// 0x54
			 0x676793ad, /// 0x58
			 0x2783bf0d, /// 0x5c
			 0x6c999a5d, /// 0x60
			 0xa4310afd, /// 0x64
			 0x9f96ea1a, /// 0x68
			 0x21437428, /// 0x6c
			 0x8635dfb9, /// 0x70
			 0xb40e61b2, /// 0x74
			 0x5d555222, /// 0x78
			 0x4cd1edf9, /// 0x7c
			 0xdf2558f5, /// 0x80
			 0x7ef522b5, /// 0x84
			 0x16d7f352, /// 0x88
			 0x8939bb1e, /// 0x8c
			 0x4c966372, /// 0x90
			 0x81f147f3, /// 0x94
			 0x30a9834d, /// 0x98
			 0x8eb8218f, /// 0x9c
			 0xfe022dd4, /// 0xa0
			 0xd8bb1125, /// 0xa4
			 0x520f3004, /// 0xa8
			 0xfe29b23f, /// 0xac
			 0xbd9c9450, /// 0xb0
			 0x8f184258, /// 0xb4
			 0x0b1b2d92, /// 0xb8
			 0xf65a9c4d, /// 0xbc
			 0x1cae78bd, /// 0xc0
			 0x55176563, /// 0xc4
			 0xbf6adfc9, /// 0xc8
			 0x84a90f37, /// 0xcc
			 0x4c24dccb, /// 0xd0
			 0x8c8c98f8, /// 0xd4
			 0x441e0ec5, /// 0xd8
			 0xf0779e8b, /// 0xdc
			 0x56a98ad0, /// 0xe0
			 0x023bf8b8, /// 0xe4
			 0x63cce123, /// 0xe8
			 0xdae4d19e, /// 0xec
			 0x5c45e297, /// 0xf0
			 0xe6b1af67, /// 0xf4
			 0xc2f1fecf, /// 0xf8
			 0x49802980, /// 0xfc
			 0x8d7f2fb3, /// 0x100
			 0x6a986670, /// 0x104
			 0x9d39eed0, /// 0x108
			 0xcb30d696, /// 0x10c
			 0xa923f731, /// 0x110
			 0xaa104453, /// 0x114
			 0xbd5de7b5, /// 0x118
			 0x46461f93, /// 0x11c
			 0xbb68a25f, /// 0x120
			 0x994e16c4, /// 0x124
			 0x674bf40c, /// 0x128
			 0x6cace579, /// 0x12c
			 0x5ba91106, /// 0x130
			 0x0103e186, /// 0x134
			 0xa41319a0, /// 0x138
			 0x2973bec7, /// 0x13c
			 0x054633d2, /// 0x140
			 0x6b71b6ac, /// 0x144
			 0x2cd6ea12, /// 0x148
			 0xc46b1d3b, /// 0x14c
			 0x5734f53f, /// 0x150
			 0xd0407adc, /// 0x154
			 0xdee80bf4, /// 0x158
			 0xba895a91, /// 0x15c
			 0xf25fab0c, /// 0x160
			 0x2a2625ac, /// 0x164
			 0x6be45609, /// 0x168
			 0xa2a64592, /// 0x16c
			 0x1a475a55, /// 0x170
			 0x76874bfa, /// 0x174
			 0xd68a4bfd, /// 0x178
			 0xafc76324, /// 0x17c
			 0x071d4e5b, /// 0x180
			 0x68d2dba1, /// 0x184
			 0xeccbfe84, /// 0x188
			 0x0a00c4e6, /// 0x18c
			 0x5f9a9bc9, /// 0x190
			 0xb673fa8d, /// 0x194
			 0xfacd6483, /// 0x198
			 0x41cd827e, /// 0x19c
			 0xfbfa9054, /// 0x1a0
			 0x81f72ec5, /// 0x1a4
			 0xf47aa5d6, /// 0x1a8
			 0xb1da2aa9, /// 0x1ac
			 0x7a123850, /// 0x1b0
			 0xb424f9ba, /// 0x1b4
			 0x993ceb2c, /// 0x1b8
			 0x9efa7deb, /// 0x1bc
			 0xeaafc6c7, /// 0x1c0
			 0x94b694e6, /// 0x1c4
			 0xd22d0718, /// 0x1c8
			 0x1cd6f55c, /// 0x1cc
			 0x5b25dc7c, /// 0x1d0
			 0x986a052e, /// 0x1d4
			 0x4d249ac1, /// 0x1d8
			 0x41faf813, /// 0x1dc
			 0xcde09d24, /// 0x1e0
			 0x973cdfe9, /// 0x1e4
			 0x408316c7, /// 0x1e8
			 0x5f2b9477, /// 0x1ec
			 0x5e5cb329, /// 0x1f0
			 0x54a5183e, /// 0x1f4
			 0x25ce1dac, /// 0x1f8
			 0x8f7f3221, /// 0x1fc
			 0x3365854a, /// 0x200
			 0xc8c3ed6c, /// 0x204
			 0x53fa5226, /// 0x208
			 0x4cddd30e, /// 0x20c
			 0x256f25ed, /// 0x210
			 0xbae45d4b, /// 0x214
			 0xde638adc, /// 0x218
			 0x12aed7e4, /// 0x21c
			 0xe58b72a4, /// 0x220
			 0xd72baebf, /// 0x224
			 0x3e1a9c18, /// 0x228
			 0x45f6458d, /// 0x22c
			 0xb83acdc0, /// 0x230
			 0xf1998508, /// 0x234
			 0xe1fc7452, /// 0x238
			 0x61953a7e, /// 0x23c
			 0x13d7a7aa, /// 0x240
			 0x93dd8833, /// 0x244
			 0x8a13865e, /// 0x248
			 0x86fee6e5, /// 0x24c
			 0xd0fe98d1, /// 0x250
			 0x57ea8172, /// 0x254
			 0xdd7c7056, /// 0x258
			 0xc136fae7, /// 0x25c
			 0x27066f7a, /// 0x260
			 0x26518815, /// 0x264
			 0x60c94ded, /// 0x268
			 0x8a8a42af, /// 0x26c
			 0xf078d3e0, /// 0x270
			 0x2aa601a7, /// 0x274
			 0xaffc994b, /// 0x278
			 0x9434d624, /// 0x27c
			 0x79b2a358, /// 0x280
			 0xd16f2558, /// 0x284
			 0x2614d20e, /// 0x288
			 0x60902683, /// 0x28c
			 0x4cb11e30, /// 0x290
			 0xed18bbb7, /// 0x294
			 0x4263259d, /// 0x298
			 0xa0600b3a, /// 0x29c
			 0x5b82e3c0, /// 0x2a0
			 0xa9b55b3c, /// 0x2a4
			 0x93b44fc1, /// 0x2a8
			 0xcf9574c3, /// 0x2ac
			 0xdbd64a41, /// 0x2b0
			 0xbd9e6fee, /// 0x2b4
			 0xfc94ec5c, /// 0x2b8
			 0x097b111e, /// 0x2bc
			 0x126dc5ef, /// 0x2c0
			 0x9350ef76, /// 0x2c4
			 0x8b249f87, /// 0x2c8
			 0x443de579, /// 0x2cc
			 0xcc1a9978, /// 0x2d0
			 0xa2213254, /// 0x2d4
			 0x62d61708, /// 0x2d8
			 0x5f1a2df6, /// 0x2dc
			 0x02115316, /// 0x2e0
			 0xaae72b2e, /// 0x2e4
			 0x9481af84, /// 0x2e8
			 0xd8716b46, /// 0x2ec
			 0xd4c62005, /// 0x2f0
			 0x94eeeb23, /// 0x2f4
			 0x8b02c880, /// 0x2f8
			 0x8d37174f, /// 0x2fc
			 0xfc4b2319, /// 0x300
			 0x661d5d2e, /// 0x304
			 0xabc1ebef, /// 0x308
			 0x9defd701, /// 0x30c
			 0x50cfcd4b, /// 0x310
			 0xe842ca76, /// 0x314
			 0x50b6bf97, /// 0x318
			 0x53b53cd7, /// 0x31c
			 0xe3c9cd52, /// 0x320
			 0xfe362ab2, /// 0x324
			 0x4861ec2b, /// 0x328
			 0xfb98b129, /// 0x32c
			 0x8e3922a2, /// 0x330
			 0x88586282, /// 0x334
			 0x816a6c05, /// 0x338
			 0x0ca7c2ff, /// 0x33c
			 0x0682b43e, /// 0x340
			 0x69c9e6ba, /// 0x344
			 0xb52a6e0b, /// 0x348
			 0xfad7521f, /// 0x34c
			 0x7d16f796, /// 0x350
			 0x8fd97c10, /// 0x354
			 0x37026f0a, /// 0x358
			 0x1838d6c1, /// 0x35c
			 0x44f6985b, /// 0x360
			 0xc928d263, /// 0x364
			 0x7887c378, /// 0x368
			 0xb3e9bed5, /// 0x36c
			 0xe7df8f7b, /// 0x370
			 0xf8ae552f, /// 0x374
			 0x3256d2e9, /// 0x378
			 0x5ff8f2a6, /// 0x37c
			 0xe8d6a117, /// 0x380
			 0xaad63938, /// 0x384
			 0xdef60cb3, /// 0x388
			 0xc0ec68e8, /// 0x38c
			 0x53efbf96, /// 0x390
			 0x7679cfc1, /// 0x394
			 0xd1f36265, /// 0x398
			 0x1250cf23, /// 0x39c
			 0x3ec7db29, /// 0x3a0
			 0x4a548774, /// 0x3a4
			 0x2402329c, /// 0x3a8
			 0x3955a2f5, /// 0x3ac
			 0xd73e9f81, /// 0x3b0
			 0x1549c85d, /// 0x3b4
			 0x81e5e0d2, /// 0x3b8
			 0x9436bda0, /// 0x3bc
			 0xb2c83368, /// 0x3c0
			 0x7f1fed87, /// 0x3c4
			 0x0a05d156, /// 0x3c8
			 0xd160bdd6, /// 0x3cc
			 0xe7803853, /// 0x3d0
			 0x657351a4, /// 0x3d4
			 0x0e2be2aa, /// 0x3d8
			 0xdbe27ea3, /// 0x3dc
			 0x5775a007, /// 0x3e0
			 0x0954d119, /// 0x3e4
			 0xe757213f, /// 0x3e8
			 0x5d9abb71, /// 0x3ec
			 0x1aa57eff, /// 0x3f0
			 0x4fe5add5, /// 0x3f4
			 0xaed8e40f, /// 0x3f8
			 0xa325b6c9, /// 0x3fc
			 0x307adf2e, /// 0x400
			 0x05eea954, /// 0x404
			 0xbf05290d, /// 0x408
			 0xd770f9c6, /// 0x40c
			 0xbb6ce402, /// 0x410
			 0xe55be5c4, /// 0x414
			 0xe38c5893, /// 0x418
			 0x79aab0e5, /// 0x41c
			 0x21206fb0, /// 0x420
			 0x6a8231d0, /// 0x424
			 0x2336c75a, /// 0x428
			 0x60bb0afe, /// 0x42c
			 0x15a73eb9, /// 0x430
			 0xd8081103, /// 0x434
			 0x528d9458, /// 0x438
			 0xc2346d20, /// 0x43c
			 0x73cb739e, /// 0x440
			 0x4e9dd7a9, /// 0x444
			 0x42900728, /// 0x448
			 0x8ee4a090, /// 0x44c
			 0x7cfda14f, /// 0x450
			 0xbee5b444, /// 0x454
			 0xe290bc27, /// 0x458
			 0x0e542f58, /// 0x45c
			 0x79336a42, /// 0x460
			 0x150529ab, /// 0x464
			 0x5bde11ef, /// 0x468
			 0xb3cdac1a, /// 0x46c
			 0x3bc3afab, /// 0x470
			 0x22b74800, /// 0x474
			 0x9b955d13, /// 0x478
			 0xacfbe5f4, /// 0x47c
			 0x99d6addb, /// 0x480
			 0x025b8cb7, /// 0x484
			 0xcdf24316, /// 0x488
			 0x80c3d940, /// 0x48c
			 0x94974827, /// 0x490
			 0xd2313d19, /// 0x494
			 0x5d02a3c8, /// 0x498
			 0xe5b5fe85, /// 0x49c
			 0xc7b8abed, /// 0x4a0
			 0x6c1f9247, /// 0x4a4
			 0xf57eefca, /// 0x4a8
			 0x9fc939de, /// 0x4ac
			 0x3e577423, /// 0x4b0
			 0xcb823c58, /// 0x4b4
			 0x22c3a159, /// 0x4b8
			 0x3759b68a, /// 0x4bc
			 0xe0d31b78, /// 0x4c0
			 0xfe7df469, /// 0x4c4
			 0x413dacf0, /// 0x4c8
			 0x9ff74f92, /// 0x4cc
			 0x2bf515a3, /// 0x4d0
			 0x4ec390ff, /// 0x4d4
			 0xe5727cb5, /// 0x4d8
			 0xc5fa0a6f, /// 0x4dc
			 0x1581c36e, /// 0x4e0
			 0x777bea99, /// 0x4e4
			 0x6bc1fab5, /// 0x4e8
			 0x53165ace, /// 0x4ec
			 0xaecf4f5c, /// 0x4f0
			 0x190aff00, /// 0x4f4
			 0x2469cbc5, /// 0x4f8
			 0x0e6aaea6, /// 0x4fc
			 0x6e5b7dd6, /// 0x500
			 0x284cb3fc, /// 0x504
			 0xde156f78, /// 0x508
			 0x7d34ddf8, /// 0x50c
			 0xe28eef6a, /// 0x510
			 0x238c2f74, /// 0x514
			 0x00b05927, /// 0x518
			 0x7522d658, /// 0x51c
			 0xb9aad831, /// 0x520
			 0x5bfc42cf, /// 0x524
			 0x590be03d, /// 0x528
			 0xdd9ebcb9, /// 0x52c
			 0x355fafca, /// 0x530
			 0x0df742a1, /// 0x534
			 0xa2682338, /// 0x538
			 0x16bd1a86, /// 0x53c
			 0x8f18c4e4, /// 0x540
			 0x36f8f1bc, /// 0x544
			 0xc1d3304c, /// 0x548
			 0x70903be8, /// 0x54c
			 0x72cb1f84, /// 0x550
			 0x68ef2e7a, /// 0x554
			 0x419ff494, /// 0x558
			 0x1928e537, /// 0x55c
			 0x84e753d3, /// 0x560
			 0xa057bea6, /// 0x564
			 0xa26f32b1, /// 0x568
			 0x8fac07e2, /// 0x56c
			 0x34c03703, /// 0x570
			 0x858798b7, /// 0x574
			 0xb1ea6dc0, /// 0x578
			 0x58505fe3, /// 0x57c
			 0xee8db259, /// 0x580
			 0x5d6ab728, /// 0x584
			 0x450d2ba4, /// 0x588
			 0xeb209d0e, /// 0x58c
			 0x33ac2410, /// 0x590
			 0x3bad0b1c, /// 0x594
			 0xea6131ee, /// 0x598
			 0x3088ef6e, /// 0x59c
			 0x37310b35, /// 0x5a0
			 0xeac19929, /// 0x5a4
			 0xc8d2051e, /// 0x5a8
			 0x7c7ce27f, /// 0x5ac
			 0x957d3b80, /// 0x5b0
			 0x4e9c885d, /// 0x5b4
			 0x62f42931, /// 0x5b8
			 0x95a38abb, /// 0x5bc
			 0x79f59a83, /// 0x5c0
			 0xc787d721, /// 0x5c4
			 0x7c487c23, /// 0x5c8
			 0x2364b8a3, /// 0x5cc
			 0x1c709619, /// 0x5d0
			 0x0010c988, /// 0x5d4
			 0x6177afb2, /// 0x5d8
			 0x53ca7948, /// 0x5dc
			 0xa45835b7, /// 0x5e0
			 0x2e915985, /// 0x5e4
			 0xa0816461, /// 0x5e8
			 0x12ae5527, /// 0x5ec
			 0xe1de3e2c, /// 0x5f0
			 0x9cbfc4ee, /// 0x5f4
			 0x665fd5f5, /// 0x5f8
			 0xde93f74d, /// 0x5fc
			 0xdecb9b83, /// 0x600
			 0x7f1a1c24, /// 0x604
			 0x2072a169, /// 0x608
			 0x2cf64da4, /// 0x60c
			 0x1dc6f8a8, /// 0x610
			 0x370de9b6, /// 0x614
			 0x06b92607, /// 0x618
			 0xd753ddfc, /// 0x61c
			 0x85884cac, /// 0x620
			 0x96621880, /// 0x624
			 0xd69db93c, /// 0x628
			 0x0bee7574, /// 0x62c
			 0x49f0321f, /// 0x630
			 0x686f28d7, /// 0x634
			 0x9c49f3f9, /// 0x638
			 0x41838b6c, /// 0x63c
			 0x79d3a37a, /// 0x640
			 0x07105ca3, /// 0x644
			 0x9f913ebe, /// 0x648
			 0x12afdcce, /// 0x64c
			 0x5f07146f, /// 0x650
			 0x0865988d, /// 0x654
			 0xb99762cd, /// 0x658
			 0x933052a8, /// 0x65c
			 0xd96a19b7, /// 0x660
			 0x7e041fa7, /// 0x664
			 0xc71a7d79, /// 0x668
			 0x9c7ae5c6, /// 0x66c
			 0x771b4e47, /// 0x670
			 0xc2db0656, /// 0x674
			 0x0ac25472, /// 0x678
			 0xfe57333c, /// 0x67c
			 0x7d1c366c, /// 0x680
			 0x36150015, /// 0x684
			 0x2768fd5b, /// 0x688
			 0x69fce24e, /// 0x68c
			 0x2085eac0, /// 0x690
			 0xb61fd239, /// 0x694
			 0x887f4171, /// 0x698
			 0xeeb9f58a, /// 0x69c
			 0x5e5d500d, /// 0x6a0
			 0x8cb64481, /// 0x6a4
			 0xea147cec, /// 0x6a8
			 0xb5146bb1, /// 0x6ac
			 0xf288f923, /// 0x6b0
			 0xdf02d33c, /// 0x6b4
			 0x0105773f, /// 0x6b8
			 0x0b1394f7, /// 0x6bc
			 0x34607e99, /// 0x6c0
			 0xa43adf53, /// 0x6c4
			 0x4222e8f0, /// 0x6c8
			 0xe3e0bf60, /// 0x6cc
			 0x21be527a, /// 0x6d0
			 0xfe58b140, /// 0x6d4
			 0x98610243, /// 0x6d8
			 0xb1c1a2ab, /// 0x6dc
			 0xbd8194f2, /// 0x6e0
			 0xe3298e76, /// 0x6e4
			 0x962ca363, /// 0x6e8
			 0xeac53327, /// 0x6ec
			 0x8390649b, /// 0x6f0
			 0xf2b1e6e6, /// 0x6f4
			 0xa14ac0b4, /// 0x6f8
			 0x1d45bc0c, /// 0x6fc
			 0x09b14d3b, /// 0x700
			 0x127abd5e, /// 0x704
			 0x5b3e5d0f, /// 0x708
			 0x569f8e9f, /// 0x70c
			 0xa3417af0, /// 0x710
			 0xfe39261c, /// 0x714
			 0x430a41b4, /// 0x718
			 0x5d20f6c9, /// 0x71c
			 0x635d79ee, /// 0x720
			 0x9418fd9b, /// 0x724
			 0x439d5787, /// 0x728
			 0x5921b7c2, /// 0x72c
			 0xfd0b93c6, /// 0x730
			 0x1a4ada6e, /// 0x734
			 0xfd496bdb, /// 0x738
			 0x9db2eae9, /// 0x73c
			 0xd968e56b, /// 0x740
			 0xd6540c0b, /// 0x744
			 0x377815de, /// 0x748
			 0xc9871e57, /// 0x74c
			 0xd479748b, /// 0x750
			 0x1b982676, /// 0x754
			 0x6ac71308, /// 0x758
			 0x28652f33, /// 0x75c
			 0x8ed48c26, /// 0x760
			 0xb5bd81c3, /// 0x764
			 0xf5a66334, /// 0x768
			 0x6a70c797, /// 0x76c
			 0xb570a4a6, /// 0x770
			 0xa2d63eca, /// 0x774
			 0x8d484ce1, /// 0x778
			 0x652ab381, /// 0x77c
			 0xfaad58b4, /// 0x780
			 0xc0d62512, /// 0x784
			 0x5fa96e22, /// 0x788
			 0x9795f5ee, /// 0x78c
			 0xa246d76a, /// 0x790
			 0x46481559, /// 0x794
			 0xa0514402, /// 0x798
			 0xd2e62e29, /// 0x79c
			 0xb9b17cc2, /// 0x7a0
			 0x29ea88ff, /// 0x7a4
			 0x05ce7ee8, /// 0x7a8
			 0xe9d18fb1, /// 0x7ac
			 0x5014a44f, /// 0x7b0
			 0x94fce679, /// 0x7b4
			 0x2a69aede, /// 0x7b8
			 0x984c13a5, /// 0x7bc
			 0x96816f26, /// 0x7c0
			 0xdbb75371, /// 0x7c4
			 0xc7f79bcf, /// 0x7c8
			 0x2fc7b387, /// 0x7cc
			 0x5cf3e50d, /// 0x7d0
			 0xe2096c48, /// 0x7d4
			 0x5b389cb6, /// 0x7d8
			 0x34323ab1, /// 0x7dc
			 0xeed6db24, /// 0x7e0
			 0xda0d9717, /// 0x7e4
			 0xd70fdf07, /// 0x7e8
			 0x18d131ac, /// 0x7ec
			 0x344a3fc0, /// 0x7f0
			 0x25ad1f8d, /// 0x7f4
			 0x7d724ce3, /// 0x7f8
			 0xa6c53446, /// 0x7fc
			 0xba6951ce, /// 0x800
			 0xff89d821, /// 0x804
			 0x034c7ac9, /// 0x808
			 0x671e2e89, /// 0x80c
			 0x16eee5c3, /// 0x810
			 0x9968b22b, /// 0x814
			 0xef4e4146, /// 0x818
			 0x237ae19e, /// 0x81c
			 0xe9c74540, /// 0x820
			 0x4ab90568, /// 0x824
			 0xb0c33ea4, /// 0x828
			 0x08ef11fe, /// 0x82c
			 0x68cb7ab4, /// 0x830
			 0x9deccb52, /// 0x834
			 0xfe253e1c, /// 0x838
			 0x7bd2bdb0, /// 0x83c
			 0xd932f80c, /// 0x840
			 0x0c4d8aef, /// 0x844
			 0x154b2e04, /// 0x848
			 0xc3938a36, /// 0x84c
			 0xd023ff9a, /// 0x850
			 0x6ad435f0, /// 0x854
			 0x0a54b986, /// 0x858
			 0xfc2330d4, /// 0x85c
			 0x4e298d0f, /// 0x860
			 0x69fde56a, /// 0x864
			 0xecb6eeaa, /// 0x868
			 0x6c59cb9b, /// 0x86c
			 0xdbad4fb3, /// 0x870
			 0x5d556df5, /// 0x874
			 0xd3ab4c9d, /// 0x878
			 0xdd6bdedd, /// 0x87c
			 0x0eba10c9, /// 0x880
			 0x46bc083d, /// 0x884
			 0x0fb4bbba, /// 0x888
			 0x5b184edd, /// 0x88c
			 0xf6503c34, /// 0x890
			 0x28f932f5, /// 0x894
			 0xcfc31051, /// 0x898
			 0x62729781, /// 0x89c
			 0x1b7e36c9, /// 0x8a0
			 0xdc148ea8, /// 0x8a4
			 0x0172b515, /// 0x8a8
			 0xd9c32c27, /// 0x8ac
			 0xf44ffe27, /// 0x8b0
			 0x9c62a869, /// 0x8b4
			 0x11e09ab7, /// 0x8b8
			 0x46711e3d, /// 0x8bc
			 0xd20fea88, /// 0x8c0
			 0x00de80e3, /// 0x8c4
			 0x5e281d8d, /// 0x8c8
			 0x64ed705d, /// 0x8cc
			 0x16dc24ec, /// 0x8d0
			 0x690eba8c, /// 0x8d4
			 0xf58ecb2c, /// 0x8d8
			 0x0d0bd8a5, /// 0x8dc
			 0x812f91d0, /// 0x8e0
			 0x50098500, /// 0x8e4
			 0x7efcc383, /// 0x8e8
			 0xddd518fb, /// 0x8ec
			 0x9c09ba11, /// 0x8f0
			 0x01ce4fd8, /// 0x8f4
			 0xcae1338a, /// 0x8f8
			 0x934f052c, /// 0x8fc
			 0x4290cfa5, /// 0x900
			 0x18f46fa6, /// 0x904
			 0x0b15d4aa, /// 0x908
			 0x345c66da, /// 0x90c
			 0xd57d9d32, /// 0x910
			 0x28ff425c, /// 0x914
			 0xc26be245, /// 0x918
			 0xa33e31ab, /// 0x91c
			 0x336fa886, /// 0x920
			 0x7d70dad3, /// 0x924
			 0x32173530, /// 0x928
			 0xaa4fdcfc, /// 0x92c
			 0xb52804b5, /// 0x930
			 0x8d3c1d81, /// 0x934
			 0x3105670a, /// 0x938
			 0x9c852461, /// 0x93c
			 0x343876dc, /// 0x940
			 0xabd9aa8d, /// 0x944
			 0xe0243f82, /// 0x948
			 0x281e75e4, /// 0x94c
			 0xfcdae6b4, /// 0x950
			 0xeef45d7b, /// 0x954
			 0x8343abe1, /// 0x958
			 0x5255efd7, /// 0x95c
			 0x22c58b62, /// 0x960
			 0x69c8dd66, /// 0x964
			 0xbe37d333, /// 0x968
			 0xd51d0b3c, /// 0x96c
			 0x150af3b5, /// 0x970
			 0xaf925b03, /// 0x974
			 0xa7a6b937, /// 0x978
			 0x796dfbcd, /// 0x97c
			 0x2f30a610, /// 0x980
			 0xc9c2db1b, /// 0x984
			 0xf1851bdd, /// 0x988
			 0xeceb419f, /// 0x98c
			 0xaa8836f6, /// 0x990
			 0xea4d5236, /// 0x994
			 0xa70b02d4, /// 0x998
			 0x7b1077e5, /// 0x99c
			 0x36947a6d, /// 0x9a0
			 0xebf1351f, /// 0x9a4
			 0x94d39b9e, /// 0x9a8
			 0xc0c8c3ba, /// 0x9ac
			 0x6342a4c4, /// 0x9b0
			 0x13e86d30, /// 0x9b4
			 0xf7c304e2, /// 0x9b8
			 0xd835198f, /// 0x9bc
			 0xa88e2556, /// 0x9c0
			 0x0dcca40c, /// 0x9c4
			 0x9a7ef4ec, /// 0x9c8
			 0x9a098de2, /// 0x9cc
			 0xa606bc9d, /// 0x9d0
			 0xba33dfeb, /// 0x9d4
			 0x4c1c39d3, /// 0x9d8
			 0xcf27d8ae, /// 0x9dc
			 0x56909dfc, /// 0x9e0
			 0xf2c1e448, /// 0x9e4
			 0x9d9e6554, /// 0x9e8
			 0x53f27d5b, /// 0x9ec
			 0x43118811, /// 0x9f0
			 0xdffce4d3, /// 0x9f4
			 0x0f54d9f3, /// 0x9f8
			 0xd78217ac, /// 0x9fc
			 0xf1bd402b, /// 0xa00
			 0xa62ad49d, /// 0xa04
			 0x56e40300, /// 0xa08
			 0xdb27f2b6, /// 0xa0c
			 0x970f73ed, /// 0xa10
			 0x3059b0ff, /// 0xa14
			 0xc9580bd1, /// 0xa18
			 0x607a3255, /// 0xa1c
			 0x7df44ca5, /// 0xa20
			 0x9663f38d, /// 0xa24
			 0x5484a2af, /// 0xa28
			 0x30c85a87, /// 0xa2c
			 0x4b153d77, /// 0xa30
			 0x48aee00e, /// 0xa34
			 0x2c781286, /// 0xa38
			 0xd5a2f7cb, /// 0xa3c
			 0xf5d7fa37, /// 0xa40
			 0xb4bda795, /// 0xa44
			 0x17b2a058, /// 0xa48
			 0x67000153, /// 0xa4c
			 0xdc787ff6, /// 0xa50
			 0x25a815ff, /// 0xa54
			 0xae947ae2, /// 0xa58
			 0xd9612735, /// 0xa5c
			 0x0edd460a, /// 0xa60
			 0xf38c4822, /// 0xa64
			 0x3c0f6e85, /// 0xa68
			 0xb3982869, /// 0xa6c
			 0xad63a2b7, /// 0xa70
			 0x1bd1b996, /// 0xa74
			 0x174fae10, /// 0xa78
			 0x6822deda, /// 0xa7c
			 0x4e0738a9, /// 0xa80
			 0x44e6cd1a, /// 0xa84
			 0xa9b48931, /// 0xa88
			 0xacf3c0df, /// 0xa8c
			 0xf5d26807, /// 0xa90
			 0xf64d706c, /// 0xa94
			 0x5ba3871e, /// 0xa98
			 0xc19f7b9e, /// 0xa9c
			 0xf89b71fb, /// 0xaa0
			 0xc5eeb25b, /// 0xaa4
			 0xcc5c4b34, /// 0xaa8
			 0x3cab863a, /// 0xaac
			 0xfff3a151, /// 0xab0
			 0x222d7c9f, /// 0xab4
			 0x3a2dc248, /// 0xab8
			 0xbe9887a1, /// 0xabc
			 0x87038b44, /// 0xac0
			 0xe24d2996, /// 0xac4
			 0xb58bc7b0, /// 0xac8
			 0xa8e819d4, /// 0xacc
			 0x896f8d3a, /// 0xad0
			 0xf999c4d3, /// 0xad4
			 0x7bd921d4, /// 0xad8
			 0x9b1ceb97, /// 0xadc
			 0xfbd1b956, /// 0xae0
			 0x2bcd7f2a, /// 0xae4
			 0x42e1799b, /// 0xae8
			 0xbb537ddb, /// 0xaec
			 0x3c07101f, /// 0xaf0
			 0x94a44b23, /// 0xaf4
			 0x18c0a806, /// 0xaf8
			 0x85e09b4f, /// 0xafc
			 0xcdfbaa4e, /// 0xb00
			 0xe37cc5c6, /// 0xb04
			 0x0e37b814, /// 0xb08
			 0xf09f6b44, /// 0xb0c
			 0xb6473fa5, /// 0xb10
			 0x352c66ba, /// 0xb14
			 0xaded122b, /// 0xb18
			 0x75efbbb5, /// 0xb1c
			 0xb9d8dc62, /// 0xb20
			 0xeed18017, /// 0xb24
			 0xcea585d2, /// 0xb28
			 0x8053cf18, /// 0xb2c
			 0x06ce32b1, /// 0xb30
			 0xde2b8b7c, /// 0xb34
			 0x0309d351, /// 0xb38
			 0x2d47f206, /// 0xb3c
			 0xdc5c8ca1, /// 0xb40
			 0xacea161b, /// 0xb44
			 0x9ceb7cab, /// 0xb48
			 0xd72df879, /// 0xb4c
			 0x4c00a020, /// 0xb50
			 0x74ed96a6, /// 0xb54
			 0xed7616f2, /// 0xb58
			 0xb135190c, /// 0xb5c
			 0x2913212a, /// 0xb60
			 0xd96190f4, /// 0xb64
			 0xf4b85d56, /// 0xb68
			 0x9df97414, /// 0xb6c
			 0x25b9ee87, /// 0xb70
			 0xdae8ece9, /// 0xb74
			 0x342d6671, /// 0xb78
			 0xf130049c, /// 0xb7c
			 0x1ba9d05f, /// 0xb80
			 0xbf89d79e, /// 0xb84
			 0x5e3dbcaf, /// 0xb88
			 0x07c95f21, /// 0xb8c
			 0x2823780d, /// 0xb90
			 0xbceeb9a0, /// 0xb94
			 0xf895a5bd, /// 0xb98
			 0xd7e29bbb, /// 0xb9c
			 0x0b1839e1, /// 0xba0
			 0xe9f596ca, /// 0xba4
			 0x269ee378, /// 0xba8
			 0x635f9bb0, /// 0xbac
			 0x3a1ffefc, /// 0xbb0
			 0xb266441b, /// 0xbb4
			 0xdf6621c0, /// 0xbb8
			 0xe22f7b10, /// 0xbbc
			 0x7ee3029e, /// 0xbc0
			 0x40546036, /// 0xbc4
			 0x6bba852f, /// 0xbc8
			 0x4e0aa922, /// 0xbcc
			 0xd5440822, /// 0xbd0
			 0x575c6a64, /// 0xbd4
			 0x3a2338dd, /// 0xbd8
			 0xaf1257d1, /// 0xbdc
			 0x43928003, /// 0xbe0
			 0xc4afc0af, /// 0xbe4
			 0x60309d9d, /// 0xbe8
			 0x9e091c96, /// 0xbec
			 0x0033d815, /// 0xbf0
			 0x38e1780c, /// 0xbf4
			 0x6ec4b990, /// 0xbf8
			 0x5938fd85, /// 0xbfc
			 0xb186a8a5, /// 0xc00
			 0xc7ff01e2, /// 0xc04
			 0xcb7c52b4, /// 0xc08
			 0x4654afaf, /// 0xc0c
			 0x59d5bd5d, /// 0xc10
			 0x6c470bd6, /// 0xc14
			 0x547ffacf, /// 0xc18
			 0x13f327eb, /// 0xc1c
			 0x4092ebbc, /// 0xc20
			 0xe07ba9f2, /// 0xc24
			 0xe3c3d27b, /// 0xc28
			 0x1ddfbd5e, /// 0xc2c
			 0x5e561dcc, /// 0xc30
			 0xb86f23fa, /// 0xc34
			 0x415c1a31, /// 0xc38
			 0xb5b546e4, /// 0xc3c
			 0xaee1e78a, /// 0xc40
			 0x8d600221, /// 0xc44
			 0x5a102350, /// 0xc48
			 0x43b87d4e, /// 0xc4c
			 0x59ccdab8, /// 0xc50
			 0x4b50e7eb, /// 0xc54
			 0x944c654b, /// 0xc58
			 0xe0fbb815, /// 0xc5c
			 0x723e571f, /// 0xc60
			 0x27dffbc4, /// 0xc64
			 0xc9548058, /// 0xc68
			 0x9380d09e, /// 0xc6c
			 0xac886f3c, /// 0xc70
			 0x626766c8, /// 0xc74
			 0xe98f77b3, /// 0xc78
			 0xd655efb0, /// 0xc7c
			 0xa05a7f68, /// 0xc80
			 0x8b75ba2d, /// 0xc84
			 0x25f10dfb, /// 0xc88
			 0x74d03347, /// 0xc8c
			 0x711f537b, /// 0xc90
			 0x3f5251f5, /// 0xc94
			 0x07d83d53, /// 0xc98
			 0xe77a4a98, /// 0xc9c
			 0xdc8c7e5d, /// 0xca0
			 0x5015bb5a, /// 0xca4
			 0x17ef33e5, /// 0xca8
			 0x7313bd44, /// 0xcac
			 0x3a345f18, /// 0xcb0
			 0x4939b2b1, /// 0xcb4
			 0xb90947de, /// 0xcb8
			 0x5021d9e0, /// 0xcbc
			 0x0c3944ce, /// 0xcc0
			 0xa21113ae, /// 0xcc4
			 0xd8f59f1b, /// 0xcc8
			 0xda0635b7, /// 0xccc
			 0xe93b94df, /// 0xcd0
			 0xf39612bb, /// 0xcd4
			 0xa89e4fc0, /// 0xcd8
			 0xe7eb9e9b, /// 0xcdc
			 0xcd1ccb77, /// 0xce0
			 0x79f0effd, /// 0xce4
			 0xbc91dd54, /// 0xce8
			 0x8d687eab, /// 0xcec
			 0x830e1e91, /// 0xcf0
			 0x4cc8f23d, /// 0xcf4
			 0xbde642a0, /// 0xcf8
			 0xc9ecb3a7, /// 0xcfc
			 0xeaaf83cf, /// 0xd00
			 0x4cc972c0, /// 0xd04
			 0xb9558a32, /// 0xd08
			 0x2514cc45, /// 0xd0c
			 0x4344549f, /// 0xd10
			 0x84873764, /// 0xd14
			 0xb6dd5b66, /// 0xd18
			 0x74585bb7, /// 0xd1c
			 0x5b494dff, /// 0xd20
			 0x3883e76a, /// 0xd24
			 0x4eb7f9e4, /// 0xd28
			 0xc4169155, /// 0xd2c
			 0xe337dfd3, /// 0xd30
			 0x5d5684f1, /// 0xd34
			 0x6985d532, /// 0xd38
			 0x2fc36c9d, /// 0xd3c
			 0xb827669b, /// 0xd40
			 0xe9a0d575, /// 0xd44
			 0xc42b96ee, /// 0xd48
			 0x88ea0519, /// 0xd4c
			 0xaa4479ca, /// 0xd50
			 0xc2dffb4a, /// 0xd54
			 0xec587ed6, /// 0xd58
			 0x448b93cb, /// 0xd5c
			 0x2b8a0487, /// 0xd60
			 0x2077f07e, /// 0xd64
			 0x444f519f, /// 0xd68
			 0x23705c1a, /// 0xd6c
			 0x6918c067, /// 0xd70
			 0x57249a47, /// 0xd74
			 0x3159c6b5, /// 0xd78
			 0xe8c398ae, /// 0xd7c
			 0x9a16e1c7, /// 0xd80
			 0x2483749e, /// 0xd84
			 0x0d5b0603, /// 0xd88
			 0x5d30ef0c, /// 0xd8c
			 0xf539f45b, /// 0xd90
			 0xd82a31a0, /// 0xd94
			 0x0bf74eea, /// 0xd98
			 0xd2b443ee, /// 0xd9c
			 0xba3e1bd5, /// 0xda0
			 0x0f9649a0, /// 0xda4
			 0x146a0cbc, /// 0xda8
			 0x7eb7ecfb, /// 0xdac
			 0x70abd3a1, /// 0xdb0
			 0xec798d49, /// 0xdb4
			 0x8e765f1e, /// 0xdb8
			 0x5e8f041e, /// 0xdbc
			 0xe36d6b67, /// 0xdc0
			 0x161c065e, /// 0xdc4
			 0xdd8a9b1d, /// 0xdc8
			 0x95efec79, /// 0xdcc
			 0xb948b1f9, /// 0xdd0
			 0xb5403c02, /// 0xdd4
			 0x0080d07f, /// 0xdd8
			 0xd0d07dcf, /// 0xddc
			 0x5084d120, /// 0xde0
			 0xc82bbb19, /// 0xde4
			 0xccf509cd, /// 0xde8
			 0x0c0a2591, /// 0xdec
			 0x2ccaa3bd, /// 0xdf0
			 0x7e8561f0, /// 0xdf4
			 0x2ae8a75d, /// 0xdf8
			 0x2d2fec79, /// 0xdfc
			 0x7a174945, /// 0xe00
			 0x867ff992, /// 0xe04
			 0x8e619911, /// 0xe08
			 0x82369710, /// 0xe0c
			 0x60e11f86, /// 0xe10
			 0xb0304c96, /// 0xe14
			 0x5a1a3fcd, /// 0xe18
			 0x58907aa2, /// 0xe1c
			 0xd2e5e5c4, /// 0xe20
			 0x8c46ba01, /// 0xe24
			 0x4cb3368a, /// 0xe28
			 0x89c191e8, /// 0xe2c
			 0x9403697b, /// 0xe30
			 0x51d18cb7, /// 0xe34
			 0x4c9be2b3, /// 0xe38
			 0x2dd7c1b5, /// 0xe3c
			 0x707c9496, /// 0xe40
			 0xca593686, /// 0xe44
			 0xfce752ce, /// 0xe48
			 0x013b40ea, /// 0xe4c
			 0x4659bc94, /// 0xe50
			 0x67bc1a5f, /// 0xe54
			 0x8017c685, /// 0xe58
			 0x90fad1c1, /// 0xe5c
			 0x623dd2c7, /// 0xe60
			 0x71643c12, /// 0xe64
			 0x2bf83938, /// 0xe68
			 0x6e09e532, /// 0xe6c
			 0xf9b94c38, /// 0xe70
			 0x7780326f, /// 0xe74
			 0xf5bf94a2, /// 0xe78
			 0xef2febd6, /// 0xe7c
			 0xff6c30a2, /// 0xe80
			 0xdbe413d3, /// 0xe84
			 0x16d4e57b, /// 0xe88
			 0xcac273a2, /// 0xe8c
			 0xd37f24b1, /// 0xe90
			 0x24bb923b, /// 0xe94
			 0x7d2c2184, /// 0xe98
			 0x97c8d930, /// 0xe9c
			 0xa202c0ef, /// 0xea0
			 0x6ab5efb6, /// 0xea4
			 0x44c2d41e, /// 0xea8
			 0xa5f106ee, /// 0xeac
			 0x3991f33d, /// 0xeb0
			 0x0c52ef91, /// 0xeb4
			 0xe7e9f28d, /// 0xeb8
			 0x3e05f8b2, /// 0xebc
			 0xa09dc7bf, /// 0xec0
			 0xd14b5191, /// 0xec4
			 0x6442484b, /// 0xec8
			 0x25f0f9a2, /// 0xecc
			 0xd7134f53, /// 0xed0
			 0xd99bc6c1, /// 0xed4
			 0x7f7f25d0, /// 0xed8
			 0xfa42a486, /// 0xedc
			 0x10a55065, /// 0xee0
			 0xcd7e5551, /// 0xee4
			 0x56c0c85d, /// 0xee8
			 0xf7e34fa6, /// 0xeec
			 0xa12b8274, /// 0xef0
			 0x17a66868, /// 0xef4
			 0x22aa18b5, /// 0xef8
			 0x26c2c33a, /// 0xefc
			 0x98a9439e, /// 0xf00
			 0x060aeea2, /// 0xf04
			 0xd12f3f7b, /// 0xf08
			 0x59142293, /// 0xf0c
			 0x0f8f4f9b, /// 0xf10
			 0x9e694557, /// 0xf14
			 0x236160cc, /// 0xf18
			 0xc165b385, /// 0xf1c
			 0xf7b31639, /// 0xf20
			 0x75684b83, /// 0xf24
			 0x2b132846, /// 0xf28
			 0xe9446b0b, /// 0xf2c
			 0x9ddf9a19, /// 0xf30
			 0x5eb3c890, /// 0xf34
			 0x3a51b046, /// 0xf38
			 0x4776a610, /// 0xf3c
			 0xf769ef69, /// 0xf40
			 0xb7a37fcb, /// 0xf44
			 0x1fd24b9d, /// 0xf48
			 0xaa52846d, /// 0xf4c
			 0x39ad2ecb, /// 0xf50
			 0x608b72fb, /// 0xf54
			 0xf5ef7f3e, /// 0xf58
			 0x6b6ddd21, /// 0xf5c
			 0x4747e750, /// 0xf60
			 0x4560c98f, /// 0xf64
			 0x412b975a, /// 0xf68
			 0x25650501, /// 0xf6c
			 0x98af7263, /// 0xf70
			 0xa50863de, /// 0xf74
			 0xab4f47ad, /// 0xf78
			 0x40c820cc, /// 0xf7c
			 0x7e7795ab, /// 0xf80
			 0x0a830da5, /// 0xf84
			 0xed699646, /// 0xf88
			 0x76f9f1f5, /// 0xf8c
			 0xa1ddf717, /// 0xf90
			 0xec49011b, /// 0xf94
			 0xb7afeaa5, /// 0xf98
			 0xb9494a14, /// 0xf9c
			 0x3a97c8c2, /// 0xfa0
			 0xbf280612, /// 0xfa4
			 0x26f63f04, /// 0xfa8
			 0x6bb921f5, /// 0xfac
			 0x4cd73e44, /// 0xfb0
			 0x9363f8a3, /// 0xfb4
			 0xb46bdca2, /// 0xfb8
			 0x7eb4f42b, /// 0xfbc
			 0x58e73129, /// 0xfc0
			 0x4e72941e, /// 0xfc4
			 0x9094be71, /// 0xfc8
			 0x01ce07a1, /// 0xfcc
			 0xe88203b8, /// 0xfd0
			 0xe61261ce, /// 0xfd4
			 0x1dfb5935, /// 0xfd8
			 0x5bbcdd0d, /// 0xfdc
			 0x86ea2bf0, /// 0xfe0
			 0xc4d66bc4, /// 0xfe4
			 0xdf77652c, /// 0xfe8
			 0x2445e8cd, /// 0xfec
			 0x05993890, /// 0xff0
			 0x7a60aadb, /// 0xff4
			 0xe6f7b269, /// 0xff8
			 0xd6ede199 /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x95f7835d, /// 0x0
			 0x5c055046, /// 0x4
			 0x7b0a8301, /// 0x8
			 0xc94baf54, /// 0xc
			 0x8617f556, /// 0x10
			 0x6c727fd9, /// 0x14
			 0xfa897c86, /// 0x18
			 0x07e29a6b, /// 0x1c
			 0x37bf9b9b, /// 0x20
			 0x8b846f4f, /// 0x24
			 0xa59211c4, /// 0x28
			 0x16962def, /// 0x2c
			 0x38f54ae2, /// 0x30
			 0xf9c01114, /// 0x34
			 0x629dcf7a, /// 0x38
			 0x00d11736, /// 0x3c
			 0x68786adc, /// 0x40
			 0xbd4e3d11, /// 0x44
			 0x58d0f21b, /// 0x48
			 0x181950a2, /// 0x4c
			 0xb2bb5c84, /// 0x50
			 0x48847107, /// 0x54
			 0xa1a8d9c4, /// 0x58
			 0x45456a6f, /// 0x5c
			 0x09e1017f, /// 0x60
			 0xb17edc67, /// 0x64
			 0x519fa234, /// 0x68
			 0x5b8768ee, /// 0x6c
			 0x8c8ad75d, /// 0x70
			 0x710dc01a, /// 0x74
			 0x6e8c5211, /// 0x78
			 0x73166df6, /// 0x7c
			 0x7d25130d, /// 0x80
			 0x853de19d, /// 0x84
			 0x3be2a874, /// 0x88
			 0x25e50b4f, /// 0x8c
			 0x70c5c95a, /// 0x90
			 0x8202a856, /// 0x94
			 0x85dd78f8, /// 0x98
			 0xcacae866, /// 0x9c
			 0x31e01e75, /// 0xa0
			 0xeb2ce85b, /// 0xa4
			 0x0f0a7b27, /// 0xa8
			 0x1feba759, /// 0xac
			 0xb37ed973, /// 0xb0
			 0xe68a168d, /// 0xb4
			 0xcbc8bafc, /// 0xb8
			 0x17048989, /// 0xbc
			 0xb71cffb6, /// 0xc0
			 0x87437ebe, /// 0xc4
			 0xc8fb1cb3, /// 0xc8
			 0x54cd2cd7, /// 0xcc
			 0x3cf6e272, /// 0xd0
			 0x4b709da1, /// 0xd4
			 0x72ed671b, /// 0xd8
			 0x54be165b, /// 0xdc
			 0xd56e3f02, /// 0xe0
			 0x36d08610, /// 0xe4
			 0xbe0a891e, /// 0xe8
			 0xe12bbe0c, /// 0xec
			 0xc365c954, /// 0xf0
			 0x26de6518, /// 0xf4
			 0xd79d477a, /// 0xf8
			 0x366b8122, /// 0xfc
			 0x893a417e, /// 0x100
			 0x08a03fdc, /// 0x104
			 0x346ecf9b, /// 0x108
			 0x7aaab05a, /// 0x10c
			 0x692ed23b, /// 0x110
			 0x1c53b4b8, /// 0x114
			 0xb80e9344, /// 0x118
			 0x3b859015, /// 0x11c
			 0x72eb8226, /// 0x120
			 0x89f6513d, /// 0x124
			 0x5d14aca3, /// 0x128
			 0xd72187aa, /// 0x12c
			 0xfedd7986, /// 0x130
			 0x714fd07e, /// 0x134
			 0x57bf9d70, /// 0x138
			 0xcf480bf3, /// 0x13c
			 0x33d2c1c7, /// 0x140
			 0xf820cacb, /// 0x144
			 0x04b6a3cb, /// 0x148
			 0x9eaf1de9, /// 0x14c
			 0xc0484880, /// 0x150
			 0x858749ee, /// 0x154
			 0x2d454dfe, /// 0x158
			 0x14081793, /// 0x15c
			 0xe50451e2, /// 0x160
			 0x7bd521d0, /// 0x164
			 0x46602dad, /// 0x168
			 0x54144bb9, /// 0x16c
			 0x5edef499, /// 0x170
			 0x01041dc7, /// 0x174
			 0x78f3b522, /// 0x178
			 0x2167e746, /// 0x17c
			 0xece9e807, /// 0x180
			 0x35f679e5, /// 0x184
			 0x6f521ead, /// 0x188
			 0x4b753662, /// 0x18c
			 0x80bd7baf, /// 0x190
			 0x061534ab, /// 0x194
			 0x47b358b5, /// 0x198
			 0x01b845de, /// 0x19c
			 0x497a4a2f, /// 0x1a0
			 0x80070288, /// 0x1a4
			 0x0b312e55, /// 0x1a8
			 0xa41076e5, /// 0x1ac
			 0x536ba145, /// 0x1b0
			 0x9e8399d5, /// 0x1b4
			 0x5afaa051, /// 0x1b8
			 0x5bc65282, /// 0x1bc
			 0x6f95909c, /// 0x1c0
			 0xe7a88ab7, /// 0x1c4
			 0x19022112, /// 0x1c8
			 0xb4ab4ff5, /// 0x1cc
			 0x953ea738, /// 0x1d0
			 0xa7c9cee0, /// 0x1d4
			 0xf4e1b638, /// 0x1d8
			 0xdb938e5a, /// 0x1dc
			 0x1f60c131, /// 0x1e0
			 0x44b0527e, /// 0x1e4
			 0x3eede22f, /// 0x1e8
			 0x5e4e2443, /// 0x1ec
			 0x9bdd74cf, /// 0x1f0
			 0x1bf45293, /// 0x1f4
			 0x3904a153, /// 0x1f8
			 0xda064b25, /// 0x1fc
			 0x3dae4175, /// 0x200
			 0x254f8e2d, /// 0x204
			 0x39fd9dbf, /// 0x208
			 0x13b853e2, /// 0x20c
			 0xf733499b, /// 0x210
			 0x163e41e0, /// 0x214
			 0x4433298b, /// 0x218
			 0xa153bbe0, /// 0x21c
			 0x109d47bf, /// 0x220
			 0x2aeaef24, /// 0x224
			 0x5075338d, /// 0x228
			 0xa84ce4c2, /// 0x22c
			 0x7636fec8, /// 0x230
			 0x6ad95143, /// 0x234
			 0x45f2962c, /// 0x238
			 0x6fa9b340, /// 0x23c
			 0xbf6de40f, /// 0x240
			 0x0583653a, /// 0x244
			 0x26920d92, /// 0x248
			 0xd36a0b8b, /// 0x24c
			 0x5f380ac5, /// 0x250
			 0x6cabf880, /// 0x254
			 0x30162ad0, /// 0x258
			 0x304c748b, /// 0x25c
			 0x18f8ded5, /// 0x260
			 0x4aa8711d, /// 0x264
			 0xe0268d0e, /// 0x268
			 0x58ee6777, /// 0x26c
			 0x34cfbd9e, /// 0x270
			 0x8e4200d6, /// 0x274
			 0x61fb25f2, /// 0x278
			 0x9a72813e, /// 0x27c
			 0xf0531d9a, /// 0x280
			 0xb3151f61, /// 0x284
			 0xe92e07cc, /// 0x288
			 0x585b5421, /// 0x28c
			 0x0dcb9f5c, /// 0x290
			 0x8f24b11f, /// 0x294
			 0xe4b317da, /// 0x298
			 0xe42f9365, /// 0x29c
			 0x9c64b7a7, /// 0x2a0
			 0xa51d25b6, /// 0x2a4
			 0x79014c7f, /// 0x2a8
			 0x1b1e8b57, /// 0x2ac
			 0x23551756, /// 0x2b0
			 0x7242b574, /// 0x2b4
			 0x8ce15367, /// 0x2b8
			 0xcc180234, /// 0x2bc
			 0x6f0c41f8, /// 0x2c0
			 0xc192aec4, /// 0x2c4
			 0xb249c720, /// 0x2c8
			 0xd4fd229b, /// 0x2cc
			 0xbaf1bb91, /// 0x2d0
			 0x1afa15a5, /// 0x2d4
			 0x3a227fd7, /// 0x2d8
			 0x6a0171c7, /// 0x2dc
			 0x6c699bd5, /// 0x2e0
			 0xf45e7374, /// 0x2e4
			 0x70c132b6, /// 0x2e8
			 0xa84232ea, /// 0x2ec
			 0x836eca8d, /// 0x2f0
			 0xf3d4c97e, /// 0x2f4
			 0xfcf57eb8, /// 0x2f8
			 0x66c8fbe7, /// 0x2fc
			 0xe0544948, /// 0x300
			 0x65367a4a, /// 0x304
			 0xc9d73438, /// 0x308
			 0x4d2fc058, /// 0x30c
			 0x2ea9b45a, /// 0x310
			 0x7533d7f4, /// 0x314
			 0x5cce3395, /// 0x318
			 0x0fbb5271, /// 0x31c
			 0x7e97502f, /// 0x320
			 0x4b977288, /// 0x324
			 0x749ada08, /// 0x328
			 0x2782d845, /// 0x32c
			 0x9a9beefb, /// 0x330
			 0x494f920e, /// 0x334
			 0x788427d6, /// 0x338
			 0x4a5499e5, /// 0x33c
			 0xed67633b, /// 0x340
			 0x2cead802, /// 0x344
			 0x952aa663, /// 0x348
			 0xaf96c2a7, /// 0x34c
			 0xf8820683, /// 0x350
			 0x5b999ee0, /// 0x354
			 0x866bb138, /// 0x358
			 0x50993d93, /// 0x35c
			 0xebd3870f, /// 0x360
			 0x0b14aea1, /// 0x364
			 0xf7ec3ed7, /// 0x368
			 0x31ad4e6d, /// 0x36c
			 0x5529d45a, /// 0x370
			 0xfaaf32cb, /// 0x374
			 0xb90dade1, /// 0x378
			 0x40661a5d, /// 0x37c
			 0x06782c10, /// 0x380
			 0x9b00192c, /// 0x384
			 0xde0862a0, /// 0x388
			 0x63fde63f, /// 0x38c
			 0xab3c07bf, /// 0x390
			 0xd59570ff, /// 0x394
			 0x70dd297f, /// 0x398
			 0x23594138, /// 0x39c
			 0x9cf85ce5, /// 0x3a0
			 0x9a3e05b7, /// 0x3a4
			 0xa3212e98, /// 0x3a8
			 0x422bc3ab, /// 0x3ac
			 0x5398ca75, /// 0x3b0
			 0xf6ea5834, /// 0x3b4
			 0x848351de, /// 0x3b8
			 0x6a307dc3, /// 0x3bc
			 0xf57924e0, /// 0x3c0
			 0x43b39905, /// 0x3c4
			 0x30bcd836, /// 0x3c8
			 0x7ca4c32e, /// 0x3cc
			 0xb8a99824, /// 0x3d0
			 0x14bf5b66, /// 0x3d4
			 0xa2ab8e74, /// 0x3d8
			 0x4a76db1e, /// 0x3dc
			 0x2558243d, /// 0x3e0
			 0x624accc6, /// 0x3e4
			 0x3185726c, /// 0x3e8
			 0x3f143bfd, /// 0x3ec
			 0xf2374d9c, /// 0x3f0
			 0x8f2a18e7, /// 0x3f4
			 0x1a1a9aee, /// 0x3f8
			 0x4d36f2f1, /// 0x3fc
			 0xd9dd1111, /// 0x400
			 0x8d87961e, /// 0x404
			 0x06209519, /// 0x408
			 0x4b01c1b6, /// 0x40c
			 0x499aac7c, /// 0x410
			 0x95e1dbe9, /// 0x414
			 0x86864585, /// 0x418
			 0x54aa2cc1, /// 0x41c
			 0x147dd309, /// 0x420
			 0x33b7af31, /// 0x424
			 0x707ff71d, /// 0x428
			 0x2ea8a30c, /// 0x42c
			 0x0062f202, /// 0x430
			 0xd0895116, /// 0x434
			 0xf6e5781a, /// 0x438
			 0xcdbe0df6, /// 0x43c
			 0xd9dc6b02, /// 0x440
			 0xfb89ac7e, /// 0x444
			 0x795615c9, /// 0x448
			 0xc9b40a7d, /// 0x44c
			 0x9e975cf7, /// 0x450
			 0x8739b007, /// 0x454
			 0x6bf4bab1, /// 0x458
			 0x49d6ca34, /// 0x45c
			 0xf57e4517, /// 0x460
			 0xf21a69c6, /// 0x464
			 0x093f8663, /// 0x468
			 0xde9c215f, /// 0x46c
			 0x298a4f42, /// 0x470
			 0xd69c7fa6, /// 0x474
			 0x216233e5, /// 0x478
			 0xe7024ce4, /// 0x47c
			 0xc8d09961, /// 0x480
			 0x15ada5ac, /// 0x484
			 0xfe42adb1, /// 0x488
			 0x33416aca, /// 0x48c
			 0xe044b921, /// 0x490
			 0x13c58f25, /// 0x494
			 0x6291dc0a, /// 0x498
			 0x1155c558, /// 0x49c
			 0xccfa0452, /// 0x4a0
			 0xcb75a085, /// 0x4a4
			 0x49a8bfa2, /// 0x4a8
			 0xf665a2a8, /// 0x4ac
			 0xc07f4cec, /// 0x4b0
			 0xcf9eb349, /// 0x4b4
			 0xa68aa670, /// 0x4b8
			 0x8c752bc8, /// 0x4bc
			 0xf8c34d10, /// 0x4c0
			 0xae33c553, /// 0x4c4
			 0x29c3b64e, /// 0x4c8
			 0xd917af02, /// 0x4cc
			 0xbff69535, /// 0x4d0
			 0x0aa059b4, /// 0x4d4
			 0x90891124, /// 0x4d8
			 0xbe531f85, /// 0x4dc
			 0x717a0c0d, /// 0x4e0
			 0x6d1f4930, /// 0x4e4
			 0x1f48a8bc, /// 0x4e8
			 0x92c5fd49, /// 0x4ec
			 0x63c11dd4, /// 0x4f0
			 0xb818930e, /// 0x4f4
			 0xa0914d7d, /// 0x4f8
			 0x4ff58884, /// 0x4fc
			 0x3a5cbc90, /// 0x500
			 0x0e20cc82, /// 0x504
			 0xfadbc08c, /// 0x508
			 0xb6e37fe6, /// 0x50c
			 0xd8e1aa3d, /// 0x510
			 0x1c07ffcc, /// 0x514
			 0x130f82dd, /// 0x518
			 0x42228593, /// 0x51c
			 0xc809810d, /// 0x520
			 0x6f927c59, /// 0x524
			 0x31b57f0d, /// 0x528
			 0x9a94202a, /// 0x52c
			 0xa228bd49, /// 0x530
			 0x9873a87b, /// 0x534
			 0x97018d36, /// 0x538
			 0x9510a5f6, /// 0x53c
			 0xf4657355, /// 0x540
			 0xaffa2b49, /// 0x544
			 0x3a343a56, /// 0x548
			 0xa1e52e9b, /// 0x54c
			 0xe2469e24, /// 0x550
			 0x10b6ddbd, /// 0x554
			 0x7f5f1c16, /// 0x558
			 0x382e394b, /// 0x55c
			 0xfdd16b13, /// 0x560
			 0x07801ea3, /// 0x564
			 0x2350ec09, /// 0x568
			 0x37f359cd, /// 0x56c
			 0xa964aa60, /// 0x570
			 0xe9da4051, /// 0x574
			 0xb0533f61, /// 0x578
			 0x41a76064, /// 0x57c
			 0x2d64c8e6, /// 0x580
			 0xfd1c6544, /// 0x584
			 0x23b97942, /// 0x588
			 0x35c60faa, /// 0x58c
			 0x729e615e, /// 0x590
			 0x8b7b324c, /// 0x594
			 0x0d250186, /// 0x598
			 0xc98bcb55, /// 0x59c
			 0x2856ce4f, /// 0x5a0
			 0xc5c16ffc, /// 0x5a4
			 0x73fbcacc, /// 0x5a8
			 0x0cd797c1, /// 0x5ac
			 0x4bede813, /// 0x5b0
			 0xa0983158, /// 0x5b4
			 0xd9443e5b, /// 0x5b8
			 0xf8263d15, /// 0x5bc
			 0x13655d31, /// 0x5c0
			 0x9f1346c0, /// 0x5c4
			 0x70bb2f16, /// 0x5c8
			 0xb4024080, /// 0x5cc
			 0xf8750587, /// 0x5d0
			 0x796110a5, /// 0x5d4
			 0xbf83f09f, /// 0x5d8
			 0x05711fbe, /// 0x5dc
			 0x725a7730, /// 0x5e0
			 0xf82f9b28, /// 0x5e4
			 0xaabf94ae, /// 0x5e8
			 0x64845771, /// 0x5ec
			 0x9aaf98f5, /// 0x5f0
			 0x425dae79, /// 0x5f4
			 0x81dc7185, /// 0x5f8
			 0x7b7ec6b3, /// 0x5fc
			 0x4aad868d, /// 0x600
			 0x314b4686, /// 0x604
			 0x0b2e94b5, /// 0x608
			 0x451edd04, /// 0x60c
			 0x7dbcf98e, /// 0x610
			 0xe094e309, /// 0x614
			 0x8040eba4, /// 0x618
			 0x6beca309, /// 0x61c
			 0x1e6abd12, /// 0x620
			 0x35963553, /// 0x624
			 0x21401ffb, /// 0x628
			 0x361775eb, /// 0x62c
			 0xb87a0cb5, /// 0x630
			 0x9cdfb0a7, /// 0x634
			 0xd48e05d1, /// 0x638
			 0xf04d6a2b, /// 0x63c
			 0xd835cbce, /// 0x640
			 0xc2261f1f, /// 0x644
			 0xd93826ca, /// 0x648
			 0x110b15bf, /// 0x64c
			 0xbc5f9b68, /// 0x650
			 0x7d01b95e, /// 0x654
			 0x1fa25a63, /// 0x658
			 0xb5447445, /// 0x65c
			 0x72d83b31, /// 0x660
			 0xb1dc38a4, /// 0x664
			 0x49da66d5, /// 0x668
			 0xb0d40a5d, /// 0x66c
			 0x33737a3e, /// 0x670
			 0x34c44625, /// 0x674
			 0xa6d3740e, /// 0x678
			 0x121c2282, /// 0x67c
			 0x539dbe2d, /// 0x680
			 0x9c2da4ff, /// 0x684
			 0x5c4517e1, /// 0x688
			 0x09435763, /// 0x68c
			 0x1cc3f39b, /// 0x690
			 0x51e73ef0, /// 0x694
			 0x1448a49f, /// 0x698
			 0x22685ae6, /// 0x69c
			 0x981d6131, /// 0x6a0
			 0x2fd1ee9c, /// 0x6a4
			 0xff2fbdc6, /// 0x6a8
			 0x23b168e9, /// 0x6ac
			 0xb60671f2, /// 0x6b0
			 0xbefc74ad, /// 0x6b4
			 0x920ccc0b, /// 0x6b8
			 0x015cf13c, /// 0x6bc
			 0x401875a9, /// 0x6c0
			 0x34fea330, /// 0x6c4
			 0xbf8c4a73, /// 0x6c8
			 0x90747c2a, /// 0x6cc
			 0x8ac05f50, /// 0x6d0
			 0xc07c9d86, /// 0x6d4
			 0x5f4f357b, /// 0x6d8
			 0xdcca6840, /// 0x6dc
			 0xb647f358, /// 0x6e0
			 0x47defae2, /// 0x6e4
			 0xfdb0a062, /// 0x6e8
			 0xcafb8b6b, /// 0x6ec
			 0x76011110, /// 0x6f0
			 0x7ef4e50f, /// 0x6f4
			 0x8f44d2c1, /// 0x6f8
			 0x47856702, /// 0x6fc
			 0xa2c17ccb, /// 0x700
			 0x7c345eb4, /// 0x704
			 0xaecdf9e7, /// 0x708
			 0x5090255c, /// 0x70c
			 0x984ce2b8, /// 0x710
			 0x780b4fe3, /// 0x714
			 0x791e389d, /// 0x718
			 0x1c72edf1, /// 0x71c
			 0x1ec76c4e, /// 0x720
			 0x4c3bae72, /// 0x724
			 0xc3a9e5dc, /// 0x728
			 0xe4393085, /// 0x72c
			 0x005fe1de, /// 0x730
			 0xd2af654b, /// 0x734
			 0xde6c4b71, /// 0x738
			 0x4328b734, /// 0x73c
			 0x4d48ebd0, /// 0x740
			 0x10a8d8dc, /// 0x744
			 0x79a8a14f, /// 0x748
			 0x890f6803, /// 0x74c
			 0xf628037d, /// 0x750
			 0xe4d6afe6, /// 0x754
			 0x1c06fe00, /// 0x758
			 0x1b0240ad, /// 0x75c
			 0x60b3ae01, /// 0x760
			 0x07955ecd, /// 0x764
			 0x48691634, /// 0x768
			 0x25d9cdf3, /// 0x76c
			 0x178d55e4, /// 0x770
			 0x13246a45, /// 0x774
			 0x04942fab, /// 0x778
			 0x0901df70, /// 0x77c
			 0xbf1b8f74, /// 0x780
			 0x099080b2, /// 0x784
			 0x84d84cd4, /// 0x788
			 0x13b7fc64, /// 0x78c
			 0xf6be6ddb, /// 0x790
			 0x856205d3, /// 0x794
			 0xc36c3181, /// 0x798
			 0xc409e515, /// 0x79c
			 0xe6aeaf30, /// 0x7a0
			 0x87b13589, /// 0x7a4
			 0x97aafc94, /// 0x7a8
			 0x0817d8b6, /// 0x7ac
			 0x55a3a7d7, /// 0x7b0
			 0x093d12ea, /// 0x7b4
			 0xcd377813, /// 0x7b8
			 0xa8827571, /// 0x7bc
			 0x9c6b742e, /// 0x7c0
			 0xb5f92b94, /// 0x7c4
			 0x4a4078e5, /// 0x7c8
			 0x1dd7fd26, /// 0x7cc
			 0x923f1ba0, /// 0x7d0
			 0xb14c0f9f, /// 0x7d4
			 0xebdb8a25, /// 0x7d8
			 0x36e3cdac, /// 0x7dc
			 0xebb206e3, /// 0x7e0
			 0x14d99246, /// 0x7e4
			 0x73f8f911, /// 0x7e8
			 0xf1cd9bee, /// 0x7ec
			 0x828b0f1c, /// 0x7f0
			 0x9626d702, /// 0x7f4
			 0x1b2f5542, /// 0x7f8
			 0xd9a6d90c, /// 0x7fc
			 0xfad841f0, /// 0x800
			 0xd54c9422, /// 0x804
			 0x872d1e88, /// 0x808
			 0x153089fd, /// 0x80c
			 0xbfa1e7f5, /// 0x810
			 0xd35240c1, /// 0x814
			 0xc4d453b1, /// 0x818
			 0xd8a85c26, /// 0x81c
			 0xae522b27, /// 0x820
			 0xdf5d8b08, /// 0x824
			 0xfc483199, /// 0x828
			 0x12fb0e6d, /// 0x82c
			 0x474cb208, /// 0x830
			 0xac69dcf5, /// 0x834
			 0x0cb1e3dc, /// 0x838
			 0xcda1a260, /// 0x83c
			 0x8a6da1de, /// 0x840
			 0xb6e9657d, /// 0x844
			 0xc2b4f377, /// 0x848
			 0xcf3c4dcd, /// 0x84c
			 0x86c12d1d, /// 0x850
			 0xe60668be, /// 0x854
			 0x0bf3111f, /// 0x858
			 0x4c518228, /// 0x85c
			 0x810aeb46, /// 0x860
			 0xdbc668f6, /// 0x864
			 0x504b7491, /// 0x868
			 0xee2f79ea, /// 0x86c
			 0xc2f0dab2, /// 0x870
			 0x3132140a, /// 0x874
			 0xda6433b6, /// 0x878
			 0x599dcec3, /// 0x87c
			 0xad774ea9, /// 0x880
			 0x2e46407d, /// 0x884
			 0x526ca11e, /// 0x888
			 0x6eacf34a, /// 0x88c
			 0x15cf29d2, /// 0x890
			 0xde5f642f, /// 0x894
			 0xe66a27e3, /// 0x898
			 0x626adc72, /// 0x89c
			 0x3fc74567, /// 0x8a0
			 0x1d918806, /// 0x8a4
			 0x7602a61c, /// 0x8a8
			 0x6015de0d, /// 0x8ac
			 0x0d1f3341, /// 0x8b0
			 0x4593f81f, /// 0x8b4
			 0x1cb11edc, /// 0x8b8
			 0x0b0b8adc, /// 0x8bc
			 0x41617f79, /// 0x8c0
			 0x5e2dcf7c, /// 0x8c4
			 0x3f4afd6c, /// 0x8c8
			 0xcbe87a89, /// 0x8cc
			 0x4d98cd65, /// 0x8d0
			 0x18f225e2, /// 0x8d4
			 0x275c3e70, /// 0x8d8
			 0xb6eed7fc, /// 0x8dc
			 0xcad3f18e, /// 0x8e0
			 0xb2472a94, /// 0x8e4
			 0x591c3d11, /// 0x8e8
			 0x4e208313, /// 0x8ec
			 0xbaf0504e, /// 0x8f0
			 0xf4cfaccc, /// 0x8f4
			 0x3870e990, /// 0x8f8
			 0xe1270f16, /// 0x8fc
			 0xdafccdfd, /// 0x900
			 0x6594c893, /// 0x904
			 0x20a24f3d, /// 0x908
			 0x888170b7, /// 0x90c
			 0x1b90d2f3, /// 0x910
			 0xfbbe3e4d, /// 0x914
			 0x29f7866a, /// 0x918
			 0xe437437a, /// 0x91c
			 0x81123361, /// 0x920
			 0x96bb7407, /// 0x924
			 0x8a86ff5a, /// 0x928
			 0x7505d847, /// 0x92c
			 0x91691420, /// 0x930
			 0x9cc0d3f9, /// 0x934
			 0x06f955c7, /// 0x938
			 0x9982ea26, /// 0x93c
			 0xcd30ab3d, /// 0x940
			 0x69dad6e6, /// 0x944
			 0x328aeb2b, /// 0x948
			 0x259cad2f, /// 0x94c
			 0x58a2b486, /// 0x950
			 0xcceea902, /// 0x954
			 0x3245c86b, /// 0x958
			 0x8e559f79, /// 0x95c
			 0x6fdafb18, /// 0x960
			 0x2bb7f384, /// 0x964
			 0xd4037446, /// 0x968
			 0xb4b3491d, /// 0x96c
			 0xc7b1dc78, /// 0x970
			 0x413219d8, /// 0x974
			 0x21b69e9e, /// 0x978
			 0xee2c58ed, /// 0x97c
			 0x070ecc7b, /// 0x980
			 0x54879c7e, /// 0x984
			 0x1531c946, /// 0x988
			 0x7e46d917, /// 0x98c
			 0x9c42ed3b, /// 0x990
			 0x815afe7e, /// 0x994
			 0xd5b6af7d, /// 0x998
			 0x08462a43, /// 0x99c
			 0xe551f4c2, /// 0x9a0
			 0xef64b31f, /// 0x9a4
			 0xb0347ac5, /// 0x9a8
			 0xd754645c, /// 0x9ac
			 0x6a66c260, /// 0x9b0
			 0xa05262d0, /// 0x9b4
			 0xdd9fea4d, /// 0x9b8
			 0xf729604c, /// 0x9bc
			 0xa32ca7e7, /// 0x9c0
			 0x899ac986, /// 0x9c4
			 0x5d715273, /// 0x9c8
			 0xef59a665, /// 0x9cc
			 0x0f0d220f, /// 0x9d0
			 0xdce52fdf, /// 0x9d4
			 0xb6afbbba, /// 0x9d8
			 0xf75b4448, /// 0x9dc
			 0xa8888d3c, /// 0x9e0
			 0xb4dbc547, /// 0x9e4
			 0xe423f86e, /// 0x9e8
			 0xbfb8d3d3, /// 0x9ec
			 0x4d73216d, /// 0x9f0
			 0xf67b0c3c, /// 0x9f4
			 0x95366170, /// 0x9f8
			 0x49fb78c4, /// 0x9fc
			 0xfd7c8719, /// 0xa00
			 0xf22ce3dc, /// 0xa04
			 0x27d7f1fb, /// 0xa08
			 0xe833eaad, /// 0xa0c
			 0x8f2210d7, /// 0xa10
			 0xc8a26c1c, /// 0xa14
			 0xdfc222a9, /// 0xa18
			 0x385c03eb, /// 0xa1c
			 0x14c8910f, /// 0xa20
			 0xdb8fb7f7, /// 0xa24
			 0x65f0ae59, /// 0xa28
			 0x59d34bca, /// 0xa2c
			 0x2c4907b8, /// 0xa30
			 0xb33a2a13, /// 0xa34
			 0x676d687c, /// 0xa38
			 0xac9f16cd, /// 0xa3c
			 0x0581d5b5, /// 0xa40
			 0x93b00cce, /// 0xa44
			 0x2252e2b6, /// 0xa48
			 0xcac0923c, /// 0xa4c
			 0x0802fb93, /// 0xa50
			 0x814106a1, /// 0xa54
			 0x3babf78d, /// 0xa58
			 0xe98b1a5e, /// 0xa5c
			 0x5597c812, /// 0xa60
			 0xfe2acc40, /// 0xa64
			 0xfa07d303, /// 0xa68
			 0xaa930c37, /// 0xa6c
			 0x17446ed7, /// 0xa70
			 0xaa6fdc66, /// 0xa74
			 0x2ff695af, /// 0xa78
			 0x90d0fe34, /// 0xa7c
			 0x626aa6d2, /// 0xa80
			 0x3e07aaef, /// 0xa84
			 0x65269898, /// 0xa88
			 0xb2682dd1, /// 0xa8c
			 0xae06deb0, /// 0xa90
			 0x24b07a2b, /// 0xa94
			 0xf87b6b2c, /// 0xa98
			 0xb6ed4e92, /// 0xa9c
			 0x6b9a2078, /// 0xaa0
			 0x25f16645, /// 0xaa4
			 0xb88f8861, /// 0xaa8
			 0x4adaddff, /// 0xaac
			 0x07814791, /// 0xab0
			 0xf6515d08, /// 0xab4
			 0xb0e80ad7, /// 0xab8
			 0x470eba89, /// 0xabc
			 0xb7cb4829, /// 0xac0
			 0xc02ba749, /// 0xac4
			 0x042b0fcd, /// 0xac8
			 0x13334773, /// 0xacc
			 0x7b0ab02c, /// 0xad0
			 0x1ccea1d1, /// 0xad4
			 0x5a8c2f31, /// 0xad8
			 0x811c5e82, /// 0xadc
			 0xe80959f9, /// 0xae0
			 0x94d95aee, /// 0xae4
			 0x330131a2, /// 0xae8
			 0x1c35f945, /// 0xaec
			 0x31395e4a, /// 0xaf0
			 0x131934c6, /// 0xaf4
			 0x0e39dd77, /// 0xaf8
			 0x48fce696, /// 0xafc
			 0x4475444e, /// 0xb00
			 0x49268887, /// 0xb04
			 0x0e7c677f, /// 0xb08
			 0xa4473f22, /// 0xb0c
			 0x33a6ec1b, /// 0xb10
			 0x2248ae48, /// 0xb14
			 0xb6c6450c, /// 0xb18
			 0xbcd4619e, /// 0xb1c
			 0xc2f7b904, /// 0xb20
			 0x28b4459f, /// 0xb24
			 0x3011c9cf, /// 0xb28
			 0x6f9697bf, /// 0xb2c
			 0x1b639472, /// 0xb30
			 0x8eb99006, /// 0xb34
			 0x844653f3, /// 0xb38
			 0x70012423, /// 0xb3c
			 0x9d90d4de, /// 0xb40
			 0xd2b2d431, /// 0xb44
			 0x09a98ee6, /// 0xb48
			 0x0032f3c8, /// 0xb4c
			 0xb2c5cf7e, /// 0xb50
			 0x91a520ad, /// 0xb54
			 0xee691424, /// 0xb58
			 0x6f4d36bf, /// 0xb5c
			 0x06531ed5, /// 0xb60
			 0xcab2fb8c, /// 0xb64
			 0x750dce9f, /// 0xb68
			 0xb22fd353, /// 0xb6c
			 0xdced3c5e, /// 0xb70
			 0x5c0971aa, /// 0xb74
			 0x49695e43, /// 0xb78
			 0x52cefc7a, /// 0xb7c
			 0x4dc20f80, /// 0xb80
			 0x1a0c3721, /// 0xb84
			 0xbc99479c, /// 0xb88
			 0xdc189c06, /// 0xb8c
			 0x1e4ce327, /// 0xb90
			 0x3155cfc6, /// 0xb94
			 0xdf8d522c, /// 0xb98
			 0x7ff106b2, /// 0xb9c
			 0x0445855a, /// 0xba0
			 0x970e8a91, /// 0xba4
			 0xb524c514, /// 0xba8
			 0xfb7e45ab, /// 0xbac
			 0x71f1e2d3, /// 0xbb0
			 0x2da1135c, /// 0xbb4
			 0x90e91b94, /// 0xbb8
			 0xb887ff91, /// 0xbbc
			 0x84830d1a, /// 0xbc0
			 0x238b802d, /// 0xbc4
			 0xae0125cc, /// 0xbc8
			 0x73ee0dad, /// 0xbcc
			 0x439e966f, /// 0xbd0
			 0x52bfc6ac, /// 0xbd4
			 0x93aa5582, /// 0xbd8
			 0xc66f39d4, /// 0xbdc
			 0x297539c6, /// 0xbe0
			 0x4f6cad8a, /// 0xbe4
			 0x126a00f5, /// 0xbe8
			 0xc7c103a5, /// 0xbec
			 0xd1f0bf5e, /// 0xbf0
			 0x95d64fd3, /// 0xbf4
			 0x5e72e8e4, /// 0xbf8
			 0x35a7a4b0, /// 0xbfc
			 0xa29e6aac, /// 0xc00
			 0xbc04fb21, /// 0xc04
			 0x370969be, /// 0xc08
			 0xa0809412, /// 0xc0c
			 0xbee12921, /// 0xc10
			 0x551af45d, /// 0xc14
			 0x29b9ecac, /// 0xc18
			 0x1ffd766e, /// 0xc1c
			 0xeae14e2a, /// 0xc20
			 0xd963049a, /// 0xc24
			 0xd0062916, /// 0xc28
			 0xea136d1d, /// 0xc2c
			 0xeb6492a5, /// 0xc30
			 0x559eb308, /// 0xc34
			 0xf9266c55, /// 0xc38
			 0x9768e89e, /// 0xc3c
			 0xbfd3c5f1, /// 0xc40
			 0x00971201, /// 0xc44
			 0xb4049171, /// 0xc48
			 0xbabd56da, /// 0xc4c
			 0x82695573, /// 0xc50
			 0xdea91e05, /// 0xc54
			 0xc658bdfb, /// 0xc58
			 0x703cc725, /// 0xc5c
			 0xc989cbaa, /// 0xc60
			 0xbc9e9cb0, /// 0xc64
			 0x21e0dfbe, /// 0xc68
			 0x985e3294, /// 0xc6c
			 0x9ab07ee3, /// 0xc70
			 0x0f472dc4, /// 0xc74
			 0x630b154f, /// 0xc78
			 0x84a6836e, /// 0xc7c
			 0xfd6f5a75, /// 0xc80
			 0x98213f1f, /// 0xc84
			 0x2524ee43, /// 0xc88
			 0xae9b7bfd, /// 0xc8c
			 0x3eb31f52, /// 0xc90
			 0x8da06dd3, /// 0xc94
			 0xd3c6b753, /// 0xc98
			 0x46b54986, /// 0xc9c
			 0x55408193, /// 0xca0
			 0xed1f4ae9, /// 0xca4
			 0xa1049824, /// 0xca8
			 0xd7089402, /// 0xcac
			 0xe19325aa, /// 0xcb0
			 0xa9d37811, /// 0xcb4
			 0xebf35559, /// 0xcb8
			 0xeb99f4f2, /// 0xcbc
			 0xf02997ff, /// 0xcc0
			 0x6267cd90, /// 0xcc4
			 0xa733f8cb, /// 0xcc8
			 0x823f5d11, /// 0xccc
			 0xdbeb19ba, /// 0xcd0
			 0xb9929ebd, /// 0xcd4
			 0xbc703860, /// 0xcd8
			 0x468256f2, /// 0xcdc
			 0xf0e3e115, /// 0xce0
			 0x6a938c05, /// 0xce4
			 0xadaa34c1, /// 0xce8
			 0x167debc3, /// 0xcec
			 0x78cf9eff, /// 0xcf0
			 0xb77453f6, /// 0xcf4
			 0x78f40d34, /// 0xcf8
			 0xbdeea56b, /// 0xcfc
			 0xae22e8e6, /// 0xd00
			 0x57d9f3a0, /// 0xd04
			 0xcabd2e23, /// 0xd08
			 0x68fd1481, /// 0xd0c
			 0x9b3b310a, /// 0xd10
			 0x89b82f31, /// 0xd14
			 0x87dbe527, /// 0xd18
			 0x0160127a, /// 0xd1c
			 0xca6144af, /// 0xd20
			 0xae0ac56b, /// 0xd24
			 0x4af867e3, /// 0xd28
			 0x6ff96c4f, /// 0xd2c
			 0x4c3ab434, /// 0xd30
			 0x21da48cc, /// 0xd34
			 0xc667e07d, /// 0xd38
			 0xd4294af0, /// 0xd3c
			 0x0d1df3df, /// 0xd40
			 0x7fb353e6, /// 0xd44
			 0x72ef42b7, /// 0xd48
			 0x50ca7fe2, /// 0xd4c
			 0x842e4655, /// 0xd50
			 0x6761ceb3, /// 0xd54
			 0xb1130a19, /// 0xd58
			 0x03be7a3b, /// 0xd5c
			 0xe90c99cb, /// 0xd60
			 0x8aeebef0, /// 0xd64
			 0xf7b8c50f, /// 0xd68
			 0xe94bf157, /// 0xd6c
			 0x083abc10, /// 0xd70
			 0x85626fd2, /// 0xd74
			 0x7403f86e, /// 0xd78
			 0x4aed8e84, /// 0xd7c
			 0x4071606c, /// 0xd80
			 0x2a203491, /// 0xd84
			 0x92861433, /// 0xd88
			 0x7de55444, /// 0xd8c
			 0xcc16c160, /// 0xd90
			 0x013711c3, /// 0xd94
			 0x0598e2be, /// 0xd98
			 0x09dd845b, /// 0xd9c
			 0x7d54f194, /// 0xda0
			 0xe7a16a3e, /// 0xda4
			 0x0d5812ab, /// 0xda8
			 0x1f5d5ef4, /// 0xdac
			 0xa6917d9d, /// 0xdb0
			 0x723a8030, /// 0xdb4
			 0x958e3390, /// 0xdb8
			 0xf078952f, /// 0xdbc
			 0x590d1857, /// 0xdc0
			 0x9d54afe6, /// 0xdc4
			 0x411ac40c, /// 0xdc8
			 0x6abde076, /// 0xdcc
			 0xa084cf1b, /// 0xdd0
			 0x4a53340a, /// 0xdd4
			 0x6380a718, /// 0xdd8
			 0xdc536bca, /// 0xddc
			 0xb84369eb, /// 0xde0
			 0x0a5e3ce1, /// 0xde4
			 0x7db99482, /// 0xde8
			 0x414d8c69, /// 0xdec
			 0xe7a00851, /// 0xdf0
			 0xe52be067, /// 0xdf4
			 0x2d4eed1a, /// 0xdf8
			 0x836bfc9c, /// 0xdfc
			 0x0927d36e, /// 0xe00
			 0x203cbfb5, /// 0xe04
			 0x458e733e, /// 0xe08
			 0xb280e59c, /// 0xe0c
			 0xbc53b780, /// 0xe10
			 0xe0d985d0, /// 0xe14
			 0xa3aa4df3, /// 0xe18
			 0x76040d4b, /// 0xe1c
			 0x7bdd7eff, /// 0xe20
			 0x3b583a46, /// 0xe24
			 0x2f0e0cf5, /// 0xe28
			 0xa4d5c91e, /// 0xe2c
			 0x10500096, /// 0xe30
			 0x979ffc99, /// 0xe34
			 0xc240762d, /// 0xe38
			 0x91c82150, /// 0xe3c
			 0x6e86e958, /// 0xe40
			 0xed7d01da, /// 0xe44
			 0x28d75ffe, /// 0xe48
			 0x8eac1afc, /// 0xe4c
			 0x5d4b5448, /// 0xe50
			 0x1e5baa99, /// 0xe54
			 0xe08af337, /// 0xe58
			 0x51c004c6, /// 0xe5c
			 0xc64f1542, /// 0xe60
			 0xbee6f9d4, /// 0xe64
			 0xd1e9aa9d, /// 0xe68
			 0xc485ef10, /// 0xe6c
			 0x2d3988d5, /// 0xe70
			 0x4c3391f6, /// 0xe74
			 0xce9d1238, /// 0xe78
			 0x2b59df5b, /// 0xe7c
			 0xe62960d0, /// 0xe80
			 0x67d2e361, /// 0xe84
			 0xb8e83355, /// 0xe88
			 0xe4c2349a, /// 0xe8c
			 0x720e19e9, /// 0xe90
			 0x14411da8, /// 0xe94
			 0x5e3f905f, /// 0xe98
			 0x610075f7, /// 0xe9c
			 0x0b19e2e9, /// 0xea0
			 0xdd37116c, /// 0xea4
			 0x459c26d1, /// 0xea8
			 0x060c55cd, /// 0xeac
			 0x507e91bc, /// 0xeb0
			 0xd6ad3462, /// 0xeb4
			 0x0b929009, /// 0xeb8
			 0xfbe4f8c5, /// 0xebc
			 0x452dfd57, /// 0xec0
			 0x7b4d6904, /// 0xec4
			 0xcdf3b17b, /// 0xec8
			 0x151755a2, /// 0xecc
			 0x1c5a2035, /// 0xed0
			 0x4e2d66ee, /// 0xed4
			 0x4ff9a3af, /// 0xed8
			 0x79eeb85c, /// 0xedc
			 0xeb8c2973, /// 0xee0
			 0xd81e5f05, /// 0xee4
			 0x4848f9ae, /// 0xee8
			 0xd89f0295, /// 0xeec
			 0x184113fc, /// 0xef0
			 0xd180060e, /// 0xef4
			 0x9fe763af, /// 0xef8
			 0xbd3bcc90, /// 0xefc
			 0xb7257fd3, /// 0xf00
			 0xd857cef8, /// 0xf04
			 0x27bbaf68, /// 0xf08
			 0x40da3340, /// 0xf0c
			 0x16459a82, /// 0xf10
			 0x9d4bd0bb, /// 0xf14
			 0xc1a644d4, /// 0xf18
			 0x07fd76f3, /// 0xf1c
			 0x164c2185, /// 0xf20
			 0x1504b3a1, /// 0xf24
			 0xe894ba72, /// 0xf28
			 0x3ab4d3d1, /// 0xf2c
			 0xfc6ec862, /// 0xf30
			 0xf1a1de6c, /// 0xf34
			 0x6910e96e, /// 0xf38
			 0x09046666, /// 0xf3c
			 0x8e73c500, /// 0xf40
			 0x145740f8, /// 0xf44
			 0x46b95aff, /// 0xf48
			 0x941780e1, /// 0xf4c
			 0x70359d79, /// 0xf50
			 0xf9b4acb9, /// 0xf54
			 0x4050c046, /// 0xf58
			 0xe422e3cc, /// 0xf5c
			 0xdcd45065, /// 0xf60
			 0x9d66879a, /// 0xf64
			 0xaee52083, /// 0xf68
			 0xe50c5606, /// 0xf6c
			 0x4a2186c7, /// 0xf70
			 0xdc1c9543, /// 0xf74
			 0x73567a2d, /// 0xf78
			 0x51b8d32b, /// 0xf7c
			 0xf5d9756f, /// 0xf80
			 0x866dd5e2, /// 0xf84
			 0xb1b4cfca, /// 0xf88
			 0x74b5d76b, /// 0xf8c
			 0x49303a0f, /// 0xf90
			 0x09694199, /// 0xf94
			 0x1bfde171, /// 0xf98
			 0xaf8d2876, /// 0xf9c
			 0x0b625e8f, /// 0xfa0
			 0x7bce1031, /// 0xfa4
			 0x30055e74, /// 0xfa8
			 0x93b30ae6, /// 0xfac
			 0xb4137db9, /// 0xfb0
			 0x0f4fd9b0, /// 0xfb4
			 0x383e7651, /// 0xfb8
			 0xfad22fd4, /// 0xfbc
			 0x391520fb, /// 0xfc0
			 0x7e0f27f0, /// 0xfc4
			 0xa99e1ffe, /// 0xfc8
			 0xcd24d6b5, /// 0xfcc
			 0xac71ad30, /// 0xfd0
			 0x444045e8, /// 0xfd4
			 0x55ed353f, /// 0xfd8
			 0x13e6b9d7, /// 0xfdc
			 0x8b260c40, /// 0xfe0
			 0x09fd2d27, /// 0xfe4
			 0x1b1d5ec0, /// 0xfe8
			 0x941450df, /// 0xfec
			 0x09ac0b83, /// 0xff0
			 0xb2e8a9e5, /// 0xff4
			 0x8b061757, /// 0xff8
			 0x17c0c39d /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x21eeb930, /// 0x0
			 0xc0276168, /// 0x4
			 0x46da45ff, /// 0x8
			 0x96c320f6, /// 0xc
			 0x3064e648, /// 0x10
			 0x648db18e, /// 0x14
			 0x274fc13b, /// 0x18
			 0xa6639c04, /// 0x1c
			 0x22cea927, /// 0x20
			 0x6f58df37, /// 0x24
			 0x3ed371d7, /// 0x28
			 0xc28a30b3, /// 0x2c
			 0xb5582870, /// 0x30
			 0x875f97ea, /// 0x34
			 0xb8bb5a34, /// 0x38
			 0x82a3a49d, /// 0x3c
			 0xf3cc35cb, /// 0x40
			 0xf01bdd61, /// 0x44
			 0x3b5c3315, /// 0x48
			 0x1a0dfc73, /// 0x4c
			 0xc627af84, /// 0x50
			 0x107af840, /// 0x54
			 0x52f2ed31, /// 0x58
			 0x45d8017a, /// 0x5c
			 0xe68e6b96, /// 0x60
			 0xd3cbba2e, /// 0x64
			 0x0cbfff4b, /// 0x68
			 0xed188b95, /// 0x6c
			 0x8822006f, /// 0x70
			 0xd48e474e, /// 0x74
			 0xab367074, /// 0x78
			 0xe0643150, /// 0x7c
			 0xd8bd823b, /// 0x80
			 0xefa87541, /// 0x84
			 0xedd9fd92, /// 0x88
			 0xea6e38cd, /// 0x8c
			 0xb21e42eb, /// 0x90
			 0xd26a16fc, /// 0x94
			 0x08794fda, /// 0x98
			 0xb372b370, /// 0x9c
			 0x31a9d367, /// 0xa0
			 0x96ae01b7, /// 0xa4
			 0xa39ab164, /// 0xa8
			 0x335746f5, /// 0xac
			 0xb1da8e71, /// 0xb0
			 0xb3e872d7, /// 0xb4
			 0x6a2f056e, /// 0xb8
			 0x5970ed65, /// 0xbc
			 0xe8530510, /// 0xc0
			 0x5ff1b8de, /// 0xc4
			 0x1b348adc, /// 0xc8
			 0x560e630c, /// 0xcc
			 0x4596ffb2, /// 0xd0
			 0x88c83a6d, /// 0xd4
			 0x0039c44b, /// 0xd8
			 0x9d33f2ea, /// 0xdc
			 0xc8ef0839, /// 0xe0
			 0x30db0d99, /// 0xe4
			 0x98626380, /// 0xe8
			 0xaf8c328b, /// 0xec
			 0x0cdb8bc0, /// 0xf0
			 0xdcb79992, /// 0xf4
			 0x9e6ba200, /// 0xf8
			 0x8632b44c, /// 0xfc
			 0x0e9a2ed4, /// 0x100
			 0x262245d5, /// 0x104
			 0xc69a2fb2, /// 0x108
			 0x5030a65a, /// 0x10c
			 0x179e2c60, /// 0x110
			 0xf93ce6d5, /// 0x114
			 0xd4b07018, /// 0x118
			 0x796f9d38, /// 0x11c
			 0x9afbc55c, /// 0x120
			 0x6504df1d, /// 0x124
			 0x54cce8df, /// 0x128
			 0x2e95a644, /// 0x12c
			 0x3a9d9d98, /// 0x130
			 0x9fbabd24, /// 0x134
			 0x60793599, /// 0x138
			 0x5e41e656, /// 0x13c
			 0x31cf14a6, /// 0x140
			 0xdf44c0be, /// 0x144
			 0xdfeef520, /// 0x148
			 0x8f909ede, /// 0x14c
			 0xce8fd728, /// 0x150
			 0x4686ad57, /// 0x154
			 0x444ac11f, /// 0x158
			 0x1c903042, /// 0x15c
			 0xb2103126, /// 0x160
			 0xb628fcee, /// 0x164
			 0x23ecd19e, /// 0x168
			 0x251ba99f, /// 0x16c
			 0xa01ee984, /// 0x170
			 0xe8ef414a, /// 0x174
			 0xb07aa578, /// 0x178
			 0x1ed103b1, /// 0x17c
			 0x2499ad6e, /// 0x180
			 0x44449530, /// 0x184
			 0x2f306728, /// 0x188
			 0xc4c201fb, /// 0x18c
			 0xc81ce67e, /// 0x190
			 0x7aea2b93, /// 0x194
			 0x07e0046d, /// 0x198
			 0x87bbf668, /// 0x19c
			 0xf8e7b4c8, /// 0x1a0
			 0xee5dbd1e, /// 0x1a4
			 0x7ab013f8, /// 0x1a8
			 0x54ad4d82, /// 0x1ac
			 0x068014b7, /// 0x1b0
			 0x5419ff80, /// 0x1b4
			 0x21e640d4, /// 0x1b8
			 0x55c5e241, /// 0x1bc
			 0x5448eb04, /// 0x1c0
			 0xd96d9620, /// 0x1c4
			 0x5567fa71, /// 0x1c8
			 0x525fdb0b, /// 0x1cc
			 0x17dd3744, /// 0x1d0
			 0x7ae1105a, /// 0x1d4
			 0x20d52dd6, /// 0x1d8
			 0x6c4f4102, /// 0x1dc
			 0xdb93653f, /// 0x1e0
			 0x358a53f6, /// 0x1e4
			 0xa5eb37d0, /// 0x1e8
			 0x102b9803, /// 0x1ec
			 0x84d68920, /// 0x1f0
			 0x25aeac6f, /// 0x1f4
			 0xbf01690f, /// 0x1f8
			 0x453061eb, /// 0x1fc
			 0xed604bd0, /// 0x200
			 0xda28b5e2, /// 0x204
			 0xc867bf91, /// 0x208
			 0xa06e90f6, /// 0x20c
			 0x1422b8ec, /// 0x210
			 0xbfbe7b82, /// 0x214
			 0x85027ff8, /// 0x218
			 0xfb5be7fc, /// 0x21c
			 0x86593634, /// 0x220
			 0xe0172642, /// 0x224
			 0xf67a135c, /// 0x228
			 0xa32251cd, /// 0x22c
			 0xa297bc68, /// 0x230
			 0xe0ab7caf, /// 0x234
			 0x491c49e7, /// 0x238
			 0x41948562, /// 0x23c
			 0x59bab275, /// 0x240
			 0x8f702111, /// 0x244
			 0x9399e117, /// 0x248
			 0x166df072, /// 0x24c
			 0x5422cc0c, /// 0x250
			 0x1275c5f7, /// 0x254
			 0x48f4486c, /// 0x258
			 0x4a70d6be, /// 0x25c
			 0xce04ed9c, /// 0x260
			 0xff3e376f, /// 0x264
			 0x53f26b13, /// 0x268
			 0xfe46f65b, /// 0x26c
			 0x2b319463, /// 0x270
			 0x7cf7aab3, /// 0x274
			 0xecb2efe6, /// 0x278
			 0xaae796ec, /// 0x27c
			 0x4ec8708f, /// 0x280
			 0xb6f29af7, /// 0x284
			 0x3cd5fc40, /// 0x288
			 0x797cd708, /// 0x28c
			 0xe10ef2d8, /// 0x290
			 0x0b9e7d5d, /// 0x294
			 0xd6a0c5d1, /// 0x298
			 0xc9bff07a, /// 0x29c
			 0xe68cfb9a, /// 0x2a0
			 0xfcb183a8, /// 0x2a4
			 0x96bd238d, /// 0x2a8
			 0x3b8f29c5, /// 0x2ac
			 0x153c2852, /// 0x2b0
			 0xc69dc39c, /// 0x2b4
			 0x03624c2a, /// 0x2b8
			 0x99925c60, /// 0x2bc
			 0x21320970, /// 0x2c0
			 0x0f486ae1, /// 0x2c4
			 0xed9c63be, /// 0x2c8
			 0x088484b8, /// 0x2cc
			 0xebacef6a, /// 0x2d0
			 0x7b586770, /// 0x2d4
			 0x0e0fad88, /// 0x2d8
			 0x7156aeb7, /// 0x2dc
			 0x17899d38, /// 0x2e0
			 0x915b0a3a, /// 0x2e4
			 0x374f47fd, /// 0x2e8
			 0x9de36b0c, /// 0x2ec
			 0xbdd283be, /// 0x2f0
			 0x03effc7a, /// 0x2f4
			 0x5aa1c0fe, /// 0x2f8
			 0xa2d58773, /// 0x2fc
			 0xf76e24c0, /// 0x300
			 0xb2ccb616, /// 0x304
			 0xd1bf6120, /// 0x308
			 0xabeba870, /// 0x30c
			 0xd2fb5a39, /// 0x310
			 0x0754274f, /// 0x314
			 0xe41669a3, /// 0x318
			 0x8c1eceac, /// 0x31c
			 0x3c3d57de, /// 0x320
			 0x79edea7a, /// 0x324
			 0xff8b3df8, /// 0x328
			 0x6b17ef73, /// 0x32c
			 0x2f5faca2, /// 0x330
			 0xcce296d8, /// 0x334
			 0x286a4886, /// 0x338
			 0x022929f6, /// 0x33c
			 0xd4d55a4e, /// 0x340
			 0x69c80472, /// 0x344
			 0x15da179d, /// 0x348
			 0xb8d1f2ef, /// 0x34c
			 0x28b87c38, /// 0x350
			 0x2e49bfc9, /// 0x354
			 0x2fb3eb10, /// 0x358
			 0x6ce26f9d, /// 0x35c
			 0x198bf3a7, /// 0x360
			 0x8701f0c6, /// 0x364
			 0x1977daa0, /// 0x368
			 0x05f7bd5f, /// 0x36c
			 0x5a9bc9e7, /// 0x370
			 0x55754d66, /// 0x374
			 0xec0315a9, /// 0x378
			 0xc82c1042, /// 0x37c
			 0x36d373c9, /// 0x380
			 0xcc9fe620, /// 0x384
			 0x6c335a2b, /// 0x388
			 0x0ea78bfb, /// 0x38c
			 0xc88c890f, /// 0x390
			 0xbfa9a448, /// 0x394
			 0xd46fb1e5, /// 0x398
			 0x0024bd95, /// 0x39c
			 0xb96d98c9, /// 0x3a0
			 0x309018dc, /// 0x3a4
			 0xb4ab5ed9, /// 0x3a8
			 0x7094c4d0, /// 0x3ac
			 0x16edcc61, /// 0x3b0
			 0xc88b4655, /// 0x3b4
			 0xab12332e, /// 0x3b8
			 0x253846e7, /// 0x3bc
			 0x0ea2e66c, /// 0x3c0
			 0xbea2c72d, /// 0x3c4
			 0xc50b65c0, /// 0x3c8
			 0xbfd4da5a, /// 0x3cc
			 0xbeb79a94, /// 0x3d0
			 0x69eeb5be, /// 0x3d4
			 0x3ae94695, /// 0x3d8
			 0xac4b32ce, /// 0x3dc
			 0x1a8f2638, /// 0x3e0
			 0xec1946d4, /// 0x3e4
			 0xbb2b7271, /// 0x3e8
			 0x2a5d1ca8, /// 0x3ec
			 0x3577affa, /// 0x3f0
			 0x59d7fcbc, /// 0x3f4
			 0x6fc4f88b, /// 0x3f8
			 0x690c7532, /// 0x3fc
			 0xefbb5019, /// 0x400
			 0x9e2883dd, /// 0x404
			 0x87704ee0, /// 0x408
			 0xc5d889d7, /// 0x40c
			 0x4a207b87, /// 0x410
			 0x6aaecb57, /// 0x414
			 0x67054ed8, /// 0x418
			 0xeae83636, /// 0x41c
			 0x8ed2987d, /// 0x420
			 0x97710127, /// 0x424
			 0xd7d6e4d6, /// 0x428
			 0x8013bf3d, /// 0x42c
			 0xa7366db6, /// 0x430
			 0x6a475c04, /// 0x434
			 0xf37becb5, /// 0x438
			 0x63ac204f, /// 0x43c
			 0xc406cfb8, /// 0x440
			 0x4863a549, /// 0x444
			 0xcc3cb43c, /// 0x448
			 0x14a6be06, /// 0x44c
			 0xaa0bdb96, /// 0x450
			 0xf7dba015, /// 0x454
			 0x56d7cc32, /// 0x458
			 0x26452783, /// 0x45c
			 0x71564b05, /// 0x460
			 0x7027f752, /// 0x464
			 0xe9fea311, /// 0x468
			 0xf34f2890, /// 0x46c
			 0x9e4574db, /// 0x470
			 0x74b9ea15, /// 0x474
			 0x53f5330b, /// 0x478
			 0x017ca1fd, /// 0x47c
			 0x258e959e, /// 0x480
			 0xc2ee67b2, /// 0x484
			 0x9b8041a3, /// 0x488
			 0x10186d7a, /// 0x48c
			 0x7161e96e, /// 0x490
			 0xd66c1992, /// 0x494
			 0x0d4b1eff, /// 0x498
			 0xfcb47e59, /// 0x49c
			 0x52a98c6c, /// 0x4a0
			 0x45494c61, /// 0x4a4
			 0x54795f10, /// 0x4a8
			 0x234d187d, /// 0x4ac
			 0x48200dae, /// 0x4b0
			 0xc853cefd, /// 0x4b4
			 0x1c4d32d8, /// 0x4b8
			 0x98a354d4, /// 0x4bc
			 0xbda247b8, /// 0x4c0
			 0x953ebf21, /// 0x4c4
			 0x232cdd1e, /// 0x4c8
			 0x69101385, /// 0x4cc
			 0xfae9f0ff, /// 0x4d0
			 0xb71f82c6, /// 0x4d4
			 0xfd3bd092, /// 0x4d8
			 0x6d59ecc0, /// 0x4dc
			 0x621bada8, /// 0x4e0
			 0x3d0bb616, /// 0x4e4
			 0x34fdd883, /// 0x4e8
			 0xfb3d5672, /// 0x4ec
			 0x34549039, /// 0x4f0
			 0x478a8bd0, /// 0x4f4
			 0x80758bc9, /// 0x4f8
			 0xbd4f9f69, /// 0x4fc
			 0x07204979, /// 0x500
			 0x970943d5, /// 0x504
			 0x4b0f73d8, /// 0x508
			 0xb167e83d, /// 0x50c
			 0xdbcbb23c, /// 0x510
			 0x4ad321a1, /// 0x514
			 0xfee9f3b0, /// 0x518
			 0x41debfb9, /// 0x51c
			 0xe96038ad, /// 0x520
			 0x82bf52b3, /// 0x524
			 0x944adf31, /// 0x528
			 0x5a08fba3, /// 0x52c
			 0x7b8981fc, /// 0x530
			 0xc5257e5d, /// 0x534
			 0xaa36dde6, /// 0x538
			 0xa5bfb016, /// 0x53c
			 0x04b2460d, /// 0x540
			 0xbdfd2543, /// 0x544
			 0x5cfac7ba, /// 0x548
			 0x76f1844a, /// 0x54c
			 0xe668eae9, /// 0x550
			 0xd2359df1, /// 0x554
			 0x4bb3d875, /// 0x558
			 0xa62b568e, /// 0x55c
			 0x1f9ef735, /// 0x560
			 0x445489fe, /// 0x564
			 0x217db911, /// 0x568
			 0x8c5f2aa0, /// 0x56c
			 0x2ad09720, /// 0x570
			 0x9a3bf147, /// 0x574
			 0x0a9887f1, /// 0x578
			 0xd112dd7a, /// 0x57c
			 0x703cfa5e, /// 0x580
			 0x0be1ebb0, /// 0x584
			 0xa6e8663b, /// 0x588
			 0x66416584, /// 0x58c
			 0xa39037c5, /// 0x590
			 0xaf457235, /// 0x594
			 0x4dce1aba, /// 0x598
			 0x985aa6d1, /// 0x59c
			 0xd9c23a95, /// 0x5a0
			 0x05cf381c, /// 0x5a4
			 0x53c8db05, /// 0x5a8
			 0xe3c918e4, /// 0x5ac
			 0x09d8c5e6, /// 0x5b0
			 0x8ddb99fa, /// 0x5b4
			 0xb0db1e62, /// 0x5b8
			 0x362db8d0, /// 0x5bc
			 0xcc7e19f8, /// 0x5c0
			 0x34253cc7, /// 0x5c4
			 0x626b2f7c, /// 0x5c8
			 0x58ba2bfc, /// 0x5cc
			 0x7ce04bde, /// 0x5d0
			 0x69873f64, /// 0x5d4
			 0x8e96b8b4, /// 0x5d8
			 0x9238cebe, /// 0x5dc
			 0x8b844e43, /// 0x5e0
			 0x9905d11d, /// 0x5e4
			 0x540d3c52, /// 0x5e8
			 0x1ddeba62, /// 0x5ec
			 0xae81e2a5, /// 0x5f0
			 0xaa792325, /// 0x5f4
			 0xb06c7260, /// 0x5f8
			 0xe901fbad, /// 0x5fc
			 0x6e4a2f66, /// 0x600
			 0x6e8feb3d, /// 0x604
			 0x782a39cd, /// 0x608
			 0x5001d161, /// 0x60c
			 0xd9ac2f9a, /// 0x610
			 0x524a5193, /// 0x614
			 0x2ff68a9c, /// 0x618
			 0xa8958cbe, /// 0x61c
			 0x9d9f7481, /// 0x620
			 0xea697364, /// 0x624
			 0x4d9a1398, /// 0x628
			 0x288db1b8, /// 0x62c
			 0xf1015907, /// 0x630
			 0xd4215c15, /// 0x634
			 0xa948c053, /// 0x638
			 0x2cac8bea, /// 0x63c
			 0xd956683b, /// 0x640
			 0x0f64e60c, /// 0x644
			 0x3d587b4b, /// 0x648
			 0xdc4fd519, /// 0x64c
			 0x2af45b72, /// 0x650
			 0xa4b49c2d, /// 0x654
			 0x02045a78, /// 0x658
			 0x8384d8f9, /// 0x65c
			 0xf73dd792, /// 0x660
			 0x1f9d41f7, /// 0x664
			 0x6c2daa7b, /// 0x668
			 0x9db12f50, /// 0x66c
			 0x20073b37, /// 0x670
			 0xc520c6a7, /// 0x674
			 0x2022133b, /// 0x678
			 0x14e720d6, /// 0x67c
			 0xa6ea2e4f, /// 0x680
			 0x62de24d0, /// 0x684
			 0xe936e878, /// 0x688
			 0x09f1bb88, /// 0x68c
			 0x33c08b65, /// 0x690
			 0x34e56bac, /// 0x694
			 0xba36d2f3, /// 0x698
			 0x95aa2fcc, /// 0x69c
			 0xc8221e3b, /// 0x6a0
			 0x2b5f0f25, /// 0x6a4
			 0x24c5f6ae, /// 0x6a8
			 0x2fd8cccb, /// 0x6ac
			 0x6ba76699, /// 0x6b0
			 0xedb531a4, /// 0x6b4
			 0xb15cd89e, /// 0x6b8
			 0xe6c2377c, /// 0x6bc
			 0x26921438, /// 0x6c0
			 0xf426c8a9, /// 0x6c4
			 0x0aa22efa, /// 0x6c8
			 0x44956409, /// 0x6cc
			 0x4446e7ef, /// 0x6d0
			 0x6f3cb195, /// 0x6d4
			 0xfa3aa1de, /// 0x6d8
			 0x1d9f9e1b, /// 0x6dc
			 0x418999e4, /// 0x6e0
			 0x4dd1a7ac, /// 0x6e4
			 0xba63b182, /// 0x6e8
			 0x2ffed970, /// 0x6ec
			 0x09007b88, /// 0x6f0
			 0xe1d5173d, /// 0x6f4
			 0xeadc7fc3, /// 0x6f8
			 0x34497962, /// 0x6fc
			 0xfeaf2ad8, /// 0x700
			 0x7aaee107, /// 0x704
			 0xb314d732, /// 0x708
			 0xca41d900, /// 0x70c
			 0xa7cf0996, /// 0x710
			 0x4c7642bd, /// 0x714
			 0xc68cfc87, /// 0x718
			 0xd8847b90, /// 0x71c
			 0xd6c7d681, /// 0x720
			 0x55cbdf07, /// 0x724
			 0x9811cff0, /// 0x728
			 0x19cc0ea4, /// 0x72c
			 0x8928de43, /// 0x730
			 0x19cc7805, /// 0x734
			 0x4dc83701, /// 0x738
			 0x2f248f4f, /// 0x73c
			 0x43f3277d, /// 0x740
			 0x353c97cf, /// 0x744
			 0xfc5927d4, /// 0x748
			 0x7bf96226, /// 0x74c
			 0x963f4461, /// 0x750
			 0x238e8314, /// 0x754
			 0x9cc27903, /// 0x758
			 0x01eefe8c, /// 0x75c
			 0xb250f703, /// 0x760
			 0xa9e18893, /// 0x764
			 0xda3259c1, /// 0x768
			 0x690a445d, /// 0x76c
			 0x87c17c26, /// 0x770
			 0x33ea3571, /// 0x774
			 0x7387e2a5, /// 0x778
			 0xa26129ac, /// 0x77c
			 0xf669e5b3, /// 0x780
			 0x653cc838, /// 0x784
			 0x663e592e, /// 0x788
			 0xe8bf57d5, /// 0x78c
			 0x63d98adf, /// 0x790
			 0xb41ddf06, /// 0x794
			 0x008e747c, /// 0x798
			 0xe47d2995, /// 0x79c
			 0xcc50d83c, /// 0x7a0
			 0xacbb4dfb, /// 0x7a4
			 0x642fdb58, /// 0x7a8
			 0x45661cf2, /// 0x7ac
			 0x1e427558, /// 0x7b0
			 0xff4286d0, /// 0x7b4
			 0x0c184b1b, /// 0x7b8
			 0x4d9b1a7d, /// 0x7bc
			 0x11b9e89f, /// 0x7c0
			 0x9a7d9474, /// 0x7c4
			 0x57cf927b, /// 0x7c8
			 0x17f9669b, /// 0x7cc
			 0xef7f1b51, /// 0x7d0
			 0x2978d1ea, /// 0x7d4
			 0xa125aeda, /// 0x7d8
			 0x8cc2edc0, /// 0x7dc
			 0xb1ac2fc6, /// 0x7e0
			 0x7ec02a4d, /// 0x7e4
			 0x9b4ac802, /// 0x7e8
			 0xab0058d0, /// 0x7ec
			 0x6bc4b83f, /// 0x7f0
			 0x8be8dccc, /// 0x7f4
			 0x10ce90e0, /// 0x7f8
			 0x68d5ef03, /// 0x7fc
			 0xfaefb101, /// 0x800
			 0xc566aae9, /// 0x804
			 0xa1c1fbce, /// 0x808
			 0x574b1d0b, /// 0x80c
			 0x1f0f617e, /// 0x810
			 0xe70c0e55, /// 0x814
			 0xab5ef167, /// 0x818
			 0x7fb8c6e9, /// 0x81c
			 0x5ac09083, /// 0x820
			 0x2f19b58a, /// 0x824
			 0x163f922d, /// 0x828
			 0xeafb62cb, /// 0x82c
			 0xe488caa3, /// 0x830
			 0xada0559a, /// 0x834
			 0xecab0940, /// 0x838
			 0x4ea35636, /// 0x83c
			 0x2c30b9f5, /// 0x840
			 0x634c5bab, /// 0x844
			 0xc2bfe7f1, /// 0x848
			 0x7f7c5149, /// 0x84c
			 0x8bb71dbe, /// 0x850
			 0x3cc8b306, /// 0x854
			 0xf2e5c6ac, /// 0x858
			 0xe9118f86, /// 0x85c
			 0x1185cf0a, /// 0x860
			 0x108d0039, /// 0x864
			 0x9b8cfc8e, /// 0x868
			 0xea34acab, /// 0x86c
			 0xd828842f, /// 0x870
			 0x88a8a4d2, /// 0x874
			 0x1d33936c, /// 0x878
			 0x3fe0ceda, /// 0x87c
			 0xd338100e, /// 0x880
			 0x27a04a25, /// 0x884
			 0xfa6b14d0, /// 0x888
			 0x11c4e283, /// 0x88c
			 0x351cc55f, /// 0x890
			 0xf25115e2, /// 0x894
			 0xe324b14e, /// 0x898
			 0xc32558af, /// 0x89c
			 0xabc18868, /// 0x8a0
			 0x8274a0a3, /// 0x8a4
			 0x6738106d, /// 0x8a8
			 0xdb345209, /// 0x8ac
			 0xd3acd2ee, /// 0x8b0
			 0x3a2170d2, /// 0x8b4
			 0x06f7ed31, /// 0x8b8
			 0x745c0c35, /// 0x8bc
			 0xf47cf8bc, /// 0x8c0
			 0x6bf200fb, /// 0x8c4
			 0x77c64e35, /// 0x8c8
			 0x04bfc33d, /// 0x8cc
			 0x7178daa9, /// 0x8d0
			 0x27c7d9b9, /// 0x8d4
			 0x1cba4e9d, /// 0x8d8
			 0x778a4704, /// 0x8dc
			 0xe176470e, /// 0x8e0
			 0x6e4bd249, /// 0x8e4
			 0xca368b5c, /// 0x8e8
			 0x55d5d724, /// 0x8ec
			 0x9184a9ad, /// 0x8f0
			 0x5dbfb07b, /// 0x8f4
			 0x414ea1e2, /// 0x8f8
			 0x72428e13, /// 0x8fc
			 0x7eb01ace, /// 0x900
			 0x2c7b5f05, /// 0x904
			 0x00467212, /// 0x908
			 0x49b540b3, /// 0x90c
			 0xf7760a92, /// 0x910
			 0xa906f960, /// 0x914
			 0x24be7454, /// 0x918
			 0xdadbed8e, /// 0x91c
			 0x3f5697be, /// 0x920
			 0xf41dd87e, /// 0x924
			 0x2c7028cb, /// 0x928
			 0xe129884b, /// 0x92c
			 0xbb79d78b, /// 0x930
			 0x0417c70d, /// 0x934
			 0x6746eec2, /// 0x938
			 0x1703df8a, /// 0x93c
			 0x6f092212, /// 0x940
			 0x96303f20, /// 0x944
			 0x00f70be5, /// 0x948
			 0xb90c75e7, /// 0x94c
			 0x775529c7, /// 0x950
			 0xfa9caa22, /// 0x954
			 0xf2cc11f2, /// 0x958
			 0x25b4991e, /// 0x95c
			 0x594ff173, /// 0x960
			 0x846f563e, /// 0x964
			 0x4fab37ed, /// 0x968
			 0xc005c080, /// 0x96c
			 0x3888e24d, /// 0x970
			 0x9d5d03d8, /// 0x974
			 0xedd0fb5e, /// 0x978
			 0xaec0ac3d, /// 0x97c
			 0xa6a8174d, /// 0x980
			 0xee8b35eb, /// 0x984
			 0xef86b8fc, /// 0x988
			 0xa88a7e4a, /// 0x98c
			 0xa00d9b75, /// 0x990
			 0x76361d05, /// 0x994
			 0x908ccefc, /// 0x998
			 0x992193bc, /// 0x99c
			 0xd22c81bc, /// 0x9a0
			 0xde55313f, /// 0x9a4
			 0xa318c787, /// 0x9a8
			 0x862a5922, /// 0x9ac
			 0xeac326b6, /// 0x9b0
			 0xc771bde2, /// 0x9b4
			 0xeafa0e4e, /// 0x9b8
			 0x9df79021, /// 0x9bc
			 0xa4835790, /// 0x9c0
			 0x2aa1757e, /// 0x9c4
			 0xe9e62ce0, /// 0x9c8
			 0xfc6ece83, /// 0x9cc
			 0x311aee51, /// 0x9d0
			 0x60f32723, /// 0x9d4
			 0x30f74492, /// 0x9d8
			 0x738d14ca, /// 0x9dc
			 0xaa16894c, /// 0x9e0
			 0x7c8e1b2b, /// 0x9e4
			 0x6f9a1fb1, /// 0x9e8
			 0xed324860, /// 0x9ec
			 0x48d85654, /// 0x9f0
			 0x2cce5e70, /// 0x9f4
			 0x13925ddf, /// 0x9f8
			 0x02297014, /// 0x9fc
			 0x3fff4903, /// 0xa00
			 0x14c0ffe4, /// 0xa04
			 0xa2fdf5f8, /// 0xa08
			 0x1decb0be, /// 0xa0c
			 0x358416c1, /// 0xa10
			 0x0a36887b, /// 0xa14
			 0x5c9d1bb1, /// 0xa18
			 0x03a28265, /// 0xa1c
			 0x67fef566, /// 0xa20
			 0x65477a20, /// 0xa24
			 0x4009bdd8, /// 0xa28
			 0xd2860ed4, /// 0xa2c
			 0x44162195, /// 0xa30
			 0x88b6ec94, /// 0xa34
			 0xf1764da8, /// 0xa38
			 0x6bd40608, /// 0xa3c
			 0x862ebc58, /// 0xa40
			 0xd0bf13e0, /// 0xa44
			 0x977afb04, /// 0xa48
			 0x2a0a51e8, /// 0xa4c
			 0xe2f12c75, /// 0xa50
			 0x267f7fc0, /// 0xa54
			 0x5f246eda, /// 0xa58
			 0x7f2063c5, /// 0xa5c
			 0x63cfce57, /// 0xa60
			 0xd9b58dbc, /// 0xa64
			 0x914d97fc, /// 0xa68
			 0x99179b5e, /// 0xa6c
			 0xfc235a52, /// 0xa70
			 0x220ae183, /// 0xa74
			 0x4cb54ec8, /// 0xa78
			 0xb64927b2, /// 0xa7c
			 0x81f06caa, /// 0xa80
			 0x213bfc76, /// 0xa84
			 0xe33857f9, /// 0xa88
			 0xd8a32702, /// 0xa8c
			 0xbe4c9a0b, /// 0xa90
			 0x0cefe381, /// 0xa94
			 0x0af6b88e, /// 0xa98
			 0x5c9b566a, /// 0xa9c
			 0x1c913784, /// 0xaa0
			 0x96f9c590, /// 0xaa4
			 0x0a5a44a1, /// 0xaa8
			 0x2a95073c, /// 0xaac
			 0xd2a75671, /// 0xab0
			 0xa6f485e6, /// 0xab4
			 0x8d4fc74e, /// 0xab8
			 0x3770d8bf, /// 0xabc
			 0x2a19c40b, /// 0xac0
			 0x9bc5d31a, /// 0xac4
			 0x2406d53e, /// 0xac8
			 0x2662c838, /// 0xacc
			 0xaf5584ab, /// 0xad0
			 0xed4987af, /// 0xad4
			 0xb49aec6f, /// 0xad8
			 0x34780a81, /// 0xadc
			 0x34bac34b, /// 0xae0
			 0xa4f04ec3, /// 0xae4
			 0x8a54d5a9, /// 0xae8
			 0x50a2dc18, /// 0xaec
			 0x7e6b32a3, /// 0xaf0
			 0xd3b31a73, /// 0xaf4
			 0xc2286931, /// 0xaf8
			 0x29204c63, /// 0xafc
			 0x50419b65, /// 0xb00
			 0xbf72b358, /// 0xb04
			 0x6837247b, /// 0xb08
			 0xb07f7454, /// 0xb0c
			 0x038b2753, /// 0xb10
			 0x170a8760, /// 0xb14
			 0xa58e1d4f, /// 0xb18
			 0x4e6a5f66, /// 0xb1c
			 0x6b3c7b26, /// 0xb20
			 0x2fbd17ce, /// 0xb24
			 0xe79b2c08, /// 0xb28
			 0xa46f8b3f, /// 0xb2c
			 0xc2b100c8, /// 0xb30
			 0xcb765119, /// 0xb34
			 0x534e003f, /// 0xb38
			 0x30091419, /// 0xb3c
			 0x5ec64bdc, /// 0xb40
			 0x34caa177, /// 0xb44
			 0x859d046c, /// 0xb48
			 0x30812ce0, /// 0xb4c
			 0x4e67e9ed, /// 0xb50
			 0xe67c19d5, /// 0xb54
			 0x1156ffb5, /// 0xb58
			 0xd95ec737, /// 0xb5c
			 0x938b4626, /// 0xb60
			 0x6f108c20, /// 0xb64
			 0xa6f4c8f2, /// 0xb68
			 0x7f7eb42b, /// 0xb6c
			 0xef9992ff, /// 0xb70
			 0xe0446d94, /// 0xb74
			 0x84504347, /// 0xb78
			 0x4c159b5c, /// 0xb7c
			 0x68262e65, /// 0xb80
			 0xd3020b4e, /// 0xb84
			 0x1738ea7e, /// 0xb88
			 0x3e322abb, /// 0xb8c
			 0x5874a72b, /// 0xb90
			 0xd5d4d618, /// 0xb94
			 0xc742dc90, /// 0xb98
			 0xa657f162, /// 0xb9c
			 0x218e1596, /// 0xba0
			 0x614e955c, /// 0xba4
			 0xb62a6447, /// 0xba8
			 0xeeef1ca1, /// 0xbac
			 0xe93aa878, /// 0xbb0
			 0x682f6622, /// 0xbb4
			 0x405d574d, /// 0xbb8
			 0xa7b2623d, /// 0xbbc
			 0x1e8b2b1e, /// 0xbc0
			 0x30e8bdf5, /// 0xbc4
			 0x0e3dc474, /// 0xbc8
			 0xf8ce58f6, /// 0xbcc
			 0x52d36305, /// 0xbd0
			 0xf25fa5b1, /// 0xbd4
			 0xff84ca25, /// 0xbd8
			 0x0eb08bd9, /// 0xbdc
			 0x28dc6fdf, /// 0xbe0
			 0xc6235e13, /// 0xbe4
			 0x99ee8826, /// 0xbe8
			 0xe1e58bf9, /// 0xbec
			 0x0b3ddc5d, /// 0xbf0
			 0x1d26933f, /// 0xbf4
			 0x52612fa9, /// 0xbf8
			 0xce1b11c3, /// 0xbfc
			 0x15e3637d, /// 0xc00
			 0xf6583bae, /// 0xc04
			 0x3c67eb53, /// 0xc08
			 0x2d5dde34, /// 0xc0c
			 0xfe0f7ad9, /// 0xc10
			 0x8ac38c17, /// 0xc14
			 0x23f5c324, /// 0xc18
			 0xeedf3cba, /// 0xc1c
			 0xf69547e8, /// 0xc20
			 0x1bc8051d, /// 0xc24
			 0xc4aef072, /// 0xc28
			 0xa40f16bc, /// 0xc2c
			 0x0778a154, /// 0xc30
			 0x3b8fac71, /// 0xc34
			 0x90ffd14c, /// 0xc38
			 0xe6db5aae, /// 0xc3c
			 0x4b75f5d6, /// 0xc40
			 0x3edf5076, /// 0xc44
			 0xecc818fa, /// 0xc48
			 0xaf8d6f0a, /// 0xc4c
			 0x39190177, /// 0xc50
			 0x508632a0, /// 0xc54
			 0x9c161d45, /// 0xc58
			 0x76ce7749, /// 0xc5c
			 0x63b8fcb6, /// 0xc60
			 0x23598e2b, /// 0xc64
			 0xea62b0c0, /// 0xc68
			 0x804dd0a3, /// 0xc6c
			 0x572b946a, /// 0xc70
			 0x59f3d253, /// 0xc74
			 0x4b24590e, /// 0xc78
			 0x3bba6242, /// 0xc7c
			 0x57cbf6f4, /// 0xc80
			 0xc4d9e04a, /// 0xc84
			 0xe252156d, /// 0xc88
			 0xdce577e9, /// 0xc8c
			 0xc240a75f, /// 0xc90
			 0x681a5ad5, /// 0xc94
			 0xfaaf10b3, /// 0xc98
			 0xbf9cb0b3, /// 0xc9c
			 0x72fc975f, /// 0xca0
			 0x1e970bcf, /// 0xca4
			 0xb4c4f0cf, /// 0xca8
			 0x7f6f3334, /// 0xcac
			 0x060ca4bb, /// 0xcb0
			 0x8d545c22, /// 0xcb4
			 0x27d47540, /// 0xcb8
			 0x86b63762, /// 0xcbc
			 0xfb34349c, /// 0xcc0
			 0x250ffdf2, /// 0xcc4
			 0x335b4529, /// 0xcc8
			 0x4c90e62f, /// 0xccc
			 0x89d34cdb, /// 0xcd0
			 0xc92904b6, /// 0xcd4
			 0x2b21d4e8, /// 0xcd8
			 0xcd9df373, /// 0xcdc
			 0xe6f5daee, /// 0xce0
			 0xcc8bea80, /// 0xce4
			 0x90464356, /// 0xce8
			 0x99b96b0a, /// 0xcec
			 0x684d359e, /// 0xcf0
			 0xc44b7ea0, /// 0xcf4
			 0x7816c017, /// 0xcf8
			 0x47b428c5, /// 0xcfc
			 0xb91f1c2e, /// 0xd00
			 0x52c5edd2, /// 0xd04
			 0xa718af57, /// 0xd08
			 0xcbe18312, /// 0xd0c
			 0x45ca9bf7, /// 0xd10
			 0x29e99129, /// 0xd14
			 0xaa5f3067, /// 0xd18
			 0xe6834f98, /// 0xd1c
			 0x0cd06035, /// 0xd20
			 0x2f9a48be, /// 0xd24
			 0x780221f4, /// 0xd28
			 0x91d91247, /// 0xd2c
			 0x8c948db1, /// 0xd30
			 0x2ccece7e, /// 0xd34
			 0xed7bddc8, /// 0xd38
			 0x02239c22, /// 0xd3c
			 0xcd35f1de, /// 0xd40
			 0x666ff767, /// 0xd44
			 0x87e6a6d4, /// 0xd48
			 0x948461ac, /// 0xd4c
			 0x441e02e1, /// 0xd50
			 0xa321afc1, /// 0xd54
			 0x343881b4, /// 0xd58
			 0xe7728363, /// 0xd5c
			 0x2f396728, /// 0xd60
			 0x3f7f2f41, /// 0xd64
			 0x84359105, /// 0xd68
			 0x28ddbccd, /// 0xd6c
			 0xca94b59c, /// 0xd70
			 0x8991eef6, /// 0xd74
			 0xd7c2c287, /// 0xd78
			 0x286bcc5c, /// 0xd7c
			 0x1f9336cc, /// 0xd80
			 0xc6962d6f, /// 0xd84
			 0xa9756797, /// 0xd88
			 0x06d29e25, /// 0xd8c
			 0x7fbddb95, /// 0xd90
			 0x261cd7e7, /// 0xd94
			 0x60c0ea86, /// 0xd98
			 0x015e339b, /// 0xd9c
			 0x3aa87042, /// 0xda0
			 0x2c0ff893, /// 0xda4
			 0x445b7a8a, /// 0xda8
			 0x7ef08baa, /// 0xdac
			 0xd5bcdd9a, /// 0xdb0
			 0x4ec037e4, /// 0xdb4
			 0x14ff4d26, /// 0xdb8
			 0x17ae6729, /// 0xdbc
			 0x9d65f5fe, /// 0xdc0
			 0x411f905e, /// 0xdc4
			 0xf8e24414, /// 0xdc8
			 0x5f23d307, /// 0xdcc
			 0x5588e7e0, /// 0xdd0
			 0x48dc4c88, /// 0xdd4
			 0x7a2c48a9, /// 0xdd8
			 0x7e939a8e, /// 0xddc
			 0x72fe6f96, /// 0xde0
			 0x50c3e960, /// 0xde4
			 0x4538ba84, /// 0xde8
			 0x5e04abe5, /// 0xdec
			 0x3db9dafb, /// 0xdf0
			 0x334e256b, /// 0xdf4
			 0xdb28d506, /// 0xdf8
			 0x158326dc, /// 0xdfc
			 0x1f60c883, /// 0xe00
			 0x896304ae, /// 0xe04
			 0xf1436a2d, /// 0xe08
			 0xbd622623, /// 0xe0c
			 0x6b7aa1d9, /// 0xe10
			 0xf3100063, /// 0xe14
			 0x60522994, /// 0xe18
			 0x04e45c6a, /// 0xe1c
			 0x642f0ab1, /// 0xe20
			 0x007ebb2e, /// 0xe24
			 0x341c39ac, /// 0xe28
			 0xf90de4de, /// 0xe2c
			 0x3eee388c, /// 0xe30
			 0x4032e20f, /// 0xe34
			 0xdc6342e4, /// 0xe38
			 0x6f3a8035, /// 0xe3c
			 0x729b5289, /// 0xe40
			 0x320d0934, /// 0xe44
			 0xa57b1ff0, /// 0xe48
			 0x5883be22, /// 0xe4c
			 0x458f3da0, /// 0xe50
			 0x4bfe337a, /// 0xe54
			 0xf30abf6c, /// 0xe58
			 0xd0e63749, /// 0xe5c
			 0x66b222d2, /// 0xe60
			 0xe641d995, /// 0xe64
			 0x19d0df18, /// 0xe68
			 0xd3475364, /// 0xe6c
			 0xb085cebc, /// 0xe70
			 0xfc0bc381, /// 0xe74
			 0x96dd1601, /// 0xe78
			 0x97d48114, /// 0xe7c
			 0x724a0029, /// 0xe80
			 0xb290db04, /// 0xe84
			 0x0c2ebb0b, /// 0xe88
			 0x55a47f26, /// 0xe8c
			 0x2a96efdb, /// 0xe90
			 0xbdef4243, /// 0xe94
			 0x210f3098, /// 0xe98
			 0x17948f80, /// 0xe9c
			 0x55fc2af6, /// 0xea0
			 0x010b66da, /// 0xea4
			 0x437abd1b, /// 0xea8
			 0x6763c874, /// 0xeac
			 0xa53f927d, /// 0xeb0
			 0x29b714f9, /// 0xeb4
			 0x98ad30e0, /// 0xeb8
			 0x1e727cec, /// 0xebc
			 0x7f792dfb, /// 0xec0
			 0xe18fbde5, /// 0xec4
			 0xb3ce66a6, /// 0xec8
			 0x8bdd9cff, /// 0xecc
			 0xecf2baa3, /// 0xed0
			 0xa4c60723, /// 0xed4
			 0xf482f85f, /// 0xed8
			 0xb5aeb69f, /// 0xedc
			 0x4948b32e, /// 0xee0
			 0xfd09a921, /// 0xee4
			 0x915a1982, /// 0xee8
			 0x2325a6f6, /// 0xeec
			 0x081d7847, /// 0xef0
			 0xa548d8fe, /// 0xef4
			 0x803cb6ed, /// 0xef8
			 0x5117e779, /// 0xefc
			 0xde890c1c, /// 0xf00
			 0xc5ba9de8, /// 0xf04
			 0xcb11c558, /// 0xf08
			 0x43db907b, /// 0xf0c
			 0x690be8a9, /// 0xf10
			 0x48974878, /// 0xf14
			 0xb7e1b115, /// 0xf18
			 0x394f8ba1, /// 0xf1c
			 0xa2c1ba6b, /// 0xf20
			 0x92f15f8e, /// 0xf24
			 0x034563a1, /// 0xf28
			 0x1a528980, /// 0xf2c
			 0x7f4c1d9b, /// 0xf30
			 0x8ff7318e, /// 0xf34
			 0x6890d250, /// 0xf38
			 0x25489c8b, /// 0xf3c
			 0x05725023, /// 0xf40
			 0xe7c5e633, /// 0xf44
			 0x5570b937, /// 0xf48
			 0x38576c1d, /// 0xf4c
			 0xeb6df909, /// 0xf50
			 0x189dbb48, /// 0xf54
			 0x40c92675, /// 0xf58
			 0x001bbfb5, /// 0xf5c
			 0x38090472, /// 0xf60
			 0x0783643a, /// 0xf64
			 0x7fd6953e, /// 0xf68
			 0x4cc391ff, /// 0xf6c
			 0x1136dbda, /// 0xf70
			 0xa61c3e26, /// 0xf74
			 0xc830908a, /// 0xf78
			 0x734f33b7, /// 0xf7c
			 0x21b663da, /// 0xf80
			 0x77578a9e, /// 0xf84
			 0xc6471450, /// 0xf88
			 0xbd8c218d, /// 0xf8c
			 0x325b5021, /// 0xf90
			 0x74f1845c, /// 0xf94
			 0xb1b5756c, /// 0xf98
			 0xd7104d27, /// 0xf9c
			 0x8c23a917, /// 0xfa0
			 0x3e001999, /// 0xfa4
			 0xe187f639, /// 0xfa8
			 0x18e51677, /// 0xfac
			 0xb7b8953e, /// 0xfb0
			 0x5cd69f35, /// 0xfb4
			 0xdbf84f65, /// 0xfb8
			 0x4bcfb2ea, /// 0xfbc
			 0x304b5844, /// 0xfc0
			 0x7ea262f0, /// 0xfc4
			 0xc2ca8b78, /// 0xfc8
			 0x431361cd, /// 0xfcc
			 0x91fad39a, /// 0xfd0
			 0xba3f4c01, /// 0xfd4
			 0xc1479cb7, /// 0xfd8
			 0xc77f1872, /// 0xfdc
			 0x976c512b, /// 0xfe0
			 0x090f0220, /// 0xfe4
			 0xfe5031ec, /// 0xfe8
			 0x7588f934, /// 0xfec
			 0xf40e7845, /// 0xff0
			 0xb05101ec, /// 0xff4
			 0xbea579de, /// 0xff8
			 0x76741b18 /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00000
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00008
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0000c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00010
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00014
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0001c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00020
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00024
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00028
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0002c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00030
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0003c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00040
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00044
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00048
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0004c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00050
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00054
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00058
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0005c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00060
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00064
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00068
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0006c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00070
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00074
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00078
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0007c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00080
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00084
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00088
			 0x3f028f5c,                                                  // 0.51                                        /// 0008c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00090
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00098
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0009c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000a0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000a4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000a8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000ac
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000b0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 000b4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000bc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000c0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000c4
			 0x0c600000,                                                  // Leading 1s:                                 /// 000c8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000cc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 000d0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000d8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000dc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000e0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0x0e000007,                                                  // Trailing 1s:                                /// 000ec
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000f8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 000fc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00104
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00108
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0010c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00114
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00118
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0011c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00120
			 0x0c780000,                                                  // Leading 1s:                                 /// 00124
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00128
			 0x0c700000,                                                  // Leading 1s:                                 /// 0012c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00130
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00134
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0013c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00144
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00148
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0014c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00150
			 0x33333333,                                                  // 4 random values                             /// 00154
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00158
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0015c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00160
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00164
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00168
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0016c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00170
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00174
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00178
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0017c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00180
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00184
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00188
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0018c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00190
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00194
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00198
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0019c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 001a0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001a4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001ac
			 0x3f028f5c,                                                  // 0.51                                        /// 001b0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 001b4
			 0x7fc00001,                                                  // signaling NaN                               /// 001b8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001bc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001c0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001c4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 001c8
			 0x80000000,                                                  // -zero                                       /// 001cc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 001d0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 001d4
			 0x7f800000,                                                  // inf                                         /// 001d8
			 0x0c600000,                                                  // Leading 1s:                                 /// 001dc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 001e0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001e8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 001ec
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001f0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001f4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001f8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 001fc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00204
			 0x00011111,                                                  // 9.7958E-41                                  /// 00208
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00210
			 0x3f028f5c,                                                  // 0.51                                        /// 00214
			 0x80011111,                                                  // -9.7958E-41                                 /// 00218
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00220
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00224
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00228
			 0x0e000001,                                                  // Trailing 1s:                                /// 0022c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00230
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00234
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00238
			 0x4b000000,                                                  // 8388608.0                                   /// 0023c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00240
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00244
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00248
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0024c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00250
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00254
			 0x0c400000,                                                  // Leading 1s:                                 /// 00258
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0025c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00260
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00264
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00268
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0026c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00270
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00278
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0027c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00280
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00284
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00288
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0028c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00290
			 0x33333333,                                                  // 4 random values                             /// 00294
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00298
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0029c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 002a0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002a4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002ac
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002b4
			 0x33333333,                                                  // 4 random values                             /// 002b8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002bc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002c0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002c4
			 0xcb000000,                                                  // -8388608.0                                  /// 002c8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 002cc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002d0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002d4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002d8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002dc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 002e0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002e4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 002e8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 002f0
			 0x00000000,                                                  // zero                                        /// 002f4
			 0x0e000003,                                                  // Trailing 1s:                                /// 002f8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002fc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00300
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00304
			 0x7fc00001,                                                  // signaling NaN                               /// 00308
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0030c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00310
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0x00011111,                                                  // 9.7958E-41                                  /// 00318
			 0xff800000,                                                  // -inf                                        /// 0031c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00320
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00324
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00328
			 0x7fc00001,                                                  // signaling NaN                               /// 0032c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00334
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00338
			 0x00011111,                                                  // 9.7958E-41                                  /// 0033c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00344
			 0x80000000,                                                  // -zero                                       /// 00348
			 0xffc00001,                                                  // -signaling NaN                              /// 0034c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00354
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00358
			 0x00011111,                                                  // 9.7958E-41                                  /// 0035c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00360
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00368
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0036c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00370
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00374
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0037c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00380
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00384
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00388
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0038c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00390
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00394
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00398
			 0x0c400000,                                                  // Leading 1s:                                 /// 0039c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 003a4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003a8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 003ac
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 003b0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003b8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003bc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 003c0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 003c4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003c8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 003cc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003d0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 003d4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 003d8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003dc
			 0x00000000,                                                  // zero                                        /// 003e0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003e4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 003e8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 003ec
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 003f4
			 0x00000000,                                                  // zero                                        /// 003f8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003fc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00400
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00404
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00408
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0040c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00410
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00414
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00418
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0041c
			 0xcb000000,                                                  // -8388608.0                                  /// 00420
			 0x0c700000,                                                  // Leading 1s:                                 /// 00424
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00428
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0042c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00430
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00434
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00438
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0043c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00444
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00448
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0044c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00450
			 0xbf028f5c,                                                  // -0.51                                       /// 00454
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0045c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00460
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00464
			 0x0c700000,                                                  // Leading 1s:                                 /// 00468
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0046c
			 0x00000000,                                                  // zero                                        /// 00470
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00474
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00478
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00480
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00484
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00488
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0048c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00490
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00494
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00498
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0049c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004a8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004ac
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004b0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 004b4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004b8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 004bc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 004c0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004c4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004c8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0xff800000,                                                  // -inf                                        /// 004d0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 004e0
			 0xff800000,                                                  // -inf                                        /// 004e4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004ec
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 004f0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004f4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004f8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004fc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00500
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00504
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0050c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00510
			 0x00000000,                                                  // zero                                        /// 00514
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00518
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0051c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00520
			 0xbf028f5c,                                                  // -0.51                                       /// 00524
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00528
			 0x4b000000,                                                  // 8388608.0                                   /// 0052c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00530
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00534
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0053c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00544
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00548
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0054c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00550
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00558
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0055c
			 0x7fc00001,                                                  // signaling NaN                               /// 00560
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00564
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00568
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0056c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00570
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00574
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00578
			 0x0e000003,                                                  // Trailing 1s:                                /// 0057c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00580
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00584
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00588
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00590
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00594
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00598
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0059c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 005a0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a4
			 0x0c600000,                                                  // Leading 1s:                                 /// 005a8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 005ac
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005b8
			 0xcb000000,                                                  // -8388608.0                                  /// 005bc
			 0x80011111,                                                  // -9.7958E-41                                 /// 005c0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005c4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 005c8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 005cc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005d0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 005d4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 005d8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 005dc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005e0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 005e4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 005ec
			 0x0e001fff,                                                  // Trailing 1s:                                /// 005f0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005f4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005fc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00600
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00608
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0060c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00610
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00614
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00618
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0061c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00620
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00624
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0062c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00630
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00634
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00638
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0063c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00640
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00644
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00648
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0064c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00650
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00654
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00658
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0065c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00660
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00664
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00668
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0066c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00670
			 0x33333333,                                                  // 4 random values                             /// 00674
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00678
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00680
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00684
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00688
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00690
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00694
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00698
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0069c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 006a0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006a4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006a8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 006ac
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006b4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006bc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006c4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 006c8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 006cc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006d0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006d4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006d8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006dc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006e0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006e4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 006e8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 006ec
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006f0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x0e000003,                                                  // Trailing 1s:                                /// 006fc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00700
			 0x00011111,                                                  // 9.7958E-41                                  /// 00704
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0070c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00710
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00714
			 0xcb000000,                                                  // -8388608.0                                  /// 00718
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0071c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00720
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00724
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00728
			 0x00011111,                                                  // 9.7958E-41                                  /// 0072c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00738
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0073c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00740
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00744
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00748
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0074c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00750
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00754
			 0xcb000000,                                                  // -8388608.0                                  /// 00758
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0075c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00760
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00764
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00768
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0076c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00770
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00774
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00778
			 0x0c600000,                                                  // Leading 1s:                                 /// 0077c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00780
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00784
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00788
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0078c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00790
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00794
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00798
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0079c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007a0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 007a4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007a8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 007ac
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007b0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0xff800000,                                                  // -inf                                        /// 007b8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007bc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007c8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 007cc
			 0x33333333,                                                  // 4 random values                             /// 007d0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007dc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007e4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007ec
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 007f0
			 0x00011111,                                                  // 9.7958E-41                                  /// 007f4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0x3f028f5c,                                                  // 0.51                                        /// 007fc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00804
			 0x55555555,                                                  // 4 random values                             /// 00808
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0080c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00810
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00818
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0081c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00820
			 0x0c700000,                                                  // Leading 1s:                                 /// 00824
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00828
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0082c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00830
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00834
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00838
			 0x7fc00001,                                                  // signaling NaN                               /// 0083c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00840
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00844
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00848
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00850
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00858
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0085c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00860
			 0xffc00001,                                                  // -signaling NaN                              /// 00864
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00870
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00874
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00878
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00880
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00888
			 0xffc00001,                                                  // -signaling NaN                              /// 0088c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00890
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00894
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00898
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0089c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008a4
			 0x4b000000,                                                  // 8388608.0                                   /// 008a8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ac
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008b0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 008b8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008bc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 008c0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008c8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 008cc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 008d0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 008d4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 008d8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 008dc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 008e0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008e4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f4
			 0xcb000000,                                                  // -8388608.0                                  /// 008f8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 008fc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00900
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00904
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00908
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0090c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00910
			 0x00011111,                                                  // 9.7958E-41                                  /// 00914
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00918
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0091c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00920
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00924
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00928
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0092c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00930
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00934
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00938
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0093c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00940
			 0x0c400000,                                                  // Leading 1s:                                 /// 00944
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00948
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0094c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00950
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00954
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00958
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0095c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00960
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00964
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00968
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0096c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00970
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00974
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00980
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00984
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00988
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0098c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00990
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00998
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0099c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009a4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009a8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 009b0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009b4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009bc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 009c0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009c4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009c8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009cc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 009d0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009dc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e0
			 0xcb000000,                                                  // -8388608.0                                  /// 009e4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009e8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ec
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f4
			 0x00011111,                                                  // 9.7958E-41                                  /// 009f8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 009fc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a00
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a08
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a0c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a10
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a18
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a1c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a20
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a24
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a28
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a30
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a38
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a3c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a40
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a44
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a48
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a4c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a50
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a54
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a5c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a68
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a6c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a70
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a74
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a78
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a7c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a84
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a88
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a8c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a90
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a94
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a98
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a9c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00aa4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00aa8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aac
			 0x3f028f5c,                                                  // 0.51                                        /// 00ab0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ab8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ac0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ac8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00acc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ad8
			 0x3f028f5c,                                                  // 0.51                                        /// 00adc
			 0x4b000000,                                                  // 8388608.0                                   /// 00ae0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ae8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00aec
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00af0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00af4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00af8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00afc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b00
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b08
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b0c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b10
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b14
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b18
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b1c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b20
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b28
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b2c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b30
			 0x55555555,                                                  // 4 random values                             /// 00b34
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b3c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b40
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b44
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b48
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b4c
			 0xbf028f5c,                                                  // -0.51                                       /// 00b50
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b54
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b58
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b5c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b60
			 0x00000000,                                                  // zero                                        /// 00b64
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b68
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b6c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b70
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b74
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b78
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b80
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b88
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b8c
			 0xbf028f5c,                                                  // -0.51                                       /// 00b90
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b94
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b98
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b9c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ba0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00bb0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00bb4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00bc0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00bc4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00bd0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00bd4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00bd8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00bdc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00be0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00be4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00be8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00bec
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf0
			 0x7fc00001,                                                  // signaling NaN                               /// 00bf4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bf8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00bfc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c00
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c04
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c08
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c0c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c10
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c14
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c18
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c1c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c20
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c24
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c28
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c2c
			 0xffc00001,                                                  // -signaling NaN                              /// 00c30
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c34
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c38
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c3c
			 0x80000000,                                                  // -zero                                       /// 00c40
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c48
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c4c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c50
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c54
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c58
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c5c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c60
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c64
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c6c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c70
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c74
			 0xffc00001,                                                  // -signaling NaN                              /// 00c78
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c7c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c80
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c84
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c88
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c8c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c90
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c94
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c98
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c9c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ca0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ca4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00cac
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00cb0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00cbc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00cc4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00cc8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ccc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00cd0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00cd4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00cd8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cdc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ce0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ce4
			 0x7fc00001,                                                  // signaling NaN                               /// 00ce8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cec
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00cf0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00cf8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00cfc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d00
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d04
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d08
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d0c
			 0x7f800000,                                                  // inf                                         /// 00d10
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d14
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d18
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d1c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d20
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d24
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d28
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d2c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d30
			 0xbf028f5c,                                                  // -0.51                                       /// 00d34
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d38
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d3c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d40
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d44
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d48
			 0x3f028f5c,                                                  // 0.51                                        /// 00d4c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d58
			 0x7fc00001,                                                  // signaling NaN                               /// 00d5c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d60
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d64
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d68
			 0xbf028f5c,                                                  // -0.51                                       /// 00d6c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d70
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d74
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d78
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d7c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d80
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d84
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d88
			 0xcb000000,                                                  // -8388608.0                                  /// 00d8c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d90
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d94
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d98
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d9c
			 0x33333333,                                                  // 4 random values                             /// 00da0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00da4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00da8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00db0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00db4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00db8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00dbc
			 0x80000000,                                                  // -zero                                       /// 00dc0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00dc4
			 0x80000000,                                                  // -zero                                       /// 00dc8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00dcc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00dd0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00dd4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00dd8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ddc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00de0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00de4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00de8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00dec
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00df8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dfc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e04
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e0c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e10
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e14
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e18
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e1c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e20
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e24
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e28
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e2c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e30
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e34
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e38
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e3c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e44
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e48
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e4c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e50
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e58
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e5c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e60
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e64
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e68
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e70
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e74
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e78
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e7c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e80
			 0xcb000000,                                                  // -8388608.0                                  /// 00e84
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e88
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e8c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e90
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e94
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e98
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e9c
			 0x7fc00001,                                                  // signaling NaN                               /// 00ea0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ea4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ea8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00eac
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00eb4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00eb8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ebc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ec0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ec8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ecc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ed4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ed8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00edc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ee4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00eec
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ef0
			 0x4b000000,                                                  // 8388608.0                                   /// 00ef4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ef8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00efc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f00
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f04
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f08
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f0c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f10
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f14
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f18
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f1c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f24
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f28
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f2c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f30
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f34
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f3c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f48
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f4c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f50
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f54
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f58
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f60
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f64
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f68
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f6c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f70
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f74
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f78
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f7c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f80
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f88
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f8c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f90
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f94
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f98
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f9c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fa0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fa4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fa8
			 0x3f028f5c,                                                  // 0.51                                        /// 00fac
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00fb0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb8
			 0xbf028f5c,                                                  // -0.51                                       /// 00fbc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00fc0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00fc4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00fc8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fcc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00fd0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00fd4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00fdc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00fe0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00fe4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00fec
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ff4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
			 0x00000200                                                  // SP - 1 bit alone set in mantissa +ve        /// last
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
			 0x000000c0,
			 0x0000060c,
			 0x000003a4,
			 0x000002a8,
			 0x00000410,
			 0x00000454,
			 0x000006f8,
			 0x000001b4,

			 /// vpu register f2
			 0x40a00000,
			 0x41500000,
			 0x41e00000,
			 0x41400000,
			 0x41980000,
			 0x41800000,
			 0x41c80000,
			 0x40a00000,

			 /// vpu register f3
			 0x41500000,
			 0x40800000,
			 0x41b80000,
			 0x41e00000,
			 0x41f80000,
			 0x41e80000,
			 0x3f800000,
			 0x41500000,

			 /// vpu register f4
			 0x41f00000,
			 0x41880000,
			 0x40e00000,
			 0x41e00000,
			 0x41c00000,
			 0x41200000,
			 0x41a80000,
			 0x41b80000,

			 /// vpu register f5
			 0x41d80000,
			 0x3f800000,
			 0x41b00000,
			 0x40000000,
			 0x41c80000,
			 0x40000000,
			 0x41c00000,
			 0x42000000,

			 /// vpu register f6
			 0x40c00000,
			 0x41e00000,
			 0x40e00000,
			 0x41700000,
			 0x41880000,
			 0x41d00000,
			 0x40e00000,
			 0x41600000,

			 /// vpu register f7
			 0x41800000,
			 0x41900000,
			 0x3f800000,
			 0x41000000,
			 0x41200000,
			 0x41100000,
			 0x41880000,
			 0x40c00000,

			 /// vpu register f8
			 0x41800000,
			 0x41100000,
			 0x41c80000,
			 0x41e00000,
			 0x41500000,
			 0x40000000,
			 0x40a00000,
			 0x41f00000,

			 /// vpu register f9
			 0x3f800000,
			 0x40800000,
			 0x41f00000,
			 0x41f00000,
			 0x41e00000,
			 0x41980000,
			 0x40800000,
			 0x41900000,

			 /// vpu register f10
			 0x41880000,
			 0x40e00000,
			 0x41b00000,
			 0x41d80000,
			 0x40a00000,
			 0x41880000,
			 0x41c00000,
			 0x40a00000,

			 /// vpu register f11
			 0x41f00000,
			 0x41000000,
			 0x41d80000,
			 0x41f00000,
			 0x41d80000,
			 0x41a80000,
			 0x41000000,
			 0x41000000,

			 /// vpu register f12
			 0x41500000,
			 0x41880000,
			 0x41600000,
			 0x41980000,
			 0x41c80000,
			 0x41900000,
			 0x41700000,
			 0x41a00000,

			 /// vpu register f13
			 0x41900000,
			 0x41a00000,
			 0x41000000,
			 0x41d80000,
			 0x41c80000,
			 0x41980000,
			 0x40400000,
			 0x41600000,

			 /// vpu register f14
			 0x41e80000,
			 0x41f00000,
			 0x40000000,
			 0x41a80000,
			 0x40e00000,
			 0x41900000,
			 0x41b80000,
			 0x40c00000,

			 /// vpu register f15
			 0x40000000,
			 0x41c00000,
			 0x40400000,
			 0x41880000,
			 0x41b80000,
			 0x41d80000,
			 0x41880000,
			 0x41f00000,

			 /// vpu register f16
			 0x41600000,
			 0x41f00000,
			 0x41a80000,
			 0x41a00000,
			 0x41f00000,
			 0x41c00000,
			 0x41880000,
			 0x41a00000,

			 /// vpu register f17
			 0x41a80000,
			 0x41d00000,
			 0x41880000,
			 0x41500000,
			 0x41c00000,
			 0x41f00000,
			 0x41f80000,
			 0x41d00000,

			 /// vpu register f18
			 0x3f800000,
			 0x41a80000,
			 0x41f00000,
			 0x41400000,
			 0x41980000,
			 0x41000000,
			 0x41400000,
			 0x41f00000,

			 /// vpu register f19
			 0x41c80000,
			 0x41b00000,
			 0x41880000,
			 0x41400000,
			 0x41500000,
			 0x40000000,
			 0x40800000,
			 0x41000000,

			 /// vpu register f20
			 0x40800000,
			 0x40a00000,
			 0x40000000,
			 0x41b80000,
			 0x41f80000,
			 0x41400000,
			 0x41f00000,
			 0x3f800000,

			 /// vpu register f21
			 0x41300000,
			 0x41100000,
			 0x41000000,
			 0x42000000,
			 0x41f00000,
			 0x41f80000,
			 0x41b80000,
			 0x40800000,

			 /// vpu register f22
			 0x40a00000,
			 0x41500000,
			 0x41b80000,
			 0x40400000,
			 0x42000000,
			 0x40c00000,
			 0x41f80000,
			 0x41b80000,

			 /// vpu register f23
			 0x41e00000,
			 0x41b80000,
			 0x41b00000,
			 0x41800000,
			 0x41880000,
			 0x40000000,
			 0x41f80000,
			 0x41300000,

			 /// vpu register f24
			 0x41c80000,
			 0x42000000,
			 0x40000000,
			 0x41c80000,
			 0x41700000,
			 0x41f80000,
			 0x41880000,
			 0x41800000,

			 /// vpu register f25
			 0x41b80000,
			 0x41200000,
			 0x41980000,
			 0x40c00000,
			 0x41d80000,
			 0x41d80000,
			 0x41c80000,
			 0x40a00000,

			 /// vpu register f26
			 0x41b00000,
			 0x41200000,
			 0x41e80000,
			 0x41100000,
			 0x41d00000,
			 0x41000000,
			 0x41000000,
			 0x41b00000,

			 /// vpu register f27
			 0x41980000,
			 0x40800000,
			 0x41c00000,
			 0x41600000,
			 0x41980000,
			 0x40e00000,
			 0x41d80000,
			 0x41300000,

			 /// vpu register f28
			 0x41b00000,
			 0x40a00000,
			 0x41980000,
			 0x3f800000,
			 0x41a80000,
			 0x41d80000,
			 0x40000000,
			 0x41880000,

			 /// vpu register f29
			 0x41000000,
			 0x41200000,
			 0x41700000,
			 0x41d00000,
			 0x40000000,
			 0x40c00000,
			 0x41800000,
			 0x41900000,

			 /// vpu register f30
			 0x40e00000,
			 0x41b00000,
			 0x42000000,
			 0x41b80000,
			 0x41600000,
			 0x41100000,
			 0x41d00000,
			 0x41f80000,

			 /// vpu register f31
			 0x41900000,
			 0x41b00000,
			 0x41900000,
			 0x40000000,
			 0x40800000,
			 0x41000000,
			 0x41880000,
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
		"flq2 f22, -1498(x13)\n"                              ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f5, 854(x14)\n"                                 ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f25, -1734(x13)\n"                              ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f24, 1060(x13)\n"                               ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f2, 1856(x13)\n"                                ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f16, 1280(x14)\n"                               ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f17, 1474(x15)\n"                               ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f23, -767(x13)\n"                               ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f30, -1782(x11)\n"                              ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f8, 747(x14)\n"                                 ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f27, -910(x14)\n"                               ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f18, 1192(x12)\n"                               ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f4, 1543(x12)\n"                                ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f28, -1332(x15)\n"                              ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f8, -1689(x11)\n"                               ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f6, 564(x11)\n"                                 ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f14, 643(x12)\n"                                ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f18, 367(x12)\n"                                ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f7, -879(x11)\n"                                ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f16, -1436(x13)\n"                              ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f29, -1616(x15)\n"                              ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f26, -539(x14)\n"                               ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f14, -1374(x14)\n"                              ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f28, 387(x13)\n"                                ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f2, 377(x11)\n"                                 ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f10, 1368(x15)\n"                               ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f12, 2043(x13)\n"                               ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f4, -1516(x14)\n"                               ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f20, -818(x15)\n"                               ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f13, 620(x12)\n"                                ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f24, -647(x12)\n"                               ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f22, 319(x12)\n"                                ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f14, -365(x14)\n"                               ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f26, 1179(x11)\n"                               ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f18, 1928(x13)\n"                               ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f29, 667(x13)\n"                                ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f26, -1937(x12)\n"                              ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f18, 343(x14)\n"                                ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f10, -1935(x13)\n"                              ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f23, -130(x14)\n"                               ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f8, 1598(x13)\n"                                ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f29, 1659(x14)\n"                               ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f16, 1256(x14)\n"                               ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f11, -742(x11)\n"                               ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f21, 737(x13)\n"                                ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f23, 802(x14)\n"                                ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f3, 1891(x14)\n"                                ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f18, 517(x13)\n"                                ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f8, 2033(x12)\n"                                ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f10, -1117(x14)\n"                              ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f14, -1682(x12)\n"                              ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f22, 399(x14)\n"                                ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f22, 1246(x14)\n"                               ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f26, -844(x15)\n"                               ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f9, -790(x13)\n"                                ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f17, 286(x13)\n"                                ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f14, -1512(x13)\n"                              ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f7, 1577(x14)\n"                                ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f18, 1608(x15)\n"                               ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f18, 1167(x14)\n"                               ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f14, 1282(x15)\n"                               ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f14, -1469(x11)\n"                              ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f5, -361(x14)\n"                                ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f18, 347(x15)\n"                                ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f18, -1305(x13)\n"                              ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f3, 1092(x15)\n"                                ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f26, 286(x14)\n"                                ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f29, 1211(x13)\n"                               ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f3, 830(x14)\n"                                 ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f18, 720(x13)\n"                                ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f19, 2009(x15)\n"                               ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f8, 1122(x11)\n"                                ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f8, 1787(x14)\n"                                ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f30, -411(x12)\n"                               ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f19, -1210(x14)\n"                              ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f14, -1834(x13)\n"                              ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f28, -944(x12)\n"                               ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f29, 982(x13)\n"                                ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f5, -1983(x15)\n"                               ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f12, 432(x12)\n"                                ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f15, -279(x15)\n"                               ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f21, 1607(x12)\n"                               ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f5, -1740(x15)\n"                               ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f25, 5(x12)\n"                                  ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f23, -1503(x13)\n"                              ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f27, 1917(x14)\n"                               ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f21, -925(x14)\n"                               ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f4, -2030(x13)\n"                               ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f28, 1481(x14)\n"                               ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f6, -773(x13)\n"                                ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f15, 396(x14)\n"                                ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f4, 376(x13)\n"                                 ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f14, 571(x14)\n"                                ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f30, -649(x11)\n"                               ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f26, -708(x15)\n"                               ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f8, -980(x15)\n"                                ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f7, -1605(x11)\n"                               ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f10, 1822(x13)\n"                               ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flq2 f8, 1461(x13)\n"                                ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
