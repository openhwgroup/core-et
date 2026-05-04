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
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00000
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00004
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00008
			 0x7f800000,                                                  // inf                                         /// 0000c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00010
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00014
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0001c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00020
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00024
			 0x0c700000,                                                  // Leading 1s:                                 /// 00028
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0002c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00030
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00034
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00038
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0003c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00040
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00044
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00048
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0004c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00050
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00054
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00058
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0005c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00060
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00064
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00068
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0006c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00070
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00074
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00078
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00080
			 0x0e000003,                                                  // Trailing 1s:                                /// 00084
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00090
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00094
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00098
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0009c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 000a0
			 0x80011111,                                                  // -9.7958E-41                                 /// 000a4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000a8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000ac
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000b0
			 0x3f028f5c,                                                  // 0.51                                        /// 000b4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000b8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000bc
			 0x0c400000,                                                  // Leading 1s:                                 /// 000c0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 000c4
			 0x0c400000,                                                  // Leading 1s:                                 /// 000c8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000cc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x0e000007,                                                  // Trailing 1s:                                /// 000d4
			 0x00000000,                                                  // zero                                        /// 000d8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 000e0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000e4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000e8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 000ec
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000f0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000f4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000f8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 000fc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00100
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00104
			 0x0c400000,                                                  // Leading 1s:                                 /// 00108
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0010c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00110
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00114
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00118
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0011c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00120
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00124
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00128
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00130
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00138
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0013c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00140
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00144
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00148
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0014c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00150
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00154
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00158
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00160
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00164
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00168
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00170
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00174
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00178
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0017c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00184
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00188
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0018c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00190
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00194
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00198
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0019c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001a0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001a8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ac
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001b0
			 0x0e000003,                                                  // Trailing 1s:                                /// 001b4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 001c0
			 0x3f028f5c,                                                  // 0.51                                        /// 001c4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 001c8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001cc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001d0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001d4
			 0x0c600000,                                                  // Leading 1s:                                 /// 001d8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001dc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001e8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ec
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 001f0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 001f4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001fc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00208
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0020c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00210
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00214
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0021c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00220
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00224
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00228
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0022c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00230
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00234
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00238
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0023c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00240
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00244
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00248
			 0x0c700000,                                                  // Leading 1s:                                 /// 0024c
			 0xcb000000,                                                  // -8388608.0                                  /// 00250
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00254
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00258
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0025c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00260
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00264
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0026c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00270
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0027c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00280
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00284
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00288
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0028c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00290
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00294
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00298
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0029c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002a4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a8
			 0x80000000,                                                  // -zero                                       /// 002ac
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002b0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002b4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002bc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002c4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c8
			 0x0c700000,                                                  // Leading 1s:                                 /// 002cc
			 0x0c700000,                                                  // Leading 1s:                                 /// 002d0
			 0x4b000000,                                                  // 8388608.0                                   /// 002d4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 002dc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 002e8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002ec
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002f4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 002f8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 002fc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00300
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00304
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00308
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0030c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00310
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00314
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00318
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0031c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00320
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00324
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00328
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0032c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00330
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00334
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00338
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0033c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00344
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00348
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0034c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00350
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00358
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0035c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00364
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00370
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00374
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00378
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0037c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00380
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00384
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00388
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0038c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00390
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0039c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003a0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 003ac
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003b4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 003b8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003bc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003c0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003c8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 003cc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 003d0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003d4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003d8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003dc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 003e0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003e4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003ec
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003f0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003f4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003f8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003fc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00400
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00404
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00408
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0040c
			 0x7fc00001,                                                  // signaling NaN                               /// 00410
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00414
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00418
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0041c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00420
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00424
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00430
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00434
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00438
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0043c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00440
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00444
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00448
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00450
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00454
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00458
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0045c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00460
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00464
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0046c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00470
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00474
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00478
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0047c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00480
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00484
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00488
			 0x00000000,                                                  // zero                                        /// 0048c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00490
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00494
			 0x0e000007,                                                  // Trailing 1s:                                /// 00498
			 0x0e000003,                                                  // Trailing 1s:                                /// 0049c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 004a0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004a4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004a8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ac
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004b0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 004b4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 004bc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 004c0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 004cc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004d4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004d8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004e0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004e4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0x0c600000,                                                  // Leading 1s:                                 /// 004ec
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004f0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004f8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004fc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00500
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00504
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00518
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0051c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00520
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00524
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00528
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0052c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00534
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00538
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0053c
			 0x3f028f5c,                                                  // 0.51                                        /// 00540
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00544
			 0x7fc00001,                                                  // signaling NaN                               /// 00548
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0054c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00550
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00554
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00558
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0055c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00564
			 0x33333333,                                                  // 4 random values                             /// 00568
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0056c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00570
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00574
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00578
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0057c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00580
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00584
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00588
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0058c
			 0xcb000000,                                                  // -8388608.0                                  /// 00590
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00594
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00598
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0059c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005a4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 005ac
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005b0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005bc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 005c4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005c8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005cc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005d0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005d4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005d8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005dc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005e0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005e4
			 0xff800000,                                                  // -inf                                        /// 005e8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005ec
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005f4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005fc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00600
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00604
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00608
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0060c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00610
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00614
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00618
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00620
			 0xffc00001,                                                  // -signaling NaN                              /// 00624
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0x3f028f5c,                                                  // 0.51                                        /// 0062c
			 0xcb000000,                                                  // -8388608.0                                  /// 00630
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00634
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00638
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0063c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00640
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00644
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00648
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0064c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00650
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00654
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00658
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0065c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00660
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00664
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00668
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00674
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00678
			 0x0e000003,                                                  // Trailing 1s:                                /// 0067c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00680
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00684
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00688
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0068c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00690
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00694
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00698
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0069c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006a0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 006a4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006a8
			 0x33333333,                                                  // 4 random values                             /// 006ac
			 0x0c780000,                                                  // Leading 1s:                                 /// 006b0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006b8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006c0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 006c4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006c8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 006cc
			 0x80000000,                                                  // -zero                                       /// 006d0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006d4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 006d8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006dc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 006e0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006e4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 006e8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006ec
			 0x0c700000,                                                  // Leading 1s:                                 /// 006f0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 006f8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 006fc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00700
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00704
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0xcb000000,                                                  // -8388608.0                                  /// 0070c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00710
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00714
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x7fc00001,                                                  // signaling NaN                               /// 0071c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00720
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x7f800000,                                                  // inf                                         /// 00728
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00730
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00734
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00738
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0073c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00740
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00744
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00748
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0074c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00750
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00754
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00758
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0075c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0xcb000000,                                                  // -8388608.0                                  /// 00764
			 0x0e000007,                                                  // Trailing 1s:                                /// 00768
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0076c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0xff800000,                                                  // -inf                                        /// 00774
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00778
			 0xcb000000,                                                  // -8388608.0                                  /// 0077c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00780
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00784
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00788
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0078c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00790
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00798
			 0x3f028f5c,                                                  // 0.51                                        /// 0079c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 007a0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 007a8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ac
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007b0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007b4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 007b8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007bc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007cc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007d4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007d8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007dc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007e0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 007e4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 007ec
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007f0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007f4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0x3f028f5c,                                                  // 0.51                                        /// 007fc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00804
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00808
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0080c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00818
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0081c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00820
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00824
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00828
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0082c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00830
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00834
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00838
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0083c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00840
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0xffc00001,                                                  // -signaling NaN                              /// 00850
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00858
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0085c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00860
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00864
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00868
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00870
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00874
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00878
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0087c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00880
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00884
			 0x0c700000,                                                  // Leading 1s:                                 /// 00888
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0088c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00890
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00894
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0089c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008a0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 008a8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008ac
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b0
			 0xbf028f5c,                                                  // -0.51                                       /// 008b4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008c0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 008cc
			 0x0e000007,                                                  // Trailing 1s:                                /// 008d0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 008d4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 008d8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008dc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 008e0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008e4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008e8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008ec
			 0x0c400000,                                                  // Leading 1s:                                 /// 008f0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f4
			 0x55555555,                                                  // 4 random values                             /// 008f8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 008fc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00900
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00904
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00908
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0090c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00910
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00914
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00918
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0091c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00920
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00924
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00928
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0092c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00930
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00934
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00938
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0093c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00940
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00944
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00948
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0094c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00950
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00954
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00958
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0095c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00960
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00964
			 0x0c780000,                                                  // Leading 1s:                                 /// 00968
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0096c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00970
			 0x0c700000,                                                  // Leading 1s:                                 /// 00974
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00978
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0097c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00980
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00984
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00988
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0098c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00990
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00994
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00998
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0099c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 009a0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009a4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 009ac
			 0x4b000000,                                                  // 8388608.0                                   /// 009b0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009b4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009bc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009c4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009cc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d0
			 0x80000000,                                                  // -zero                                       /// 009d4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009d8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009dc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 009e4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 009e8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 009ec
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009f8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009fc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a00
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a04
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a08
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a0c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a10
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a18
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a1c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a20
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a28
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a34
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a38
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a3c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a40
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a44
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a48
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a4c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a50
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a54
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a58
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a5c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a60
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a64
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a68
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a6c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a70
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a74
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a78
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a80
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a84
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a88
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a8c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a90
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a94
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a98
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a9c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00aa0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00aa8
			 0xff800000,                                                  // -inf                                        /// 00aac
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ab0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ab8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00abc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ac0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ac4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ac8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00acc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ad4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ad8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00adc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ae0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ae4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00af4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b00
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b04
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b08
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b0c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b10
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b14
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b18
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b1c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b20
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b24
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b28
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b30
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b34
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b38
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b40
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b44
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b48
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0xbf028f5c,                                                  // -0.51                                       /// 00b50
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b54
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b58
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b5c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b60
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b64
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b68
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b6c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b70
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b74
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b78
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0xffc00001,                                                  // -signaling NaN                              /// 00b80
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b84
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b88
			 0x7f800000,                                                  // inf                                         /// 00b8c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b90
			 0xbf028f5c,                                                  // -0.51                                       /// 00b94
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b98
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b9c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ba0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ba4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ba8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bac
			 0x3f028f5c,                                                  // 0.51                                        /// 00bb0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00bb8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0x7f800000,                                                  // inf                                         /// 00bc0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00bc4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc8
			 0x4b000000,                                                  // 8388608.0                                   /// 00bcc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00bd0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00bd4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0xbf028f5c,                                                  // -0.51                                       /// 00bdc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00be0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00be8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00bec
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bf0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00bf4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00bf8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c00
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c04
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c08
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c0c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c10
			 0x7f800000,                                                  // inf                                         /// 00c14
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c18
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c1c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c20
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c24
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c28
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c2c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c30
			 0x4b000000,                                                  // 8388608.0                                   /// 00c34
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c38
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c3c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c40
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c48
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c4c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c50
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c54
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c58
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c60
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c68
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c6c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c70
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c74
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c78
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c7c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c80
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c84
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c88
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c8c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c90
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c94
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c98
			 0xffc00001,                                                  // -signaling NaN                              /// 00c9c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ca0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca8
			 0xbf028f5c,                                                  // -0.51                                       /// 00cac
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00cb0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00cb8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cbc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00cc0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00cc4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00cc8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ccc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00cd0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00cd4
			 0x00000000,                                                  // zero                                        /// 00cd8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00cdc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ce0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ce8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cec
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00cf4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00cf8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00cfc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d00
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d04
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d08
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d0c
			 0xff800000,                                                  // -inf                                        /// 00d10
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d14
			 0x4b000000,                                                  // 8388608.0                                   /// 00d18
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d1c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d20
			 0x80000000,                                                  // -zero                                       /// 00d24
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d2c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d30
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d34
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d38
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d40
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d44
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d48
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d4c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d50
			 0xff800000,                                                  // -inf                                        /// 00d54
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x7f800000,                                                  // inf                                         /// 00d5c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d60
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d64
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0x7f800000,                                                  // inf                                         /// 00d74
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d78
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d7c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d80
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d84
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d88
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d94
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d98
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d9c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00db0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00db8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dbc
			 0x4b000000,                                                  // 8388608.0                                   /// 00dc0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00dc8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00dcc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00dd4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00dd8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ddc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00de0
			 0x80000000,                                                  // -zero                                       /// 00de4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00dec
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00df0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00df8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dfc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e04
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e08
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e0c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e10
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e14
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e18
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e1c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e20
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e24
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e28
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e34
			 0xff800000,                                                  // -inf                                        /// 00e38
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e3c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e44
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e48
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e4c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e50
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e54
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e58
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e5c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e60
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e64
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e68
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e70
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e74
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e78
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e7c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e80
			 0x7fc00001,                                                  // signaling NaN                               /// 00e84
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e88
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e8c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e94
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e98
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e9c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ea0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ea8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00eac
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00eb0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00eb4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ebc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ec0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ec4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ec8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ecc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ed0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ed4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ed8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00edc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ee0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ee4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ee8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ef8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x00000000,                                                  // zero                                        /// 00f00
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f04
			 0xcb000000,                                                  // -8388608.0                                  /// 00f08
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f10
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f14
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f1c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f20
			 0xffc00001,                                                  // -signaling NaN                              /// 00f24
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f2c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f30
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f34
			 0x80000000,                                                  // -zero                                       /// 00f38
			 0x4b000000,                                                  // 8388608.0                                   /// 00f3c
			 0xbf028f5c,                                                  // -0.51                                       /// 00f40
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f48
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f4c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f54
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f58
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f5c
			 0x4b000000,                                                  // 8388608.0                                   /// 00f60
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f64
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f68
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f6c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f70
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f74
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f78
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f7c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f80
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f84
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f88
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f8c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f90
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f94
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f98
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f9c
			 0xbf028f5c,                                                  // -0.51                                       /// 00fa0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00fa4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00fa8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00fac
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fb0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00fb4
			 0xffc00001,                                                  // -signaling NaN                              /// 00fb8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fbc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00fc0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00fc8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fcc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00fd0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00fd4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fd8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00fe4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00fec
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ff0
			 0xff800000,                                                  // -inf                                        /// 00ff4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ff8
			 0x7f800001 // SNaN                                          // SP +ve numbers                              /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x3f028f5c,                                                  // 0.51                                        /// 00000
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00004
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00008
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0000c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00010
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00014
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00018
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0001c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00020
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00024
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00028
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0002c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00030
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00034
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00038
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0003c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00040
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00044
			 0x55555555,                                                  // 4 random values                             /// 00048
			 0x0e000007,                                                  // Trailing 1s:                                /// 0004c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00054
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00058
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0005c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00060
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00064
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00068
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0006c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00070
			 0x7fc00001,                                                  // signaling NaN                               /// 00074
			 0x4b000000,                                                  // 8388608.0                                   /// 00078
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00080
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0x0e000001,                                                  // Trailing 1s:                                /// 00088
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0008c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00090
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00094
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0xcb000000,                                                  // -8388608.0                                  /// 0009c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000a4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000a8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000ac
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000b0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000b4
			 0x55555555,                                                  // 4 random values                             /// 000b8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000bc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000c0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000cc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 000d4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000d8
			 0x0c400000,                                                  // Leading 1s:                                 /// 000dc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000e0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000e8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000ec
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 000f0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000f4
			 0x0e000003,                                                  // Trailing 1s:                                /// 000f8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000fc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00104
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00108
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0010c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00110
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00114
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00118
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0011c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00120
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00124
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0012c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00130
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00134
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00138
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0013c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00148
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0014c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00150
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00154
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00158
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0015c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00160
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00164
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00168
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0016c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00170
			 0x33333333,                                                  // 4 random values                             /// 00174
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00178
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0017c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00180
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00184
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00188
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0018c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00190
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00194
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00198
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0019c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001a0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 001a4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 001a8
			 0x7fc00001,                                                  // signaling NaN                               /// 001ac
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001b4
			 0x80000000,                                                  // -zero                                       /// 001b8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001bc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001cc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 001d0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001d4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001dc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001e0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 001e4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001e8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001ec
			 0xffc00001,                                                  // -signaling NaN                              /// 001f0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001f4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 001f8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001fc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00200
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00204
			 0x0e000003,                                                  // Trailing 1s:                                /// 00208
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0020c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00214
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00218
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00220
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00224
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00228
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0022c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00230
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00234
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00238
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0023c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00240
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00244
			 0xbf028f5c,                                                  // -0.51                                       /// 00248
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0024c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00250
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00254
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00258
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0025c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00260
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00264
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00268
			 0x00011111,                                                  // 9.7958E-41                                  /// 0026c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00270
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00274
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00278
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00280
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00284
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00288
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0028c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00290
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00298
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0029c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002a4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 002a8
			 0x0e000001,                                                  // Trailing 1s:                                /// 002ac
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002b4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b8
			 0x0e000001,                                                  // Trailing 1s:                                /// 002bc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 002c0
			 0x0c780000,                                                  // Leading 1s:                                 /// 002c4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002c8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002cc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 002d0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002d8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002dc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 002e4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 002e8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002ec
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 002f0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002f4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002f8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00300
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00304
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00308
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0030c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00310
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00314
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00318
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0031c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00320
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00324
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00328
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0032c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00330
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00334
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00338
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00340
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00344
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00348
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0034c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00350
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00358
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0035c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00360
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00364
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00368
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0036c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00374
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0037c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00380
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00384
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00388
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0038c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00390
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00394
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 003a0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003a4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 003a8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 003b4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003b8
			 0x4b000000,                                                  // 8388608.0                                   /// 003bc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003c4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003c8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 003cc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003d0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 003d4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003d8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003dc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003e0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003e4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003e8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003ec
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 003f0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003f4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 003fc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00404
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00408
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0040c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00410
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00414
			 0xffc00001,                                                  // -signaling NaN                              /// 00418
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0041c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00420
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00424
			 0xbf028f5c,                                                  // -0.51                                       /// 00428
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00430
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00434
			 0xff800000,                                                  // -inf                                        /// 00438
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0043c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00440
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00448
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0044c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00454
			 0xbf028f5c,                                                  // -0.51                                       /// 00458
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0045c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00460
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00468
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0046c
			 0x55555555,                                                  // 4 random values                             /// 00470
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00474
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00478
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0047c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00480
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00484
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00488
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0048c
			 0x7fc00001,                                                  // signaling NaN                               /// 00490
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00498
			 0xbf028f5c,                                                  // -0.51                                       /// 0049c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 004a0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004a4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 004a8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004ac
			 0x0c780000,                                                  // Leading 1s:                                 /// 004b0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 004b8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004bc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004c0
			 0x7f800000,                                                  // inf                                         /// 004c4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 004c8
			 0x80000000,                                                  // -zero                                       /// 004cc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 004d0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 004d4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 004d8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004e0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 004e4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 004e8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 004ec
			 0x80011111,                                                  // -9.7958E-41                                 /// 004f0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004f4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004f8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 004fc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00500
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00504
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00508
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00510
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00518
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0051c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00520
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00524
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00528
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00530
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00534
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00538
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00540
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00548
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0054c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00550
			 0x00000000,                                                  // zero                                        /// 00554
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00558
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0055c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00560
			 0x0c780000,                                                  // Leading 1s:                                 /// 00564
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00568
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0056c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00570
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00574
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00578
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0057c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00584
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00588
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0058c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00594
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00598
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0059c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 005a0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005a4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005ac
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 005b0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005b4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005b8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 005bc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005c4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005c8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005cc
			 0x33333333,                                                  // 4 random values                             /// 005d0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005d4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0x3f028f5c,                                                  // 0.51                                        /// 005dc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005e0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005e4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005e8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 005ec
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 005f0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005f4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 005f8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005fc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00600
			 0xffc00001,                                                  // -signaling NaN                              /// 00604
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00608
			 0x7fc00001,                                                  // signaling NaN                               /// 0060c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00610
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00614
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00618
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0061c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00620
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00624
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00628
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0062c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00630
			 0x55555555,                                                  // 4 random values                             /// 00634
			 0x7fc00001,                                                  // signaling NaN                               /// 00638
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0063c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00640
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00644
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00648
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0064c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00650
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00654
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00658
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0065c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00660
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00664
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00670
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00674
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00678
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0067c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00684
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00688
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0068c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00690
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00694
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00698
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0069c
			 0x55555555,                                                  // 4 random values                             /// 006a0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 006a8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006ac
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006b0
			 0x4b000000,                                                  // 8388608.0                                   /// 006b4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006b8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006bc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006c0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 006c4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 006cc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006d0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 006d4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 006d8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 006dc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006e0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006e4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006e8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006ec
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006f0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006fc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00700
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00704
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00708
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0070c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00710
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00714
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0071c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00720
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00724
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00728
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0072c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00730
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00734
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00738
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0073c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00740
			 0x0e000007,                                                  // Trailing 1s:                                /// 00744
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00748
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0074c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00750
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00754
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00758
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0075c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x80000000,                                                  // -zero                                       /// 00764
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00768
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0076c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00774
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00778
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0077c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00784
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00788
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00790
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00794
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00798
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0079c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007a0
			 0x0e000003,                                                  // Trailing 1s:                                /// 007a4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007ac
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007b0
			 0x0e000003,                                                  // Trailing 1s:                                /// 007b4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007c0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007c4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007cc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 007d4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007dc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 007e4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007e8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 007ec
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 007f0
			 0x7f800000,                                                  // inf                                         /// 007f4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007f8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 007fc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00804
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00808
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0080c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00810
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00814
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00818
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0081c
			 0x80000000,                                                  // -zero                                       /// 00820
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00824
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00828
			 0xcb000000,                                                  // -8388608.0                                  /// 0082c
			 0x7fc00001,                                                  // signaling NaN                               /// 00830
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00834
			 0xcb000000,                                                  // -8388608.0                                  /// 00838
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00840
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00848
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00850
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00854
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00858
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0085c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00860
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00864
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00868
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0086c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00870
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00874
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00880
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00884
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00888
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00890
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00894
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00898
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0089c
			 0x55555555,                                                  // 4 random values                             /// 008a0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 008a4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 008ac
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b0
			 0x00011111,                                                  // 9.7958E-41                                  /// 008b4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008b8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 008bc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 008c0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008c4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008c8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008cc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008d0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008d4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008d8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008dc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008e0
			 0x0c780000,                                                  // Leading 1s:                                 /// 008e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e8
			 0xcb000000,                                                  // -8388608.0                                  /// 008ec
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 008f0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 008f4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0xffc00001,                                                  // -signaling NaN                              /// 008fc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00900
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00904
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00908
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0090c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00910
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00914
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0091c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00920
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00924
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00928
			 0x55555555,                                                  // 4 random values                             /// 0092c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00930
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00934
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00938
			 0x7f800000,                                                  // inf                                         /// 0093c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00940
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00944
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00948
			 0xcb000000,                                                  // -8388608.0                                  /// 0094c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00950
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00954
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00958
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0095c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00964
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00968
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0096c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00970
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00974
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00978
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00980
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00984
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0098c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00990
			 0x7fc00001,                                                  // signaling NaN                               /// 00994
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00998
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0099c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009a4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009a8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009ac
			 0x4b000000,                                                  // 8388608.0                                   /// 009b0
			 0x80011111,                                                  // -9.7958E-41                                 /// 009b4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 009b8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009bc
			 0x7fc00001,                                                  // signaling NaN                               /// 009c0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 009c8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 009cc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009dc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 009e0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009e4
			 0x33333333,                                                  // 4 random values                             /// 009e8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009ec
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 009f0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009f8
			 0x0c600000,                                                  // Leading 1s:                                 /// 009fc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a04
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a08
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a0c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a10
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a14
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a18
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a1c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a20
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a24
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a28
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a2c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a30
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a34
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a38
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a3c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a40
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a44
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a48
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a4c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a54
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a5c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a64
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a68
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a6c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a70
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a78
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a7c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a80
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a88
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a8c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a90
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a94
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a98
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a9c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00aa4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00aa8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00aac
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ab0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ab4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00abc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ac0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ac4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00acc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ad0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ad4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad8
			 0x55555555,                                                  // 4 random values                             /// 00adc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ae4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ae8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00af0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00af4
			 0x7f800000,                                                  // inf                                         /// 00af8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00afc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b04
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b08
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b0c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b10
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b14
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b18
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b1c
			 0x80000000,                                                  // -zero                                       /// 00b20
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b24
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b28
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b2c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b30
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b34
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b38
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b3c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b40
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b44
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b4c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b50
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b54
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b58
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b5c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b64
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b68
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b6c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b70
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b74
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b78
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b7c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b80
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b84
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b88
			 0x80000000,                                                  // -zero                                       /// 00b8c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b90
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b94
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b98
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b9c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ba0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x33333333,                                                  // 4 random values                             /// 00ba8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00bac
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00bb0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00bb8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bbc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00bc8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00bcc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00bd4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00bd8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00bdc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00be0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00bec
			 0x4b000000,                                                  // 8388608.0                                   /// 00bf0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00bf8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00bfc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c00
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c08
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c0c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c14
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c18
			 0x7fc00001,                                                  // signaling NaN                               /// 00c1c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c20
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c24
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c28
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c2c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c30
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c34
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c38
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c3c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c40
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c44
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c48
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c4c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c50
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c54
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c58
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c5c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c60
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c64
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c68
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c6c
			 0xcb000000,                                                  // -8388608.0                                  /// 00c70
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c74
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c78
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c7c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c80
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c84
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c88
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c8c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c90
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c9c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ca0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ca4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cac
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00cb0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00cb4
			 0x55555555,                                                  // 4 random values                             /// 00cb8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00cbc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00cc0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00cd4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cd8
			 0xffc00001,                                                  // -signaling NaN                              /// 00cdc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00cec
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00cf0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00cf4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00cf8
			 0xffc00001,                                                  // -signaling NaN                              /// 00cfc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d00
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d04
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d08
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d0c
			 0xbf028f5c,                                                  // -0.51                                       /// 00d10
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d14
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d18
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d1c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d20
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d24
			 0x00000000,                                                  // zero                                        /// 00d28
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d2c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d30
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d34
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d38
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d3c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d40
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d44
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d48
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d4c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d50
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d54
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x55555555,                                                  // 4 random values                             /// 00d5c
			 0x7fc00001,                                                  // signaling NaN                               /// 00d60
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d64
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d68
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d6c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d70
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d74
			 0xbf028f5c,                                                  // -0.51                                       /// 00d78
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d7c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d80
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d88
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d8c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d90
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d94
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d98
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d9c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00da0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00da4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00dac
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00db4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00dbc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00dc0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00dc4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00dcc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00dd0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00dd4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ddc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00de0
			 0x7f800000,                                                  // inf                                         /// 00de4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00de8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00dec
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00df0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00df4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00df8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00dfc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e00
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e04
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e08
			 0x80000000,                                                  // -zero                                       /// 00e0c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e10
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e14
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e18
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e20
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e28
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e34
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e38
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e3c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e40
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e44
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e48
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e4c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e50
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e54
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e58
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e5c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e60
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e64
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e6c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e70
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e74
			 0xbf028f5c,                                                  // -0.51                                       /// 00e78
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e7c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e80
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e84
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e88
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e8c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e90
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e94
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e98
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e9c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ea0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ea8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00eb0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00eb4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00eb8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ec4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ec8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ecc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ed4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ee0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ee4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ee8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00eec
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ef0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ef4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ef8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00efc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f00
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f04
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f08
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f0c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f10
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f14
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f18
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f1c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f20
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f28
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f2c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f30
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f34
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f38
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f3c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f40
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f44
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f48
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f4c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f50
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f54
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f58
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f60
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f64
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f68
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f6c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f70
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f78
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f7c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f80
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f84
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f88
			 0x00000000,                                                  // zero                                        /// 00f8c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f90
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f94
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f98
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f9c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00fa4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fa8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fac
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00fb0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00fb4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00fb8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00fbc
			 0x7fc00001,                                                  // signaling NaN                               /// 00fc0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00fc4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00fc8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fcc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00fd0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00fd4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00fdc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fe0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ff0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ff4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
			 0x0c7fc000                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0xbbc91ea7, /// 0x0
			 0x28c6a883, /// 0x4
			 0x86a4447a, /// 0x8
			 0xf348ae68, /// 0xc
			 0xb68bae06, /// 0x10
			 0x62f7568b, /// 0x14
			 0xf27358e9, /// 0x18
			 0xbe726159, /// 0x1c
			 0x46aa21ec, /// 0x20
			 0x2accd5f2, /// 0x24
			 0x0d56e342, /// 0x28
			 0xf595df77, /// 0x2c
			 0xd62caffb, /// 0x30
			 0x4169e78b, /// 0x34
			 0x5a70e3ce, /// 0x38
			 0x2d64e62c, /// 0x3c
			 0x726341fb, /// 0x40
			 0x1fbecd88, /// 0x44
			 0x285f9937, /// 0x48
			 0x93f18872, /// 0x4c
			 0x5d9a78f0, /// 0x50
			 0x7e5f4dd6, /// 0x54
			 0x4a12364a, /// 0x58
			 0x6e5e8704, /// 0x5c
			 0x338e4f28, /// 0x60
			 0x7c1ae271, /// 0x64
			 0x1be2d7c8, /// 0x68
			 0xa9216959, /// 0x6c
			 0x62a8484e, /// 0x70
			 0x901af977, /// 0x74
			 0x46e77a47, /// 0x78
			 0x22b0793b, /// 0x7c
			 0x17b002ce, /// 0x80
			 0xc86660c1, /// 0x84
			 0x5a86ce69, /// 0x88
			 0x7c5a6191, /// 0x8c
			 0xb5ccdc7f, /// 0x90
			 0xe901f74d, /// 0x94
			 0x7a1a9d90, /// 0x98
			 0xaadda5df, /// 0x9c
			 0x67164a0a, /// 0xa0
			 0xb89094fa, /// 0xa4
			 0xf7fc4292, /// 0xa8
			 0xaf505277, /// 0xac
			 0xb9d240bc, /// 0xb0
			 0x4c4e1ea3, /// 0xb4
			 0x57b5fbf3, /// 0xb8
			 0xe59766a6, /// 0xbc
			 0x0da5f2ab, /// 0xc0
			 0x8541be78, /// 0xc4
			 0x88aaf728, /// 0xc8
			 0xa321f489, /// 0xcc
			 0x2a361c5b, /// 0xd0
			 0xed064b43, /// 0xd4
			 0x09cdb65a, /// 0xd8
			 0xffb72f5d, /// 0xdc
			 0xb579a4f2, /// 0xe0
			 0x0e678282, /// 0xe4
			 0x1766680e, /// 0xe8
			 0xa84ca10f, /// 0xec
			 0xd4ac262d, /// 0xf0
			 0x6631df92, /// 0xf4
			 0x4e9b6404, /// 0xf8
			 0xf6d955c8, /// 0xfc
			 0x88346838, /// 0x100
			 0x14046191, /// 0x104
			 0xd8731328, /// 0x108
			 0xb411916b, /// 0x10c
			 0xcc09b729, /// 0x110
			 0x0015df66, /// 0x114
			 0x0473b855, /// 0x118
			 0x980457bd, /// 0x11c
			 0xf6113fb3, /// 0x120
			 0x7a8df501, /// 0x124
			 0x11a3019a, /// 0x128
			 0x6ada9594, /// 0x12c
			 0x61f4a3c9, /// 0x130
			 0xf6f4f947, /// 0x134
			 0x6caf4a80, /// 0x138
			 0x406a781e, /// 0x13c
			 0x256947f7, /// 0x140
			 0x98453a8e, /// 0x144
			 0x8cc87dc8, /// 0x148
			 0x5b272fbf, /// 0x14c
			 0x2aa5b12f, /// 0x150
			 0xeee43f6a, /// 0x154
			 0xf7aa2ba9, /// 0x158
			 0x12326d00, /// 0x15c
			 0xd0634f2e, /// 0x160
			 0xc6152cf5, /// 0x164
			 0x256e10be, /// 0x168
			 0xa508a6c9, /// 0x16c
			 0x266b8ad2, /// 0x170
			 0x52985fe9, /// 0x174
			 0xaace1cff, /// 0x178
			 0xea0df2ee, /// 0x17c
			 0xf881d1e5, /// 0x180
			 0xb8ad5fe4, /// 0x184
			 0x33e3dd3b, /// 0x188
			 0x58ee5c1a, /// 0x18c
			 0x390eaa87, /// 0x190
			 0xc119644e, /// 0x194
			 0x936908c5, /// 0x198
			 0x36dda076, /// 0x19c
			 0xb69a8df2, /// 0x1a0
			 0xc34b24fc, /// 0x1a4
			 0xa5ad3f41, /// 0x1a8
			 0x05ef632c, /// 0x1ac
			 0xd3aea278, /// 0x1b0
			 0x43c09738, /// 0x1b4
			 0x990bd195, /// 0x1b8
			 0x6383879a, /// 0x1bc
			 0x6a5c9d95, /// 0x1c0
			 0x28457450, /// 0x1c4
			 0x9161770f, /// 0x1c8
			 0xb2afd4cf, /// 0x1cc
			 0xecb3a669, /// 0x1d0
			 0xbd91dfdd, /// 0x1d4
			 0xe0312a3c, /// 0x1d8
			 0x672770ec, /// 0x1dc
			 0x92dcec91, /// 0x1e0
			 0x46912318, /// 0x1e4
			 0x53a48ef6, /// 0x1e8
			 0x9b664c38, /// 0x1ec
			 0xbaf9dce4, /// 0x1f0
			 0x3c645021, /// 0x1f4
			 0x179c2c30, /// 0x1f8
			 0xae5b051f, /// 0x1fc
			 0x39d17688, /// 0x200
			 0xf59e2990, /// 0x204
			 0x14d79471, /// 0x208
			 0x703f7a69, /// 0x20c
			 0x3256c827, /// 0x210
			 0x35db8c07, /// 0x214
			 0x4cb54996, /// 0x218
			 0x5acb7fe6, /// 0x21c
			 0x6ecd17af, /// 0x220
			 0xce713685, /// 0x224
			 0x4dab237b, /// 0x228
			 0xe7752ce4, /// 0x22c
			 0x206af698, /// 0x230
			 0x597139e1, /// 0x234
			 0xe20f7b65, /// 0x238
			 0xab35fcd8, /// 0x23c
			 0x34483efa, /// 0x240
			 0x16fc0d21, /// 0x244
			 0xa5d1d115, /// 0x248
			 0x64629925, /// 0x24c
			 0x1c9405e8, /// 0x250
			 0xe2ca28b0, /// 0x254
			 0x2dcd46a8, /// 0x258
			 0x4f161d26, /// 0x25c
			 0x45096bad, /// 0x260
			 0xdbf082f8, /// 0x264
			 0xb5d9a2ab, /// 0x268
			 0xc7136f2f, /// 0x26c
			 0x97cca0ff, /// 0x270
			 0x7a1019f5, /// 0x274
			 0xe65881d8, /// 0x278
			 0x51fb0657, /// 0x27c
			 0xfc9f9c92, /// 0x280
			 0x156ee2ca, /// 0x284
			 0x4cca5ccf, /// 0x288
			 0xbc64ddfe, /// 0x28c
			 0x38bb30c2, /// 0x290
			 0x6189421d, /// 0x294
			 0x66bb282d, /// 0x298
			 0x480f12b8, /// 0x29c
			 0x02d461da, /// 0x2a0
			 0x4c4ec530, /// 0x2a4
			 0xed858fa3, /// 0x2a8
			 0x2754da3c, /// 0x2ac
			 0xff929c03, /// 0x2b0
			 0x32441070, /// 0x2b4
			 0xc6f75fb0, /// 0x2b8
			 0x6dbd5292, /// 0x2bc
			 0x620afece, /// 0x2c0
			 0xe1456c21, /// 0x2c4
			 0x3c8a15dd, /// 0x2c8
			 0x59ec3101, /// 0x2cc
			 0x70b2490d, /// 0x2d0
			 0x38f576fd, /// 0x2d4
			 0xd9c34111, /// 0x2d8
			 0x167f91ea, /// 0x2dc
			 0xb0323af7, /// 0x2e0
			 0x746077c7, /// 0x2e4
			 0xceb34a94, /// 0x2e8
			 0x616b8743, /// 0x2ec
			 0x7d4d08d5, /// 0x2f0
			 0xd371106b, /// 0x2f4
			 0xe33c8f0f, /// 0x2f8
			 0xd7eddde3, /// 0x2fc
			 0xaa012a75, /// 0x300
			 0xfda0145a, /// 0x304
			 0x3c0f5c02, /// 0x308
			 0xee2fe5ca, /// 0x30c
			 0x9fc465f1, /// 0x310
			 0x3eed17d5, /// 0x314
			 0x2a75e4b2, /// 0x318
			 0x6f4cba26, /// 0x31c
			 0x96e069b0, /// 0x320
			 0x16d4aee0, /// 0x324
			 0x847f07ec, /// 0x328
			 0x5b4a9cee, /// 0x32c
			 0x132f0d1d, /// 0x330
			 0x6358a42f, /// 0x334
			 0xa35acbc6, /// 0x338
			 0xf0df57a0, /// 0x33c
			 0x04d832f5, /// 0x340
			 0x2d952fdd, /// 0x344
			 0x244c3c13, /// 0x348
			 0x2c9e197e, /// 0x34c
			 0x26b57449, /// 0x350
			 0xdbd6b9d1, /// 0x354
			 0x0a15b8dc, /// 0x358
			 0x5f40988c, /// 0x35c
			 0x955bf4eb, /// 0x360
			 0x19027774, /// 0x364
			 0xa15359a2, /// 0x368
			 0xb5ae50d5, /// 0x36c
			 0x0849151e, /// 0x370
			 0x1a08c9b7, /// 0x374
			 0x1b189ebf, /// 0x378
			 0xe79ee6f5, /// 0x37c
			 0x434a3753, /// 0x380
			 0xb109480f, /// 0x384
			 0x4e31dad0, /// 0x388
			 0x2db668ac, /// 0x38c
			 0xd40b550d, /// 0x390
			 0xa9548abb, /// 0x394
			 0x57eabce2, /// 0x398
			 0x3d1fd7c6, /// 0x39c
			 0x4d38f991, /// 0x3a0
			 0xfc8e51fc, /// 0x3a4
			 0xfb6bdecf, /// 0x3a8
			 0x4a1b9d8e, /// 0x3ac
			 0x802dbd74, /// 0x3b0
			 0xc40804e2, /// 0x3b4
			 0xcf7a2276, /// 0x3b8
			 0x000a1ea3, /// 0x3bc
			 0xa83b613f, /// 0x3c0
			 0x2751daf7, /// 0x3c4
			 0xea9beb04, /// 0x3c8
			 0x52e2caa4, /// 0x3cc
			 0x99d251e3, /// 0x3d0
			 0x559498a5, /// 0x3d4
			 0xe692346a, /// 0x3d8
			 0xedc3e4b0, /// 0x3dc
			 0x6536808b, /// 0x3e0
			 0xc9495af3, /// 0x3e4
			 0x793b0ee5, /// 0x3e8
			 0x464bc172, /// 0x3ec
			 0x2b3a63c8, /// 0x3f0
			 0xedab7a4c, /// 0x3f4
			 0xa9b06242, /// 0x3f8
			 0xd6fe9042, /// 0x3fc
			 0x824a9c33, /// 0x400
			 0x3299b959, /// 0x404
			 0x1ced240b, /// 0x408
			 0xe138abca, /// 0x40c
			 0xd7c76015, /// 0x410
			 0x90500b2b, /// 0x414
			 0x4d5fd97b, /// 0x418
			 0xacb95ef1, /// 0x41c
			 0x1c379343, /// 0x420
			 0x4e8235b9, /// 0x424
			 0xda52c5a1, /// 0x428
			 0xed1f259d, /// 0x42c
			 0xc8ecae96, /// 0x430
			 0x2efd8399, /// 0x434
			 0x0f029f7a, /// 0x438
			 0x175a6551, /// 0x43c
			 0xbda73afb, /// 0x440
			 0xd86cec16, /// 0x444
			 0x88cd39cf, /// 0x448
			 0xfaf03048, /// 0x44c
			 0xc6e4c111, /// 0x450
			 0x3656f2d4, /// 0x454
			 0xa54c45d8, /// 0x458
			 0x2bb68485, /// 0x45c
			 0xba05876c, /// 0x460
			 0xfff02d9c, /// 0x464
			 0xc8a3fa21, /// 0x468
			 0x354e1d40, /// 0x46c
			 0x442db298, /// 0x470
			 0xee09e9a8, /// 0x474
			 0x5b15a3a0, /// 0x478
			 0x88f40cd1, /// 0x47c
			 0x34feb9b2, /// 0x480
			 0x348484b4, /// 0x484
			 0x0e09da7e, /// 0x488
			 0x240d3236, /// 0x48c
			 0x8eae6746, /// 0x490
			 0x1c2792bb, /// 0x494
			 0x5a667620, /// 0x498
			 0x8a0008d9, /// 0x49c
			 0xc940b9b9, /// 0x4a0
			 0x6a5a9219, /// 0x4a4
			 0x303cb887, /// 0x4a8
			 0x1d4a3dcf, /// 0x4ac
			 0xc03029eb, /// 0x4b0
			 0xb32a187c, /// 0x4b4
			 0xfa048cf0, /// 0x4b8
			 0xcb5734df, /// 0x4bc
			 0x6bae1b66, /// 0x4c0
			 0x1fb366aa, /// 0x4c4
			 0xadbe9703, /// 0x4c8
			 0x6e44a5da, /// 0x4cc
			 0x67521c28, /// 0x4d0
			 0xadac1e69, /// 0x4d4
			 0x074ca375, /// 0x4d8
			 0xf5e66018, /// 0x4dc
			 0x62e04906, /// 0x4e0
			 0x6bc06eda, /// 0x4e4
			 0x18b5f3d3, /// 0x4e8
			 0xa3c0c734, /// 0x4ec
			 0x30a2a690, /// 0x4f0
			 0x4cdba0b7, /// 0x4f4
			 0x67c55f23, /// 0x4f8
			 0xc84581a4, /// 0x4fc
			 0x95171fb5, /// 0x500
			 0xee82fe6c, /// 0x504
			 0xb11b1ef5, /// 0x508
			 0x51719b8f, /// 0x50c
			 0xecd357b3, /// 0x510
			 0x201a16ae, /// 0x514
			 0x0b64f035, /// 0x518
			 0xeecee9a5, /// 0x51c
			 0xc39dfe89, /// 0x520
			 0xb1d07607, /// 0x524
			 0x644d5d55, /// 0x528
			 0xe3e69621, /// 0x52c
			 0x55e7b5a0, /// 0x530
			 0x3d862be1, /// 0x534
			 0x30df8425, /// 0x538
			 0xa58448f6, /// 0x53c
			 0xf16fa9e0, /// 0x540
			 0xde574f87, /// 0x544
			 0x2d5b9711, /// 0x548
			 0x422fe764, /// 0x54c
			 0xedb95454, /// 0x550
			 0xc438bbfc, /// 0x554
			 0x5ca8b581, /// 0x558
			 0x200b8db6, /// 0x55c
			 0xa3c073ef, /// 0x560
			 0xdb8c6978, /// 0x564
			 0x62f14580, /// 0x568
			 0x1709596e, /// 0x56c
			 0xbdce9412, /// 0x570
			 0x1e0a130d, /// 0x574
			 0x9baefaee, /// 0x578
			 0x416d6d4b, /// 0x57c
			 0x45bec995, /// 0x580
			 0x33dcf051, /// 0x584
			 0x9ddfb72e, /// 0x588
			 0x9312b49b, /// 0x58c
			 0x498959a6, /// 0x590
			 0x88ed6f8f, /// 0x594
			 0xf4d2d230, /// 0x598
			 0xc267521d, /// 0x59c
			 0x757eb204, /// 0x5a0
			 0xe3fb90b5, /// 0x5a4
			 0xba7337a0, /// 0x5a8
			 0x45cb3aa6, /// 0x5ac
			 0x85ba2d2a, /// 0x5b0
			 0x770a5945, /// 0x5b4
			 0xcdb00415, /// 0x5b8
			 0xa76858f7, /// 0x5bc
			 0x01004ac6, /// 0x5c0
			 0x3d411c4a, /// 0x5c4
			 0x6fa9a833, /// 0x5c8
			 0x55ce4448, /// 0x5cc
			 0x36b55725, /// 0x5d0
			 0xd3ce46d3, /// 0x5d4
			 0x4936db4c, /// 0x5d8
			 0xf63724b6, /// 0x5dc
			 0xc2b80bfc, /// 0x5e0
			 0xac91de13, /// 0x5e4
			 0x523a4b89, /// 0x5e8
			 0xa82e3ef4, /// 0x5ec
			 0x8bb88044, /// 0x5f0
			 0x7c3618cb, /// 0x5f4
			 0x42091a0f, /// 0x5f8
			 0x657345ac, /// 0x5fc
			 0xbad85710, /// 0x600
			 0x06f59244, /// 0x604
			 0xe4d211d4, /// 0x608
			 0x7e35ac0f, /// 0x60c
			 0x4d443a53, /// 0x610
			 0xe4e53a60, /// 0x614
			 0x6ca08730, /// 0x618
			 0xf36a7ad6, /// 0x61c
			 0x1809602f, /// 0x620
			 0x3158aafc, /// 0x624
			 0x0d409e53, /// 0x628
			 0x7f26a820, /// 0x62c
			 0x510db06a, /// 0x630
			 0x127a30d7, /// 0x634
			 0x1fad6031, /// 0x638
			 0xe793137e, /// 0x63c
			 0x17baf632, /// 0x640
			 0xa008e7de, /// 0x644
			 0x4a659b95, /// 0x648
			 0x6f531053, /// 0x64c
			 0xe11b403d, /// 0x650
			 0x165b2c89, /// 0x654
			 0x1fb048c1, /// 0x658
			 0x266aa353, /// 0x65c
			 0x842f0c0f, /// 0x660
			 0x00de022c, /// 0x664
			 0x4c3797dc, /// 0x668
			 0x05962982, /// 0x66c
			 0x92228d87, /// 0x670
			 0xd62bfdb0, /// 0x674
			 0xcc741be2, /// 0x678
			 0x61eccaf5, /// 0x67c
			 0xa5643d57, /// 0x680
			 0x9cbbdb81, /// 0x684
			 0xe39128cc, /// 0x688
			 0x3eed0c7f, /// 0x68c
			 0x78859e31, /// 0x690
			 0xa97fbd7c, /// 0x694
			 0xfc7fdb3d, /// 0x698
			 0x89d8746d, /// 0x69c
			 0x267e163b, /// 0x6a0
			 0xf1c065ac, /// 0x6a4
			 0xe41b1685, /// 0x6a8
			 0xd2888c87, /// 0x6ac
			 0x4e44457d, /// 0x6b0
			 0x0dc59af8, /// 0x6b4
			 0x80b6452f, /// 0x6b8
			 0x462b713b, /// 0x6bc
			 0x91585219, /// 0x6c0
			 0x801f2019, /// 0x6c4
			 0x28829729, /// 0x6c8
			 0x43c0e0af, /// 0x6cc
			 0xbd1e7918, /// 0x6d0
			 0xdfcf202f, /// 0x6d4
			 0x5d4731d5, /// 0x6d8
			 0x10bf5a77, /// 0x6dc
			 0xdc7390f6, /// 0x6e0
			 0x367c3969, /// 0x6e4
			 0x2c6f3f72, /// 0x6e8
			 0x0609df25, /// 0x6ec
			 0x5cf74cf0, /// 0x6f0
			 0x0ae7cb85, /// 0x6f4
			 0xb3f09e98, /// 0x6f8
			 0xd22a98a0, /// 0x6fc
			 0xe50e9482, /// 0x700
			 0xbaaa4627, /// 0x704
			 0x54e02ff7, /// 0x708
			 0xd0b005a3, /// 0x70c
			 0x2cef9338, /// 0x710
			 0x1e11466b, /// 0x714
			 0x9be331fd, /// 0x718
			 0xcfd8a59c, /// 0x71c
			 0x65290f86, /// 0x720
			 0x44e55dc6, /// 0x724
			 0xd14958a4, /// 0x728
			 0x92c879ce, /// 0x72c
			 0x00f8799e, /// 0x730
			 0x4f44ed9f, /// 0x734
			 0x0ad22f3d, /// 0x738
			 0x78ae3f9a, /// 0x73c
			 0x227d7367, /// 0x740
			 0x9ee71a8f, /// 0x744
			 0x882c14cb, /// 0x748
			 0xa4bf35c2, /// 0x74c
			 0xde5bbc69, /// 0x750
			 0xa3909937, /// 0x754
			 0x40ed4d5a, /// 0x758
			 0x30017699, /// 0x75c
			 0x063faa07, /// 0x760
			 0x1eb2c80a, /// 0x764
			 0xa13163af, /// 0x768
			 0xc326ed88, /// 0x76c
			 0x8388655e, /// 0x770
			 0xa1bd973e, /// 0x774
			 0x14e9e506, /// 0x778
			 0x3ad894ca, /// 0x77c
			 0xcdabf69b, /// 0x780
			 0x43de13f0, /// 0x784
			 0xb8f6d0f5, /// 0x788
			 0x5942da99, /// 0x78c
			 0x5e15d41f, /// 0x790
			 0x91e763c9, /// 0x794
			 0xf91772d8, /// 0x798
			 0x5615d4c5, /// 0x79c
			 0x8ab2bc09, /// 0x7a0
			 0x189478e8, /// 0x7a4
			 0xe06859d0, /// 0x7a8
			 0xf49cd8f4, /// 0x7ac
			 0xcf80270a, /// 0x7b0
			 0xc13e6626, /// 0x7b4
			 0x75aa5b4e, /// 0x7b8
			 0xc0676bff, /// 0x7bc
			 0x69e1e494, /// 0x7c0
			 0xbe228b0a, /// 0x7c4
			 0xb761e50d, /// 0x7c8
			 0x245fff08, /// 0x7cc
			 0x92fa24dd, /// 0x7d0
			 0xe6b09644, /// 0x7d4
			 0x3cb2c736, /// 0x7d8
			 0xcd9eba94, /// 0x7dc
			 0xba9327bc, /// 0x7e0
			 0xa3fd85de, /// 0x7e4
			 0xe102c72e, /// 0x7e8
			 0xa94e2bc0, /// 0x7ec
			 0x958db759, /// 0x7f0
			 0x67b35ff4, /// 0x7f4
			 0x214edf91, /// 0x7f8
			 0x21b06a04, /// 0x7fc
			 0xa33b7d11, /// 0x800
			 0x3f6bb1fb, /// 0x804
			 0x512811fb, /// 0x808
			 0xe07d19ce, /// 0x80c
			 0x9db572d2, /// 0x810
			 0x9956797f, /// 0x814
			 0xc06caca6, /// 0x818
			 0xd1286ce3, /// 0x81c
			 0xe25d7647, /// 0x820
			 0xf5b864dd, /// 0x824
			 0x77e16e37, /// 0x828
			 0xe95f5dc8, /// 0x82c
			 0xd0664e08, /// 0x830
			 0xd01e9791, /// 0x834
			 0x5a37c0ac, /// 0x838
			 0x23b40b76, /// 0x83c
			 0xd4306b61, /// 0x840
			 0xf9171b3c, /// 0x844
			 0x149d089e, /// 0x848
			 0xb5e079b5, /// 0x84c
			 0x4d6388ea, /// 0x850
			 0xaa52bbb8, /// 0x854
			 0x565ca355, /// 0x858
			 0x601f87ff, /// 0x85c
			 0xa95d6326, /// 0x860
			 0x895ee384, /// 0x864
			 0x1e3774fc, /// 0x868
			 0x8ef88b2d, /// 0x86c
			 0x987b8912, /// 0x870
			 0xb32fb7a9, /// 0x874
			 0xca47a0c9, /// 0x878
			 0xe7c96ab2, /// 0x87c
			 0xfe53960d, /// 0x880
			 0x8427b4c2, /// 0x884
			 0x1b4b5f73, /// 0x888
			 0x589ea163, /// 0x88c
			 0xf76a6527, /// 0x890
			 0xdd125242, /// 0x894
			 0x5f352004, /// 0x898
			 0xaeaf1b74, /// 0x89c
			 0xca067236, /// 0x8a0
			 0xa6743c69, /// 0x8a4
			 0x469eb81a, /// 0x8a8
			 0xab3f57d0, /// 0x8ac
			 0x3221dc7e, /// 0x8b0
			 0x0219b195, /// 0x8b4
			 0x6895da59, /// 0x8b8
			 0x45576459, /// 0x8bc
			 0xa19b8f4f, /// 0x8c0
			 0xe2bd7fee, /// 0x8c4
			 0x0e91d603, /// 0x8c8
			 0xce7aff33, /// 0x8cc
			 0xa4676900, /// 0x8d0
			 0x3d04ead8, /// 0x8d4
			 0xe01fba9b, /// 0x8d8
			 0x887b63e7, /// 0x8dc
			 0x3497002f, /// 0x8e0
			 0x671c1c51, /// 0x8e4
			 0x0f762ecd, /// 0x8e8
			 0xc2925d86, /// 0x8ec
			 0x9eb99d9c, /// 0x8f0
			 0x4ed9da27, /// 0x8f4
			 0xf489065d, /// 0x8f8
			 0x39bd0e2f, /// 0x8fc
			 0xe04dfcbe, /// 0x900
			 0x9961ff71, /// 0x904
			 0x4fccdf61, /// 0x908
			 0xa03e071e, /// 0x90c
			 0x834e056a, /// 0x910
			 0x5c1ea191, /// 0x914
			 0xa552f678, /// 0x918
			 0x36d8589f, /// 0x91c
			 0x7be862e0, /// 0x920
			 0x4b3edfc9, /// 0x924
			 0x6c688378, /// 0x928
			 0x2f689293, /// 0x92c
			 0x7c2362c4, /// 0x930
			 0xe2f1fb08, /// 0x934
			 0xb60520c3, /// 0x938
			 0x427aed67, /// 0x93c
			 0xcf794b9f, /// 0x940
			 0x70dfe93c, /// 0x944
			 0x5fb38cd4, /// 0x948
			 0xda5b0af7, /// 0x94c
			 0x79ed73db, /// 0x950
			 0x0988a4f4, /// 0x954
			 0xe7e75296, /// 0x958
			 0xb7098aa8, /// 0x95c
			 0x44b5fa07, /// 0x960
			 0x393be9aa, /// 0x964
			 0xc534614d, /// 0x968
			 0x61840fda, /// 0x96c
			 0xb3f840ed, /// 0x970
			 0xe183ce0f, /// 0x974
			 0xc7a51cd9, /// 0x978
			 0xf645b559, /// 0x97c
			 0x12be7961, /// 0x980
			 0xc852b902, /// 0x984
			 0xc4ed62ce, /// 0x988
			 0xabfd6810, /// 0x98c
			 0xe4c2ae34, /// 0x990
			 0xed8f0e93, /// 0x994
			 0x0213fab5, /// 0x998
			 0x0274ec84, /// 0x99c
			 0xc5ff314f, /// 0x9a0
			 0xc38c2d18, /// 0x9a4
			 0xdbdfe0f0, /// 0x9a8
			 0xf6068583, /// 0x9ac
			 0x9f4a2a04, /// 0x9b0
			 0x239f5e0c, /// 0x9b4
			 0xe79867af, /// 0x9b8
			 0x11470304, /// 0x9bc
			 0x386a7114, /// 0x9c0
			 0xbe5584a1, /// 0x9c4
			 0x21cc0922, /// 0x9c8
			 0x825c0d36, /// 0x9cc
			 0x01f573b0, /// 0x9d0
			 0x53ee3a58, /// 0x9d4
			 0x3607a5af, /// 0x9d8
			 0xdbd92c85, /// 0x9dc
			 0x142e53d1, /// 0x9e0
			 0x36ffd119, /// 0x9e4
			 0x6f15f47a, /// 0x9e8
			 0x6511e1bc, /// 0x9ec
			 0xf2eb20cf, /// 0x9f0
			 0xb01a94a5, /// 0x9f4
			 0x55e36a9c, /// 0x9f8
			 0x17538b2b, /// 0x9fc
			 0x7a05c1ae, /// 0xa00
			 0x1b58e84d, /// 0xa04
			 0xbebacb82, /// 0xa08
			 0x91f15a71, /// 0xa0c
			 0xeb25245f, /// 0xa10
			 0x54d4d81e, /// 0xa14
			 0x8cb168fb, /// 0xa18
			 0x5a582f39, /// 0xa1c
			 0x7bb4c791, /// 0xa20
			 0xac905d5e, /// 0xa24
			 0x99ce71f6, /// 0xa28
			 0x30007d9b, /// 0xa2c
			 0x8ea240a4, /// 0xa30
			 0x128a4b08, /// 0xa34
			 0xc1244e23, /// 0xa38
			 0xc929fd95, /// 0xa3c
			 0x2c2d4a9b, /// 0xa40
			 0x3431433a, /// 0xa44
			 0xfc5a39e6, /// 0xa48
			 0x9d89f305, /// 0xa4c
			 0xb13812c0, /// 0xa50
			 0x34ced426, /// 0xa54
			 0x51db4569, /// 0xa58
			 0x902b7fc8, /// 0xa5c
			 0x0bd996cb, /// 0xa60
			 0x5b9cf154, /// 0xa64
			 0xdc28830c, /// 0xa68
			 0xb1c8be5a, /// 0xa6c
			 0x7e3e6cf3, /// 0xa70
			 0xd43e74c2, /// 0xa74
			 0x623761c3, /// 0xa78
			 0x2cc05c41, /// 0xa7c
			 0xf555d2f4, /// 0xa80
			 0x743c23b0, /// 0xa84
			 0xbd664381, /// 0xa88
			 0x8f24a3e8, /// 0xa8c
			 0x21167d2f, /// 0xa90
			 0x6c941d12, /// 0xa94
			 0x65c36c40, /// 0xa98
			 0x504ecfdb, /// 0xa9c
			 0xe1971d8b, /// 0xaa0
			 0x5cd0a3d0, /// 0xaa4
			 0xee9113a7, /// 0xaa8
			 0xe0a8a8a3, /// 0xaac
			 0xb520cede, /// 0xab0
			 0x25c7fcc1, /// 0xab4
			 0x511c8242, /// 0xab8
			 0x07834245, /// 0xabc
			 0x16d2367b, /// 0xac0
			 0x784175b7, /// 0xac4
			 0xdca70d37, /// 0xac8
			 0xd3770f3d, /// 0xacc
			 0xb9c8f441, /// 0xad0
			 0xe0a93ba7, /// 0xad4
			 0x72140fea, /// 0xad8
			 0x50e58c7f, /// 0xadc
			 0xefe8e571, /// 0xae0
			 0x5583954c, /// 0xae4
			 0xc8142687, /// 0xae8
			 0x548cfd9f, /// 0xaec
			 0x302c2350, /// 0xaf0
			 0x93eb2eff, /// 0xaf4
			 0x7f14fbd1, /// 0xaf8
			 0x40796762, /// 0xafc
			 0x8582c88d, /// 0xb00
			 0x52f25a26, /// 0xb04
			 0xeab31869, /// 0xb08
			 0xdb77e386, /// 0xb0c
			 0x8f906b05, /// 0xb10
			 0x4dee4951, /// 0xb14
			 0x97bdd244, /// 0xb18
			 0x0e0bc67d, /// 0xb1c
			 0xec7bbf54, /// 0xb20
			 0x45a729bf, /// 0xb24
			 0xe73009f2, /// 0xb28
			 0xfd707893, /// 0xb2c
			 0x3521245d, /// 0xb30
			 0x11d5adcd, /// 0xb34
			 0xbfe56a02, /// 0xb38
			 0xb311c906, /// 0xb3c
			 0xedd63e93, /// 0xb40
			 0xfa7ae63e, /// 0xb44
			 0x85dee532, /// 0xb48
			 0xaef44cbf, /// 0xb4c
			 0xa13e30cc, /// 0xb50
			 0x903a718b, /// 0xb54
			 0xc0db8187, /// 0xb58
			 0x0dd66fd7, /// 0xb5c
			 0xe843321a, /// 0xb60
			 0x717ac9ac, /// 0xb64
			 0xc00d8abd, /// 0xb68
			 0x3daa8d4e, /// 0xb6c
			 0x29041bbf, /// 0xb70
			 0x791fa891, /// 0xb74
			 0xf23f6973, /// 0xb78
			 0x585727d1, /// 0xb7c
			 0xa85574a2, /// 0xb80
			 0x052d12bc, /// 0xb84
			 0xc0a00078, /// 0xb88
			 0x2914b684, /// 0xb8c
			 0xa23c4d05, /// 0xb90
			 0x2ee05d45, /// 0xb94
			 0xaa2b9c04, /// 0xb98
			 0xba6bb152, /// 0xb9c
			 0x503038f8, /// 0xba0
			 0x54dfef21, /// 0xba4
			 0x106b4ab6, /// 0xba8
			 0xb29c055a, /// 0xbac
			 0x986450dd, /// 0xbb0
			 0xe4bb2b98, /// 0xbb4
			 0x8de39606, /// 0xbb8
			 0x716a9e2f, /// 0xbbc
			 0xfd0649ac, /// 0xbc0
			 0x59c09585, /// 0xbc4
			 0x29d55ac6, /// 0xbc8
			 0x22b5d1e5, /// 0xbcc
			 0x0f1ed257, /// 0xbd0
			 0x9b773ace, /// 0xbd4
			 0xfd20632f, /// 0xbd8
			 0x2898106d, /// 0xbdc
			 0xe561244e, /// 0xbe0
			 0xf80d9524, /// 0xbe4
			 0xe63c3f39, /// 0xbe8
			 0xda5f5ae1, /// 0xbec
			 0xa5073146, /// 0xbf0
			 0x8178ca37, /// 0xbf4
			 0x20df269d, /// 0xbf8
			 0xd5085f07, /// 0xbfc
			 0x820906fc, /// 0xc00
			 0x39dd02a8, /// 0xc04
			 0xc917b5c7, /// 0xc08
			 0xe8efe4dd, /// 0xc0c
			 0x526f0cf1, /// 0xc10
			 0x22b447f4, /// 0xc14
			 0x7e73c638, /// 0xc18
			 0xeca1d49e, /// 0xc1c
			 0xa68cb907, /// 0xc20
			 0x5f613c22, /// 0xc24
			 0x31703bf9, /// 0xc28
			 0x34117584, /// 0xc2c
			 0xc5ef7ddd, /// 0xc30
			 0xec367406, /// 0xc34
			 0x58d5d6df, /// 0xc38
			 0xd19b08fb, /// 0xc3c
			 0xc3822e68, /// 0xc40
			 0x9d57ac59, /// 0xc44
			 0x500c766a, /// 0xc48
			 0x1b0f533f, /// 0xc4c
			 0x26fa63c9, /// 0xc50
			 0xf0023e4b, /// 0xc54
			 0x9629b7d4, /// 0xc58
			 0x9cc5cac8, /// 0xc5c
			 0x3f562a6b, /// 0xc60
			 0x3420965d, /// 0xc64
			 0x60dfb5a0, /// 0xc68
			 0xf506e098, /// 0xc6c
			 0x4ae8637e, /// 0xc70
			 0x6ab55abf, /// 0xc74
			 0x8146f68e, /// 0xc78
			 0x883b5ee0, /// 0xc7c
			 0x3ffd44e4, /// 0xc80
			 0xaba01b34, /// 0xc84
			 0xcee17dc2, /// 0xc88
			 0x91b88453, /// 0xc8c
			 0xb1a9daad, /// 0xc90
			 0x7fae9370, /// 0xc94
			 0x8aeab8d2, /// 0xc98
			 0x71e1c7f7, /// 0xc9c
			 0xc20e33d4, /// 0xca0
			 0x7e964e84, /// 0xca4
			 0x10b47731, /// 0xca8
			 0x014b4b63, /// 0xcac
			 0x95309912, /// 0xcb0
			 0x9a904e80, /// 0xcb4
			 0x770bb0c8, /// 0xcb8
			 0x439ff741, /// 0xcbc
			 0x611d5aa6, /// 0xcc0
			 0xbb788148, /// 0xcc4
			 0x5dcf41d2, /// 0xcc8
			 0xbd6bef46, /// 0xccc
			 0xec2920b0, /// 0xcd0
			 0x10e83aa9, /// 0xcd4
			 0xe3eafb54, /// 0xcd8
			 0x8740d1a1, /// 0xcdc
			 0x72a68455, /// 0xce0
			 0xf132a098, /// 0xce4
			 0x05f81f82, /// 0xce8
			 0xd04acea2, /// 0xcec
			 0x8598daa7, /// 0xcf0
			 0x6577c911, /// 0xcf4
			 0x8e0f232c, /// 0xcf8
			 0xfc777507, /// 0xcfc
			 0x3fce3ef7, /// 0xd00
			 0x97a78725, /// 0xd04
			 0xde40973a, /// 0xd08
			 0x4c59242c, /// 0xd0c
			 0x0a7922ef, /// 0xd10
			 0xcd9a7238, /// 0xd14
			 0x30a0dcb5, /// 0xd18
			 0x953fd080, /// 0xd1c
			 0x3b1850dd, /// 0xd20
			 0xbfb8adfc, /// 0xd24
			 0x02e23937, /// 0xd28
			 0xc2308f61, /// 0xd2c
			 0x4d5885fa, /// 0xd30
			 0x33cfb182, /// 0xd34
			 0x59e95139, /// 0xd38
			 0x96df1b8b, /// 0xd3c
			 0x48b0e8a4, /// 0xd40
			 0x4d8ebce6, /// 0xd44
			 0xb0add842, /// 0xd48
			 0xd755da18, /// 0xd4c
			 0x0f4fbbe9, /// 0xd50
			 0x0d79e149, /// 0xd54
			 0xc83cb439, /// 0xd58
			 0x6fb001fc, /// 0xd5c
			 0xfd443858, /// 0xd60
			 0x2ff19506, /// 0xd64
			 0xba79d222, /// 0xd68
			 0x41698755, /// 0xd6c
			 0x03d02274, /// 0xd70
			 0x6e8600f7, /// 0xd74
			 0xbeb849a6, /// 0xd78
			 0x515a3bf1, /// 0xd7c
			 0xc6e6f7a6, /// 0xd80
			 0xa0233d5c, /// 0xd84
			 0x0ffe7189, /// 0xd88
			 0xa5322dbc, /// 0xd8c
			 0xdcd8012d, /// 0xd90
			 0x848b50de, /// 0xd94
			 0xaa026f31, /// 0xd98
			 0x1a1c4e66, /// 0xd9c
			 0x2b55f645, /// 0xda0
			 0xb60512f3, /// 0xda4
			 0x7726500d, /// 0xda8
			 0x370a0f52, /// 0xdac
			 0xe6706cd0, /// 0xdb0
			 0xf008332b, /// 0xdb4
			 0x855b25f8, /// 0xdb8
			 0xe4945d28, /// 0xdbc
			 0xd4cd576a, /// 0xdc0
			 0x1b68bb50, /// 0xdc4
			 0xc397f820, /// 0xdc8
			 0xf6c78123, /// 0xdcc
			 0x490ef1e2, /// 0xdd0
			 0x7b4a50bc, /// 0xdd4
			 0x6eda2a6d, /// 0xdd8
			 0x8a473dab, /// 0xddc
			 0x66700628, /// 0xde0
			 0x6febcfd3, /// 0xde4
			 0x783ada72, /// 0xde8
			 0x6f837bdb, /// 0xdec
			 0x19f473f1, /// 0xdf0
			 0xdeaa9c58, /// 0xdf4
			 0x6fa182c4, /// 0xdf8
			 0xbefb4510, /// 0xdfc
			 0xd4e78ff4, /// 0xe00
			 0x0fd08f8c, /// 0xe04
			 0xc27a4cd2, /// 0xe08
			 0xb2f6e754, /// 0xe0c
			 0x8c61a34a, /// 0xe10
			 0x4f29714c, /// 0xe14
			 0x4e072ddd, /// 0xe18
			 0x5861ddc5, /// 0xe1c
			 0xe7429d6b, /// 0xe20
			 0xeac53f0b, /// 0xe24
			 0x508853a0, /// 0xe28
			 0x0b723188, /// 0xe2c
			 0x3dadce06, /// 0xe30
			 0xa2913c3b, /// 0xe34
			 0xb2b8c199, /// 0xe38
			 0xc77e411d, /// 0xe3c
			 0x4a5c5fa7, /// 0xe40
			 0xad04c1b4, /// 0xe44
			 0x078d414c, /// 0xe48
			 0x9f9a25e3, /// 0xe4c
			 0x95f63c35, /// 0xe50
			 0x8ae33c30, /// 0xe54
			 0x68577ec6, /// 0xe58
			 0x7df57013, /// 0xe5c
			 0x21fef334, /// 0xe60
			 0x61c3c895, /// 0xe64
			 0xe5432905, /// 0xe68
			 0xbdf10a52, /// 0xe6c
			 0x855733c3, /// 0xe70
			 0xbda46cea, /// 0xe74
			 0x3f1096e4, /// 0xe78
			 0xc1d8c3aa, /// 0xe7c
			 0xc3b2505f, /// 0xe80
			 0xad4e8558, /// 0xe84
			 0x648f8139, /// 0xe88
			 0xf8e38276, /// 0xe8c
			 0x31040133, /// 0xe90
			 0xbccb874a, /// 0xe94
			 0xe86612cf, /// 0xe98
			 0x54789e44, /// 0xe9c
			 0x459e5c9a, /// 0xea0
			 0x0235d122, /// 0xea4
			 0x66e1cfc5, /// 0xea8
			 0x79718c47, /// 0xeac
			 0x732f85b2, /// 0xeb0
			 0xb30d1b9d, /// 0xeb4
			 0x0079c2d2, /// 0xeb8
			 0x18ddebde, /// 0xebc
			 0xd2d452dd, /// 0xec0
			 0x2a889cf3, /// 0xec4
			 0x5c3ba582, /// 0xec8
			 0x631070aa, /// 0xecc
			 0x0db84247, /// 0xed0
			 0xcfa3da48, /// 0xed4
			 0xcfdfb6ef, /// 0xed8
			 0x3a1a74bd, /// 0xedc
			 0x788c755f, /// 0xee0
			 0x7704b497, /// 0xee4
			 0xe04b78b3, /// 0xee8
			 0xc32ff34d, /// 0xeec
			 0xafc6c8ae, /// 0xef0
			 0x2df382df, /// 0xef4
			 0x996d0435, /// 0xef8
			 0xc027ad43, /// 0xefc
			 0xfdcc966a, /// 0xf00
			 0x8e87e8f2, /// 0xf04
			 0x41218575, /// 0xf08
			 0x52a25f40, /// 0xf0c
			 0xfa4af046, /// 0xf10
			 0x74410f1a, /// 0xf14
			 0x13237da7, /// 0xf18
			 0x22df3981, /// 0xf1c
			 0x1971738e, /// 0xf20
			 0x12851848, /// 0xf24
			 0xc212a3ad, /// 0xf28
			 0xc75b34f0, /// 0xf2c
			 0xd846db72, /// 0xf30
			 0x2d25eff9, /// 0xf34
			 0x30ef8e56, /// 0xf38
			 0x3ea83753, /// 0xf3c
			 0xdb52c2b4, /// 0xf40
			 0xd7139ccf, /// 0xf44
			 0xaa70de9d, /// 0xf48
			 0x8092a3ae, /// 0xf4c
			 0x1614d17a, /// 0xf50
			 0xbb8bfa67, /// 0xf54
			 0x1573308f, /// 0xf58
			 0x4293e173, /// 0xf5c
			 0xe6516fe5, /// 0xf60
			 0xf300004a, /// 0xf64
			 0xf201ce75, /// 0xf68
			 0x46ce8ba3, /// 0xf6c
			 0x50e4667b, /// 0xf70
			 0x8b1b4637, /// 0xf74
			 0xd5571b20, /// 0xf78
			 0xc9e79734, /// 0xf7c
			 0x301dd0a5, /// 0xf80
			 0x7751f03a, /// 0xf84
			 0xcd0ad7e2, /// 0xf88
			 0x2517dcdf, /// 0xf8c
			 0x6036c0ec, /// 0xf90
			 0xe9ef66be, /// 0xf94
			 0x6d097160, /// 0xf98
			 0x124ea942, /// 0xf9c
			 0x80fcdefb, /// 0xfa0
			 0x59d07780, /// 0xfa4
			 0x55a308a6, /// 0xfa8
			 0x0d475baa, /// 0xfac
			 0x28115ce7, /// 0xfb0
			 0x3021f8fa, /// 0xfb4
			 0x770d4fef, /// 0xfb8
			 0x6152e0d4, /// 0xfbc
			 0xd6b965dc, /// 0xfc0
			 0x06be0f96, /// 0xfc4
			 0xa2f6c04b, /// 0xfc8
			 0xc648c913, /// 0xfcc
			 0x7f517157, /// 0xfd0
			 0x81a702de, /// 0xfd4
			 0x92dbf1c5, /// 0xfd8
			 0x33436671, /// 0xfdc
			 0x2e822927, /// 0xfe0
			 0x0da26594, /// 0xfe4
			 0x35af44b8, /// 0xfe8
			 0xd9cb202c, /// 0xfec
			 0x1de91cb0, /// 0xff0
			 0x17375e9f, /// 0xff4
			 0xe4d246e7, /// 0xff8
			 0xe563df8a /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x56aae39d, /// 0x0
			 0xf50f74be, /// 0x4
			 0x30022ea2, /// 0x8
			 0x936b1d70, /// 0xc
			 0x6c29f3cd, /// 0x10
			 0x48bb397d, /// 0x14
			 0x899a1118, /// 0x18
			 0x82512b4c, /// 0x1c
			 0xabc60299, /// 0x20
			 0xe9d8ed3f, /// 0x24
			 0x99c1208e, /// 0x28
			 0xd8e0e03d, /// 0x2c
			 0x33f98efc, /// 0x30
			 0xe5fbb111, /// 0x34
			 0xd72f63a6, /// 0x38
			 0xba363e25, /// 0x3c
			 0x1a654584, /// 0x40
			 0x52d09e00, /// 0x44
			 0x98e33fbd, /// 0x48
			 0x4a95b363, /// 0x4c
			 0x8c13ba92, /// 0x50
			 0xa988bcb6, /// 0x54
			 0xcb27dee9, /// 0x58
			 0x6cb8ab98, /// 0x5c
			 0x7c41f187, /// 0x60
			 0xeb8c9456, /// 0x64
			 0xf22ffcdf, /// 0x68
			 0x64ab8d76, /// 0x6c
			 0x8fe311bb, /// 0x70
			 0xc64a28eb, /// 0x74
			 0xb9f843bf, /// 0x78
			 0xe1f43010, /// 0x7c
			 0x80231474, /// 0x80
			 0xf954d376, /// 0x84
			 0x68703b0a, /// 0x88
			 0x605f1d86, /// 0x8c
			 0xc43c8ef2, /// 0x90
			 0x5f967abe, /// 0x94
			 0x6b7aaab3, /// 0x98
			 0x5d99b6c0, /// 0x9c
			 0xf546a22e, /// 0xa0
			 0xf765f464, /// 0xa4
			 0x04d8637e, /// 0xa8
			 0xda591401, /// 0xac
			 0x7c9ce950, /// 0xb0
			 0xf6e46b6b, /// 0xb4
			 0x4cb2ad9b, /// 0xb8
			 0x9292b47b, /// 0xbc
			 0x7117cafe, /// 0xc0
			 0x44d5e56e, /// 0xc4
			 0x90898229, /// 0xc8
			 0xab94e58a, /// 0xcc
			 0x28356913, /// 0xd0
			 0x3501b298, /// 0xd4
			 0xf29e2b57, /// 0xd8
			 0xa984b044, /// 0xdc
			 0x7ce48adf, /// 0xe0
			 0x8d3aac6c, /// 0xe4
			 0x7169d6ea, /// 0xe8
			 0x26c37bd1, /// 0xec
			 0x9fd823ce, /// 0xf0
			 0x55f737f5, /// 0xf4
			 0x80b709a1, /// 0xf8
			 0xf50f6d53, /// 0xfc
			 0xdab1611c, /// 0x100
			 0x33ee9844, /// 0x104
			 0x41579278, /// 0x108
			 0x9ed07d2e, /// 0x10c
			 0x9280f876, /// 0x110
			 0xfb88f8e9, /// 0x114
			 0x52620640, /// 0x118
			 0xea4b329d, /// 0x11c
			 0x70f592d8, /// 0x120
			 0x4d731035, /// 0x124
			 0x5949a6e8, /// 0x128
			 0x5be2a52a, /// 0x12c
			 0x1744694e, /// 0x130
			 0xc03e4ee0, /// 0x134
			 0x41d6cc3c, /// 0x138
			 0x0db03e22, /// 0x13c
			 0x848a4804, /// 0x140
			 0x00eb9d10, /// 0x144
			 0x1aed5a9e, /// 0x148
			 0x4dc58446, /// 0x14c
			 0xb51c36f7, /// 0x150
			 0x56e91c8f, /// 0x154
			 0x4f053cdc, /// 0x158
			 0xb83a9370, /// 0x15c
			 0xa8afd4c6, /// 0x160
			 0xf265eaab, /// 0x164
			 0x7feb39e6, /// 0x168
			 0xa23ba4e6, /// 0x16c
			 0xdfea28bf, /// 0x170
			 0x3357f00b, /// 0x174
			 0xf498d6a6, /// 0x178
			 0x44e4bb04, /// 0x17c
			 0x28705ddb, /// 0x180
			 0x2200533d, /// 0x184
			 0xc3247db5, /// 0x188
			 0x2d1f36d3, /// 0x18c
			 0xee7c3af5, /// 0x190
			 0x9ecb6aa3, /// 0x194
			 0xdc0fedd9, /// 0x198
			 0x55ace20b, /// 0x19c
			 0xb07b2a9c, /// 0x1a0
			 0xe846d2fc, /// 0x1a4
			 0x2dc8a33c, /// 0x1a8
			 0x0f329b69, /// 0x1ac
			 0xcc143c19, /// 0x1b0
			 0xe1d48064, /// 0x1b4
			 0xbddebaf1, /// 0x1b8
			 0xb489f979, /// 0x1bc
			 0x267bd5bd, /// 0x1c0
			 0x77067835, /// 0x1c4
			 0x64a212cd, /// 0x1c8
			 0x19e66c7e, /// 0x1cc
			 0xe74601f9, /// 0x1d0
			 0x4b8194d9, /// 0x1d4
			 0xeeca769e, /// 0x1d8
			 0xe1c96a7d, /// 0x1dc
			 0xa5c56764, /// 0x1e0
			 0x8e5567d3, /// 0x1e4
			 0xa1de53c0, /// 0x1e8
			 0x2f8e3de9, /// 0x1ec
			 0xed36171f, /// 0x1f0
			 0x734bb728, /// 0x1f4
			 0xb7200748, /// 0x1f8
			 0xc48d6ae3, /// 0x1fc
			 0xd76b6172, /// 0x200
			 0xc1b62340, /// 0x204
			 0x37156a2b, /// 0x208
			 0xad7d9395, /// 0x20c
			 0x205e3ead, /// 0x210
			 0x829c1ed2, /// 0x214
			 0x0e53852f, /// 0x218
			 0xa3dcac95, /// 0x21c
			 0xc00977ce, /// 0x220
			 0x96ec1464, /// 0x224
			 0xf2c8646a, /// 0x228
			 0xf3d9ad64, /// 0x22c
			 0xda38b328, /// 0x230
			 0x6f2834bd, /// 0x234
			 0xbe03bc74, /// 0x238
			 0x7c313d2e, /// 0x23c
			 0x3c0b9f65, /// 0x240
			 0x33780d20, /// 0x244
			 0x4855c484, /// 0x248
			 0x7307290c, /// 0x24c
			 0x645e2b28, /// 0x250
			 0xe9957c63, /// 0x254
			 0xcf1353f8, /// 0x258
			 0x95ac51c8, /// 0x25c
			 0x3361fc5f, /// 0x260
			 0xbe5fc93b, /// 0x264
			 0x5bc06452, /// 0x268
			 0x52f73b44, /// 0x26c
			 0x6be62fcb, /// 0x270
			 0x7844a0bf, /// 0x274
			 0xec60a7c4, /// 0x278
			 0xaa1dc452, /// 0x27c
			 0x59abaa06, /// 0x280
			 0xd40d57ba, /// 0x284
			 0x9a8e9329, /// 0x288
			 0x7489d8fe, /// 0x28c
			 0xd601174e, /// 0x290
			 0x2ac81aa9, /// 0x294
			 0x90ad84a0, /// 0x298
			 0xb74fed81, /// 0x29c
			 0x1b6390e1, /// 0x2a0
			 0xff55786b, /// 0x2a4
			 0xda67672f, /// 0x2a8
			 0xc7ca9701, /// 0x2ac
			 0xbb490cf3, /// 0x2b0
			 0xb2ef2e77, /// 0x2b4
			 0x4ecb52f1, /// 0x2b8
			 0xc77bc709, /// 0x2bc
			 0xd9d7c957, /// 0x2c0
			 0x267032f0, /// 0x2c4
			 0x7d060bfa, /// 0x2c8
			 0x7aff16b9, /// 0x2cc
			 0xdc810246, /// 0x2d0
			 0xcf71261b, /// 0x2d4
			 0xcc85dfa8, /// 0x2d8
			 0xd377d5cc, /// 0x2dc
			 0x5aaddf8e, /// 0x2e0
			 0x100c8352, /// 0x2e4
			 0xfce6ce83, /// 0x2e8
			 0xd819aae6, /// 0x2ec
			 0x91d0e691, /// 0x2f0
			 0x44de0897, /// 0x2f4
			 0xdb5ced9e, /// 0x2f8
			 0xa684da21, /// 0x2fc
			 0xf59216e1, /// 0x300
			 0x19ffc889, /// 0x304
			 0xbd71915a, /// 0x308
			 0xee2f45d4, /// 0x30c
			 0xaa9008e1, /// 0x310
			 0x851c09b5, /// 0x314
			 0xc144195b, /// 0x318
			 0x62e614b3, /// 0x31c
			 0x38f8353d, /// 0x320
			 0xffea28e8, /// 0x324
			 0xb150a6d3, /// 0x328
			 0xcbd0dd3c, /// 0x32c
			 0xdeee53b9, /// 0x330
			 0xf56c6b3f, /// 0x334
			 0x5013e9ad, /// 0x338
			 0x2e5914d7, /// 0x33c
			 0x4e3e6d40, /// 0x340
			 0x91698920, /// 0x344
			 0xf0a2d0ee, /// 0x348
			 0xdd32011f, /// 0x34c
			 0xd6d0f601, /// 0x350
			 0x9243ba4a, /// 0x354
			 0x2cc0d161, /// 0x358
			 0x3e252e00, /// 0x35c
			 0x3fdcd267, /// 0x360
			 0x17978fb3, /// 0x364
			 0x7c2af242, /// 0x368
			 0xe4c05f99, /// 0x36c
			 0x83b14103, /// 0x370
			 0x4c11d244, /// 0x374
			 0x0545ca91, /// 0x378
			 0xc401e909, /// 0x37c
			 0x8497245c, /// 0x380
			 0xa5d22c3b, /// 0x384
			 0x913c0d25, /// 0x388
			 0x851c5fcd, /// 0x38c
			 0x31588472, /// 0x390
			 0x8244a6c8, /// 0x394
			 0xc6385a62, /// 0x398
			 0x4505ff70, /// 0x39c
			 0x7dd6cad2, /// 0x3a0
			 0x3938bbe6, /// 0x3a4
			 0xd15423ad, /// 0x3a8
			 0x96d2bcdb, /// 0x3ac
			 0xed71569a, /// 0x3b0
			 0xb1ea3138, /// 0x3b4
			 0x175d351c, /// 0x3b8
			 0x05445224, /// 0x3bc
			 0x088ae25f, /// 0x3c0
			 0x7fbaba1a, /// 0x3c4
			 0xec66079c, /// 0x3c8
			 0x15244176, /// 0x3cc
			 0x9963314c, /// 0x3d0
			 0xdd527891, /// 0x3d4
			 0x3b22c90d, /// 0x3d8
			 0x39bb462f, /// 0x3dc
			 0xa303dda6, /// 0x3e0
			 0x3753aa45, /// 0x3e4
			 0x888e6204, /// 0x3e8
			 0xadbf8261, /// 0x3ec
			 0x3a6a2ae6, /// 0x3f0
			 0xacfe114e, /// 0x3f4
			 0x04f0b28c, /// 0x3f8
			 0xd17db73f, /// 0x3fc
			 0xaac51ead, /// 0x400
			 0x188e4884, /// 0x404
			 0x3bf8fa17, /// 0x408
			 0xc7ea0494, /// 0x40c
			 0x3eaedf49, /// 0x410
			 0x56b815f8, /// 0x414
			 0x40b852a7, /// 0x418
			 0x718d3733, /// 0x41c
			 0xbf52ce82, /// 0x420
			 0x9ee429da, /// 0x424
			 0xfb7c8527, /// 0x428
			 0xbdacc586, /// 0x42c
			 0x919ad2be, /// 0x430
			 0x18414e33, /// 0x434
			 0x3de89be1, /// 0x438
			 0xfc7aae30, /// 0x43c
			 0x00bad7fb, /// 0x440
			 0x7c645b06, /// 0x444
			 0x25db0166, /// 0x448
			 0xb927b6e3, /// 0x44c
			 0xf00f524d, /// 0x450
			 0xe7a732c6, /// 0x454
			 0x41d31160, /// 0x458
			 0xadd57ffe, /// 0x45c
			 0xbf426c92, /// 0x460
			 0x65e0259b, /// 0x464
			 0xf1c7cc3e, /// 0x468
			 0x32bf0c9a, /// 0x46c
			 0x18a23f20, /// 0x470
			 0x79fde6cf, /// 0x474
			 0xbb0286bd, /// 0x478
			 0xb5c65c44, /// 0x47c
			 0xd9285dab, /// 0x480
			 0xd76e9f77, /// 0x484
			 0x064a4a4b, /// 0x488
			 0x9f1d3476, /// 0x48c
			 0x4b33333e, /// 0x490
			 0xffdc41af, /// 0x494
			 0xa78eed54, /// 0x498
			 0x61b68fca, /// 0x49c
			 0x54ba003d, /// 0x4a0
			 0xaa28f4f5, /// 0x4a4
			 0xfc26514f, /// 0x4a8
			 0x8df16941, /// 0x4ac
			 0x34283f76, /// 0x4b0
			 0x0db03cb0, /// 0x4b4
			 0x65b03bb6, /// 0x4b8
			 0x4a36825e, /// 0x4bc
			 0x2397c5ad, /// 0x4c0
			 0xb82173ef, /// 0x4c4
			 0x1f5f02cb, /// 0x4c8
			 0x324fc197, /// 0x4cc
			 0xec2f69fa, /// 0x4d0
			 0x62397bfe, /// 0x4d4
			 0x330386fe, /// 0x4d8
			 0x882b9256, /// 0x4dc
			 0x29b7c617, /// 0x4e0
			 0xcd097a06, /// 0x4e4
			 0x753aa705, /// 0x4e8
			 0x0e7d60af, /// 0x4ec
			 0x1df38840, /// 0x4f0
			 0xc55973e8, /// 0x4f4
			 0xb2f720d2, /// 0x4f8
			 0xfb00addd, /// 0x4fc
			 0xd5f9116a, /// 0x500
			 0xbad958a2, /// 0x504
			 0x985fcdd1, /// 0x508
			 0xf7d62151, /// 0x50c
			 0x2f63e27c, /// 0x510
			 0x3ee1b721, /// 0x514
			 0x0e136e11, /// 0x518
			 0x99e3b9b3, /// 0x51c
			 0x811f2732, /// 0x520
			 0x16cd973e, /// 0x524
			 0xe19fe180, /// 0x528
			 0x2d009c97, /// 0x52c
			 0x38e0abf0, /// 0x530
			 0x2537a36b, /// 0x534
			 0x9769587f, /// 0x538
			 0x10a1d781, /// 0x53c
			 0xd4be4584, /// 0x540
			 0x8604a2f4, /// 0x544
			 0x6ae8b58e, /// 0x548
			 0x678f121f, /// 0x54c
			 0xcb7c9f83, /// 0x550
			 0xe018e98c, /// 0x554
			 0xc5e8dd44, /// 0x558
			 0xd5351914, /// 0x55c
			 0x86c17cae, /// 0x560
			 0x4517a18c, /// 0x564
			 0x41d305d1, /// 0x568
			 0xdbc4b8c3, /// 0x56c
			 0x3ffe6c3c, /// 0x570
			 0x978172ad, /// 0x574
			 0xdf61e786, /// 0x578
			 0x8f00a2ee, /// 0x57c
			 0x8ac58486, /// 0x580
			 0x5421a5c1, /// 0x584
			 0x7d6bae58, /// 0x588
			 0xdd513d0e, /// 0x58c
			 0x53cee0db, /// 0x590
			 0xbfdc9408, /// 0x594
			 0x139b9f06, /// 0x598
			 0xf8d35bcb, /// 0x59c
			 0x8840cd5c, /// 0x5a0
			 0x0afe937a, /// 0x5a4
			 0x697d0889, /// 0x5a8
			 0x306e3288, /// 0x5ac
			 0x896a3154, /// 0x5b0
			 0xad6fb680, /// 0x5b4
			 0xef121e43, /// 0x5b8
			 0x7e41996c, /// 0x5bc
			 0xc2e966ab, /// 0x5c0
			 0x045d7585, /// 0x5c4
			 0x04ffa9b3, /// 0x5c8
			 0x56ef70ee, /// 0x5cc
			 0x654f96a3, /// 0x5d0
			 0xdb24d906, /// 0x5d4
			 0xec371b2b, /// 0x5d8
			 0xa2259184, /// 0x5dc
			 0x38dfec15, /// 0x5e0
			 0xd29d7b97, /// 0x5e4
			 0xe891683a, /// 0x5e8
			 0x883374d5, /// 0x5ec
			 0x1ebb50a6, /// 0x5f0
			 0x908caad6, /// 0x5f4
			 0xe8c91fff, /// 0x5f8
			 0x5a346ce6, /// 0x5fc
			 0xa68bb188, /// 0x600
			 0xbdc47b8d, /// 0x604
			 0x13f9a718, /// 0x608
			 0xa62c091c, /// 0x60c
			 0x1f4bb15c, /// 0x610
			 0xa56e4098, /// 0x614
			 0x3dcb1750, /// 0x618
			 0xdeb0eb95, /// 0x61c
			 0x85925052, /// 0x620
			 0xfb9fa921, /// 0x624
			 0xabacd773, /// 0x628
			 0xa670677a, /// 0x62c
			 0xab72ee2c, /// 0x630
			 0x8a6e81d6, /// 0x634
			 0x5fcf1b47, /// 0x638
			 0xb5e3a488, /// 0x63c
			 0x7876388b, /// 0x640
			 0x57c23932, /// 0x644
			 0x40fbfd83, /// 0x648
			 0xca28e256, /// 0x64c
			 0x1e80207f, /// 0x650
			 0xcce1e1f4, /// 0x654
			 0x2cc94fd3, /// 0x658
			 0x9061ae8b, /// 0x65c
			 0x7c8c7e09, /// 0x660
			 0x97b021e7, /// 0x664
			 0xd3c1e0e3, /// 0x668
			 0x03fa85ee, /// 0x66c
			 0x64a01a45, /// 0x670
			 0xa712dad4, /// 0x674
			 0x7f5d340c, /// 0x678
			 0x7ef77a4b, /// 0x67c
			 0x01767cd9, /// 0x680
			 0x6706ea10, /// 0x684
			 0xf963bfc6, /// 0x688
			 0x926c3561, /// 0x68c
			 0xd975cc5d, /// 0x690
			 0x05624b9a, /// 0x694
			 0x29da252b, /// 0x698
			 0x97690b02, /// 0x69c
			 0xc1ada45b, /// 0x6a0
			 0xcbcc8be3, /// 0x6a4
			 0x96c95477, /// 0x6a8
			 0x5531e434, /// 0x6ac
			 0xbe6c3654, /// 0x6b0
			 0x0bdf9b44, /// 0x6b4
			 0xb30be7dd, /// 0x6b8
			 0x281b3320, /// 0x6bc
			 0x50b40851, /// 0x6c0
			 0xc5a94427, /// 0x6c4
			 0xcf3c48db, /// 0x6c8
			 0xe4704911, /// 0x6cc
			 0xde1b3d38, /// 0x6d0
			 0x486900d6, /// 0x6d4
			 0x9a714cf1, /// 0x6d8
			 0xe05b7610, /// 0x6dc
			 0x89d7128f, /// 0x6e0
			 0xc2d1be09, /// 0x6e4
			 0x6f8a6fae, /// 0x6e8
			 0xd46709ca, /// 0x6ec
			 0xed1a87eb, /// 0x6f0
			 0x12935651, /// 0x6f4
			 0x6c1b2e9c, /// 0x6f8
			 0x09407762, /// 0x6fc
			 0x190e07ba, /// 0x700
			 0xaaea1ecc, /// 0x704
			 0x67b9fade, /// 0x708
			 0xe05187d0, /// 0x70c
			 0xff021110, /// 0x710
			 0xab7e62db, /// 0x714
			 0x22b9e1dc, /// 0x718
			 0x6e743bc8, /// 0x71c
			 0x5e99b0f4, /// 0x720
			 0x8382d735, /// 0x724
			 0xda3096c8, /// 0x728
			 0xe0af467a, /// 0x72c
			 0x9620cc04, /// 0x730
			 0xde807380, /// 0x734
			 0x0a4c7325, /// 0x738
			 0x5af9131e, /// 0x73c
			 0x61c02f9e, /// 0x740
			 0x356c0d68, /// 0x744
			 0x9da890ae, /// 0x748
			 0x9ad188dd, /// 0x74c
			 0x0a00e438, /// 0x750
			 0xdaff907e, /// 0x754
			 0xcdb38385, /// 0x758
			 0x8478c88d, /// 0x75c
			 0x1760f1cb, /// 0x760
			 0x283c68d8, /// 0x764
			 0xcd98772f, /// 0x768
			 0xee1a42b7, /// 0x76c
			 0x50e3e049, /// 0x770
			 0x13740203, /// 0x774
			 0x267ec7ed, /// 0x778
			 0xdf970491, /// 0x77c
			 0x7d3c7a1f, /// 0x780
			 0x3816abaf, /// 0x784
			 0x69b7b0d9, /// 0x788
			 0x2f03f109, /// 0x78c
			 0x2c86d8cd, /// 0x790
			 0x495622ae, /// 0x794
			 0x6d9c61c3, /// 0x798
			 0xdfb9d187, /// 0x79c
			 0xbd060774, /// 0x7a0
			 0xfd0dcd83, /// 0x7a4
			 0x55917615, /// 0x7a8
			 0xd5da1cfd, /// 0x7ac
			 0xf6722e33, /// 0x7b0
			 0x2b0d2137, /// 0x7b4
			 0xb69c96cf, /// 0x7b8
			 0x5a6eb544, /// 0x7bc
			 0xaff17242, /// 0x7c0
			 0xb926c2c6, /// 0x7c4
			 0x04096a80, /// 0x7c8
			 0x587d7c34, /// 0x7cc
			 0x7a6d8a20, /// 0x7d0
			 0x323a03dd, /// 0x7d4
			 0xedc7cd9c, /// 0x7d8
			 0x8cd225b5, /// 0x7dc
			 0xa5eefe8b, /// 0x7e0
			 0xa11cfabb, /// 0x7e4
			 0x128115e6, /// 0x7e8
			 0x5da2e39b, /// 0x7ec
			 0x6ddf8faf, /// 0x7f0
			 0x9ababd6e, /// 0x7f4
			 0x28bd3f00, /// 0x7f8
			 0x072d4e2a, /// 0x7fc
			 0xa7d95efd, /// 0x800
			 0x1cb6b53d, /// 0x804
			 0x4c777cd4, /// 0x808
			 0xf056b808, /// 0x80c
			 0xe6002897, /// 0x810
			 0x5c6cbafa, /// 0x814
			 0x73562579, /// 0x818
			 0x39f1ef50, /// 0x81c
			 0xb0fbc222, /// 0x820
			 0xccfeb282, /// 0x824
			 0x7264e7af, /// 0x828
			 0x6f2613c6, /// 0x82c
			 0x548eb452, /// 0x830
			 0x8f87cb4a, /// 0x834
			 0x0557e56a, /// 0x838
			 0xfea2eede, /// 0x83c
			 0x0bb99212, /// 0x840
			 0x4e85cb36, /// 0x844
			 0xe46e3ac1, /// 0x848
			 0x5cae2c02, /// 0x84c
			 0x5c0ba6c3, /// 0x850
			 0x7c8ff2e6, /// 0x854
			 0x4b973b91, /// 0x858
			 0x7d476b24, /// 0x85c
			 0x3a3d5d81, /// 0x860
			 0x6235c5c0, /// 0x864
			 0xfbaef7d1, /// 0x868
			 0x4f37cedc, /// 0x86c
			 0xc212d865, /// 0x870
			 0x5add8cf3, /// 0x874
			 0xb792485e, /// 0x878
			 0x9000c2c9, /// 0x87c
			 0x6ac0b21a, /// 0x880
			 0x3874ae27, /// 0x884
			 0x59a9ae61, /// 0x888
			 0x75d9325a, /// 0x88c
			 0x4dc59faf, /// 0x890
			 0x4eae1ce9, /// 0x894
			 0xceaa6e2b, /// 0x898
			 0x2969fa6c, /// 0x89c
			 0x34099a93, /// 0x8a0
			 0xb6b1e0a5, /// 0x8a4
			 0x334acd17, /// 0x8a8
			 0xcf886df0, /// 0x8ac
			 0x291dfc89, /// 0x8b0
			 0x126def8a, /// 0x8b4
			 0x9a6f71ea, /// 0x8b8
			 0x9b446302, /// 0x8bc
			 0xc87d3d67, /// 0x8c0
			 0x3842bba6, /// 0x8c4
			 0xdeeb2efd, /// 0x8c8
			 0xa3194b59, /// 0x8cc
			 0x61912d53, /// 0x8d0
			 0xf01ceae3, /// 0x8d4
			 0x5954864d, /// 0x8d8
			 0x3f91788a, /// 0x8dc
			 0xb218425c, /// 0x8e0
			 0x3a0c2611, /// 0x8e4
			 0xe45eebc5, /// 0x8e8
			 0x97d08147, /// 0x8ec
			 0x03e7890a, /// 0x8f0
			 0x09c56c0b, /// 0x8f4
			 0x9b50a89b, /// 0x8f8
			 0x4d4c5b65, /// 0x8fc
			 0xf41259f1, /// 0x900
			 0xce42d5fd, /// 0x904
			 0x34e06d25, /// 0x908
			 0x7c93d4d5, /// 0x90c
			 0xe952e0d2, /// 0x910
			 0x743c3ee6, /// 0x914
			 0x13f5fde5, /// 0x918
			 0xa9f73e78, /// 0x91c
			 0x930d2f67, /// 0x920
			 0x07554ce8, /// 0x924
			 0x9848c806, /// 0x928
			 0x340392ed, /// 0x92c
			 0x9286c257, /// 0x930
			 0x3ff12990, /// 0x934
			 0xbe0bca46, /// 0x938
			 0xae607b3a, /// 0x93c
			 0xd07d2e77, /// 0x940
			 0x337b3ee7, /// 0x944
			 0x518681e5, /// 0x948
			 0xa74052f4, /// 0x94c
			 0x3915a847, /// 0x950
			 0xd1af0f2c, /// 0x954
			 0x2a02c745, /// 0x958
			 0x0bcf6f8c, /// 0x95c
			 0xf4d30b91, /// 0x960
			 0x58d9e6b6, /// 0x964
			 0x4e467b24, /// 0x968
			 0x6894e57c, /// 0x96c
			 0x915f4d2a, /// 0x970
			 0xe396005b, /// 0x974
			 0x193e3579, /// 0x978
			 0x0c146c11, /// 0x97c
			 0x81f23a3e, /// 0x980
			 0xafb00cfb, /// 0x984
			 0xf729f1d2, /// 0x988
			 0x9d5f7033, /// 0x98c
			 0x0b3d2d00, /// 0x990
			 0x28a8bf16, /// 0x994
			 0x82e17441, /// 0x998
			 0x5e9b69bb, /// 0x99c
			 0x4d8dc069, /// 0x9a0
			 0xeec310d0, /// 0x9a4
			 0xd0b6f5f3, /// 0x9a8
			 0xa31571ce, /// 0x9ac
			 0x4cab7df8, /// 0x9b0
			 0x4b80ebba, /// 0x9b4
			 0xcaa85c05, /// 0x9b8
			 0x489b4438, /// 0x9bc
			 0x3997291e, /// 0x9c0
			 0x92ce6a52, /// 0x9c4
			 0x6a7d6a9b, /// 0x9c8
			 0x1e129e03, /// 0x9cc
			 0xf8da9728, /// 0x9d0
			 0x02e960aa, /// 0x9d4
			 0x06011a32, /// 0x9d8
			 0x0982d88e, /// 0x9dc
			 0xbfa21816, /// 0x9e0
			 0x51b6f0a4, /// 0x9e4
			 0xdb33d89e, /// 0x9e8
			 0x536bc8e8, /// 0x9ec
			 0x151a4474, /// 0x9f0
			 0x9aca7193, /// 0x9f4
			 0xd9946c34, /// 0x9f8
			 0xa5a68a93, /// 0x9fc
			 0x81e4748f, /// 0xa00
			 0x94d528b0, /// 0xa04
			 0xb7fd30f2, /// 0xa08
			 0x0ca08a17, /// 0xa0c
			 0xe0e41a4f, /// 0xa10
			 0x7b390a42, /// 0xa14
			 0x1325db37, /// 0xa18
			 0x4000a9b0, /// 0xa1c
			 0x6dc59ae4, /// 0xa20
			 0x251b3917, /// 0xa24
			 0x3f2a334b, /// 0xa28
			 0xabd084ef, /// 0xa2c
			 0xf3c3bf5f, /// 0xa30
			 0x80874368, /// 0xa34
			 0xc3c81e7f, /// 0xa38
			 0xa85751ea, /// 0xa3c
			 0x1fe2614f, /// 0xa40
			 0xb29d2114, /// 0xa44
			 0x75a2d782, /// 0xa48
			 0xfc471d4a, /// 0xa4c
			 0xe428a5aa, /// 0xa50
			 0xe234e869, /// 0xa54
			 0x1cc9668c, /// 0xa58
			 0xdf2b46db, /// 0xa5c
			 0xcd4f30b2, /// 0xa60
			 0x35b5f92c, /// 0xa64
			 0xd2f921c0, /// 0xa68
			 0xe1b50701, /// 0xa6c
			 0xf13a2e75, /// 0xa70
			 0x670c7aa8, /// 0xa74
			 0x89de976d, /// 0xa78
			 0x4c03fef5, /// 0xa7c
			 0x29996b01, /// 0xa80
			 0xa21c7893, /// 0xa84
			 0xb7297ea6, /// 0xa88
			 0x17b586fb, /// 0xa8c
			 0xbcde1b05, /// 0xa90
			 0x95d7d1e2, /// 0xa94
			 0x0b276f7c, /// 0xa98
			 0xf2382b01, /// 0xa9c
			 0x6874bb47, /// 0xaa0
			 0xe4444236, /// 0xaa4
			 0x33b4f765, /// 0xaa8
			 0x59b43b22, /// 0xaac
			 0x418b842e, /// 0xab0
			 0xbf8496d8, /// 0xab4
			 0xc635e336, /// 0xab8
			 0xe0ba413b, /// 0xabc
			 0x51055a20, /// 0xac0
			 0xa9b0e65a, /// 0xac4
			 0x37f286fb, /// 0xac8
			 0x990793d9, /// 0xacc
			 0xf001f18f, /// 0xad0
			 0x4d1861fe, /// 0xad4
			 0x10405ae3, /// 0xad8
			 0xba25526f, /// 0xadc
			 0x56bd1cee, /// 0xae0
			 0x6b6973e6, /// 0xae4
			 0x9791748f, /// 0xae8
			 0xfe3ee953, /// 0xaec
			 0xecefa605, /// 0xaf0
			 0x61115895, /// 0xaf4
			 0xc9024466, /// 0xaf8
			 0x98b24f6c, /// 0xafc
			 0x5ac04cbe, /// 0xb00
			 0x7c6e4773, /// 0xb04
			 0xbfd4b4dc, /// 0xb08
			 0xfa32f44a, /// 0xb0c
			 0x99273356, /// 0xb10
			 0x1e7e189c, /// 0xb14
			 0xc7370baf, /// 0xb18
			 0x1f9dd532, /// 0xb1c
			 0xd0ede6a3, /// 0xb20
			 0x39eae94c, /// 0xb24
			 0x71d69cc9, /// 0xb28
			 0xdbd78832, /// 0xb2c
			 0xb5fe64a0, /// 0xb30
			 0x332b6da9, /// 0xb34
			 0x9c51f427, /// 0xb38
			 0x3218610c, /// 0xb3c
			 0xca064885, /// 0xb40
			 0xe4ffe632, /// 0xb44
			 0x67a5fa7e, /// 0xb48
			 0xccc74722, /// 0xb4c
			 0x855645b6, /// 0xb50
			 0x532de8a8, /// 0xb54
			 0x5fc94263, /// 0xb58
			 0x4fb91f2d, /// 0xb5c
			 0x9b180358, /// 0xb60
			 0x56482ca1, /// 0xb64
			 0x5816425e, /// 0xb68
			 0x634ed250, /// 0xb6c
			 0x181c19db, /// 0xb70
			 0xd6c2b2f4, /// 0xb74
			 0x6b7c7ece, /// 0xb78
			 0xcce3272b, /// 0xb7c
			 0x13044a45, /// 0xb80
			 0xc0eab1ea, /// 0xb84
			 0xef5eb2ec, /// 0xb88
			 0x1728d468, /// 0xb8c
			 0x6e0e5365, /// 0xb90
			 0x78541e48, /// 0xb94
			 0x1810aa24, /// 0xb98
			 0x55c353d8, /// 0xb9c
			 0xe913d94b, /// 0xba0
			 0x50666261, /// 0xba4
			 0x272d01fa, /// 0xba8
			 0x459e50ea, /// 0xbac
			 0x380d3661, /// 0xbb0
			 0xa32da7d7, /// 0xbb4
			 0x8a33923d, /// 0xbb8
			 0x33838cf4, /// 0xbbc
			 0xe3108628, /// 0xbc0
			 0x92f94047, /// 0xbc4
			 0x7a93127c, /// 0xbc8
			 0x414e0c25, /// 0xbcc
			 0xbfc8124a, /// 0xbd0
			 0xff6e5dd5, /// 0xbd4
			 0x0f70b46d, /// 0xbd8
			 0x94b1a310, /// 0xbdc
			 0xb2dc9196, /// 0xbe0
			 0xa219e090, /// 0xbe4
			 0xce972345, /// 0xbe8
			 0xdf064993, /// 0xbec
			 0x4c35a1d3, /// 0xbf0
			 0x30829b74, /// 0xbf4
			 0x806f0d88, /// 0xbf8
			 0xe1b100f9, /// 0xbfc
			 0x58cd241e, /// 0xc00
			 0x71daf29d, /// 0xc04
			 0x2bf9a21e, /// 0xc08
			 0x4b0fca00, /// 0xc0c
			 0x497172f0, /// 0xc10
			 0x30ce095a, /// 0xc14
			 0xa345b4f2, /// 0xc18
			 0xa43ef4de, /// 0xc1c
			 0x60fb6d0f, /// 0xc20
			 0xc970e85c, /// 0xc24
			 0xad55a59f, /// 0xc28
			 0xbaa05b49, /// 0xc2c
			 0x43d2d15c, /// 0xc30
			 0x80cfa61c, /// 0xc34
			 0xd6b61f98, /// 0xc38
			 0xdb58ee7d, /// 0xc3c
			 0x4916bb27, /// 0xc40
			 0x8e2a17ec, /// 0xc44
			 0xb9c17c54, /// 0xc48
			 0xa5dcc8a5, /// 0xc4c
			 0x258cfe80, /// 0xc50
			 0xe8c22a1a, /// 0xc54
			 0x6636f1f4, /// 0xc58
			 0x2815ccb6, /// 0xc5c
			 0xa9314291, /// 0xc60
			 0xbc77f62e, /// 0xc64
			 0xc2a70f36, /// 0xc68
			 0x6750d0b7, /// 0xc6c
			 0xd808ee9d, /// 0xc70
			 0x173c86ad, /// 0xc74
			 0xb4dd98b1, /// 0xc78
			 0xe69185b3, /// 0xc7c
			 0x9227365b, /// 0xc80
			 0x65fbae0b, /// 0xc84
			 0xb5330b2c, /// 0xc88
			 0xa3c2d417, /// 0xc8c
			 0xd7754e85, /// 0xc90
			 0x816fea25, /// 0xc94
			 0x70ec7a61, /// 0xc98
			 0x7171627c, /// 0xc9c
			 0x9eba4a55, /// 0xca0
			 0x3f92fbd3, /// 0xca4
			 0xcfcf378a, /// 0xca8
			 0x04062008, /// 0xcac
			 0x85b61cf7, /// 0xcb0
			 0x1bf95638, /// 0xcb4
			 0x292e38fc, /// 0xcb8
			 0xdb75d509, /// 0xcbc
			 0x2f8c306c, /// 0xcc0
			 0xf62c3306, /// 0xcc4
			 0xd4738b5b, /// 0xcc8
			 0x1dd12922, /// 0xccc
			 0x8f06e974, /// 0xcd0
			 0x21c78459, /// 0xcd4
			 0x6b27d399, /// 0xcd8
			 0xf6007275, /// 0xcdc
			 0x19eb016f, /// 0xce0
			 0x178c6d51, /// 0xce4
			 0x652ad36d, /// 0xce8
			 0xed849553, /// 0xcec
			 0x7e864f35, /// 0xcf0
			 0xa00ed083, /// 0xcf4
			 0x3b381352, /// 0xcf8
			 0xe5b94f73, /// 0xcfc
			 0x08b200ce, /// 0xd00
			 0xe02adb83, /// 0xd04
			 0x72bad727, /// 0xd08
			 0x7501db28, /// 0xd0c
			 0x26c5fcdb, /// 0xd10
			 0x28620653, /// 0xd14
			 0xd21bf37e, /// 0xd18
			 0x73ebe414, /// 0xd1c
			 0x3695c350, /// 0xd20
			 0xafab1b06, /// 0xd24
			 0x62fcd876, /// 0xd28
			 0x12a81e9b, /// 0xd2c
			 0x884d7070, /// 0xd30
			 0xa5970981, /// 0xd34
			 0xf7581222, /// 0xd38
			 0x22118372, /// 0xd3c
			 0xc3e54b39, /// 0xd40
			 0x54be8788, /// 0xd44
			 0xccc3a3db, /// 0xd48
			 0x65c07101, /// 0xd4c
			 0x5a0b36a2, /// 0xd50
			 0xdd82fe1e, /// 0xd54
			 0x86d414e0, /// 0xd58
			 0xec3c9e5f, /// 0xd5c
			 0xd959f95a, /// 0xd60
			 0x140405a2, /// 0xd64
			 0x6defc5b1, /// 0xd68
			 0x559718d8, /// 0xd6c
			 0x395c3eba, /// 0xd70
			 0xde3d6b65, /// 0xd74
			 0x5e84a8f1, /// 0xd78
			 0x456f64cc, /// 0xd7c
			 0xf6605ea9, /// 0xd80
			 0x833fad1e, /// 0xd84
			 0x8888a9e5, /// 0xd88
			 0xd478933d, /// 0xd8c
			 0x3672ba1a, /// 0xd90
			 0x50ca9328, /// 0xd94
			 0xb206ffc0, /// 0xd98
			 0x374600bd, /// 0xd9c
			 0x694710af, /// 0xda0
			 0x2aa2104b, /// 0xda4
			 0x34a09e0e, /// 0xda8
			 0x349d55db, /// 0xdac
			 0x81cbfabf, /// 0xdb0
			 0x4aa1f4a1, /// 0xdb4
			 0x6d5a4edb, /// 0xdb8
			 0x426d312c, /// 0xdbc
			 0x1f87fcb1, /// 0xdc0
			 0xf9c31b4e, /// 0xdc4
			 0xb7a93869, /// 0xdc8
			 0x8c58b1a6, /// 0xdcc
			 0x24490bbe, /// 0xdd0
			 0xd17c790d, /// 0xdd4
			 0x4903a5e9, /// 0xdd8
			 0x9122791a, /// 0xddc
			 0x14586f08, /// 0xde0
			 0x2960557c, /// 0xde4
			 0x2a624c32, /// 0xde8
			 0x1166da04, /// 0xdec
			 0x226526fd, /// 0xdf0
			 0xa7c269c5, /// 0xdf4
			 0x9f5508bf, /// 0xdf8
			 0x52c74251, /// 0xdfc
			 0x07bb42db, /// 0xe00
			 0x7c4d44a6, /// 0xe04
			 0x6d0a84ec, /// 0xe08
			 0x50dc9fce, /// 0xe0c
			 0xde07a11d, /// 0xe10
			 0xab0559ce, /// 0xe14
			 0xd660f2ed, /// 0xe18
			 0x4b404c90, /// 0xe1c
			 0x5cc279d2, /// 0xe20
			 0x18bf2a6c, /// 0xe24
			 0xef16a99c, /// 0xe28
			 0xdf68f5d4, /// 0xe2c
			 0x1b372b31, /// 0xe30
			 0xd6a3155a, /// 0xe34
			 0x9e346dcd, /// 0xe38
			 0x55f9c878, /// 0xe3c
			 0x73cb94dd, /// 0xe40
			 0x164b7003, /// 0xe44
			 0x352c0531, /// 0xe48
			 0x474e4bee, /// 0xe4c
			 0xa1a8ca7c, /// 0xe50
			 0xdfb28578, /// 0xe54
			 0x9b05c78a, /// 0xe58
			 0xdd3d1ad6, /// 0xe5c
			 0x7b3ef9dc, /// 0xe60
			 0x048b65e0, /// 0xe64
			 0xc26ed2a3, /// 0xe68
			 0x80074939, /// 0xe6c
			 0x08a3cddb, /// 0xe70
			 0x5d3179dd, /// 0xe74
			 0x6a4c7570, /// 0xe78
			 0x897d8f30, /// 0xe7c
			 0x0d9a2124, /// 0xe80
			 0xc70ae16e, /// 0xe84
			 0xd50c917e, /// 0xe88
			 0xa83204f7, /// 0xe8c
			 0xfbb20826, /// 0xe90
			 0xbe0bb0ef, /// 0xe94
			 0xa1aee872, /// 0xe98
			 0xf9982132, /// 0xe9c
			 0x157e05f7, /// 0xea0
			 0x38ed013e, /// 0xea4
			 0x99de17e6, /// 0xea8
			 0x945aa20c, /// 0xeac
			 0xe68d0086, /// 0xeb0
			 0x0861bfe3, /// 0xeb4
			 0x1a7ff046, /// 0xeb8
			 0x5b290a70, /// 0xebc
			 0xb7948b51, /// 0xec0
			 0x5f5d620f, /// 0xec4
			 0x5126223e, /// 0xec8
			 0xc6b725ca, /// 0xecc
			 0xe85f8b37, /// 0xed0
			 0xeefe069e, /// 0xed4
			 0x28a5f848, /// 0xed8
			 0x39ccd6b5, /// 0xedc
			 0x1a25c717, /// 0xee0
			 0xa124c2f4, /// 0xee4
			 0xd5150d4f, /// 0xee8
			 0xa65c6e7d, /// 0xeec
			 0x3a337e97, /// 0xef0
			 0x51d93585, /// 0xef4
			 0xcbc362fc, /// 0xef8
			 0x63cbd44c, /// 0xefc
			 0x0160ab54, /// 0xf00
			 0xefea9945, /// 0xf04
			 0x78419c71, /// 0xf08
			 0xfb9e907b, /// 0xf0c
			 0xb0f0dd3a, /// 0xf10
			 0x99547673, /// 0xf14
			 0xed0030ba, /// 0xf18
			 0xd776ccac, /// 0xf1c
			 0xd9c8e011, /// 0xf20
			 0xc62abbb1, /// 0xf24
			 0x9aa6c8a5, /// 0xf28
			 0x43b4062f, /// 0xf2c
			 0xfa8a03ff, /// 0xf30
			 0xf2312430, /// 0xf34
			 0xacd1a272, /// 0xf38
			 0x7ffebccc, /// 0xf3c
			 0xc008120b, /// 0xf40
			 0x844986d8, /// 0xf44
			 0xcf01d64d, /// 0xf48
			 0x236295b8, /// 0xf4c
			 0xa7d460a7, /// 0xf50
			 0xb8fd2e74, /// 0xf54
			 0xb0ce3053, /// 0xf58
			 0x146f69c9, /// 0xf5c
			 0xf96ef5c1, /// 0xf60
			 0xb1ec43b4, /// 0xf64
			 0x584f9ae4, /// 0xf68
			 0xc2a466b0, /// 0xf6c
			 0x4414453c, /// 0xf70
			 0x39c0b780, /// 0xf74
			 0xb3969c4c, /// 0xf78
			 0x0afbff73, /// 0xf7c
			 0x2d4c77e3, /// 0xf80
			 0x3166f281, /// 0xf84
			 0xb70c40c4, /// 0xf88
			 0xc1b63dc3, /// 0xf8c
			 0x8a1c3ab0, /// 0xf90
			 0x264a9262, /// 0xf94
			 0x819a8958, /// 0xf98
			 0xe441469b, /// 0xf9c
			 0xc5bf5c68, /// 0xfa0
			 0xd617cd67, /// 0xfa4
			 0x00eea639, /// 0xfa8
			 0x183b97a9, /// 0xfac
			 0x1a6d7cc5, /// 0xfb0
			 0x71193b49, /// 0xfb4
			 0x126852b7, /// 0xfb8
			 0xc24008c5, /// 0xfbc
			 0x18705c5f, /// 0xfc0
			 0xd0b0ea60, /// 0xfc4
			 0xac79e967, /// 0xfc8
			 0xaf1e8012, /// 0xfcc
			 0xecc89cf8, /// 0xfd0
			 0x550aeb89, /// 0xfd4
			 0xe25bd425, /// 0xfd8
			 0x3dca8eed, /// 0xfdc
			 0x7810deb3, /// 0xfe0
			 0xac9014e2, /// 0xfe4
			 0xd1f9d557, /// 0xfe8
			 0x414cb381, /// 0xfec
			 0x324c424e, /// 0xff0
			 0xb1a40a64, /// 0xff4
			 0xe93bea39, /// 0xff8
			 0x5623f492 /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00000
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00008
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0000c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00010
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00014
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00018
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0001c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00020
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00024
			 0x0e000003,                                                  // Trailing 1s:                                /// 00028
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0002c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00030
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00034
			 0x80011111,                                                  // -9.7958E-41                                 /// 00038
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0003c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00044
			 0xffc00001,                                                  // -signaling NaN                              /// 00048
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0004c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00050
			 0x3f028f5c,                                                  // 0.51                                        /// 00054
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00058
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0005c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00060
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00064
			 0x4b000000,                                                  // 8388608.0                                   /// 00068
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0006c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00070
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00074
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00078
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00080
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00084
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00088
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00090
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00094
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00098
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0009c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000a0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 000a4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a8
			 0x7f800000,                                                  // inf                                         /// 000ac
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 000b0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 000b4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000b8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 000bc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 000c4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000c8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000cc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000d4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x00000000,                                                  // zero                                        /// 000dc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000e0
			 0xcb000000,                                                  // -8388608.0                                  /// 000e4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000e8
			 0x0e000007,                                                  // Trailing 1s:                                /// 000ec
			 0x80000000,                                                  // -zero                                       /// 000f0
			 0xff800000,                                                  // -inf                                        /// 000f4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000f8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00100
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00104
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00108
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0010c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00114
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0011c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00120
			 0x7fc00001,                                                  // signaling NaN                               /// 00124
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0xffc00001,                                                  // -signaling NaN                              /// 0012c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00130
			 0x7f800000,                                                  // inf                                         /// 00134
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0013c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00144
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00148
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0014c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00150
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00154
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00158
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0015c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00160
			 0x0c600000,                                                  // Leading 1s:                                 /// 00164
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00168
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0016c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00170
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00174
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00178
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0017c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00180
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00184
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00188
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0018c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00190
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00194
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00198
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0019c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001a0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001a4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001a8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 001ac
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001b0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 001b4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001bc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 001c0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001c4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001cc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 001d0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001d4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001d8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0x4b000000,                                                  // 8388608.0                                   /// 001e8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001ec
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001f4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 001f8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001fc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00200
			 0x0c780000,                                                  // Leading 1s:                                 /// 00204
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00208
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0020c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00210
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00214
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00218
			 0x80011111,                                                  // -9.7958E-41                                 /// 0021c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00220
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00224
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00228
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0022c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00230
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00234
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00238
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0023c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00240
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00244
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00248
			 0x0e000007,                                                  // Trailing 1s:                                /// 0024c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00250
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00254
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00260
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00264
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00268
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0026c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00270
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00278
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0027c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00280
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00284
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00288
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0028c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00290
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00298
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0029c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 002a0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 002a4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 002a8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 002ac
			 0x4b000000,                                                  // 8388608.0                                   /// 002b0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 002bc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002c0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c4
			 0xcb000000,                                                  // -8388608.0                                  /// 002c8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002cc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002d0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002d4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 002d8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002dc
			 0xcb000000,                                                  // -8388608.0                                  /// 002e0
			 0x7f800000,                                                  // inf                                         /// 002e4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 002ec
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002f4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002f8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 002fc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00300
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00304
			 0x80000000,                                                  // -zero                                       /// 00308
			 0x0c600000,                                                  // Leading 1s:                                 /// 0030c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00310
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00318
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0031c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00320
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00324
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00328
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0032c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00330
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00334
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00338
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0033c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00340
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00344
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00348
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0034c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00350
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0x0e000001,                                                  // Trailing 1s:                                /// 00358
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0035c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00364
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0036c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00370
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00374
			 0x7f800000,                                                  // inf                                         /// 00378
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0037c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00380
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00388
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0038c
			 0x7f800000,                                                  // inf                                         /// 00390
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00394
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0039c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003a0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003a4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003a8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 003ac
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003b0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003b4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 003bc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003c0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003c4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 003c8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 003cc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 003d0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 003d4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003d8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003dc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 003e0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003e8
			 0x4b000000,                                                  // 8388608.0                                   /// 003ec
			 0xff800000,                                                  // -inf                                        /// 003f0
			 0xcb000000,                                                  // -8388608.0                                  /// 003f4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003fc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00400
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00404
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00408
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00410
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00414
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00418
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0041c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00420
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00424
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00428
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0042c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00430
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00434
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00438
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0043c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00440
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00444
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00448
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0044c
			 0xff800000,                                                  // -inf                                        /// 00450
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00454
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00458
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0045c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00464
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00468
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00470
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00474
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00478
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00480
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00484
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00488
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0048c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00490
			 0xffc00001,                                                  // -signaling NaN                              /// 00494
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00498
			 0xffc00001,                                                  // -signaling NaN                              /// 0049c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 004a0
			 0x3f028f5c,                                                  // 0.51                                        /// 004a4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004ac
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 004b4
			 0x0c700000,                                                  // Leading 1s:                                 /// 004b8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004bc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004c0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 004c4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004c8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 004cc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 004d0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 004d4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d8
			 0xff800000,                                                  // -inf                                        /// 004dc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004e0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004e4
			 0xbf028f5c,                                                  // -0.51                                       /// 004e8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004ec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 004f0
			 0xff800000,                                                  // -inf                                        /// 004f4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 004f8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 004fc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00500
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00504
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00508
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0050c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00510
			 0x0c600000,                                                  // Leading 1s:                                 /// 00514
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00518
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0051c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00520
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00524
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00528
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00530
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00538
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00540
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00544
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00548
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0054c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00550
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00558
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0055c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00564
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00568
			 0x0e000001,                                                  // Trailing 1s:                                /// 0056c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00570
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00574
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0057c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00584
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00588
			 0x0e000003,                                                  // Trailing 1s:                                /// 0058c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00590
			 0x00011111,                                                  // 9.7958E-41                                  /// 00594
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00598
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0059c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005a0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005a4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005ac
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 005b0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 005b8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 005bc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005c0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 005c4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 005c8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005cc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005d0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005d4
			 0x00011111,                                                  // 9.7958E-41                                  /// 005d8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005dc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005e0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 005e4
			 0x7f800000,                                                  // inf                                         /// 005e8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005ec
			 0x0c700000,                                                  // Leading 1s:                                 /// 005f0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 005f8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00600
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00604
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00608
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0060c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00610
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00614
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00618
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0061c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00620
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00624
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00628
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0062c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00630
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00634
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0063c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00640
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00644
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00648
			 0x55555555,                                                  // 4 random values                             /// 0064c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00650
			 0x00011111,                                                  // 9.7958E-41                                  /// 00654
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00658
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0065c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00660
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00664
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00668
			 0x0c700000,                                                  // Leading 1s:                                 /// 0066c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00670
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00674
			 0x3f028f5c,                                                  // 0.51                                        /// 00678
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00680
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00684
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00688
			 0x80000000,                                                  // -zero                                       /// 0068c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00690
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00694
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00698
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0069c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006a8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006ac
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 006b0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 006b4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006bc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006c0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c4
			 0x0c780000,                                                  // Leading 1s:                                 /// 006c8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 006cc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 006d0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006d4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 006dc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006e0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006e4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ec
			 0x0e00000f,                                                  // Trailing 1s:                                /// 006f0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006f4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x0c700000,                                                  // Leading 1s:                                 /// 006fc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00700
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00704
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00708
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0070c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00710
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00714
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0071c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00720
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00724
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00728
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0072c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00730
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00734
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00738
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0073c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00740
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00744
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00748
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0074c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00750
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00754
			 0x80011111,                                                  // -9.7958E-41                                 /// 00758
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0075c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00760
			 0xcb000000,                                                  // -8388608.0                                  /// 00764
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00768
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0076c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00770
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00774
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00778
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0077c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00780
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00784
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00788
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0078c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00794
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00798
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0079c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007a0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007a4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 007a8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ac
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007b0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 007b8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 007bc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 007c0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007c4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007cc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007d0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 007d8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007dc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 007e4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 007e8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007f0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007f8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007fc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00800
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00804
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00808
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0080c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00810
			 0x3f028f5c,                                                  // 0.51                                        /// 00814
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00818
			 0xffc00001,                                                  // -signaling NaN                              /// 0081c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00820
			 0x00011111,                                                  // 9.7958E-41                                  /// 00824
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00828
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0082c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00834
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00838
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00840
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00844
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00848
			 0xff800000,                                                  // -inf                                        /// 0084c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00850
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00858
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0085c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00860
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00864
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00868
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0086c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00870
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00874
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00878
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0087c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00880
			 0x0c780000,                                                  // Leading 1s:                                 /// 00884
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00888
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00890
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00894
			 0x4b000000,                                                  // 8388608.0                                   /// 00898
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0089c
			 0xffc00001,                                                  // -signaling NaN                              /// 008a0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008a4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008a8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008ac
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008b0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 008b4
			 0x0e000003,                                                  // Trailing 1s:                                /// 008b8
			 0x7fc00001,                                                  // signaling NaN                               /// 008bc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0x4b000000,                                                  // 8388608.0                                   /// 008c4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 008cc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008d0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 008d4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008d8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008dc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008e4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008e8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008ec
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008f0
			 0x0c600000,                                                  // Leading 1s:                                 /// 008f4
			 0xbf028f5c,                                                  // -0.51                                       /// 008f8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 008fc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00900
			 0x80011111,                                                  // -9.7958E-41                                 /// 00904
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00908
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00910
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00914
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00918
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0091c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00920
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00924
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00928
			 0xffc00001,                                                  // -signaling NaN                              /// 0092c
			 0x80000000,                                                  // -zero                                       /// 00930
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00934
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00938
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0093c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00940
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00944
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00948
			 0x33333333,                                                  // 4 random values                             /// 0094c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00950
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00954
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00958
			 0x55555555,                                                  // 4 random values                             /// 0095c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00960
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00964
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00968
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0096c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00970
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00974
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00978
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00980
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00984
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00988
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0098c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00990
			 0x00011111,                                                  // 9.7958E-41                                  /// 00994
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0xff800000,                                                  // -inf                                        /// 0099c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009a0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 009a8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 009ac
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009b8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009bc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009c0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 009c8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009d0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009d4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 009d8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009dc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 009e0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009e4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ec
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 009f4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009f8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009fc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a00
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a08
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a0c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a14
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a18
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a1c
			 0xbf028f5c,                                                  // -0.51                                       /// 00a20
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a24
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a30
			 0x3f028f5c,                                                  // 0.51                                        /// 00a34
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a38
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a3c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a40
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a44
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a48
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a4c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a50
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a54
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a58
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a60
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a68
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a6c
			 0x4b000000,                                                  // 8388608.0                                   /// 00a70
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a74
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a78
			 0x7f800000,                                                  // inf                                         /// 00a7c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a80
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a84
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a88
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a8c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a90
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a94
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a98
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a9c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00aa0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00aa4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00aa8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00aac
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ab0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ab4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ab8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00abc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ac4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ac8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00acc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ad0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ad4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00adc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ae4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ae8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00af0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00af8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00afc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0xbf028f5c,                                                  // -0.51                                       /// 00b04
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b08
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b0c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b10
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b14
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b18
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b1c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b20
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b24
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b2c
			 0x55555555,                                                  // 4 random values                             /// 00b30
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b34
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b38
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b3c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b40
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b44
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b48
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b4c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b50
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b58
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b5c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b60
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b64
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b6c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b74
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b78
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b7c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b84
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b88
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b8c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b90
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b94
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b98
			 0x00000000,                                                  // zero                                        /// 00b9c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ba0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ba4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ba8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bac
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bb4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00bb8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00bbc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00bc0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00bcc
			 0xcb000000,                                                  // -8388608.0                                  /// 00bd0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00bd4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00bd8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bdc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00be0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00be4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00be8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00bf0
			 0xbf028f5c,                                                  // -0.51                                       /// 00bf4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00bf8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c04
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c08
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c0c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c10
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c14
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c18
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c1c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c20
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c24
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c28
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c30
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c34
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c38
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c3c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c40
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c48
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c4c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c50
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c54
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c58
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c5c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c60
			 0xbf028f5c,                                                  // -0.51                                       /// 00c64
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c6c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c70
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c74
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c78
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c7c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c80
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c84
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c88
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c8c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c90
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c94
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c9c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ca4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ca8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00cac
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00cb0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00cb4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00cb8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00cbc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00cc0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00cc4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00cc8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ccc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cd0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00cd4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00cd8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00cdc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ce0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ce8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cec
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00cf0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00cf4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00cf8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00cfc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d00
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d04
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d08
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d0c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d10
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d18
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d24
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d28
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d2c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d30
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d34
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d38
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d3c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d40
			 0x7fc00001,                                                  // signaling NaN                               /// 00d44
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d48
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d4c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d50
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d54
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d58
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d5c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d60
			 0x7f800000,                                                  // inf                                         /// 00d64
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d68
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d6c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d70
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d74
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d78
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d7c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d80
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d84
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d88
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d8c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d94
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d98
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d9c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00da0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00da4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00dac
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00db4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00db8
			 0x4b000000,                                                  // 8388608.0                                   /// 00dbc
			 0xbf028f5c,                                                  // -0.51                                       /// 00dc0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00dc4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00dcc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00dd0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00dd4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00dd8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ddc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00de0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00de4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00de8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00dec
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00df0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00df4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00df8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00dfc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e04
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e08
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e0c
			 0xff800000,                                                  // -inf                                        /// 00e10
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e14
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e18
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e1c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e20
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e24
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e2c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e30
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e34
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e38
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e3c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e40
			 0xffc00001,                                                  // -signaling NaN                              /// 00e44
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e48
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e4c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e50
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e54
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e58
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e5c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e60
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e64
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e68
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e6c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e70
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e74
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e78
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e7c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e80
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e84
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e88
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e8c
			 0x7f800000,                                                  // inf                                         /// 00e90
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e94
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e98
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e9c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ea0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ea4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ea8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0x0c600000,                                                  // Leading 1s:                                 /// 00eb0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00eb4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00eb8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ebc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ec0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ec8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ecc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ed0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ed4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ed8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00edc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ee0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ee4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00eec
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ef0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ef4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ef8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00efc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f00
			 0x33333333,                                                  // 4 random values                             /// 00f04
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f08
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f0c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f10
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f14
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f18
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f20
			 0x55555555,                                                  // 4 random values                             /// 00f24
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f28
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f2c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f30
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f34
			 0xbf028f5c,                                                  // -0.51                                       /// 00f38
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f3c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f40
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f44
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f4c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f50
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f54
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f58
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f5c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f60
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f64
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f68
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f6c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f70
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f74
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f78
			 0x80000000,                                                  // -zero                                       /// 00f7c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f84
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f88
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f8c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f90
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f98
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00fa0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00fa4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00fa8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00fac
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00fb0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00fb4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00fb8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fbc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00fc0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0x3f028f5c,                                                  // 0.51                                        /// 00fc8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fcc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00fd4
			 0x7f800000,                                                  // inf                                         /// 00fd8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00fdc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fe4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00fe8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ff0
			 0x4b000000,                                                  // 8388608.0                                   /// 00ff4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ff8
			 0x0f7ffffd                                                  // all bit of mantissa set upto -3ulp          /// last
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
			 0x000005e8,
			 0x000005e4,
			 0x00000764,
			 0x00000334,
			 0x00000584,
			 0x00000644,
			 0x00000704,
			 0x00000018,

			 /// vpu register f2
			 0x41f00000,
			 0x3f800000,
			 0x41000000,
			 0x42000000,
			 0x41980000,
			 0x40000000,
			 0x40a00000,
			 0x41b00000,

			 /// vpu register f3
			 0x41b80000,
			 0x41f80000,
			 0x40400000,
			 0x41400000,
			 0x41b80000,
			 0x41a00000,
			 0x41d80000,
			 0x41300000,

			 /// vpu register f4
			 0x41a80000,
			 0x41d80000,
			 0x41a00000,
			 0x41880000,
			 0x41d80000,
			 0x3f800000,
			 0x41f80000,
			 0x3f800000,

			 /// vpu register f5
			 0x41980000,
			 0x41f00000,
			 0x41800000,
			 0x41900000,
			 0x41600000,
			 0x41900000,
			 0x41880000,
			 0x41c80000,

			 /// vpu register f6
			 0x40a00000,
			 0x41c00000,
			 0x41980000,
			 0x40e00000,
			 0x40000000,
			 0x40400000,
			 0x41a00000,
			 0x41d80000,

			 /// vpu register f7
			 0x41300000,
			 0x41c80000,
			 0x41a80000,
			 0x42000000,
			 0x41d80000,
			 0x40400000,
			 0x40a00000,
			 0x41d00000,

			 /// vpu register f8
			 0x41a00000,
			 0x41b00000,
			 0x41a00000,
			 0x40c00000,
			 0x41a80000,
			 0x40400000,
			 0x41e00000,
			 0x41b00000,

			 /// vpu register f9
			 0x41c80000,
			 0x41600000,
			 0x41f80000,
			 0x41a80000,
			 0x41f80000,
			 0x41a00000,
			 0x41b80000,
			 0x40a00000,

			 /// vpu register f10
			 0x40000000,
			 0x41800000,
			 0x41500000,
			 0x41b80000,
			 0x41500000,
			 0x41300000,
			 0x41c80000,
			 0x41980000,

			 /// vpu register f11
			 0x41c00000,
			 0x41e00000,
			 0x41300000,
			 0x40400000,
			 0x41500000,
			 0x41200000,
			 0x41d00000,
			 0x42000000,

			 /// vpu register f12
			 0x42000000,
			 0x41300000,
			 0x41400000,
			 0x41500000,
			 0x3f800000,
			 0x40c00000,
			 0x41d00000,
			 0x41880000,

			 /// vpu register f13
			 0x41f00000,
			 0x41300000,
			 0x41d00000,
			 0x41e80000,
			 0x41a80000,
			 0x41f80000,
			 0x41980000,
			 0x41100000,

			 /// vpu register f14
			 0x40800000,
			 0x41100000,
			 0x41b00000,
			 0x41500000,
			 0x41e80000,
			 0x41980000,
			 0x41d80000,
			 0x41f00000,

			 /// vpu register f15
			 0x41700000,
			 0x40400000,
			 0x3f800000,
			 0x41600000,
			 0x41b80000,
			 0x41c00000,
			 0x41b80000,
			 0x40000000,

			 /// vpu register f16
			 0x41200000,
			 0x40400000,
			 0x40400000,
			 0x41b80000,
			 0x41800000,
			 0x41f80000,
			 0x41800000,
			 0x41e00000,

			 /// vpu register f17
			 0x41880000,
			 0x41e80000,
			 0x41500000,
			 0x41400000,
			 0x41b00000,
			 0x41700000,
			 0x41b00000,
			 0x41b00000,

			 /// vpu register f18
			 0x41e00000,
			 0x41800000,
			 0x41f00000,
			 0x42000000,
			 0x40e00000,
			 0x41400000,
			 0x41d00000,
			 0x41880000,

			 /// vpu register f19
			 0x41f00000,
			 0x41200000,
			 0x41400000,
			 0x41400000,
			 0x41a80000,
			 0x41400000,
			 0x41700000,
			 0x41c80000,

			 /// vpu register f20
			 0x41e00000,
			 0x40a00000,
			 0x41d80000,
			 0x40000000,
			 0x41b80000,
			 0x41500000,
			 0x42000000,
			 0x41e00000,

			 /// vpu register f21
			 0x41f80000,
			 0x41980000,
			 0x41a00000,
			 0x41e00000,
			 0x40e00000,
			 0x40a00000,
			 0x41880000,
			 0x41d80000,

			 /// vpu register f22
			 0x40e00000,
			 0x40000000,
			 0x41e00000,
			 0x41600000,
			 0x41980000,
			 0x41880000,
			 0x41d80000,
			 0x40a00000,

			 /// vpu register f23
			 0x42000000,
			 0x41d80000,
			 0x41a00000,
			 0x41100000,
			 0x41a80000,
			 0x41d00000,
			 0x41d80000,
			 0x41d00000,

			 /// vpu register f24
			 0x41500000,
			 0x40000000,
			 0x40e00000,
			 0x41c80000,
			 0x41100000,
			 0x40000000,
			 0x41b80000,
			 0x41f00000,

			 /// vpu register f25
			 0x42000000,
			 0x41980000,
			 0x41600000,
			 0x40000000,
			 0x40e00000,
			 0x41200000,
			 0x40a00000,
			 0x41d80000,

			 /// vpu register f26
			 0x41900000,
			 0x3f800000,
			 0x41a80000,
			 0x41d00000,
			 0x40000000,
			 0x41c00000,
			 0x41980000,
			 0x41400000,

			 /// vpu register f27
			 0x41e80000,
			 0x41c80000,
			 0x40000000,
			 0x40000000,
			 0x41b80000,
			 0x41d00000,
			 0x41100000,
			 0x41e00000,

			 /// vpu register f28
			 0x40800000,
			 0x42000000,
			 0x41c80000,
			 0x41d80000,
			 0x41c00000,
			 0x41a00000,
			 0x41c00000,
			 0x41200000,

			 /// vpu register f29
			 0x41880000,
			 0x41b80000,
			 0x41800000,
			 0x41f80000,
			 0x41880000,
			 0x42000000,
			 0x41500000,
			 0x41b00000,

			 /// vpu register f30
			 0x41600000,
			 0x41300000,
			 0x41200000,
			 0x41100000,
			 0x41c80000,
			 0x40e00000,
			 0x41b00000,
			 0x41400000,

			 /// vpu register f31
			 0x41f80000,
			 0x41a80000,
			 0x41100000,
			 0x40000000,
			 0x41800000,
			 0x41e00000,
			 0x42000000,
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
		"fmv.s.x f27, x12\n"                                  ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f17, x13\n"                                  ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f24, x13\n"                                  ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f4, x15\n"                                   ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f29, x14\n"                                  ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f9, x11\n"                                   ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f6, x11\n"                                   ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f16, x14\n"                                  ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f28, x12\n"                                  ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f23, x12\n"                                  ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f5, x15\n"                                   ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f2, x13\n"                                   ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f16, x11\n"                                  ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f3, x13\n"                                   ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f15, x15\n"                                  ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f9, x15\n"                                   ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f22, x14\n"                                  ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f22, x13\n"                                  ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f21, x12\n"                                  ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f28, x14\n"                                  ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f11, x15\n"                                  ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f25, x12\n"                                  ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f12, x12\n"                                  ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f26, x14\n"                                  ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f9, x13\n"                                   ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f6, x14\n"                                   ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f15, x15\n"                                  ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f2, x11\n"                                   ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f27, x14\n"                                  ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f6, x13\n"                                   ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f26, x15\n"                                  ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f20, x12\n"                                  ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f22, x14\n"                                  ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f30, x13\n"                                  ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f2, x14\n"                                   ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f17, x11\n"                                  ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f26, x14\n"                                  ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f14, x15\n"                                  ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f3, x14\n"                                   ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f30, x13\n"                                  ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f20, x13\n"                                  ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f12, x15\n"                                  ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f18, x15\n"                                  ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f16, x12\n"                                  ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f27, x11\n"                                  ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f7, x15\n"                                   ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f21, x14\n"                                  ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f16, x13\n"                                  ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f3, x12\n"                                   ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f17, x11\n"                                  ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f25, x14\n"                                  ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f13, x13\n"                                  ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f26, x11\n"                                  ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f28, x12\n"                                  ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f8, x13\n"                                   ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f6, x12\n"                                   ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f15, x11\n"                                  ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f26, x14\n"                                  ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f20, x13\n"                                  ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f20, x15\n"                                  ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f5, x12\n"                                   ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f25, x12\n"                                  ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f11, x11\n"                                  ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f5, x14\n"                                   ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f7, x15\n"                                   ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f25, x13\n"                                  ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f12, x12\n"                                  ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f30, x14\n"                                  ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f13, x15\n"                                  ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f24, x11\n"                                  ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f3, x13\n"                                   ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f2, x12\n"                                   ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f15, x14\n"                                  ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f11, x11\n"                                  ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f24, x14\n"                                  ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f28, x11\n"                                  ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f11, x14\n"                                  ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f5, x14\n"                                   ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f11, x14\n"                                  ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f23, x11\n"                                  ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f28, x12\n"                                  ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f13, x15\n"                                  ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f4, x12\n"                                   ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f24, x13\n"                                  ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f22, x14\n"                                  ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f10, x15\n"                                  ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f10, x14\n"                                  ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f8, x14\n"                                   ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f30, x14\n"                                  ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f22, x13\n"                                  ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f28, x12\n"                                  ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f28, x15\n"                                  ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f6, x15\n"                                   ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f18, x13\n"                                  ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f11, x11\n"                                  ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f2, x15\n"                                   ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f9, x15\n"                                   ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f23, x15\n"                                  ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmv.s.x f27, x13\n"                                  ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
