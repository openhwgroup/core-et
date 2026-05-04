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
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00000
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00004
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00008
			 0x0c400000,                                                  // Leading 1s:                                 /// 0000c
			 0xffc00001,                                                  // -signaling NaN                              /// 00010
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00014
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00018
			 0xffc00001,                                                  // -signaling NaN                              /// 0001c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00020
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00024
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00028
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0002c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00030
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00034
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0003c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00040
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00044
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00048
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0004c
			 0x3f028f5c,                                                  // 0.51                                        /// 00050
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00054
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00058
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0005c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00060
			 0x0e000007,                                                  // Trailing 1s:                                /// 00064
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0006c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00070
			 0x4b000000,                                                  // 8388608.0                                   /// 00074
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00078
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0007c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00080
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00088
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0008c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00090
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00098
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0009c
			 0x00011111,                                                  // 9.7958E-41                                  /// 000a0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000a4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 000ac
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 000b0
			 0xff800000,                                                  // -inf                                        /// 000b4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 000c0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 000c4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000c8
			 0x0c600000,                                                  // Leading 1s:                                 /// 000cc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000d0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000d4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 000d8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000dc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000e0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000e4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000ec
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000f0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 000f4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000f8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000fc
			 0x55555555,                                                  // 4 random values                             /// 00100
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00104
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00108
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0010c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00114
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00118
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0011c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00124
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00128
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0012c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00130
			 0x00000000,                                                  // zero                                        /// 00134
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0013c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00140
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0014c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00150
			 0x4b000000,                                                  // 8388608.0                                   /// 00154
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00158
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0015c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00160
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00164
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00168
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0016c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00170
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00174
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00178
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0017c
			 0x3f028f5c,                                                  // 0.51                                        /// 00180
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00184
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00188
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0018c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00190
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00194
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00198
			 0xff800000,                                                  // -inf                                        /// 0019c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001a0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 001a4
			 0x00000000,                                                  // zero                                        /// 001a8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001ac
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 001b4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 001bc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 001c4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001c8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001cc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001d0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 001d4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001d8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001dc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001e0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001e4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001f4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001fc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00200
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00204
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0x0e000001,                                                  // Trailing 1s:                                /// 0020c
			 0x3f028f5c,                                                  // 0.51                                        /// 00210
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00214
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00218
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00220
			 0x0e000003,                                                  // Trailing 1s:                                /// 00224
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00228
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0022c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00230
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00234
			 0x80000000,                                                  // -zero                                       /// 00238
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0023c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00240
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00244
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00248
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0024c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00250
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00254
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00258
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0025c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00260
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00264
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00268
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0026c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00270
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00278
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0027c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00280
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00284
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00288
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0028c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00290
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00294
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00298
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0029c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a4
			 0x0c700000,                                                  // Leading 1s:                                 /// 002a8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002ac
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002b0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002b4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002b8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 002bc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002c0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 002c4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 002c8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 002cc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002d0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 002d4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002d8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002dc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002e0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002e8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002ec
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002f0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002f4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 002f8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 002fc
			 0xffc00001,                                                  // -signaling NaN                              /// 00300
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00304
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00308
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0030c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00310
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00318
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0031c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00320
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00324
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00328
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0032c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00330
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00334
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0033c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00340
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00344
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00348
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0034c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00350
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00354
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00358
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0035c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00360
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00364
			 0x4b000000,                                                  // 8388608.0                                   /// 00368
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0036c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00370
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00378
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0037c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00380
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00384
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00388
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0038c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00390
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00398
			 0xbf028f5c,                                                  // -0.51                                       /// 0039c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003a4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 003ac
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 003b0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 003b4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003bc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 003c4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 003c8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0x0c780000,                                                  // Leading 1s:                                 /// 003d4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003d8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003dc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 003e0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003e4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003ec
			 0x0e00000f,                                                  // Trailing 1s:                                /// 003f0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003f8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003fc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00400
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00404
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00408
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0040c
			 0x33333333,                                                  // 4 random values                             /// 00410
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00414
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00418
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0041c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00420
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00424
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00428
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0042c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00430
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00434
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00438
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0043c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00440
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00444
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00448
			 0x4b000000,                                                  // 8388608.0                                   /// 0044c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00454
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00458
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0045c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00460
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00464
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00468
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0046c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00470
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00474
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00478
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0047c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00480
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00484
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00488
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0048c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00490
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00494
			 0x0c780000,                                                  // Leading 1s:                                 /// 00498
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0049c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004a0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 004a4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004a8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 004ac
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004b0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004c0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004c4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004cc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004d0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 004d4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004d8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 004dc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004e0
			 0xbf028f5c,                                                  // -0.51                                       /// 004e4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004e8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004ec
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004f0
			 0x0c780000,                                                  // Leading 1s:                                 /// 004f4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f8
			 0x80000000,                                                  // -zero                                       /// 004fc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00500
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00504
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00508
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0050c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0xcb000000,                                                  // -8388608.0                                  /// 00514
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0x4b000000,                                                  // 8388608.0                                   /// 00520
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00524
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00528
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0052c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00530
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00534
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00538
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00540
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00544
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00548
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0054c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00550
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00554
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00558
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0055c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00564
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00568
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0056c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00570
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00574
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00578
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0057c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00580
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00584
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00588
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0058c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00590
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00594
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00598
			 0x80000000,                                                  // -zero                                       /// 0059c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 005a4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005a8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005ac
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005b8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005bc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005c0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 005c8
			 0x00011111,                                                  // 9.7958E-41                                  /// 005cc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 005d0
			 0x00011111,                                                  // 9.7958E-41                                  /// 005d4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005d8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005dc
			 0xbf028f5c,                                                  // -0.51                                       /// 005e0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005e4
			 0xff800000,                                                  // -inf                                        /// 005e8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 005ec
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 005f0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 005f4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005f8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 005fc
			 0x7fc00001,                                                  // signaling NaN                               /// 00600
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0060c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00610
			 0x0e000001,                                                  // Trailing 1s:                                /// 00614
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00618
			 0x00000000,                                                  // zero                                        /// 0061c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00620
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00624
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00628
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0062c
			 0x7f800000,                                                  // inf                                         /// 00630
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00634
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0063c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00640
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00644
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00648
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0064c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00650
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00654
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00658
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0065c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00660
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0066c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00670
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00674
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0067c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00680
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00684
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00688
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0068c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00690
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00694
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00698
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0069c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 006a0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006a4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x7fc00001,                                                  // signaling NaN                               /// 006ac
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0x0e000003,                                                  // Trailing 1s:                                /// 006b4
			 0x0c600000,                                                  // Leading 1s:                                 /// 006b8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 006bc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006c0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 006c4
			 0x80011111,                                                  // -9.7958E-41                                 /// 006c8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 006cc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 006d0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006d8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006dc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006e0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 006e4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 006e8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006ec
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006f0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 006f4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006f8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006fc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00700
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00704
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00708
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0070c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00714
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0071c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00720
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00724
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00728
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0072c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0x7f800000,                                                  // inf                                         /// 00738
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0073c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00740
			 0x80000000,                                                  // -zero                                       /// 00744
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00748
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0074c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00750
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00754
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00758
			 0x0c600000,                                                  // Leading 1s:                                 /// 0075c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00760
			 0x00000000,                                                  // zero                                        /// 00764
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00768
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00774
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00778
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0077c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00784
			 0x0c780000,                                                  // Leading 1s:                                 /// 00788
			 0x55555555,                                                  // 4 random values                             /// 0078c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00790
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00794
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00798
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0079c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007a0
			 0x33333333,                                                  // 4 random values                             /// 007a4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007a8
			 0x33333333,                                                  // 4 random values                             /// 007ac
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007b0
			 0x0e000001,                                                  // Trailing 1s:                                /// 007b4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007b8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007bc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007c4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007c8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 007cc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 007d4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007d8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 007dc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007e0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007e4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007ec
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007f0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007f4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 007f8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00800
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00804
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00808
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0080c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00810
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00814
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00818
			 0x7fc00001,                                                  // signaling NaN                               /// 0081c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00820
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00824
			 0x80000000,                                                  // -zero                                       /// 00828
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0082c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00830
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00834
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00838
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0083c
			 0xcb000000,                                                  // -8388608.0                                  /// 00840
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00844
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00848
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0084c
			 0x7fc00001,                                                  // signaling NaN                               /// 00850
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00854
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00858
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0085c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00864
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00868
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0086c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00870
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00874
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00878
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0087c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00884
			 0x00011111,                                                  // 9.7958E-41                                  /// 00888
			 0x0c600000,                                                  // Leading 1s:                                 /// 0088c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00890
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00894
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00898
			 0x0e000001,                                                  // Trailing 1s:                                /// 0089c
			 0x0e000007,                                                  // Trailing 1s:                                /// 008a0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 008a4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 008ac
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008b4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008b8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008bc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008c0
			 0xbf028f5c,                                                  // -0.51                                       /// 008c4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008c8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 008cc
			 0xbf028f5c,                                                  // -0.51                                       /// 008d0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 008d4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008d8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 008dc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008e0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008e4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008ec
			 0x0e00000f,                                                  // Trailing 1s:                                /// 008f0
			 0x7f800000,                                                  // inf                                         /// 008f4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008f8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008fc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00908
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0090c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00910
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00914
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00918
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0091c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00920
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00924
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00928
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0092c
			 0x00000000,                                                  // zero                                        /// 00930
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00934
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00938
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0093c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00940
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00944
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00948
			 0xbf028f5c,                                                  // -0.51                                       /// 0094c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00950
			 0x0c700000,                                                  // Leading 1s:                                 /// 00954
			 0x0e000001,                                                  // Trailing 1s:                                /// 00958
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0095c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00960
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00964
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00968
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0096c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00970
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00974
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0097c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00980
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00984
			 0x0e000003,                                                  // Trailing 1s:                                /// 00988
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0098c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00990
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00998
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0099c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 009b0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009b4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009b8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009bc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 009c0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009c4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009c8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009cc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009d0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009d4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009d8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009dc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 009e0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009e4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009e8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 009ec
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 009f0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009f4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009fc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a04
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a08
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a0c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a10
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a18
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a1c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a20
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a24
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a28
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a2c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a34
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a38
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a3c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a40
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a44
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a48
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a4c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a50
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a54
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a58
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a5c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a60
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a64
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a68
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a6c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a70
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a74
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a78
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a7c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a84
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a8c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a90
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a94
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a98
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a9c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00aa0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00aa4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00aa8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0xff800000,                                                  // -inf                                        /// 00ab8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00abc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ac0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ac4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ac8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00acc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ad0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ad8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00adc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ae0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ae4
			 0x4b000000,                                                  // 8388608.0                                   /// 00ae8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00aec
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00af0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00af4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00af8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00afc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b04
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b08
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b0c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b10
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b14
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b18
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b1c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b20
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b24
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b28
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b2c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b30
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b34
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b38
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b3c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b40
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b44
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b48
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b50
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b54
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b58
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b5c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b60
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b64
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b6c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b74
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b78
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b7c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b80
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0x4b000000,                                                  // 8388608.0                                   /// 00b88
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b8c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b90
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b94
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b98
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b9c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ba0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ba8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00bb4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00bb8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bbc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00bc0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00bc4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00bc8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00bcc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00bd0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bdc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00be4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00be8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00bec
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00bf4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00bf8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c00
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c04
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c08
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c0c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c10
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c18
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c1c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c24
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c28
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c2c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c30
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c34
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c38
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c3c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c40
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c44
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c48
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c4c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c54
			 0xcb000000,                                                  // -8388608.0                                  /// 00c58
			 0xff800000,                                                  // -inf                                        /// 00c5c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c60
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c64
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0xffc00001,                                                  // -signaling NaN                              /// 00c6c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c70
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c74
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c78
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c7c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c80
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c84
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c88
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c8c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c90
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c94
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c98
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c9c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ca0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ca4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ca8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00cac
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00cb0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00cb4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cbc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00cc0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00cc4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00cc8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ccc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00cd0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00cd8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cdc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ce4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ce8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cec
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cf0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00cf4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00cf8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d00
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d04
			 0xcb000000,                                                  // -8388608.0                                  /// 00d08
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d0c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d14
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d1c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d24
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d28
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d2c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d30
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d34
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d38
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d3c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d40
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d44
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d48
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d4c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d50
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d54
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d5c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d60
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d64
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d68
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d6c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d70
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d74
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d78
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d7c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d80
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d88
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d90
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d94
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d98
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d9c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00da0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00da4
			 0xff800000,                                                  // -inf                                        /// 00da8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00db0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00db4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00db8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dbc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00dc0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00dc8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00dcc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00dd0
			 0x80000000,                                                  // -zero                                       /// 00dd4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00dd8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ddc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00de4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00de8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dec
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00df0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00dfc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e00
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e04
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e0c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e10
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e14
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e18
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e1c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e20
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e24
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e28
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e2c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e30
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e34
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e3c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e40
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e44
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e48
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e4c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e50
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e54
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e58
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e5c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e60
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e64
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e68
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e6c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e70
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e74
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e78
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e7c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e80
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e84
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e88
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e8c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e90
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e94
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e9c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ea0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ea4
			 0xff800000,                                                  // -inf                                        /// 00ea8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00eac
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00eb0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00eb8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ebc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ec0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ec8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ecc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ed0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ed8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ee0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ee4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ee8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00eec
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ef4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ef8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00efc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f00
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f04
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f08
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f0c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f10
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f14
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f18
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f1c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f20
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f24
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f28
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f2c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f30
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f34
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f38
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f3c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f48
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f4c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f58
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f5c
			 0x55555555,                                                  // 4 random values                             /// 00f60
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f64
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f68
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f6c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f70
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f74
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f78
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f7c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f80
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f88
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f8c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f94
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f98
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f9c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0x00000000,                                                  // zero                                        /// 00fa4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00fa8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00fac
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fb8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00fbc
			 0x80000000,                                                  // -zero                                       /// 00fc0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00fc4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fc8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00fcc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00fd0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00fd4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00fdc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00fe4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fe8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00fec
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ff0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ff4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ff8
			 0x80000100                                                  // SP - 1 bit alone set in mantissa -ve        /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00000
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00004
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00008
			 0xbf028f5c,                                                  // -0.51                                       /// 0000c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00010
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00014
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00018
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0001c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00020
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00024
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00028
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0002c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00030
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00038
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0003c
			 0x7f800000,                                                  // inf                                         /// 00040
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00044
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00048
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0004c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00050
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00054
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00058
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0005c
			 0xbf028f5c,                                                  // -0.51                                       /// 00060
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00064
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00068
			 0x0c600000,                                                  // Leading 1s:                                 /// 0006c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00070
			 0x0e000007,                                                  // Trailing 1s:                                /// 00074
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00078
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0007c
			 0x7f800000,                                                  // inf                                         /// 00080
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00084
			 0x0c780000,                                                  // Leading 1s:                                 /// 00088
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0008c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00090
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00094
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0009c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 000a0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 000a4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000a8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 000ac
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b0
			 0x33333333,                                                  // 4 random values                             /// 000b4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 000bc
			 0x0c780000,                                                  // Leading 1s:                                 /// 000c0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000c4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 000c8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000cc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x55555555,                                                  // 4 random values                             /// 000d4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 000d8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000e0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000e8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000ec
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 000f0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000f4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000fc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00104
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0010c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00110
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00114
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00118
			 0x7fc00001,                                                  // signaling NaN                               /// 0011c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00120
			 0x80011111,                                                  // -9.7958E-41                                 /// 00124
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00128
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0012c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00130
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00134
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00138
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0013c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00140
			 0x0e000003,                                                  // Trailing 1s:                                /// 00144
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00148
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0014c
			 0xbf028f5c,                                                  // -0.51                                       /// 00150
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00154
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0015c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00160
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00164
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00168
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00170
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00174
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00178
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0017c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00180
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00184
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00188
			 0x4b000000,                                                  // 8388608.0                                   /// 0018c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00190
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00194
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00198
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0019c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001a0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 001a8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 001ac
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001b4
			 0x7f800000,                                                  // inf                                         /// 001b8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001bc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001c4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 001d0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 001d4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 001dc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001e4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001e8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 001ec
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 001f0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001f4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 001f8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001fc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00200
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00204
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00208
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00210
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00214
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00218
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0021c
			 0xcb000000,                                                  // -8388608.0                                  /// 00220
			 0x0c400000,                                                  // Leading 1s:                                 /// 00224
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00228
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00230
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00234
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00238
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00240
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00244
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00248
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0024c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00250
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00254
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00258
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0025c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00260
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00264
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00268
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0026c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00270
			 0x0e000003,                                                  // Trailing 1s:                                /// 00274
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00278
			 0x80011111,                                                  // -9.7958E-41                                 /// 0027c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00280
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00284
			 0x0c700000,                                                  // Leading 1s:                                 /// 00288
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0028c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00290
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00298
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0029c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 002a0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002a4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002b0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002b4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002b8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002bc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002c0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002c4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002cc
			 0x33333333,                                                  // 4 random values                             /// 002d0
			 0xffc00001,                                                  // -signaling NaN                              /// 002d4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002d8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002dc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002e0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 002e4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002e8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002f4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00300
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00304
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00308
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0030c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00314
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00318
			 0x00011111,                                                  // 9.7958E-41                                  /// 0031c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00320
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x0c700000,                                                  // Leading 1s:                                 /// 00328
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0xffc00001,                                                  // -signaling NaN                              /// 00330
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00338
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0033c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00344
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00348
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0034c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00358
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0035c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00360
			 0x00011111,                                                  // 9.7958E-41                                  /// 00364
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00368
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0036c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00370
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00374
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00378
			 0xbf028f5c,                                                  // -0.51                                       /// 0037c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00380
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00388
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0038c
			 0xcb000000,                                                  // -8388608.0                                  /// 00390
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00394
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00398
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003a0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003a4
			 0xff800000,                                                  // -inf                                        /// 003a8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 003b0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 003b8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003bc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003c0
			 0x0c600000,                                                  // Leading 1s:                                 /// 003c4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c8
			 0x7f800000,                                                  // inf                                         /// 003cc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 003d0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 003d4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 003d8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 003dc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003e8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ec
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003f0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 003f4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 003f8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00400
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00404
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00408
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0040c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00414
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00418
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0041c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00420
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00424
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00428
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0042c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00430
			 0x80011111,                                                  // -9.7958E-41                                 /// 00434
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00438
			 0x0c600000,                                                  // Leading 1s:                                 /// 0043c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00440
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00444
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00448
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0044c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00450
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00454
			 0x00011111,                                                  // 9.7958E-41                                  /// 00458
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0xbf028f5c,                                                  // -0.51                                       /// 00464
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00468
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0046c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00470
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00474
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00478
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0047c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00480
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00484
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00488
			 0x80000000,                                                  // -zero                                       /// 0048c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00490
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0049c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004a0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 004a4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 004a8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0x80011111,                                                  // -9.7958E-41                                 /// 004b0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004b4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004bc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004c0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004c4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004c8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 004cc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 004d0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 004d4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004d8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004ec
			 0x0c700000,                                                  // Leading 1s:                                 /// 004f0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004f4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004f8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004fc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00500
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00504
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00508
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0050c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00510
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00514
			 0xbf028f5c,                                                  // -0.51                                       /// 00518
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0051c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00520
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00524
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00528
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0052c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00530
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00534
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00538
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0053c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00540
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00544
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00548
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0054c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00550
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00554
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00558
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0055c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00560
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00564
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00568
			 0x33333333,                                                  // 4 random values                             /// 0056c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00570
			 0x00011111,                                                  // 9.7958E-41                                  /// 00574
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00578
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0057c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00580
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00584
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0058c
			 0xff800000,                                                  // -inf                                        /// 00590
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00594
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00598
			 0x7f800000,                                                  // inf                                         /// 0059c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005a0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 005a4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005a8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005ac
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 005b0
			 0x0c700000,                                                  // Leading 1s:                                 /// 005b4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 005b8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005bc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 005c8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 005cc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005d0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x80011111,                                                  // -9.7958E-41                                 /// 005d8
			 0x0c400000,                                                  // Leading 1s:                                 /// 005dc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005e0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005e8
			 0x00000000,                                                  // zero                                        /// 005ec
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 005f0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005f4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 005fc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00600
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00604
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0060c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00610
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00614
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x0e000007,                                                  // Trailing 1s:                                /// 0061c
			 0xcb000000,                                                  // -8388608.0                                  /// 00620
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00624
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00628
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0062c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00630
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00634
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00638
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0063c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00640
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00644
			 0x80000000,                                                  // -zero                                       /// 00648
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0064c
			 0x80000000,                                                  // -zero                                       /// 00650
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00654
			 0x7f800000,                                                  // inf                                         /// 00658
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0065c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00660
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00664
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00668
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0066c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00670
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00678
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0067c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00680
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00684
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0068c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00690
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00694
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00698
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0069c
			 0x33333333,                                                  // 4 random values                             /// 006a0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 006a4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x7fc00001,                                                  // signaling NaN                               /// 006ac
			 0x7fc00001,                                                  // signaling NaN                               /// 006b0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 006b4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006b8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006bc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006c0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006c8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006cc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006d0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006d4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d8
			 0x80000000,                                                  // -zero                                       /// 006dc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006e0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 006e4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006e8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 006ec
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006f0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006f4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006f8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006fc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00704
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0070c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00710
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00714
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00718
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0071c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00720
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00724
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00728
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0072c
			 0xff800000,                                                  // -inf                                        /// 00730
			 0x0c700000,                                                  // Leading 1s:                                 /// 00734
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00740
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00744
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00748
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0074c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00750
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00758
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0075c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00760
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00764
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00768
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0076c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00770
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00774
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00780
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00784
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00788
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00794
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00798
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0079c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007a0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007a4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ac
			 0xbf028f5c,                                                  // -0.51                                       /// 007b0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007b4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007b8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 007bc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 007c0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 007c4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007c8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 007cc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007d0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007d4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007d8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007dc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 007e0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007e4
			 0x0c700000,                                                  // Leading 1s:                                 /// 007e8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ec
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 007f0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007f4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00804
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00808
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0080c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00810
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00814
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00818
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0081c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00820
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00824
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00828
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00830
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00834
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00838
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00840
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00844
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00848
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0084c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00850
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00854
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00858
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0085c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00860
			 0xbf028f5c,                                                  // -0.51                                       /// 00864
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00868
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0086c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0x55555555,                                                  // 4 random values                             /// 00874
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00878
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0087c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00880
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00884
			 0x0c700000,                                                  // Leading 1s:                                 /// 00888
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0xcb000000,                                                  // -8388608.0                                  /// 00890
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00894
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00898
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0089c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 008a0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 008a8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ac
			 0xbf028f5c,                                                  // -0.51                                       /// 008b0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 008b4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 008b8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008bc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008c0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008c4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 008c8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008cc
			 0x0c780000,                                                  // Leading 1s:                                 /// 008d0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d4
			 0x0c780000,                                                  // Leading 1s:                                 /// 008d8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 008dc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e0
			 0x4b000000,                                                  // 8388608.0                                   /// 008e4
			 0x55555555,                                                  // 4 random values                             /// 008e8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 008ec
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008f0
			 0xff800000,                                                  // -inf                                        /// 008f4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008f8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008fc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00900
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00904
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00908
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0090c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00910
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00914
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00918
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0091c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00924
			 0x7f800000,                                                  // inf                                         /// 00928
			 0x33333333,                                                  // 4 random values                             /// 0092c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00930
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00934
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00938
			 0x3f028f5c,                                                  // 0.51                                        /// 0093c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00940
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00944
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00948
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0094c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0x55555555,                                                  // 4 random values                             /// 00954
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00958
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0095c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00960
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00964
			 0x4b000000,                                                  // 8388608.0                                   /// 00968
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0096c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00970
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00974
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00978
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0097c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00980
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00984
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00988
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0098c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00990
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00994
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0099c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009a0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009a4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009a8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009ac
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 009b0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 009b8
			 0xcb000000,                                                  // -8388608.0                                  /// 009bc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009c0
			 0xffc00001,                                                  // -signaling NaN                              /// 009c4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009cc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009d0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 009d8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009dc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 009ec
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 009f4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009fc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a00
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a04
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a08
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a0c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a10
			 0xffc00001,                                                  // -signaling NaN                              /// 00a14
			 0x33333333,                                                  // 4 random values                             /// 00a18
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a1c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a20
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a24
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a28
			 0x3f028f5c,                                                  // 0.51                                        /// 00a2c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a30
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a34
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a38
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a3c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a40
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a48
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a4c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a50
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a54
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a5c
			 0x7fc00001,                                                  // signaling NaN                               /// 00a60
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a64
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a68
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a6c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a70
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a74
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a78
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a7c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a84
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a88
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a8c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a94
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a98
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a9c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00aa4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00aa8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00aac
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ab0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ab8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00abc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ac0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ac8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00acc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ad0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ad8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00adc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ae0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ae4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ae8
			 0xff800000,                                                  // -inf                                        /// 00aec
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00af4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00af8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00afc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b00
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b04
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b08
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b0c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b10
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b14
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b18
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b20
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b24
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b28
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b2c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b30
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b34
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b3c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b40
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b44
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b48
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b4c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b50
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b54
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b58
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b5c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b60
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b64
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b70
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b74
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b78
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b7c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b80
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b84
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b88
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b8c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b90
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b94
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b9c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ba0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ba4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ba8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00bac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00bb0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00bb4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00bb8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00bbc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00bc4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00bcc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00bd0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bd4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00bd8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00bdc
			 0xcb000000,                                                  // -8388608.0                                  /// 00be0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00be4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00be8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00bec
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00bf0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00bf4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00bfc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c00
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c04
			 0x80000000,                                                  // -zero                                       /// 00c08
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c0c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c10
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c14
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c18
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c1c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c24
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c28
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c2c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c30
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c34
			 0xcb000000,                                                  // -8388608.0                                  /// 00c38
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c3c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c40
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c48
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c4c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c50
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c54
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c58
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c60
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c64
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0x80000000,                                                  // -zero                                       /// 00c6c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c70
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c74
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c78
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c7c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c80
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c84
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c88
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c8c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c90
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c94
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c98
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c9c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ca0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ca4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ca8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00cac
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00cb0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00cb4
			 0xff800000,                                                  // -inf                                        /// 00cb8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cbc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00cc0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00cc4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ccc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cd0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00cdc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ce0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ce4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ce8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00cec
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00cf0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00cf4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00cf8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00cfc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d00
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d04
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d08
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d0c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d10
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d14
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d18
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d1c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d24
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d2c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d30
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d34
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d38
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d3c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d40
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d44
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d48
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d4c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d50
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d54
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d58
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d5c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d60
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d64
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d68
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d70
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d78
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d7c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d80
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d84
			 0xff800000,                                                  // -inf                                        /// 00d88
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d8c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d90
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d94
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d98
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d9c
			 0x7f800000,                                                  // inf                                         /// 00da0
			 0x33333333,                                                  // 4 random values                             /// 00da4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00da8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00dac
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00db0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00db8
			 0x80000000,                                                  // -zero                                       /// 00dbc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00dc0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00dc4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00dc8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00dcc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00dd8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ddc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00de0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00de8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00dec
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00df0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00df4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00df8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dfc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e00
			 0x7f800000,                                                  // inf                                         /// 00e04
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e0c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e10
			 0x3f028f5c,                                                  // 0.51                                        /// 00e14
			 0x55555555,                                                  // 4 random values                             /// 00e18
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e1c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e24
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e28
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e2c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e30
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e34
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0x00000000,                                                  // zero                                        /// 00e3c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e40
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e44
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e4c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e50
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e58
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e5c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e60
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e64
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e68
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e6c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e70
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e74
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e78
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e7c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e80
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e84
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e88
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e8c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e94
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e98
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e9c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ea0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ea4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00eac
			 0x3f028f5c,                                                  // 0.51                                        /// 00eb0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00eb8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ebc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ec0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ec4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ec8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ecc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ed0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ed4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00edc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ee0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ee4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ee8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00eec
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef0
			 0x80000000,                                                  // -zero                                       /// 00ef4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ef8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00efc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f00
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f04
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f08
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f0c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f10
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f14
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f18
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f1c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0x80000000,                                                  // -zero                                       /// 00f24
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0x00000000,                                                  // zero                                        /// 00f2c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f30
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f34
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f38
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f3c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f40
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f44
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f48
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f4c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f50
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f54
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f58
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f5c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f60
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f64
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f68
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f6c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f70
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f74
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f7c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f80
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f84
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f88
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f8c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f90
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f94
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f9c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00fa0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fa8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fac
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00fb0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fb4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00fb8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fc4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fcc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00fd4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00fd8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00fdc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00fe0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fe4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ff0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ff4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ff8
			 0x0e00007f                                                  // Trailing 1s:                                /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x0a42900a, /// 0x0
			 0x15f272c7, /// 0x4
			 0x9e1250b4, /// 0x8
			 0x979ecc6c, /// 0xc
			 0x4ceee416, /// 0x10
			 0xa2a37ee1, /// 0x14
			 0x78056eec, /// 0x18
			 0xdc0fe897, /// 0x1c
			 0x14132107, /// 0x20
			 0x968ce32d, /// 0x24
			 0xd2f3b7e3, /// 0x28
			 0xf2651b2d, /// 0x2c
			 0xf72c46db, /// 0x30
			 0x0045bd4e, /// 0x34
			 0x5fcb7f12, /// 0x38
			 0xa0d63606, /// 0x3c
			 0x7bc0461d, /// 0x40
			 0x5c2445db, /// 0x44
			 0x5f192ca7, /// 0x48
			 0x1e314722, /// 0x4c
			 0xda612fb0, /// 0x50
			 0x117a379e, /// 0x54
			 0x6dc08767, /// 0x58
			 0xdfcf6364, /// 0x5c
			 0x6489a996, /// 0x60
			 0x0e08fad4, /// 0x64
			 0xa509065a, /// 0x68
			 0xeb9e6d57, /// 0x6c
			 0x0c776ba1, /// 0x70
			 0x99bf5d8d, /// 0x74
			 0xd3883844, /// 0x78
			 0xc024032a, /// 0x7c
			 0x5239958f, /// 0x80
			 0x9b5ac63d, /// 0x84
			 0x32aeb819, /// 0x88
			 0xae9cd5e4, /// 0x8c
			 0xf240f6e6, /// 0x90
			 0x7a395498, /// 0x94
			 0xd459421f, /// 0x98
			 0x4e4fc903, /// 0x9c
			 0xf37fae30, /// 0xa0
			 0x10826971, /// 0xa4
			 0x97f868bd, /// 0xa8
			 0x829f7601, /// 0xac
			 0x9cbd6244, /// 0xb0
			 0x198ad8b1, /// 0xb4
			 0x35e3c77f, /// 0xb8
			 0x6d089446, /// 0xbc
			 0x049f2c47, /// 0xc0
			 0xadfa2b25, /// 0xc4
			 0xf8aa34e0, /// 0xc8
			 0x079739b0, /// 0xcc
			 0x45e7c572, /// 0xd0
			 0x3ae88f9a, /// 0xd4
			 0x43c82588, /// 0xd8
			 0x74387144, /// 0xdc
			 0xc9653759, /// 0xe0
			 0xbc122c77, /// 0xe4
			 0xe3bd7085, /// 0xe8
			 0x64fcf287, /// 0xec
			 0xf4275cc2, /// 0xf0
			 0x357b6468, /// 0xf4
			 0x1302dc4a, /// 0xf8
			 0xa901f885, /// 0xfc
			 0xdeac3c16, /// 0x100
			 0x253cf411, /// 0x104
			 0xe698f563, /// 0x108
			 0x7481d905, /// 0x10c
			 0xde87ff3d, /// 0x110
			 0x9ce04d4f, /// 0x114
			 0xd073d81d, /// 0x118
			 0x963869b3, /// 0x11c
			 0x87d2171b, /// 0x120
			 0xf3bb049a, /// 0x124
			 0x7842a944, /// 0x128
			 0x6865c2b3, /// 0x12c
			 0x2dc96835, /// 0x130
			 0xdd284519, /// 0x134
			 0x68f0261b, /// 0x138
			 0x5a3df5c2, /// 0x13c
			 0x45313257, /// 0x140
			 0x13b446a9, /// 0x144
			 0x09121e4c, /// 0x148
			 0x476b21ff, /// 0x14c
			 0xef46de89, /// 0x150
			 0xc783f29e, /// 0x154
			 0xd51a29f3, /// 0x158
			 0x25709acd, /// 0x15c
			 0xe33723a6, /// 0x160
			 0x594efa97, /// 0x164
			 0x4643e0e0, /// 0x168
			 0x695f71f3, /// 0x16c
			 0xc73443fb, /// 0x170
			 0xc878a247, /// 0x174
			 0x987265bc, /// 0x178
			 0x3fbc6ca0, /// 0x17c
			 0x18b16c4b, /// 0x180
			 0xc90ca904, /// 0x184
			 0xddbe5b92, /// 0x188
			 0x16dfaf2b, /// 0x18c
			 0xf0439c3a, /// 0x190
			 0xfd92c72e, /// 0x194
			 0xad9362cd, /// 0x198
			 0x1e024124, /// 0x19c
			 0x075e6094, /// 0x1a0
			 0x5d65c822, /// 0x1a4
			 0x84445a16, /// 0x1a8
			 0xbaeeee58, /// 0x1ac
			 0xcec2af95, /// 0x1b0
			 0xc5136e50, /// 0x1b4
			 0x6aa04c68, /// 0x1b8
			 0x60328c9a, /// 0x1bc
			 0xa05528fa, /// 0x1c0
			 0x5fbf5f20, /// 0x1c4
			 0x9146634b, /// 0x1c8
			 0x36042ad5, /// 0x1cc
			 0x78e41e0d, /// 0x1d0
			 0xf4c87fa0, /// 0x1d4
			 0xaff74c95, /// 0x1d8
			 0x9f6d0017, /// 0x1dc
			 0xb1243f3d, /// 0x1e0
			 0x4ae23bac, /// 0x1e4
			 0x13a3af68, /// 0x1e8
			 0x910842c2, /// 0x1ec
			 0xe44fedbb, /// 0x1f0
			 0x51d6a685, /// 0x1f4
			 0x8352d3f9, /// 0x1f8
			 0xa17c5ac5, /// 0x1fc
			 0xab326d3d, /// 0x200
			 0x4551983c, /// 0x204
			 0x86eae813, /// 0x208
			 0xc20fe2a5, /// 0x20c
			 0xcf67f4d8, /// 0x210
			 0x39768028, /// 0x214
			 0x8d1579de, /// 0x218
			 0x62d4d3f0, /// 0x21c
			 0xcd91c450, /// 0x220
			 0x237ad130, /// 0x224
			 0x25b52c03, /// 0x228
			 0x0b46b701, /// 0x22c
			 0x33f389af, /// 0x230
			 0x6a83535e, /// 0x234
			 0x0394a799, /// 0x238
			 0xdab95613, /// 0x23c
			 0x517dc9a9, /// 0x240
			 0x583e98e6, /// 0x244
			 0xdb6b2c63, /// 0x248
			 0x00d230d9, /// 0x24c
			 0x91a48a2e, /// 0x250
			 0x89001646, /// 0x254
			 0xa74917fc, /// 0x258
			 0xebe5d80c, /// 0x25c
			 0x9a38a709, /// 0x260
			 0x5d0e16dd, /// 0x264
			 0x9e9d70d8, /// 0x268
			 0x49ea4eac, /// 0x26c
			 0x9339e457, /// 0x270
			 0x360e758b, /// 0x274
			 0x1b6b42e3, /// 0x278
			 0xe146c6db, /// 0x27c
			 0x9279d890, /// 0x280
			 0xb7eb0865, /// 0x284
			 0x8bb858d8, /// 0x288
			 0xc3d12c15, /// 0x28c
			 0xf36ac33a, /// 0x290
			 0xb501f57e, /// 0x294
			 0x6dd35b44, /// 0x298
			 0xc3df2565, /// 0x29c
			 0xa6a8eab7, /// 0x2a0
			 0xe1e673f2, /// 0x2a4
			 0x37a59443, /// 0x2a8
			 0xa5623456, /// 0x2ac
			 0x0f527398, /// 0x2b0
			 0x1550dbdb, /// 0x2b4
			 0xf6772c2f, /// 0x2b8
			 0x23317157, /// 0x2bc
			 0xe026cdc0, /// 0x2c0
			 0xb6db617a, /// 0x2c4
			 0x4dc8058a, /// 0x2c8
			 0xf7f43219, /// 0x2cc
			 0xa5cbd280, /// 0x2d0
			 0x382208d7, /// 0x2d4
			 0x3effa1d6, /// 0x2d8
			 0xd181e276, /// 0x2dc
			 0x87c6f386, /// 0x2e0
			 0xcddc37cd, /// 0x2e4
			 0xcc4e7f37, /// 0x2e8
			 0xbd2a6089, /// 0x2ec
			 0xd7fa7d9b, /// 0x2f0
			 0x87270eb4, /// 0x2f4
			 0xad487ad3, /// 0x2f8
			 0x6c1efc7a, /// 0x2fc
			 0x8e738830, /// 0x300
			 0xd9ad8e86, /// 0x304
			 0xd47eb605, /// 0x308
			 0x621a0efc, /// 0x30c
			 0x3867be78, /// 0x310
			 0x6045a868, /// 0x314
			 0x6c8a318a, /// 0x318
			 0xc1d4752e, /// 0x31c
			 0xade963ea, /// 0x320
			 0xb09469b1, /// 0x324
			 0xaf3179aa, /// 0x328
			 0x64ad9a72, /// 0x32c
			 0x947767ad, /// 0x330
			 0x6145d78d, /// 0x334
			 0xd296f269, /// 0x338
			 0xf86857af, /// 0x33c
			 0x7312c63b, /// 0x340
			 0xc1cecc99, /// 0x344
			 0x6e15a654, /// 0x348
			 0xa0439052, /// 0x34c
			 0x21b7fab4, /// 0x350
			 0x3fa2e6c7, /// 0x354
			 0x08a3a061, /// 0x358
			 0x0eb84313, /// 0x35c
			 0x4c06cba6, /// 0x360
			 0x3b070bed, /// 0x364
			 0x3dfa7e4a, /// 0x368
			 0x5ce3a663, /// 0x36c
			 0x5983cf63, /// 0x370
			 0x6e6a58d7, /// 0x374
			 0xeedb26b7, /// 0x378
			 0x9a787684, /// 0x37c
			 0x9e3ebf24, /// 0x380
			 0x022c101e, /// 0x384
			 0x72e4dda2, /// 0x388
			 0xfc651ed3, /// 0x38c
			 0x583b8a36, /// 0x390
			 0x482a6be3, /// 0x394
			 0xdedeec01, /// 0x398
			 0x2388cebb, /// 0x39c
			 0x102d0364, /// 0x3a0
			 0x83317c66, /// 0x3a4
			 0xf27edf70, /// 0x3a8
			 0x1c62f3c8, /// 0x3ac
			 0x7d5b0344, /// 0x3b0
			 0xbabcfa06, /// 0x3b4
			 0xf86d235d, /// 0x3b8
			 0xccadbc20, /// 0x3bc
			 0x98228659, /// 0x3c0
			 0xd8778d31, /// 0x3c4
			 0x2f42a76f, /// 0x3c8
			 0xb6091303, /// 0x3cc
			 0x58aaa9fe, /// 0x3d0
			 0x513a67bc, /// 0x3d4
			 0xac6cf01b, /// 0x3d8
			 0x8e2f43ee, /// 0x3dc
			 0x655775a1, /// 0x3e0
			 0x40b01e37, /// 0x3e4
			 0xf30faf4b, /// 0x3e8
			 0xefcb1013, /// 0x3ec
			 0x5215b5d5, /// 0x3f0
			 0x7268e142, /// 0x3f4
			 0x15597ea4, /// 0x3f8
			 0x7b36f36d, /// 0x3fc
			 0xcbd260d7, /// 0x400
			 0x838fa257, /// 0x404
			 0x94e00b07, /// 0x408
			 0xbd08c1c9, /// 0x40c
			 0x63a12b08, /// 0x410
			 0xb01f29d7, /// 0x414
			 0xe4a6589a, /// 0x418
			 0x56701221, /// 0x41c
			 0x47e9b6ef, /// 0x420
			 0x01235503, /// 0x424
			 0xa88d7732, /// 0x428
			 0x3c0a7f74, /// 0x42c
			 0x02b56e59, /// 0x430
			 0x4aa1fb88, /// 0x434
			 0x1d9c1515, /// 0x438
			 0x59f868f9, /// 0x43c
			 0xbd59b01a, /// 0x440
			 0xba631d95, /// 0x444
			 0x42d6f797, /// 0x448
			 0x4f9281b8, /// 0x44c
			 0xb9cafad2, /// 0x450
			 0xbf337257, /// 0x454
			 0xd46b74ab, /// 0x458
			 0xfc035b21, /// 0x45c
			 0xe0150ff0, /// 0x460
			 0x0bb46e93, /// 0x464
			 0xf9d268d9, /// 0x468
			 0xa7e14990, /// 0x46c
			 0x33cc5f75, /// 0x470
			 0x788d7a84, /// 0x474
			 0x9d783a64, /// 0x478
			 0xb5f7eb6c, /// 0x47c
			 0x771a6115, /// 0x480
			 0x1167f0f6, /// 0x484
			 0x6822432c, /// 0x488
			 0xf7ef4d45, /// 0x48c
			 0x8c938185, /// 0x490
			 0x4d3990ff, /// 0x494
			 0xbc77feb8, /// 0x498
			 0x3b2284dd, /// 0x49c
			 0xfeee2784, /// 0x4a0
			 0xc0fff04b, /// 0x4a4
			 0xe8f8d627, /// 0x4a8
			 0xe30313e8, /// 0x4ac
			 0xf42c7147, /// 0x4b0
			 0xd53cc494, /// 0x4b4
			 0x9f68bf23, /// 0x4b8
			 0x91dace4c, /// 0x4bc
			 0x08276e5e, /// 0x4c0
			 0x0f133fc7, /// 0x4c4
			 0x26e2f4de, /// 0x4c8
			 0x04970122, /// 0x4cc
			 0xf1bd2e04, /// 0x4d0
			 0x9d1ba53d, /// 0x4d4
			 0xcc40e982, /// 0x4d8
			 0xf94b293c, /// 0x4dc
			 0xd97350f2, /// 0x4e0
			 0x8d564151, /// 0x4e4
			 0xf1724c9b, /// 0x4e8
			 0x4b18981b, /// 0x4ec
			 0x277dafe4, /// 0x4f0
			 0xf16a76d7, /// 0x4f4
			 0x86d1cca0, /// 0x4f8
			 0xb8656d52, /// 0x4fc
			 0xa979c71d, /// 0x500
			 0x231357a8, /// 0x504
			 0x068a0e87, /// 0x508
			 0xb4e8e51b, /// 0x50c
			 0x793ee7f3, /// 0x510
			 0x071ecbad, /// 0x514
			 0xfdff1264, /// 0x518
			 0xb8567b44, /// 0x51c
			 0xc6380579, /// 0x520
			 0xc16a024a, /// 0x524
			 0x1c22e0da, /// 0x528
			 0x0ed2292a, /// 0x52c
			 0xba42c5d2, /// 0x530
			 0x157b7169, /// 0x534
			 0xf6147e65, /// 0x538
			 0xc702ffbc, /// 0x53c
			 0xb15d6382, /// 0x540
			 0x106ab0d2, /// 0x544
			 0x0a2e3368, /// 0x548
			 0xcd2a26b8, /// 0x54c
			 0xbdc0cc62, /// 0x550
			 0x695f6d96, /// 0x554
			 0xb7b87d28, /// 0x558
			 0x61536c4f, /// 0x55c
			 0xaa7c6445, /// 0x560
			 0x6553279a, /// 0x564
			 0x04823680, /// 0x568
			 0x56cc1132, /// 0x56c
			 0x466d2bff, /// 0x570
			 0xa2837e02, /// 0x574
			 0xee9492d2, /// 0x578
			 0x1465b6df, /// 0x57c
			 0xa3e1abca, /// 0x580
			 0xc10eac99, /// 0x584
			 0x9f1ac376, /// 0x588
			 0xe014468d, /// 0x58c
			 0x2ea0ab07, /// 0x590
			 0x101f27b7, /// 0x594
			 0x724f6116, /// 0x598
			 0x669aa3fe, /// 0x59c
			 0x787c063b, /// 0x5a0
			 0x358a06ce, /// 0x5a4
			 0x4dc31b66, /// 0x5a8
			 0x3ba027ea, /// 0x5ac
			 0x5e77b228, /// 0x5b0
			 0xda531af9, /// 0x5b4
			 0xc019dfb7, /// 0x5b8
			 0x00b557dd, /// 0x5bc
			 0x96149438, /// 0x5c0
			 0x525f2446, /// 0x5c4
			 0x06bcc929, /// 0x5c8
			 0xe6595531, /// 0x5cc
			 0x1e9d1a38, /// 0x5d0
			 0xcc0d660b, /// 0x5d4
			 0x8cfd5c18, /// 0x5d8
			 0xa01a45d4, /// 0x5dc
			 0x742d691d, /// 0x5e0
			 0x416e62eb, /// 0x5e4
			 0x1d9e8397, /// 0x5e8
			 0xb7c87232, /// 0x5ec
			 0x6e3ce2e1, /// 0x5f0
			 0xe6ccacf6, /// 0x5f4
			 0x7c65966f, /// 0x5f8
			 0x3a3581fe, /// 0x5fc
			 0xc5322f39, /// 0x600
			 0x25ad14fc, /// 0x604
			 0xa8b067f9, /// 0x608
			 0x56e1e2ac, /// 0x60c
			 0xa1751fe0, /// 0x610
			 0xc37cb9ab, /// 0x614
			 0x25f136c3, /// 0x618
			 0xe101f2eb, /// 0x61c
			 0xe99373a7, /// 0x620
			 0xb0a2f2d7, /// 0x624
			 0x7831092a, /// 0x628
			 0x7c04a0b0, /// 0x62c
			 0xd15b010a, /// 0x630
			 0x6a598c62, /// 0x634
			 0x98faa06c, /// 0x638
			 0x7abe24bb, /// 0x63c
			 0xbd71c74b, /// 0x640
			 0x8cd60409, /// 0x644
			 0xd9d5e529, /// 0x648
			 0x0de474ee, /// 0x64c
			 0xe7020386, /// 0x650
			 0xc39fea45, /// 0x654
			 0x50e4be07, /// 0x658
			 0xcb9cfef0, /// 0x65c
			 0x12d639c3, /// 0x660
			 0xf322dbf1, /// 0x664
			 0x3038576f, /// 0x668
			 0x78a33452, /// 0x66c
			 0x481d63c6, /// 0x670
			 0x7d536ae6, /// 0x674
			 0x5ce4babd, /// 0x678
			 0x1bc4266a, /// 0x67c
			 0x7d13d1e3, /// 0x680
			 0xc85fa3a0, /// 0x684
			 0xea0d2134, /// 0x688
			 0x54dd1652, /// 0x68c
			 0x64ebe189, /// 0x690
			 0xc07c9e34, /// 0x694
			 0x0d73dec6, /// 0x698
			 0xff392b44, /// 0x69c
			 0x18c6899c, /// 0x6a0
			 0xcaf713ee, /// 0x6a4
			 0xeda1e51b, /// 0x6a8
			 0x7025e2e3, /// 0x6ac
			 0xf7cde920, /// 0x6b0
			 0x63acc7c4, /// 0x6b4
			 0xe8de0847, /// 0x6b8
			 0x44c2c7ee, /// 0x6bc
			 0x294075e9, /// 0x6c0
			 0xab3a28f7, /// 0x6c4
			 0xdbc1f5bb, /// 0x6c8
			 0x79e87d3e, /// 0x6cc
			 0xb6849ce8, /// 0x6d0
			 0x2eec7744, /// 0x6d4
			 0x5c4ace44, /// 0x6d8
			 0xae13c995, /// 0x6dc
			 0x2613b676, /// 0x6e0
			 0x4ee7c910, /// 0x6e4
			 0x2a12133e, /// 0x6e8
			 0xe32b7a38, /// 0x6ec
			 0x0cd148ed, /// 0x6f0
			 0x914b0859, /// 0x6f4
			 0x471f0a16, /// 0x6f8
			 0x5cc97032, /// 0x6fc
			 0x7fea642f, /// 0x700
			 0xee4fec6c, /// 0x704
			 0x2c271236, /// 0x708
			 0x5a14eab8, /// 0x70c
			 0x788227ba, /// 0x710
			 0x6aa7f6ce, /// 0x714
			 0xeabee5ea, /// 0x718
			 0xfa6ae971, /// 0x71c
			 0x4e523955, /// 0x720
			 0xf5acd87b, /// 0x724
			 0x383eaed7, /// 0x728
			 0x59d8025e, /// 0x72c
			 0x1f10f00f, /// 0x730
			 0x4aab2438, /// 0x734
			 0xde4617e9, /// 0x738
			 0x93aa644c, /// 0x73c
			 0xe12d99c9, /// 0x740
			 0xb5a563f3, /// 0x744
			 0x2b9787c9, /// 0x748
			 0xe2eca4d3, /// 0x74c
			 0x9dfd93c7, /// 0x750
			 0x5b1db434, /// 0x754
			 0x5b508ab2, /// 0x758
			 0xbb902e71, /// 0x75c
			 0x4bcc5a63, /// 0x760
			 0x06ab4095, /// 0x764
			 0xfcbec255, /// 0x768
			 0x10872a47, /// 0x76c
			 0xd4f4a4c1, /// 0x770
			 0x733306ba, /// 0x774
			 0x31aebe3b, /// 0x778
			 0xa81e1986, /// 0x77c
			 0x397ccaae, /// 0x780
			 0xa1cae47b, /// 0x784
			 0x3521c1f8, /// 0x788
			 0xa0c83a9e, /// 0x78c
			 0x08c1eb3d, /// 0x790
			 0x06925b0b, /// 0x794
			 0xbf1c4d50, /// 0x798
			 0x3763dd9e, /// 0x79c
			 0x6d3e31b7, /// 0x7a0
			 0x8272bdac, /// 0x7a4
			 0x9b2d9f0c, /// 0x7a8
			 0x88ada925, /// 0x7ac
			 0x088cb946, /// 0x7b0
			 0xe52e721b, /// 0x7b4
			 0xe6421195, /// 0x7b8
			 0xa3c8a0de, /// 0x7bc
			 0xda4ab6d3, /// 0x7c0
			 0x8787b523, /// 0x7c4
			 0xfdd950b6, /// 0x7c8
			 0x6fd59bb9, /// 0x7cc
			 0xe289d2a9, /// 0x7d0
			 0xace93ac1, /// 0x7d4
			 0x5e20a3a6, /// 0x7d8
			 0x331d5f6e, /// 0x7dc
			 0x74a41ba0, /// 0x7e0
			 0x2cfcbf44, /// 0x7e4
			 0x5931c8c5, /// 0x7e8
			 0x0c23046f, /// 0x7ec
			 0x4cbb3a41, /// 0x7f0
			 0x28077476, /// 0x7f4
			 0xdef9e80a, /// 0x7f8
			 0x98526682, /// 0x7fc
			 0xac34a7d2, /// 0x800
			 0xc89093dc, /// 0x804
			 0xb2cde93f, /// 0x808
			 0xe0b6c7d9, /// 0x80c
			 0x26f0e67f, /// 0x810
			 0xd22f52d2, /// 0x814
			 0xdbf0f8c0, /// 0x818
			 0x0c12d9c9, /// 0x81c
			 0x68cc17fd, /// 0x820
			 0x9025654f, /// 0x824
			 0xf361e7dc, /// 0x828
			 0x44d37d54, /// 0x82c
			 0x8979fed5, /// 0x830
			 0x9e50503b, /// 0x834
			 0x316ee934, /// 0x838
			 0xc86805fc, /// 0x83c
			 0x6f057541, /// 0x840
			 0x649cddc5, /// 0x844
			 0x4a32c380, /// 0x848
			 0x13733bd1, /// 0x84c
			 0xbb70fae7, /// 0x850
			 0x7f7bacc9, /// 0x854
			 0x0b179844, /// 0x858
			 0xf93eeb12, /// 0x85c
			 0xa1c989ae, /// 0x860
			 0x47f5a423, /// 0x864
			 0x3df6323f, /// 0x868
			 0x22d4591f, /// 0x86c
			 0x0c857846, /// 0x870
			 0xba68d582, /// 0x874
			 0x07492a4b, /// 0x878
			 0x90d147b4, /// 0x87c
			 0x25a49b8e, /// 0x880
			 0x5538445e, /// 0x884
			 0x5e1ece53, /// 0x888
			 0x67d09313, /// 0x88c
			 0xa28ba89d, /// 0x890
			 0xdd17d448, /// 0x894
			 0x129c89e1, /// 0x898
			 0x436d2f72, /// 0x89c
			 0xad664c85, /// 0x8a0
			 0xca4b4175, /// 0x8a4
			 0x19575ab3, /// 0x8a8
			 0x0bf56bca, /// 0x8ac
			 0x2d7aa15e, /// 0x8b0
			 0xc0346424, /// 0x8b4
			 0x36baa5e8, /// 0x8b8
			 0x174b937a, /// 0x8bc
			 0x5a418573, /// 0x8c0
			 0x259314c3, /// 0x8c4
			 0xb3c20cdb, /// 0x8c8
			 0x16a5c0d0, /// 0x8cc
			 0xbbc9e78d, /// 0x8d0
			 0x661dce9c, /// 0x8d4
			 0x186203bc, /// 0x8d8
			 0x77795f79, /// 0x8dc
			 0xa85b057e, /// 0x8e0
			 0x1905a109, /// 0x8e4
			 0x9ab80e57, /// 0x8e8
			 0xfaab5059, /// 0x8ec
			 0x21a41a7f, /// 0x8f0
			 0x70f166dc, /// 0x8f4
			 0xbab151c8, /// 0x8f8
			 0x2b7d43d5, /// 0x8fc
			 0xcd3662e4, /// 0x900
			 0x22d47069, /// 0x904
			 0xba8e0d6a, /// 0x908
			 0xfa4a8fbd, /// 0x90c
			 0x616c587e, /// 0x910
			 0x82b571b9, /// 0x914
			 0xdbe25cc8, /// 0x918
			 0x1cb8e513, /// 0x91c
			 0x23bde204, /// 0x920
			 0x404a4bdc, /// 0x924
			 0x90e9fc49, /// 0x928
			 0xb6b81a41, /// 0x92c
			 0x142c4dbb, /// 0x930
			 0xa614a6df, /// 0x934
			 0xfc75e11c, /// 0x938
			 0x664ba106, /// 0x93c
			 0x27f90b04, /// 0x940
			 0xb80d555e, /// 0x944
			 0xd49ae073, /// 0x948
			 0xdb44072a, /// 0x94c
			 0x87d31a92, /// 0x950
			 0xc1b78314, /// 0x954
			 0x02f50acd, /// 0x958
			 0x436ab968, /// 0x95c
			 0x164d0f07, /// 0x960
			 0xdcb7513e, /// 0x964
			 0xb7dc7967, /// 0x968
			 0x16d06c7e, /// 0x96c
			 0xd9a14159, /// 0x970
			 0x4b1e9a6b, /// 0x974
			 0x11f21873, /// 0x978
			 0xd86846d3, /// 0x97c
			 0x3cd91e42, /// 0x980
			 0x9fe2f6e6, /// 0x984
			 0xb72d9001, /// 0x988
			 0xd4adc62f, /// 0x98c
			 0xd1f4d478, /// 0x990
			 0x74ef1401, /// 0x994
			 0x18336f8c, /// 0x998
			 0x501b3033, /// 0x99c
			 0x5ef82aba, /// 0x9a0
			 0x8bcb53e6, /// 0x9a4
			 0xcfcbcc08, /// 0x9a8
			 0x7b9f2d85, /// 0x9ac
			 0xd70137e2, /// 0x9b0
			 0x386ea043, /// 0x9b4
			 0x006d2dfb, /// 0x9b8
			 0x748b1343, /// 0x9bc
			 0xad3aeb2a, /// 0x9c0
			 0x5cfa3623, /// 0x9c4
			 0xdbf36560, /// 0x9c8
			 0x67b6d9fe, /// 0x9cc
			 0x5d3d18a3, /// 0x9d0
			 0xa97b7f66, /// 0x9d4
			 0x83f1a4fc, /// 0x9d8
			 0x6ad5c30d, /// 0x9dc
			 0x3e8f6535, /// 0x9e0
			 0x8deea4a8, /// 0x9e4
			 0xff82681b, /// 0x9e8
			 0xf8e70666, /// 0x9ec
			 0x16e3c29a, /// 0x9f0
			 0x71866b99, /// 0x9f4
			 0xbd4d533b, /// 0x9f8
			 0x405edb77, /// 0x9fc
			 0x775e29e1, /// 0xa00
			 0x5f7d2e33, /// 0xa04
			 0x09cf9784, /// 0xa08
			 0x82c1da69, /// 0xa0c
			 0x863dfb9b, /// 0xa10
			 0xad23d217, /// 0xa14
			 0xc23cc598, /// 0xa18
			 0xcabeeb89, /// 0xa1c
			 0xe4e0a056, /// 0xa20
			 0x2677fe9b, /// 0xa24
			 0xf40109ce, /// 0xa28
			 0x3e2b04ae, /// 0xa2c
			 0x617e9c8a, /// 0xa30
			 0xa0c13c45, /// 0xa34
			 0x80cae17f, /// 0xa38
			 0x853e9de6, /// 0xa3c
			 0xbcf8afed, /// 0xa40
			 0x56094611, /// 0xa44
			 0xd5de9615, /// 0xa48
			 0x68b6bb3a, /// 0xa4c
			 0x93fe43cd, /// 0xa50
			 0x4fe3dded, /// 0xa54
			 0x2c26b570, /// 0xa58
			 0xb7dccfb7, /// 0xa5c
			 0xccb73e4f, /// 0xa60
			 0x85e02b30, /// 0xa64
			 0xfe2742d9, /// 0xa68
			 0x9e64db36, /// 0xa6c
			 0x6d692799, /// 0xa70
			 0xdc667e2c, /// 0xa74
			 0x98e052aa, /// 0xa78
			 0x875ab54f, /// 0xa7c
			 0x31d3f763, /// 0xa80
			 0x34f5c941, /// 0xa84
			 0x59420788, /// 0xa88
			 0x5afa81c5, /// 0xa8c
			 0x54ca60a0, /// 0xa90
			 0xd8bebad0, /// 0xa94
			 0x9c119320, /// 0xa98
			 0xead902aa, /// 0xa9c
			 0x5bbbb237, /// 0xaa0
			 0x3a829faa, /// 0xaa4
			 0xbec73c56, /// 0xaa8
			 0xe962fa1e, /// 0xaac
			 0xc814f401, /// 0xab0
			 0x6dc3879b, /// 0xab4
			 0x09ee2028, /// 0xab8
			 0x04535360, /// 0xabc
			 0x9fc3bd9d, /// 0xac0
			 0xde7bd169, /// 0xac4
			 0x3b21890e, /// 0xac8
			 0xd62c6051, /// 0xacc
			 0x8b324960, /// 0xad0
			 0x33101aef, /// 0xad4
			 0xea521ea5, /// 0xad8
			 0xe83cac9e, /// 0xadc
			 0x175f7965, /// 0xae0
			 0xb02bd933, /// 0xae4
			 0x03b0baca, /// 0xae8
			 0xae876743, /// 0xaec
			 0xfc639b38, /// 0xaf0
			 0x2bfaa295, /// 0xaf4
			 0x9ea52b0e, /// 0xaf8
			 0x011312ab, /// 0xafc
			 0x251dadab, /// 0xb00
			 0x1c590568, /// 0xb04
			 0xa47f7ad3, /// 0xb08
			 0x09b294ca, /// 0xb0c
			 0xd73c2059, /// 0xb10
			 0x7f2ae4e8, /// 0xb14
			 0x8e20a504, /// 0xb18
			 0xb232df80, /// 0xb1c
			 0xea279011, /// 0xb20
			 0x2ef4ddf6, /// 0xb24
			 0xb08bd9d7, /// 0xb28
			 0x4a9dfc7c, /// 0xb2c
			 0x070fc608, /// 0xb30
			 0x5d9f2b4e, /// 0xb34
			 0xa7d42919, /// 0xb38
			 0x158eb3e7, /// 0xb3c
			 0x20e3a64c, /// 0xb40
			 0x24e4f2e6, /// 0xb44
			 0x3cfb5cc2, /// 0xb48
			 0x813e5337, /// 0xb4c
			 0x026b628b, /// 0xb50
			 0x1391090b, /// 0xb54
			 0x3fd5fb47, /// 0xb58
			 0x3f28a3f9, /// 0xb5c
			 0x6e1cce89, /// 0xb60
			 0x742ebb16, /// 0xb64
			 0x21795f5e, /// 0xb68
			 0xb76cbd65, /// 0xb6c
			 0x33a34ee0, /// 0xb70
			 0x089c3601, /// 0xb74
			 0xcf38e910, /// 0xb78
			 0xbb129bbb, /// 0xb7c
			 0x78316ff7, /// 0xb80
			 0x1a3d3fe3, /// 0xb84
			 0xcb1d401f, /// 0xb88
			 0x72e44fca, /// 0xb8c
			 0x0c6f8159, /// 0xb90
			 0x19d01d31, /// 0xb94
			 0x677cb334, /// 0xb98
			 0x8f8eb6cb, /// 0xb9c
			 0x52369c37, /// 0xba0
			 0xf14c450a, /// 0xba4
			 0xebae3104, /// 0xba8
			 0xe3410ab4, /// 0xbac
			 0x29ee3af6, /// 0xbb0
			 0xc73791a1, /// 0xbb4
			 0x56126946, /// 0xbb8
			 0x4e35aa9a, /// 0xbbc
			 0x30fb96d1, /// 0xbc0
			 0xad8347d9, /// 0xbc4
			 0xf9b8dc98, /// 0xbc8
			 0x06cc1850, /// 0xbcc
			 0x965bf2a8, /// 0xbd0
			 0x29862984, /// 0xbd4
			 0xa0660c9c, /// 0xbd8
			 0x447df148, /// 0xbdc
			 0x25e0b12f, /// 0xbe0
			 0x78daa9eb, /// 0xbe4
			 0x538b0b35, /// 0xbe8
			 0xd4a5cd63, /// 0xbec
			 0xe1a9d1b4, /// 0xbf0
			 0xa09e7006, /// 0xbf4
			 0x88735692, /// 0xbf8
			 0x5b5b0443, /// 0xbfc
			 0xa6105e83, /// 0xc00
			 0x999d8bf7, /// 0xc04
			 0xbe5426cf, /// 0xc08
			 0x167cc865, /// 0xc0c
			 0xaf10f7d1, /// 0xc10
			 0x27692377, /// 0xc14
			 0x1e98c7aa, /// 0xc18
			 0x73c4d154, /// 0xc1c
			 0xfe1c8288, /// 0xc20
			 0xce18b278, /// 0xc24
			 0xe5d0350f, /// 0xc28
			 0x9b8f2718, /// 0xc2c
			 0x19de358f, /// 0xc30
			 0xdc8bf374, /// 0xc34
			 0xe7e9ce4b, /// 0xc38
			 0x56fe6f04, /// 0xc3c
			 0x112ab01a, /// 0xc40
			 0xfacad3f4, /// 0xc44
			 0x65adf492, /// 0xc48
			 0x867f44e7, /// 0xc4c
			 0xcad246da, /// 0xc50
			 0x3f190028, /// 0xc54
			 0x0a652bcc, /// 0xc58
			 0x19ddb266, /// 0xc5c
			 0x5acfb36f, /// 0xc60
			 0xc6e0084c, /// 0xc64
			 0xa3a842eb, /// 0xc68
			 0x665c774d, /// 0xc6c
			 0xf2f31a5f, /// 0xc70
			 0x92f13d85, /// 0xc74
			 0x627e7193, /// 0xc78
			 0x8b851274, /// 0xc7c
			 0xd761e553, /// 0xc80
			 0x8a349b75, /// 0xc84
			 0x4cadc479, /// 0xc88
			 0xaac946dc, /// 0xc8c
			 0x87468ad9, /// 0xc90
			 0x83934efe, /// 0xc94
			 0xacf3a0fa, /// 0xc98
			 0x1fe36399, /// 0xc9c
			 0x300daff1, /// 0xca0
			 0x1eb2ed8e, /// 0xca4
			 0xf419ca0e, /// 0xca8
			 0xb66af04f, /// 0xcac
			 0x36869225, /// 0xcb0
			 0xc636d6b4, /// 0xcb4
			 0xe7600379, /// 0xcb8
			 0x4d05810f, /// 0xcbc
			 0x8549852b, /// 0xcc0
			 0x53a27895, /// 0xcc4
			 0xfd022c8d, /// 0xcc8
			 0x7ff025bf, /// 0xccc
			 0xfce482fb, /// 0xcd0
			 0xa4aba0c2, /// 0xcd4
			 0xee6c4fb3, /// 0xcd8
			 0x42ade6dc, /// 0xcdc
			 0x1b8bc97f, /// 0xce0
			 0xf037d100, /// 0xce4
			 0xac273364, /// 0xce8
			 0xc39df0f4, /// 0xcec
			 0xfd431978, /// 0xcf0
			 0x542de916, /// 0xcf4
			 0x3b2e99da, /// 0xcf8
			 0x9cfeb63d, /// 0xcfc
			 0xaddd7885, /// 0xd00
			 0x899af6cd, /// 0xd04
			 0xfb392b59, /// 0xd08
			 0x8ec0c2ed, /// 0xd0c
			 0x41e089af, /// 0xd10
			 0x61112ccc, /// 0xd14
			 0x0b7a5bdf, /// 0xd18
			 0x8b5a2d51, /// 0xd1c
			 0x3bde93b1, /// 0xd20
			 0xc1db8b56, /// 0xd24
			 0x82ce6935, /// 0xd28
			 0x3ce123a0, /// 0xd2c
			 0xd0c68ce5, /// 0xd30
			 0xbe844bf6, /// 0xd34
			 0x627ca484, /// 0xd38
			 0xa97bb6c5, /// 0xd3c
			 0x398bc950, /// 0xd40
			 0x296528ab, /// 0xd44
			 0x91583a3a, /// 0xd48
			 0x76563358, /// 0xd4c
			 0x2c2c4436, /// 0xd50
			 0xb90f49b1, /// 0xd54
			 0xc546c464, /// 0xd58
			 0x3788f5d4, /// 0xd5c
			 0xd9743c7f, /// 0xd60
			 0x8faca6eb, /// 0xd64
			 0x9939492f, /// 0xd68
			 0x05b4780a, /// 0xd6c
			 0x3032c99f, /// 0xd70
			 0xfa1bb8c2, /// 0xd74
			 0x5d0662c1, /// 0xd78
			 0xdad6fcac, /// 0xd7c
			 0x16b0ee1b, /// 0xd80
			 0x7183f2cb, /// 0xd84
			 0xa95a0aff, /// 0xd88
			 0x4510f3f9, /// 0xd8c
			 0x81ea6bba, /// 0xd90
			 0xcd4b02fd, /// 0xd94
			 0xadea9d4d, /// 0xd98
			 0x6ee31a33, /// 0xd9c
			 0x2051cc68, /// 0xda0
			 0xfdaa40b8, /// 0xda4
			 0x32c9e415, /// 0xda8
			 0x720b329c, /// 0xdac
			 0x1c38b202, /// 0xdb0
			 0x9f1579a0, /// 0xdb4
			 0x85616e6a, /// 0xdb8
			 0xe470112f, /// 0xdbc
			 0x2a276024, /// 0xdc0
			 0x502866ab, /// 0xdc4
			 0x9d09d97d, /// 0xdc8
			 0x1c7b0349, /// 0xdcc
			 0xa198976b, /// 0xdd0
			 0x01ad52b4, /// 0xdd4
			 0xf1f5d7b8, /// 0xdd8
			 0x0cdd4a0c, /// 0xddc
			 0x2ea54d19, /// 0xde0
			 0x16ce5f59, /// 0xde4
			 0x28690991, /// 0xde8
			 0xe21bd3aa, /// 0xdec
			 0x3fdddb6e, /// 0xdf0
			 0xcd70a4c0, /// 0xdf4
			 0x042a2189, /// 0xdf8
			 0x08639a70, /// 0xdfc
			 0x956a0a74, /// 0xe00
			 0x0dd8af4c, /// 0xe04
			 0x1bf3f8ab, /// 0xe08
			 0x216e3516, /// 0xe0c
			 0x7cc54e7b, /// 0xe10
			 0x9de52a2f, /// 0xe14
			 0x815a9191, /// 0xe18
			 0x9a5d675f, /// 0xe1c
			 0x0da34c29, /// 0xe20
			 0xf6c10ac7, /// 0xe24
			 0xfeeba976, /// 0xe28
			 0xa511c675, /// 0xe2c
			 0x2a7a4fcc, /// 0xe30
			 0x1d98a503, /// 0xe34
			 0xf377edd4, /// 0xe38
			 0x02ec7b62, /// 0xe3c
			 0x401d4efe, /// 0xe40
			 0x91d593c2, /// 0xe44
			 0x55f5570c, /// 0xe48
			 0xb0468647, /// 0xe4c
			 0x1694c88f, /// 0xe50
			 0x372ac5f5, /// 0xe54
			 0x304ae544, /// 0xe58
			 0x84a5ed86, /// 0xe5c
			 0xa2985e01, /// 0xe60
			 0xcfd81b06, /// 0xe64
			 0x52dba761, /// 0xe68
			 0x4fe203af, /// 0xe6c
			 0x191b63e6, /// 0xe70
			 0x8e09d803, /// 0xe74
			 0xe74318a5, /// 0xe78
			 0xa0ba4603, /// 0xe7c
			 0x87363c7e, /// 0xe80
			 0x66024850, /// 0xe84
			 0x49c2c17c, /// 0xe88
			 0x4e6a7071, /// 0xe8c
			 0x7e75f183, /// 0xe90
			 0xb14ed126, /// 0xe94
			 0x25650d4b, /// 0xe98
			 0x8e66a1e2, /// 0xe9c
			 0x584fc680, /// 0xea0
			 0xdd138846, /// 0xea4
			 0x4f4073a0, /// 0xea8
			 0xf80ab8b6, /// 0xeac
			 0x7c473082, /// 0xeb0
			 0xa90da6e1, /// 0xeb4
			 0xd2673dde, /// 0xeb8
			 0x3af131e0, /// 0xebc
			 0x2ef18d65, /// 0xec0
			 0xb344f4af, /// 0xec4
			 0x2b35d43b, /// 0xec8
			 0xd676042b, /// 0xecc
			 0x58212129, /// 0xed0
			 0x7e81020c, /// 0xed4
			 0x5aae42ac, /// 0xed8
			 0xa2096173, /// 0xedc
			 0x1a2daa68, /// 0xee0
			 0x1cb925a5, /// 0xee4
			 0x89142a67, /// 0xee8
			 0x05de07d6, /// 0xeec
			 0xc28e65d8, /// 0xef0
			 0xdc6ad704, /// 0xef4
			 0x149a92f4, /// 0xef8
			 0xd03f8b4d, /// 0xefc
			 0xf9d5010c, /// 0xf00
			 0xe4080568, /// 0xf04
			 0x6222baf3, /// 0xf08
			 0x0fe7858e, /// 0xf0c
			 0xf94ea8a2, /// 0xf10
			 0x64594fa7, /// 0xf14
			 0x39e50e5b, /// 0xf18
			 0x61479372, /// 0xf1c
			 0xa91f763d, /// 0xf20
			 0xce27bbc2, /// 0xf24
			 0x3cc7505d, /// 0xf28
			 0xb316ab65, /// 0xf2c
			 0x3ddd04cf, /// 0xf30
			 0xcf345e1d, /// 0xf34
			 0x240d8907, /// 0xf38
			 0x4c4ee311, /// 0xf3c
			 0xea7eddca, /// 0xf40
			 0x32236c7b, /// 0xf44
			 0x2408dbf1, /// 0xf48
			 0x6c9632d8, /// 0xf4c
			 0x96db5642, /// 0xf50
			 0x30e3f40a, /// 0xf54
			 0xee753221, /// 0xf58
			 0xa9355550, /// 0xf5c
			 0x3cdeeefe, /// 0xf60
			 0x75be1cce, /// 0xf64
			 0x375aefba, /// 0xf68
			 0x2d649e44, /// 0xf6c
			 0xe7bf1df4, /// 0xf70
			 0xb6ccc615, /// 0xf74
			 0x3bcf3e2e, /// 0xf78
			 0x84da1a2a, /// 0xf7c
			 0x2e9d8a63, /// 0xf80
			 0xbfea4ea9, /// 0xf84
			 0xa8f17b6a, /// 0xf88
			 0xa63e6bc8, /// 0xf8c
			 0x0b6bbfeb, /// 0xf90
			 0x156f8e81, /// 0xf94
			 0xcf31bdb6, /// 0xf98
			 0x85cb95cf, /// 0xf9c
			 0x6027327a, /// 0xfa0
			 0x74dfa3f6, /// 0xfa4
			 0x8b384bd9, /// 0xfa8
			 0x8d467cdc, /// 0xfac
			 0x7f942c56, /// 0xfb0
			 0xda605bc8, /// 0xfb4
			 0x7e8cc229, /// 0xfb8
			 0x2f6c4bb8, /// 0xfbc
			 0xbe7088e1, /// 0xfc0
			 0x07f6bbd2, /// 0xfc4
			 0x7b9ada3d, /// 0xfc8
			 0x78982add, /// 0xfcc
			 0xa6ec2111, /// 0xfd0
			 0x78d942f5, /// 0xfd4
			 0xf812b0d8, /// 0xfd8
			 0x263c0569, /// 0xfdc
			 0x5e2cf00a, /// 0xfe0
			 0x2668b633, /// 0xfe4
			 0x1b9e3916, /// 0xfe8
			 0xb4d79d62, /// 0xfec
			 0xa20624bd, /// 0xff0
			 0x6daac1e6, /// 0xff4
			 0xbcde98e5, /// 0xff8
			 0xb6c771d2 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00000
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00004
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00008
			 0xbf028f5c,                                                  // -0.51                                       /// 0000c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00010
			 0x0e000001,                                                  // Trailing 1s:                                /// 00014
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00018
			 0x80000000,                                                  // -zero                                       /// 0001c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00020
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00024
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00028
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0002c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00030
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00038
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0003c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00040
			 0x0c700000,                                                  // Leading 1s:                                 /// 00044
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00048
			 0x0e000001,                                                  // Trailing 1s:                                /// 0004c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00050
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00054
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00058
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0005c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00060
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00064
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00068
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0006c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00070
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00074
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00078
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0007c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00080
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00088
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0008c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00090
			 0x7f800000,                                                  // inf                                         /// 00094
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0009c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 000a0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000a8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000ac
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 000b0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000b4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 000b8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000bc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000c0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 000c4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000cc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 000d0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 000d8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000dc
			 0x7f800000,                                                  // inf                                         /// 000e0
			 0x00000000,                                                  // zero                                        /// 000e4
			 0x33333333,                                                  // 4 random values                             /// 000e8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000ec
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000f0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 000f4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000f8
			 0x55555555,                                                  // 4 random values                             /// 000fc
			 0x33333333,                                                  // 4 random values                             /// 00100
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00104
			 0x3f028f5c,                                                  // 0.51                                        /// 00108
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0010c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00110
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00114
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00118
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0011c
			 0xbf028f5c,                                                  // -0.51                                       /// 00120
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00124
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00128
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0012c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00130
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00134
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00138
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0013c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00140
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00144
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00148
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0014c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00160
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00164
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00168
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0016c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00170
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00174
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00178
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0017c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00184
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00188
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0x7f800000,                                                  // inf                                         /// 00190
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00194
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00198
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0019c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 001ac
			 0xcb000000,                                                  // -8388608.0                                  /// 001b0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001b4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001b8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x00000000,                                                  // zero                                        /// 001c0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001c4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 001c8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 001cc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001d0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001d4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001d8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001dc
			 0x0e000007,                                                  // Trailing 1s:                                /// 001e0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 001e4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 001e8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001ec
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 001f4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 001f8
			 0x0e000007,                                                  // Trailing 1s:                                /// 001fc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00200
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00204
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00208
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0020c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00210
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00214
			 0x7fc00001,                                                  // signaling NaN                               /// 00218
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0021c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00220
			 0x7f800000,                                                  // inf                                         /// 00224
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00228
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0022c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00230
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00234
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00238
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0023c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00240
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00248
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0024c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00250
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00254
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00258
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0025c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00260
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00264
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00268
			 0xff800000,                                                  // -inf                                        /// 0026c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00270
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00274
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00278
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00280
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00284
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00288
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0028c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00290
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00294
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00298
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0029c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 002a4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 002a8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 002ac
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002b0
			 0x7fc00001,                                                  // signaling NaN                               /// 002b4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 002b8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002bc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002c4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 002c8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002cc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 002d0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002d4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 002d8
			 0x7f800000,                                                  // inf                                         /// 002dc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 002e0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002e4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 002e8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 002ec
			 0xcb000000,                                                  // -8388608.0                                  /// 002f0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002f4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 002fc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00304
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00308
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00310
			 0x3f028f5c,                                                  // 0.51                                        /// 00314
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00318
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0031c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00320
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00328
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0032c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00330
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00334
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0033c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00340
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00344
			 0x7f800000,                                                  // inf                                         /// 00348
			 0x80000000,                                                  // -zero                                       /// 0034c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00350
			 0x0c600000,                                                  // Leading 1s:                                 /// 00354
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00358
			 0x80011111,                                                  // -9.7958E-41                                 /// 0035c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00360
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00368
			 0xcb000000,                                                  // -8388608.0                                  /// 0036c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00370
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00374
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00378
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0037c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00384
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00388
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0038c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00390
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00398
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003a0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003a4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 003a8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 003ac
			 0x00000000,                                                  // zero                                        /// 003b0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0xcb000000,                                                  // -8388608.0                                  /// 003b8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 003bc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003c0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003c8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 003d0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003d4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003d8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 003e4
			 0x0c700000,                                                  // Leading 1s:                                 /// 003e8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003ec
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003f0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 003f4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003fc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00400
			 0x0c400000,                                                  // Leading 1s:                                 /// 00404
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00408
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0040c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00410
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00414
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00418
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0041c
			 0x00000000,                                                  // zero                                        /// 00420
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00424
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0042c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00430
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00434
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00438
			 0x0c700000,                                                  // Leading 1s:                                 /// 0043c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00444
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00448
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0044c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00450
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00454
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00458
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0045c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00460
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00468
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00470
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00474
			 0xffc00001,                                                  // -signaling NaN                              /// 00478
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0047c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00488
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0048c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00490
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00494
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00498
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0049c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004a0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a4
			 0x7fc00001,                                                  // signaling NaN                               /// 004a8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004ac
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004b0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004bc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004c0
			 0x7fc00001,                                                  // signaling NaN                               /// 004c4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004c8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004cc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 004d4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 004d8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 004e4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 004e8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004ec
			 0xffc00001,                                                  // -signaling NaN                              /// 004f0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f4
			 0x0c600000,                                                  // Leading 1s:                                 /// 004f8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 004fc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00500
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00504
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00508
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0050c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00510
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00514
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0x80011111,                                                  // -9.7958E-41                                 /// 0051c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00520
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00524
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00528
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0052c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00530
			 0x0c400000,                                                  // Leading 1s:                                 /// 00534
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0x55555555,                                                  // 4 random values                             /// 0053c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00540
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0054c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00550
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00554
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00558
			 0x0e000003,                                                  // Trailing 1s:                                /// 0055c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00560
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00564
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00568
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0056c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00570
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00574
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00578
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0057c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00580
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00584
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00588
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0058c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00590
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00594
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00598
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0059c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 005a0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 005a4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 005a8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005ac
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 005b0
			 0x3f028f5c,                                                  // 0.51                                        /// 005b4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 005b8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 005bc
			 0x00000000,                                                  // zero                                        /// 005c0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005c8
			 0x55555555,                                                  // 4 random values                             /// 005cc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005d0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 005d4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005d8
			 0x0c600000,                                                  // Leading 1s:                                 /// 005dc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e0
			 0x80011111,                                                  // -9.7958E-41                                 /// 005e4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005e8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 005ec
			 0x33333333,                                                  // 4 random values                             /// 005f0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 005f8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005fc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00600
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0060c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00610
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00614
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00618
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0061c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00620
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00624
			 0xcb000000,                                                  // -8388608.0                                  /// 00628
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0062c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00630
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00634
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00638
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0063c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00640
			 0x3f028f5c,                                                  // 0.51                                        /// 00644
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00648
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0064c
			 0xbf028f5c,                                                  // -0.51                                       /// 00650
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00654
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00658
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0065c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00660
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00664
			 0x80011111,                                                  // -9.7958E-41                                 /// 00668
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0066c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00670
			 0x0e000001,                                                  // Trailing 1s:                                /// 00674
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00678
			 0xbf028f5c,                                                  // -0.51                                       /// 0067c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00680
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00688
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0068c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00690
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00694
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00698
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0069c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006a4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006a8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006ac
			 0x00011111,                                                  // 9.7958E-41                                  /// 006b0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 006b4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 006b8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 006bc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006c0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006c4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 006c8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006cc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 006d0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 006d4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006d8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006dc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 006e0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 006e4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006ec
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006f0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006f4
			 0x0e000001,                                                  // Trailing 1s:                                /// 006f8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006fc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00700
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00708
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0070c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00714
			 0x0e000007,                                                  // Trailing 1s:                                /// 00718
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0071c
			 0x55555555,                                                  // 4 random values                             /// 00720
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x3f028f5c,                                                  // 0.51                                        /// 00728
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0072c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00730
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00734
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0x3f028f5c,                                                  // 0.51                                        /// 0073c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00740
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00744
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00748
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0074c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00750
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00754
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00758
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0075c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00760
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00764
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00768
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00770
			 0x7f800000,                                                  // inf                                         /// 00774
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00778
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0077c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00780
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00784
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0078c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00790
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00794
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00798
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0079c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 007a0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007a4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 007a8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ac
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007b0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 007b4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 007bc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007c0
			 0x80011111,                                                  // -9.7958E-41                                 /// 007c4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007c8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007cc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 007d0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007d8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007dc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 007e0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007e4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007e8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007ec
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007f0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007f4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007f8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007fc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00800
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00804
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00808
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0080c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00814
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00818
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0081c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00820
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00824
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00828
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0082c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00830
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00834
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00840
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00844
			 0x0e000003,                                                  // Trailing 1s:                                /// 00848
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0084c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00850
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00854
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00858
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0085c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00860
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00868
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0086c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00870
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00874
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00878
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0087c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00880
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00884
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00888
			 0x0e000001,                                                  // Trailing 1s:                                /// 0088c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00890
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00894
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00898
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0089c
			 0xffc00001,                                                  // -signaling NaN                              /// 008a0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008a8
			 0x0e000007,                                                  // Trailing 1s:                                /// 008ac
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008b0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008b4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 008b8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008bc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008c4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0x80000000,                                                  // -zero                                       /// 008cc
			 0xffc00001,                                                  // -signaling NaN                              /// 008d0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008d4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008dc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008e0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008e4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 008e8
			 0x33333333,                                                  // 4 random values                             /// 008ec
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008f0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008f4
			 0x0c400000,                                                  // Leading 1s:                                 /// 008f8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008fc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00900
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00904
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00908
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0090c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00914
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00918
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0091c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00924
			 0x0e000001,                                                  // Trailing 1s:                                /// 00928
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0092c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00930
			 0x0e000001,                                                  // Trailing 1s:                                /// 00934
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00938
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0093c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00940
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00944
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00948
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0094c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00954
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00958
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0095c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00960
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00964
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00968
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0096c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00970
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0097c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00980
			 0x0c780000,                                                  // Leading 1s:                                 /// 00984
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00988
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0098c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00990
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00998
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0099c
			 0x00011111,                                                  // 9.7958E-41                                  /// 009a0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 009a4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 009a8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009ac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009b0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009b8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 009bc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 009c4
			 0x80011111,                                                  // -9.7958E-41                                 /// 009c8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009cc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009d0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009dc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009e0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009e8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009ec
			 0x0c780000,                                                  // Leading 1s:                                 /// 009f0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009f8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 009fc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a00
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a08
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a0c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a10
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a18
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a1c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a20
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a24
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a28
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a2c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a30
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a34
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a38
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a3c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a40
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a44
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a48
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a4c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a50
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a54
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a5c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a60
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a64
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a68
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a6c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a70
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a74
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a78
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a80
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a84
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a88
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a8c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a90
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a94
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a98
			 0xcb000000,                                                  // -8388608.0                                  /// 00a9c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00aa4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00aac
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ab4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ab8
			 0x33333333,                                                  // 4 random values                             /// 00abc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ac0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ac4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00acc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ad4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ad8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00adc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ae0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ae4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ae8
			 0x80000000,                                                  // -zero                                       /// 00aec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00af4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00af8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b00
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b04
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b08
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b0c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b10
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b14
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b1c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b20
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b28
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b2c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b30
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b34
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b38
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b3c
			 0xffc00001,                                                  // -signaling NaN                              /// 00b40
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b44
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b48
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b4c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b50
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b58
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b5c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b60
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b64
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b68
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b6c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b70
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b74
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b78
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b80
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b88
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b90
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b94
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b98
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b9c
			 0x3f028f5c,                                                  // 0.51                                        /// 00ba0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ba4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ba8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bac
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00bb0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00bb4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00bbc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc0
			 0x7fc00001,                                                  // signaling NaN                               /// 00bc4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00bc8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bcc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00bd0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0x7f800000,                                                  // inf                                         /// 00bdc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00be0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00be8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00bec
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00bf4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00bf8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00bfc
			 0x33333333,                                                  // 4 random values                             /// 00c00
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c04
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c08
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c10
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c18
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c1c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c20
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c28
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c2c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c30
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c34
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c38
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c3c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c40
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c44
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c48
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c4c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c50
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c54
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c58
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c5c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c60
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c64
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c68
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c6c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c70
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c74
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c78
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c84
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c88
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c8c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c90
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c94
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c98
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c9c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ca0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ca8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00cac
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00cb0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00cb4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00cbc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00cc0
			 0x7fc00001,                                                  // signaling NaN                               /// 00cc4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00cc8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ccc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00cd4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ce0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ce4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ce8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00cec
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00cf0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00cf4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d00
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d04
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d08
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d0c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d10
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d14
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d1c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d20
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d24
			 0x7f800000,                                                  // inf                                         /// 00d28
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d2c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d30
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d34
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d38
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d3c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d40
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d44
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d48
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d4c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d50
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d54
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d58
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d5c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d60
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d64
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d68
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d6c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d70
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d74
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d7c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d80
			 0x7fc00001,                                                  // signaling NaN                               /// 00d84
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d88
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d8c
			 0x80000000,                                                  // -zero                                       /// 00d90
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d94
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d98
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d9c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00da0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00da4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00dac
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00db0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00db8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00dbc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00dc0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00dc4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00dc8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00dcc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00dd0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00dd4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00dd8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ddc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00de0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00de4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00de8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00dec
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00df0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00df4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df8
			 0x3f028f5c,                                                  // 0.51                                        /// 00dfc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e00
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e04
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e08
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e0c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e10
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e14
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e18
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e1c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e20
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e28
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e2c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e38
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e3c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e40
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e44
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e48
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e4c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e50
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e54
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e58
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e5c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e60
			 0xff800000,                                                  // -inf                                        /// 00e64
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e68
			 0x33333333,                                                  // 4 random values                             /// 00e6c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e70
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e74
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e78
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e80
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e84
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e88
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e8c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e90
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e94
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e98
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e9c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ea0
			 0x80000000,                                                  // -zero                                       /// 00ea4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea8
			 0x00000000,                                                  // zero                                        /// 00eac
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00eb0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00eb4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00eb8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ebc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ec0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ec8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ecc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ed4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ed8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0x55555555,                                                  // 4 random values                             /// 00ee0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ee4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ee8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00eec
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ef8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f00
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f04
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f08
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f0c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f10
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f14
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f1c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f20
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f24
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f2c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f30
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f34
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f38
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f3c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f40
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f44
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f48
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f4c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f50
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f58
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f5c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f60
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f64
			 0x80000000,                                                  // -zero                                       /// 00f68
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f6c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f70
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f78
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f7c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f80
			 0x7f800000,                                                  // inf                                         /// 00f84
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f88
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f8c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f90
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f94
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f9c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fa0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00fa4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00fa8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00fac
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00fb0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00fb4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00fb8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00fc0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00fc8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fcc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00fd0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00fd4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00fe0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00fe4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00fe8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fec
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ff0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ff4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ff8
			 0x00001000                                                  // SP - 1 bit alone set in mantissa +ve        /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0xd7802dc4, /// 0x0
			 0x97a9142a, /// 0x4
			 0xc876ff74, /// 0x8
			 0x3ef5a132, /// 0xc
			 0xcc50f146, /// 0x10
			 0x8f9bede4, /// 0x14
			 0x21bc332f, /// 0x18
			 0xf2c3120b, /// 0x1c
			 0xcc18552f, /// 0x20
			 0xd5ae3947, /// 0x24
			 0x73a57da8, /// 0x28
			 0xda9a65ff, /// 0x2c
			 0x9e3661ab, /// 0x30
			 0xef2d3555, /// 0x34
			 0x6928f53d, /// 0x38
			 0x12515933, /// 0x3c
			 0xbeca93ce, /// 0x40
			 0x116b7fc0, /// 0x44
			 0xb1cb3d0e, /// 0x48
			 0x82b4cfe8, /// 0x4c
			 0x97829cfe, /// 0x50
			 0x4606c1de, /// 0x54
			 0x6996423d, /// 0x58
			 0x03585e0d, /// 0x5c
			 0xbe315e24, /// 0x60
			 0xefedc845, /// 0x64
			 0x9dba2721, /// 0x68
			 0xbd7c4245, /// 0x6c
			 0x5d8337fc, /// 0x70
			 0xe3f65925, /// 0x74
			 0x1dacc1d3, /// 0x78
			 0xbf737551, /// 0x7c
			 0xa3a1203e, /// 0x80
			 0x706a799e, /// 0x84
			 0x279238ac, /// 0x88
			 0xe46e898a, /// 0x8c
			 0x6e523e69, /// 0x90
			 0x82c45367, /// 0x94
			 0x57efadcd, /// 0x98
			 0x15dfba1f, /// 0x9c
			 0x9eade4f8, /// 0xa0
			 0xa275c97b, /// 0xa4
			 0x9249383d, /// 0xa8
			 0xb186f480, /// 0xac
			 0x03cd55ce, /// 0xb0
			 0xf6589d97, /// 0xb4
			 0x606c6db7, /// 0xb8
			 0x2a31e755, /// 0xbc
			 0x67fc95e9, /// 0xc0
			 0x40c2f0a4, /// 0xc4
			 0x53f57ae6, /// 0xc8
			 0xa833b44b, /// 0xcc
			 0xf3a92ef8, /// 0xd0
			 0xb581e8bb, /// 0xd4
			 0x298d5a5f, /// 0xd8
			 0xffb1c94d, /// 0xdc
			 0x7276b02b, /// 0xe0
			 0x394fba1a, /// 0xe4
			 0x14186df5, /// 0xe8
			 0x5cd232c8, /// 0xec
			 0x6ce21bf9, /// 0xf0
			 0xb2abe921, /// 0xf4
			 0x5526d8d0, /// 0xf8
			 0xbcfe63af, /// 0xfc
			 0xe870eda3, /// 0x100
			 0x35d8f358, /// 0x104
			 0x98377f45, /// 0x108
			 0xcd1b5d2a, /// 0x10c
			 0xda020f2d, /// 0x110
			 0x887511fd, /// 0x114
			 0x206dcf84, /// 0x118
			 0x481643de, /// 0x11c
			 0xc91edbf5, /// 0x120
			 0x1f7a9090, /// 0x124
			 0x126d891a, /// 0x128
			 0x5648d9ee, /// 0x12c
			 0x1234454d, /// 0x130
			 0x48a111b7, /// 0x134
			 0xfefde36d, /// 0x138
			 0x79294b77, /// 0x13c
			 0x66209dff, /// 0x140
			 0xc4caa3af, /// 0x144
			 0x72d29dbe, /// 0x148
			 0x4248a7fd, /// 0x14c
			 0x308f9ea0, /// 0x150
			 0xdb647771, /// 0x154
			 0xf38143f2, /// 0x158
			 0xfef1a540, /// 0x15c
			 0x8e594ecf, /// 0x160
			 0x7f4c1a65, /// 0x164
			 0x144e8bc1, /// 0x168
			 0x7bb4594c, /// 0x16c
			 0x3bdbebbc, /// 0x170
			 0x06eca398, /// 0x174
			 0x19e81acf, /// 0x178
			 0xc806748d, /// 0x17c
			 0xea1e8601, /// 0x180
			 0xf7e0580d, /// 0x184
			 0xc1fabe9a, /// 0x188
			 0xa79393f7, /// 0x18c
			 0x31fd3f95, /// 0x190
			 0xd4046308, /// 0x194
			 0x86bf17dd, /// 0x198
			 0x7a082887, /// 0x19c
			 0xd50c91d2, /// 0x1a0
			 0xa9831b50, /// 0x1a4
			 0x126e01fd, /// 0x1a8
			 0xea2a5018, /// 0x1ac
			 0x530d4ae6, /// 0x1b0
			 0x5f350206, /// 0x1b4
			 0x0b8ac69b, /// 0x1b8
			 0x9cb80743, /// 0x1bc
			 0xb7f8c480, /// 0x1c0
			 0x4a216926, /// 0x1c4
			 0x954ac657, /// 0x1c8
			 0x46685e52, /// 0x1cc
			 0xbebc1a37, /// 0x1d0
			 0x6224e582, /// 0x1d4
			 0x43dc285d, /// 0x1d8
			 0x381470b7, /// 0x1dc
			 0x637e70df, /// 0x1e0
			 0x44fa5af9, /// 0x1e4
			 0x5a678f88, /// 0x1e8
			 0xf7d88905, /// 0x1ec
			 0x2b0c9318, /// 0x1f0
			 0x487e5905, /// 0x1f4
			 0x9b09b65d, /// 0x1f8
			 0x38bda8fb, /// 0x1fc
			 0xd9ede063, /// 0x200
			 0x6a140b89, /// 0x204
			 0x27a27b67, /// 0x208
			 0xd78032ba, /// 0x20c
			 0x59f10aec, /// 0x210
			 0xe5532348, /// 0x214
			 0x4f22c71f, /// 0x218
			 0xad3f743c, /// 0x21c
			 0x215adbaa, /// 0x220
			 0x3a99f488, /// 0x224
			 0xa468693c, /// 0x228
			 0xf776ccf6, /// 0x22c
			 0x03eb85ed, /// 0x230
			 0xa5f5708c, /// 0x234
			 0x6f0a4275, /// 0x238
			 0x3cdcfd6f, /// 0x23c
			 0x8ba1db9d, /// 0x240
			 0xeb4ca1f5, /// 0x244
			 0xf2aba299, /// 0x248
			 0xc0604d04, /// 0x24c
			 0xc930d52c, /// 0x250
			 0x59482c8c, /// 0x254
			 0x979e2028, /// 0x258
			 0x71f6f221, /// 0x25c
			 0x3336f84e, /// 0x260
			 0xb6e45fc8, /// 0x264
			 0xadae068a, /// 0x268
			 0xae2ad734, /// 0x26c
			 0x51747b43, /// 0x270
			 0xa6cf001e, /// 0x274
			 0x24c061b0, /// 0x278
			 0x6c8bf07e, /// 0x27c
			 0x2052c244, /// 0x280
			 0xe60eb84c, /// 0x284
			 0xa3fcafe9, /// 0x288
			 0x735db146, /// 0x28c
			 0x149415b6, /// 0x290
			 0xd5b45b6f, /// 0x294
			 0x8ee1b60c, /// 0x298
			 0x236664a1, /// 0x29c
			 0x75fcaf7a, /// 0x2a0
			 0x8939f316, /// 0x2a4
			 0x1af04003, /// 0x2a8
			 0xc9cae79d, /// 0x2ac
			 0xa472882e, /// 0x2b0
			 0x14ae4afd, /// 0x2b4
			 0xc4dcd258, /// 0x2b8
			 0x0d904b1b, /// 0x2bc
			 0x7cbbebba, /// 0x2c0
			 0xebb25378, /// 0x2c4
			 0x1e4482c0, /// 0x2c8
			 0x16ebb3a5, /// 0x2cc
			 0xacb04084, /// 0x2d0
			 0x35843a04, /// 0x2d4
			 0x054f6d85, /// 0x2d8
			 0x75cab905, /// 0x2dc
			 0x4e83c8b0, /// 0x2e0
			 0xfa6fddcf, /// 0x2e4
			 0xde35f654, /// 0x2e8
			 0x9bd58a1c, /// 0x2ec
			 0xd1944d77, /// 0x2f0
			 0x7946b40c, /// 0x2f4
			 0x56dcbd78, /// 0x2f8
			 0xe2b34d89, /// 0x2fc
			 0x40703f76, /// 0x300
			 0x6e542830, /// 0x304
			 0x15cb7f68, /// 0x308
			 0xa857e4d2, /// 0x30c
			 0x5f132462, /// 0x310
			 0xa9c8158a, /// 0x314
			 0x16978141, /// 0x318
			 0xf9b5e175, /// 0x31c
			 0x4feac960, /// 0x320
			 0x7755494c, /// 0x324
			 0x383c79c4, /// 0x328
			 0xcfbaa16d, /// 0x32c
			 0x395d01eb, /// 0x330
			 0x16e7608f, /// 0x334
			 0x82f5bd62, /// 0x338
			 0xd174e7eb, /// 0x33c
			 0x9e37a7a5, /// 0x340
			 0xd18f1d67, /// 0x344
			 0xdce77dec, /// 0x348
			 0x7387a276, /// 0x34c
			 0x3af0e39e, /// 0x350
			 0xd55e724f, /// 0x354
			 0x2bf0e3a2, /// 0x358
			 0x35f47a9b, /// 0x35c
			 0x845517bf, /// 0x360
			 0x6dea4f10, /// 0x364
			 0x7537d768, /// 0x368
			 0x5ad55f6a, /// 0x36c
			 0xf35927f4, /// 0x370
			 0x9e42363b, /// 0x374
			 0xf15de4fc, /// 0x378
			 0x0f556c35, /// 0x37c
			 0x3e826e56, /// 0x380
			 0x9750947f, /// 0x384
			 0x1dd3fe8d, /// 0x388
			 0x226b6f99, /// 0x38c
			 0xb69b28d2, /// 0x390
			 0xb7080e09, /// 0x394
			 0x556a0a12, /// 0x398
			 0x72a8c0e7, /// 0x39c
			 0x167679ec, /// 0x3a0
			 0x5e7fda40, /// 0x3a4
			 0x694cb39f, /// 0x3a8
			 0xf7084c06, /// 0x3ac
			 0xafb0844c, /// 0x3b0
			 0xdefd5645, /// 0x3b4
			 0x0176ea76, /// 0x3b8
			 0x8dc43a47, /// 0x3bc
			 0x0dc6756f, /// 0x3c0
			 0xbb6fbce8, /// 0x3c4
			 0xf9043d54, /// 0x3c8
			 0xabf2a939, /// 0x3cc
			 0x44649bc3, /// 0x3d0
			 0x7b759c0e, /// 0x3d4
			 0x4e10040f, /// 0x3d8
			 0xa515a301, /// 0x3dc
			 0x3bbfa538, /// 0x3e0
			 0xe777fa93, /// 0x3e4
			 0xd4a16552, /// 0x3e8
			 0x6489256b, /// 0x3ec
			 0x8eb69085, /// 0x3f0
			 0x44c95892, /// 0x3f4
			 0xd40960a2, /// 0x3f8
			 0xdf76356a, /// 0x3fc
			 0xe92ba0e4, /// 0x400
			 0x5b308d46, /// 0x404
			 0x90bdab4e, /// 0x408
			 0x3a030ecf, /// 0x40c
			 0x58a3babe, /// 0x410
			 0x7a002514, /// 0x414
			 0xd246feea, /// 0x418
			 0xe7377a5d, /// 0x41c
			 0x2c746b7d, /// 0x420
			 0xe46ad1d0, /// 0x424
			 0x05f06b77, /// 0x428
			 0xce3b604d, /// 0x42c
			 0xd74ccced, /// 0x430
			 0x3135b36f, /// 0x434
			 0x76604b1f, /// 0x438
			 0x436dc5a4, /// 0x43c
			 0x7a6dff9d, /// 0x440
			 0xa8ca70bd, /// 0x444
			 0x8d6a189b, /// 0x448
			 0x05a042fe, /// 0x44c
			 0x0dbc31d2, /// 0x450
			 0x5b6a134b, /// 0x454
			 0x7731b015, /// 0x458
			 0x94f8ee3e, /// 0x45c
			 0x12f5db7b, /// 0x460
			 0x9092a854, /// 0x464
			 0x60d0e317, /// 0x468
			 0xaf09ab87, /// 0x46c
			 0xb9f1b2d0, /// 0x470
			 0x35724f02, /// 0x474
			 0x0bb15995, /// 0x478
			 0x0908f026, /// 0x47c
			 0xa3092c01, /// 0x480
			 0xe5f9c63e, /// 0x484
			 0xb3d2b837, /// 0x488
			 0x33dc3765, /// 0x48c
			 0x06f89337, /// 0x490
			 0x0f897458, /// 0x494
			 0x0b5ab365, /// 0x498
			 0xf3a548a4, /// 0x49c
			 0xe5055ad3, /// 0x4a0
			 0x6a72c6f6, /// 0x4a4
			 0xd4417dde, /// 0x4a8
			 0x20e76e42, /// 0x4ac
			 0x0a19528f, /// 0x4b0
			 0x7bcc6330, /// 0x4b4
			 0x9d1cbbc7, /// 0x4b8
			 0x16a88aa9, /// 0x4bc
			 0x098e0746, /// 0x4c0
			 0x58dc456e, /// 0x4c4
			 0x8e26e828, /// 0x4c8
			 0x95fc18a6, /// 0x4cc
			 0x4fba053a, /// 0x4d0
			 0x5f230173, /// 0x4d4
			 0x2800a6a8, /// 0x4d8
			 0xca6a68ec, /// 0x4dc
			 0x54c42238, /// 0x4e0
			 0xfdc80030, /// 0x4e4
			 0x918b6fd3, /// 0x4e8
			 0x0b58a2e3, /// 0x4ec
			 0xc3e29675, /// 0x4f0
			 0x98ca8577, /// 0x4f4
			 0xe4fe79ca, /// 0x4f8
			 0x48672d60, /// 0x4fc
			 0x3ddb07da, /// 0x500
			 0x182ed26b, /// 0x504
			 0xbe509f73, /// 0x508
			 0xb5ee580b, /// 0x50c
			 0xd945612e, /// 0x510
			 0x50128677, /// 0x514
			 0xc6497257, /// 0x518
			 0xc3134ad6, /// 0x51c
			 0xeb3155ff, /// 0x520
			 0xa01316ad, /// 0x524
			 0xa24a9f00, /// 0x528
			 0x5a81e91d, /// 0x52c
			 0x772a23f9, /// 0x530
			 0xf604ec62, /// 0x534
			 0xd92b355f, /// 0x538
			 0xde0a9762, /// 0x53c
			 0xc397a25d, /// 0x540
			 0x51683c0f, /// 0x544
			 0xa1e9bf18, /// 0x548
			 0xe507dc3b, /// 0x54c
			 0x7ee8b31b, /// 0x550
			 0xa769c142, /// 0x554
			 0xc3ef37e2, /// 0x558
			 0x34064ebb, /// 0x55c
			 0x8c204a1b, /// 0x560
			 0x69542def, /// 0x564
			 0xb459e7fa, /// 0x568
			 0x00d44a53, /// 0x56c
			 0x361ac08a, /// 0x570
			 0xcc6baa92, /// 0x574
			 0x2b971454, /// 0x578
			 0xea8f5404, /// 0x57c
			 0x38bf711a, /// 0x580
			 0x9cbe321b, /// 0x584
			 0x770be311, /// 0x588
			 0x9fb610ee, /// 0x58c
			 0x5cf5903b, /// 0x590
			 0xfa93e345, /// 0x594
			 0x62b8c724, /// 0x598
			 0xb5e7b2d3, /// 0x59c
			 0x7bcf9757, /// 0x5a0
			 0x94af12b3, /// 0x5a4
			 0x58ee2d56, /// 0x5a8
			 0xa38d2de2, /// 0x5ac
			 0xd8b7fff7, /// 0x5b0
			 0x4d3435ea, /// 0x5b4
			 0xd1426d0c, /// 0x5b8
			 0x9769fdc1, /// 0x5bc
			 0x12e11524, /// 0x5c0
			 0x8906b94e, /// 0x5c4
			 0xdfeb329a, /// 0x5c8
			 0x5a564499, /// 0x5cc
			 0x640cb676, /// 0x5d0
			 0x54c1ed9f, /// 0x5d4
			 0xbcfd0e01, /// 0x5d8
			 0xba7e057c, /// 0x5dc
			 0x37345da9, /// 0x5e0
			 0x0e0c3c30, /// 0x5e4
			 0x324e170d, /// 0x5e8
			 0x539bbd47, /// 0x5ec
			 0x7084d3d5, /// 0x5f0
			 0x62a1fb29, /// 0x5f4
			 0xbc83323a, /// 0x5f8
			 0xb226c24f, /// 0x5fc
			 0x30736784, /// 0x600
			 0x3f616b12, /// 0x604
			 0xe09aa2fc, /// 0x608
			 0x15de3862, /// 0x60c
			 0xfd9ecff4, /// 0x610
			 0xee002a09, /// 0x614
			 0x7c9e3c6f, /// 0x618
			 0x143c0cbf, /// 0x61c
			 0x0e9e1ede, /// 0x620
			 0xcc9d3613, /// 0x624
			 0x30806d5a, /// 0x628
			 0xf3fba36a, /// 0x62c
			 0x4ac0c874, /// 0x630
			 0x4589e5e2, /// 0x634
			 0x0fc48064, /// 0x638
			 0x52f3de1f, /// 0x63c
			 0xa919f329, /// 0x640
			 0x70923f2a, /// 0x644
			 0x5b845e5b, /// 0x648
			 0xdec8d193, /// 0x64c
			 0x0ade83c5, /// 0x650
			 0xf934f223, /// 0x654
			 0x48c1e410, /// 0x658
			 0x36e590b0, /// 0x65c
			 0xa10f416b, /// 0x660
			 0x1d79544c, /// 0x664
			 0x759d4190, /// 0x668
			 0x6af77c14, /// 0x66c
			 0x530a30b8, /// 0x670
			 0x87cf4559, /// 0x674
			 0x43828747, /// 0x678
			 0xf8d550bc, /// 0x67c
			 0x57dc6339, /// 0x680
			 0x0ef7da7a, /// 0x684
			 0x96ff33b0, /// 0x688
			 0xe56dd228, /// 0x68c
			 0x20c5b66b, /// 0x690
			 0x369a68ef, /// 0x694
			 0x1b16980e, /// 0x698
			 0x7da2643e, /// 0x69c
			 0x526c77da, /// 0x6a0
			 0xe60ccafe, /// 0x6a4
			 0x6478f07c, /// 0x6a8
			 0x73bc8b9b, /// 0x6ac
			 0xd1779cfc, /// 0x6b0
			 0xbb9c0e2a, /// 0x6b4
			 0xc26232f7, /// 0x6b8
			 0x2ca16247, /// 0x6bc
			 0x9e8cc0bb, /// 0x6c0
			 0x1dcb1065, /// 0x6c4
			 0x6b5adaf8, /// 0x6c8
			 0x188aeac9, /// 0x6cc
			 0x9c589289, /// 0x6d0
			 0xa0e944de, /// 0x6d4
			 0x561fa944, /// 0x6d8
			 0x86ee8fae, /// 0x6dc
			 0x836ab1e8, /// 0x6e0
			 0x15a33b96, /// 0x6e4
			 0x3c3c70d7, /// 0x6e8
			 0x71485939, /// 0x6ec
			 0x3ba88600, /// 0x6f0
			 0x66cf451e, /// 0x6f4
			 0xec08d2e3, /// 0x6f8
			 0x408c0687, /// 0x6fc
			 0x71257f64, /// 0x700
			 0xcac21ad1, /// 0x704
			 0xee1972b2, /// 0x708
			 0x3cbca68c, /// 0x70c
			 0xb47a0e76, /// 0x710
			 0x697bb12c, /// 0x714
			 0x67320543, /// 0x718
			 0x9e4ae31b, /// 0x71c
			 0xf6e48c79, /// 0x720
			 0x31873278, /// 0x724
			 0x501202a7, /// 0x728
			 0x49cebe1a, /// 0x72c
			 0x63adffd3, /// 0x730
			 0x971ca07c, /// 0x734
			 0x8c40fa77, /// 0x738
			 0xcc15b81c, /// 0x73c
			 0xdf8f7f1e, /// 0x740
			 0x58ea66f0, /// 0x744
			 0xd7bee5b5, /// 0x748
			 0x1ad6de8e, /// 0x74c
			 0x5953dfdf, /// 0x750
			 0xb844712c, /// 0x754
			 0xffa35b9d, /// 0x758
			 0xf4f26860, /// 0x75c
			 0xe9cb3c6d, /// 0x760
			 0x6bb97390, /// 0x764
			 0xd4e1b2be, /// 0x768
			 0x9865a6ca, /// 0x76c
			 0x63b3e003, /// 0x770
			 0x1c11803d, /// 0x774
			 0xf3a2d28e, /// 0x778
			 0x2ca4c303, /// 0x77c
			 0xe331e295, /// 0x780
			 0x9c364006, /// 0x784
			 0x47dc7d97, /// 0x788
			 0x60d383f1, /// 0x78c
			 0xc80ab3c0, /// 0x790
			 0xea91e1c0, /// 0x794
			 0x16031639, /// 0x798
			 0x0b2c2409, /// 0x79c
			 0xb78d816c, /// 0x7a0
			 0x537b101b, /// 0x7a4
			 0x8783b1f6, /// 0x7a8
			 0xf1903234, /// 0x7ac
			 0xa863722e, /// 0x7b0
			 0xa6e535b8, /// 0x7b4
			 0x8df61013, /// 0x7b8
			 0x5e8fcced, /// 0x7bc
			 0x53885f03, /// 0x7c0
			 0xb58a6e5c, /// 0x7c4
			 0x58807f3d, /// 0x7c8
			 0xddf4bbcc, /// 0x7cc
			 0x2f195208, /// 0x7d0
			 0x974e2242, /// 0x7d4
			 0xcd26596d, /// 0x7d8
			 0xfe8f84c6, /// 0x7dc
			 0x0c85baae, /// 0x7e0
			 0xc0578972, /// 0x7e4
			 0x8cb0d88e, /// 0x7e8
			 0x7fe1a406, /// 0x7ec
			 0xe406b8a5, /// 0x7f0
			 0x4ea0f901, /// 0x7f4
			 0xdd44df86, /// 0x7f8
			 0xa98254f1, /// 0x7fc
			 0xfeb0a880, /// 0x800
			 0x3fe01608, /// 0x804
			 0x35ccf28d, /// 0x808
			 0x8c63a323, /// 0x80c
			 0x5c10f9f7, /// 0x810
			 0xa68e8ad8, /// 0x814
			 0xf01bf415, /// 0x818
			 0xb02f7a6a, /// 0x81c
			 0xcfc6dfd6, /// 0x820
			 0xda7a253c, /// 0x824
			 0x28d6399c, /// 0x828
			 0x058a91f0, /// 0x82c
			 0x456d27b0, /// 0x830
			 0x0b5054c0, /// 0x834
			 0x6aafc996, /// 0x838
			 0x200f4bc7, /// 0x83c
			 0x8b56177d, /// 0x840
			 0xb63138ee, /// 0x844
			 0x7782262a, /// 0x848
			 0xfab15917, /// 0x84c
			 0xd1a251d1, /// 0x850
			 0xc5c75137, /// 0x854
			 0x915fc76e, /// 0x858
			 0x5148c32f, /// 0x85c
			 0x6b34d75c, /// 0x860
			 0x50cc2414, /// 0x864
			 0x5b5efd4f, /// 0x868
			 0x6ab98359, /// 0x86c
			 0x7ad6cce7, /// 0x870
			 0x85109be9, /// 0x874
			 0x2f20c5bc, /// 0x878
			 0x3474d898, /// 0x87c
			 0x34af682f, /// 0x880
			 0xa4149b88, /// 0x884
			 0x403fc035, /// 0x888
			 0x00b27977, /// 0x88c
			 0xb5736c4e, /// 0x890
			 0x25e039d1, /// 0x894
			 0x683e18dd, /// 0x898
			 0xeeab7e6b, /// 0x89c
			 0x0d7b390e, /// 0x8a0
			 0xae882e2f, /// 0x8a4
			 0x3459d4de, /// 0x8a8
			 0x2ef86e8c, /// 0x8ac
			 0x5a21c508, /// 0x8b0
			 0x5a6a3a89, /// 0x8b4
			 0xb9d4d862, /// 0x8b8
			 0xd1c816dc, /// 0x8bc
			 0xc9662628, /// 0x8c0
			 0x8e892d22, /// 0x8c4
			 0x8384baf3, /// 0x8c8
			 0x9c4421fe, /// 0x8cc
			 0x3388c1d9, /// 0x8d0
			 0xf6fe0304, /// 0x8d4
			 0x7e94734a, /// 0x8d8
			 0x2ab9dd44, /// 0x8dc
			 0x9819b3ca, /// 0x8e0
			 0xeb4281c6, /// 0x8e4
			 0xe0226a08, /// 0x8e8
			 0x7e7d6629, /// 0x8ec
			 0xdb795453, /// 0x8f0
			 0x35efeeac, /// 0x8f4
			 0xc3d15a7a, /// 0x8f8
			 0xa32f7a00, /// 0x8fc
			 0x3d905162, /// 0x900
			 0x875e0d0b, /// 0x904
			 0x2904c22b, /// 0x908
			 0x2b057ff5, /// 0x90c
			 0x60ad203b, /// 0x910
			 0x6edcda05, /// 0x914
			 0x5a1d6fb5, /// 0x918
			 0x6364dcd6, /// 0x91c
			 0x1dd98eee, /// 0x920
			 0x5b01b340, /// 0x924
			 0xaaadd9f7, /// 0x928
			 0x8d860380, /// 0x92c
			 0x7e092bec, /// 0x930
			 0x06f1511e, /// 0x934
			 0xf4340b90, /// 0x938
			 0x17bced9b, /// 0x93c
			 0x3be1be66, /// 0x940
			 0x2d353208, /// 0x944
			 0x89346fd1, /// 0x948
			 0x24367f78, /// 0x94c
			 0x1174849f, /// 0x950
			 0xf57f9a43, /// 0x954
			 0xed9c5942, /// 0x958
			 0xf2b37aa7, /// 0x95c
			 0xc6d95a7e, /// 0x960
			 0x4ff83c70, /// 0x964
			 0x9eef50b8, /// 0x968
			 0xd7f51cbc, /// 0x96c
			 0x507d414b, /// 0x970
			 0xa4d85851, /// 0x974
			 0x0379378f, /// 0x978
			 0x45f9a410, /// 0x97c
			 0xa1e2d71f, /// 0x980
			 0x57a48037, /// 0x984
			 0xf2c4ea40, /// 0x988
			 0xa586016a, /// 0x98c
			 0xbd75f609, /// 0x990
			 0xd1cb9408, /// 0x994
			 0x8bf87206, /// 0x998
			 0x6f31cfed, /// 0x99c
			 0x15db8a05, /// 0x9a0
			 0x16446c3f, /// 0x9a4
			 0xa063211d, /// 0x9a8
			 0x5d4dc820, /// 0x9ac
			 0x87c8d150, /// 0x9b0
			 0x8acd8134, /// 0x9b4
			 0xb21ba300, /// 0x9b8
			 0xeaadc589, /// 0x9bc
			 0xdf710878, /// 0x9c0
			 0xc5f90a00, /// 0x9c4
			 0x575c69c4, /// 0x9c8
			 0x329d3a92, /// 0x9cc
			 0x5a952fbf, /// 0x9d0
			 0x05eb8f00, /// 0x9d4
			 0xbe05da5e, /// 0x9d8
			 0x6dc73c5b, /// 0x9dc
			 0x12d5a23e, /// 0x9e0
			 0xb2e54778, /// 0x9e4
			 0x2c172d90, /// 0x9e8
			 0x72617f14, /// 0x9ec
			 0x96eca50b, /// 0x9f0
			 0xe046a7ef, /// 0x9f4
			 0x8a68009b, /// 0x9f8
			 0xd4f7c6ba, /// 0x9fc
			 0xb9260ede, /// 0xa00
			 0x6fd0b5c7, /// 0xa04
			 0x7b545f41, /// 0xa08
			 0x42fc814b, /// 0xa0c
			 0x139b23d5, /// 0xa10
			 0x3c6aba16, /// 0xa14
			 0x87b787e9, /// 0xa18
			 0x2f6107ec, /// 0xa1c
			 0x0aa757a9, /// 0xa20
			 0xb0e9a095, /// 0xa24
			 0xd2f73ce5, /// 0xa28
			 0x920753f9, /// 0xa2c
			 0x7c1b6e56, /// 0xa30
			 0xa6a1a5f6, /// 0xa34
			 0xb42b8dec, /// 0xa38
			 0x387f8da7, /// 0xa3c
			 0x4e270345, /// 0xa40
			 0x1fe49079, /// 0xa44
			 0x87370457, /// 0xa48
			 0xa883139a, /// 0xa4c
			 0x8b1b947b, /// 0xa50
			 0x308065e2, /// 0xa54
			 0x21e44d69, /// 0xa58
			 0xa7d8fab8, /// 0xa5c
			 0x67779982, /// 0xa60
			 0xa61eff87, /// 0xa64
			 0x1cfb1477, /// 0xa68
			 0xe2fc536b, /// 0xa6c
			 0xbc8bfd09, /// 0xa70
			 0xa3b1d54e, /// 0xa74
			 0xaffa7955, /// 0xa78
			 0xfbb4e4de, /// 0xa7c
			 0xc71fda48, /// 0xa80
			 0x73a5531d, /// 0xa84
			 0xa9ee1054, /// 0xa88
			 0xd70531a5, /// 0xa8c
			 0x59c94d0e, /// 0xa90
			 0xaf57a9da, /// 0xa94
			 0xfdc7f663, /// 0xa98
			 0xc91fb857, /// 0xa9c
			 0xcdb5a88f, /// 0xaa0
			 0x66b2b1be, /// 0xaa4
			 0x170bae25, /// 0xaa8
			 0xeab4f793, /// 0xaac
			 0xc010f4d5, /// 0xab0
			 0xb71302db, /// 0xab4
			 0xe9a8f6f3, /// 0xab8
			 0xb3ebd861, /// 0xabc
			 0xd651d6df, /// 0xac0
			 0x901e3a59, /// 0xac4
			 0x7990c4ee, /// 0xac8
			 0x92dea036, /// 0xacc
			 0x5938caa1, /// 0xad0
			 0xa65e3e02, /// 0xad4
			 0xe6d253ca, /// 0xad8
			 0x45d1f36e, /// 0xadc
			 0x04d14abf, /// 0xae0
			 0xdfdbc36b, /// 0xae4
			 0x7b2d2cac, /// 0xae8
			 0x429bd702, /// 0xaec
			 0xe0fdf93e, /// 0xaf0
			 0xc809a05d, /// 0xaf4
			 0x879bd362, /// 0xaf8
			 0x4dc0c781, /// 0xafc
			 0x8a17c490, /// 0xb00
			 0x07b6118d, /// 0xb04
			 0x3ddd85ce, /// 0xb08
			 0xc79193a6, /// 0xb0c
			 0xfdb7d7d6, /// 0xb10
			 0x264c5be0, /// 0xb14
			 0x679a0580, /// 0xb18
			 0xd8c7f482, /// 0xb1c
			 0xa57c364a, /// 0xb20
			 0x7644562b, /// 0xb24
			 0x0216f226, /// 0xb28
			 0x0efdb46a, /// 0xb2c
			 0xe56f623b, /// 0xb30
			 0x1d2e290e, /// 0xb34
			 0xaa64a71f, /// 0xb38
			 0x40361773, /// 0xb3c
			 0xb00c832e, /// 0xb40
			 0x73a98d90, /// 0xb44
			 0x25a0e713, /// 0xb48
			 0xe520b0c9, /// 0xb4c
			 0xa0f675c0, /// 0xb50
			 0x82258888, /// 0xb54
			 0x07319c6a, /// 0xb58
			 0x532805b2, /// 0xb5c
			 0x51333b06, /// 0xb60
			 0x1d05ba75, /// 0xb64
			 0xd67cc965, /// 0xb68
			 0x8cc7e195, /// 0xb6c
			 0xd36c138b, /// 0xb70
			 0xe98c125e, /// 0xb74
			 0xd52c7c0a, /// 0xb78
			 0x1c58b2b1, /// 0xb7c
			 0xa45832e6, /// 0xb80
			 0xa618bb61, /// 0xb84
			 0xa711bc58, /// 0xb88
			 0x3373cece, /// 0xb8c
			 0x2aec1395, /// 0xb90
			 0xfd2a2bac, /// 0xb94
			 0x2dcfb3b0, /// 0xb98
			 0x5ed517b4, /// 0xb9c
			 0x36ad32f0, /// 0xba0
			 0x1cfe3821, /// 0xba4
			 0xa3f769af, /// 0xba8
			 0x240b5b01, /// 0xbac
			 0xbe58e8b8, /// 0xbb0
			 0x9fdcdf94, /// 0xbb4
			 0xa893558e, /// 0xbb8
			 0xada57b3d, /// 0xbbc
			 0xaa21bbbd, /// 0xbc0
			 0x1575e344, /// 0xbc4
			 0xb858448c, /// 0xbc8
			 0x90343704, /// 0xbcc
			 0xec6c583c, /// 0xbd0
			 0xefa1d5a3, /// 0xbd4
			 0xb43f9ee4, /// 0xbd8
			 0xdeb2da9d, /// 0xbdc
			 0xdc64c41e, /// 0xbe0
			 0x056f7951, /// 0xbe4
			 0x06aba5cc, /// 0xbe8
			 0x5e27ae24, /// 0xbec
			 0x6b041406, /// 0xbf0
			 0xa89993d7, /// 0xbf4
			 0x4d5028b2, /// 0xbf8
			 0xb48bb788, /// 0xbfc
			 0x87b971bb, /// 0xc00
			 0x143ce508, /// 0xc04
			 0x9645d4c4, /// 0xc08
			 0xa36fc3e3, /// 0xc0c
			 0xaecd05a2, /// 0xc10
			 0xbd38114c, /// 0xc14
			 0x767599fd, /// 0xc18
			 0xca5e6f6a, /// 0xc1c
			 0xe0613056, /// 0xc20
			 0xb8302928, /// 0xc24
			 0x75ede317, /// 0xc28
			 0x01902c96, /// 0xc2c
			 0xae797c5d, /// 0xc30
			 0xc43e67a2, /// 0xc34
			 0xabaa7211, /// 0xc38
			 0x4e202a85, /// 0xc3c
			 0x7cad2e60, /// 0xc40
			 0x187f141a, /// 0xc44
			 0x70ff3fde, /// 0xc48
			 0xe35061ca, /// 0xc4c
			 0xba7019fa, /// 0xc50
			 0xa62c033c, /// 0xc54
			 0x9002626c, /// 0xc58
			 0x5b2722de, /// 0xc5c
			 0x0050c45a, /// 0xc60
			 0xfc1ca349, /// 0xc64
			 0x95d345cf, /// 0xc68
			 0x7648d8de, /// 0xc6c
			 0x8c4f5503, /// 0xc70
			 0x41786631, /// 0xc74
			 0x15187f7f, /// 0xc78
			 0xce8fbefa, /// 0xc7c
			 0x9d34b841, /// 0xc80
			 0x87075220, /// 0xc84
			 0xf0af2ac5, /// 0xc88
			 0x8631ba0d, /// 0xc8c
			 0xce07b959, /// 0xc90
			 0x6dd71fbd, /// 0xc94
			 0x5850ecc6, /// 0xc98
			 0xee7246f6, /// 0xc9c
			 0xb1c9c315, /// 0xca0
			 0x3c877a0c, /// 0xca4
			 0x5d6ac909, /// 0xca8
			 0xa876db32, /// 0xcac
			 0x57a81faa, /// 0xcb0
			 0x8054dead, /// 0xcb4
			 0x166d3441, /// 0xcb8
			 0x46739895, /// 0xcbc
			 0xb0e7ab2a, /// 0xcc0
			 0xf5c9b330, /// 0xcc4
			 0xec3609a3, /// 0xcc8
			 0x2357e344, /// 0xccc
			 0x8cf86a86, /// 0xcd0
			 0x04197bc4, /// 0xcd4
			 0x0f802404, /// 0xcd8
			 0x8c5a85c1, /// 0xcdc
			 0xc9f20b65, /// 0xce0
			 0x5a29b3c4, /// 0xce4
			 0xb4536f46, /// 0xce8
			 0x51f228db, /// 0xcec
			 0xcf144e3b, /// 0xcf0
			 0x9acc247e, /// 0xcf4
			 0x2b2cdcab, /// 0xcf8
			 0x12d429ab, /// 0xcfc
			 0x453d0be3, /// 0xd00
			 0x8e8a6353, /// 0xd04
			 0x0ac2fb1b, /// 0xd08
			 0x0f09f0b3, /// 0xd0c
			 0xf12e24fe, /// 0xd10
			 0xbdeb8fb6, /// 0xd14
			 0x42d45512, /// 0xd18
			 0xb29ec47c, /// 0xd1c
			 0x16c3fda4, /// 0xd20
			 0xc4e5f49f, /// 0xd24
			 0x538e1d3d, /// 0xd28
			 0xce38bb13, /// 0xd2c
			 0x1920e53b, /// 0xd30
			 0xf5e35be5, /// 0xd34
			 0x35e921ec, /// 0xd38
			 0x9fe0465a, /// 0xd3c
			 0x0585fe8b, /// 0xd40
			 0xbd80aa67, /// 0xd44
			 0xff9aa7c2, /// 0xd48
			 0xc0bb5e79, /// 0xd4c
			 0x03ec5d11, /// 0xd50
			 0x9390a932, /// 0xd54
			 0x9362e243, /// 0xd58
			 0xbc3087cc, /// 0xd5c
			 0xc1349989, /// 0xd60
			 0xb9586a48, /// 0xd64
			 0xd5de4a64, /// 0xd68
			 0xc09a58fb, /// 0xd6c
			 0x87084e64, /// 0xd70
			 0x77861967, /// 0xd74
			 0x2599d2e3, /// 0xd78
			 0x17318408, /// 0xd7c
			 0xfe5f389e, /// 0xd80
			 0x0949db95, /// 0xd84
			 0xadd523a6, /// 0xd88
			 0xf9c4de7c, /// 0xd8c
			 0x301d40cc, /// 0xd90
			 0x61b116d1, /// 0xd94
			 0x4c0e8187, /// 0xd98
			 0x13048f81, /// 0xd9c
			 0xfeb78392, /// 0xda0
			 0x0ee9eeb0, /// 0xda4
			 0x910ac15f, /// 0xda8
			 0xe3c03786, /// 0xdac
			 0x84a88b79, /// 0xdb0
			 0x4f648ec3, /// 0xdb4
			 0x4f789f88, /// 0xdb8
			 0xda3a4c64, /// 0xdbc
			 0xd79b8a69, /// 0xdc0
			 0x2007be7f, /// 0xdc4
			 0x4a7d4353, /// 0xdc8
			 0x16ba82aa, /// 0xdcc
			 0x77108856, /// 0xdd0
			 0x006690bf, /// 0xdd4
			 0x59b01390, /// 0xdd8
			 0xc827ae5e, /// 0xddc
			 0xf4c1bf93, /// 0xde0
			 0xe3365ef1, /// 0xde4
			 0x30ab4f2f, /// 0xde8
			 0xa9c7d9f6, /// 0xdec
			 0x8e319aff, /// 0xdf0
			 0x3b5766f6, /// 0xdf4
			 0x7706848e, /// 0xdf8
			 0x3f00af14, /// 0xdfc
			 0x81e79b35, /// 0xe00
			 0xd298c6fd, /// 0xe04
			 0xc9736040, /// 0xe08
			 0xa006f775, /// 0xe0c
			 0x5acd75c5, /// 0xe10
			 0x23682310, /// 0xe14
			 0x6c72f31c, /// 0xe18
			 0xe3f15397, /// 0xe1c
			 0x23e9ac31, /// 0xe20
			 0x441f8eca, /// 0xe24
			 0xb1f392b4, /// 0xe28
			 0x445e82b2, /// 0xe2c
			 0xc1191720, /// 0xe30
			 0xebc15f4b, /// 0xe34
			 0x2d93f744, /// 0xe38
			 0x526bc310, /// 0xe3c
			 0xc4ee14d1, /// 0xe40
			 0x2174cdf0, /// 0xe44
			 0x9d5b5dbc, /// 0xe48
			 0x63b1aeb8, /// 0xe4c
			 0x6a3ec45a, /// 0xe50
			 0xcea850bc, /// 0xe54
			 0xfaf880c4, /// 0xe58
			 0xd4d0bcbb, /// 0xe5c
			 0x76de2c06, /// 0xe60
			 0x904e9f01, /// 0xe64
			 0x78845c5b, /// 0xe68
			 0x8aa70c2d, /// 0xe6c
			 0x2e07e002, /// 0xe70
			 0x62f82052, /// 0xe74
			 0x8dfca163, /// 0xe78
			 0xa071cb9b, /// 0xe7c
			 0x6773f334, /// 0xe80
			 0x475a1a10, /// 0xe84
			 0x8b4dec22, /// 0xe88
			 0x2b603ad6, /// 0xe8c
			 0x2ab66bc3, /// 0xe90
			 0x568d84ed, /// 0xe94
			 0x75ff6605, /// 0xe98
			 0x10904188, /// 0xe9c
			 0x7d85c94d, /// 0xea0
			 0x13efb686, /// 0xea4
			 0x0dd1b617, /// 0xea8
			 0x608da8bc, /// 0xeac
			 0x015b66db, /// 0xeb0
			 0xcde17ea7, /// 0xeb4
			 0xa674ae20, /// 0xeb8
			 0xc9e09b44, /// 0xebc
			 0x0ef44e85, /// 0xec0
			 0x2c1581d5, /// 0xec4
			 0xb2c15032, /// 0xec8
			 0x7feae091, /// 0xecc
			 0x5c87efeb, /// 0xed0
			 0xd424b5f5, /// 0xed4
			 0xc5d51c3b, /// 0xed8
			 0x49530cd7, /// 0xedc
			 0x20a71c03, /// 0xee0
			 0x5510b7bc, /// 0xee4
			 0x54c68104, /// 0xee8
			 0x83592f20, /// 0xeec
			 0x92413a61, /// 0xef0
			 0x603ddbe5, /// 0xef4
			 0xae413a6b, /// 0xef8
			 0x181cebae, /// 0xefc
			 0x522a51ed, /// 0xf00
			 0xe6ada240, /// 0xf04
			 0x11a6a687, /// 0xf08
			 0xec5c1209, /// 0xf0c
			 0xc9fe1d97, /// 0xf10
			 0x9e6244b0, /// 0xf14
			 0x6d4995fe, /// 0xf18
			 0xc7c08e1d, /// 0xf1c
			 0xd34d920a, /// 0xf20
			 0x9afd7d13, /// 0xf24
			 0x7d69b8e0, /// 0xf28
			 0xa0743945, /// 0xf2c
			 0x2323e18f, /// 0xf30
			 0xc7a64af3, /// 0xf34
			 0xe9fd3e8e, /// 0xf38
			 0xf5e4986a, /// 0xf3c
			 0x28f4f938, /// 0xf40
			 0x4eef9f27, /// 0xf44
			 0xda1d4ad8, /// 0xf48
			 0xeb24249d, /// 0xf4c
			 0xa3ec0188, /// 0xf50
			 0x5c9618a6, /// 0xf54
			 0x5788b450, /// 0xf58
			 0x5726ebd9, /// 0xf5c
			 0x9f72474e, /// 0xf60
			 0x2153ce60, /// 0xf64
			 0x5e388f4b, /// 0xf68
			 0x7b81a46d, /// 0xf6c
			 0x67f432dc, /// 0xf70
			 0x3f04cdcb, /// 0xf74
			 0xf980366f, /// 0xf78
			 0xa03e84f0, /// 0xf7c
			 0x6d8793dc, /// 0xf80
			 0xef8ad39f, /// 0xf84
			 0x51bf74b9, /// 0xf88
			 0x41d74af0, /// 0xf8c
			 0x62ca546f, /// 0xf90
			 0x8ef24229, /// 0xf94
			 0xbc3a3a3d, /// 0xf98
			 0x09b36c88, /// 0xf9c
			 0xd6c79071, /// 0xfa0
			 0x8ebaf120, /// 0xfa4
			 0x78358697, /// 0xfa8
			 0x52290446, /// 0xfac
			 0xf40432d1, /// 0xfb0
			 0x431af655, /// 0xfb4
			 0xc023c501, /// 0xfb8
			 0xaa6cb6b0, /// 0xfbc
			 0xdf830752, /// 0xfc0
			 0x04cfe061, /// 0xfc4
			 0x454f5ff7, /// 0xfc8
			 0x8ab5b57b, /// 0xfcc
			 0xfe7b2103, /// 0xfd0
			 0xc870d608, /// 0xfd4
			 0x97bc6dff, /// 0xfd8
			 0x4828e524, /// 0xfdc
			 0x4d8603d2, /// 0xfe0
			 0xed94bbaf, /// 0xfe4
			 0xd4d7fecf, /// 0xfe8
			 0x5962bc59, /// 0xfec
			 0x774fa377, /// 0xff0
			 0x07aefe19, /// 0xff4
			 0x9a14b845, /// 0xff8
			 0xf3f69514 /// last
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
			 0x00000348,
			 0x0000069c,
			 0x00000004,
			 0x0000071c,
			 0x0000022c,
			 0x000007a4,
			 0x000006bc,
			 0x000003c4,

			 /// vpu register f2
			 0x41880000,
			 0x41c00000,
			 0x41500000,
			 0x41c80000,
			 0x41600000,
			 0x41880000,
			 0x41f00000,
			 0x41880000,

			 /// vpu register f3
			 0x41900000,
			 0x41800000,
			 0x41000000,
			 0x41100000,
			 0x41c80000,
			 0x40800000,
			 0x42000000,
			 0x41d00000,

			 /// vpu register f4
			 0x41a80000,
			 0x41100000,
			 0x40000000,
			 0x41600000,
			 0x41200000,
			 0x41e80000,
			 0x41600000,
			 0x41500000,

			 /// vpu register f5
			 0x40a00000,
			 0x41e80000,
			 0x41c80000,
			 0x41a80000,
			 0x41a00000,
			 0x41a00000,
			 0x41880000,
			 0x41b80000,

			 /// vpu register f6
			 0x40a00000,
			 0x41f80000,
			 0x41600000,
			 0x41c00000,
			 0x41d00000,
			 0x41000000,
			 0x41d80000,
			 0x41f80000,

			 /// vpu register f7
			 0x40400000,
			 0x40e00000,
			 0x41c80000,
			 0x41400000,
			 0x41d00000,
			 0x41900000,
			 0x41e00000,
			 0x41e80000,

			 /// vpu register f8
			 0x41100000,
			 0x41c80000,
			 0x40c00000,
			 0x41d00000,
			 0x41980000,
			 0x41400000,
			 0x41a80000,
			 0x41100000,

			 /// vpu register f9
			 0x40a00000,
			 0x40800000,
			 0x41300000,
			 0x41e00000,
			 0x3f800000,
			 0x41700000,
			 0x41f00000,
			 0x41300000,

			 /// vpu register f10
			 0x41d80000,
			 0x41a00000,
			 0x41700000,
			 0x40800000,
			 0x40400000,
			 0x41200000,
			 0x40e00000,
			 0x41a00000,

			 /// vpu register f11
			 0x3f800000,
			 0x41e80000,
			 0x40c00000,
			 0x41f00000,
			 0x40400000,
			 0x3f800000,
			 0x41880000,
			 0x3f800000,

			 /// vpu register f12
			 0x40e00000,
			 0x41a80000,
			 0x41400000,
			 0x41400000,
			 0x41700000,
			 0x41880000,
			 0x40000000,
			 0x41c80000,

			 /// vpu register f13
			 0x41500000,
			 0x41e00000,
			 0x3f800000,
			 0x41b00000,
			 0x41c00000,
			 0x41d80000,
			 0x41a00000,
			 0x41c80000,

			 /// vpu register f14
			 0x41100000,
			 0x41800000,
			 0x42000000,
			 0x41a00000,
			 0x41b00000,
			 0x42000000,
			 0x41a80000,
			 0x41d00000,

			 /// vpu register f15
			 0x41000000,
			 0x40e00000,
			 0x41f80000,
			 0x41800000,
			 0x41300000,
			 0x40c00000,
			 0x40400000,
			 0x41b00000,

			 /// vpu register f16
			 0x41c80000,
			 0x40400000,
			 0x41900000,
			 0x41a80000,
			 0x41c00000,
			 0x3f800000,
			 0x41700000,
			 0x40800000,

			 /// vpu register f17
			 0x41100000,
			 0x41100000,
			 0x40a00000,
			 0x3f800000,
			 0x41400000,
			 0x41700000,
			 0x41a00000,
			 0x41a80000,

			 /// vpu register f18
			 0x41100000,
			 0x40000000,
			 0x41600000,
			 0x41e80000,
			 0x41500000,
			 0x41a80000,
			 0x41200000,
			 0x40e00000,

			 /// vpu register f19
			 0x40000000,
			 0x41400000,
			 0x41880000,
			 0x41880000,
			 0x41500000,
			 0x41200000,
			 0x41500000,
			 0x41c00000,

			 /// vpu register f20
			 0x41980000,
			 0x40800000,
			 0x41c00000,
			 0x41900000,
			 0x40800000,
			 0x40400000,
			 0x41c00000,
			 0x41f80000,

			 /// vpu register f21
			 0x40000000,
			 0x40e00000,
			 0x40c00000,
			 0x41f00000,
			 0x41d80000,
			 0x41d00000,
			 0x41600000,
			 0x3f800000,

			 /// vpu register f22
			 0x41980000,
			 0x41900000,
			 0x41e80000,
			 0x41800000,
			 0x3f800000,
			 0x41980000,
			 0x40c00000,
			 0x41600000,

			 /// vpu register f23
			 0x41100000,
			 0x40e00000,
			 0x41e80000,
			 0x41e80000,
			 0x41d80000,
			 0x41a00000,
			 0x41f80000,
			 0x41c80000,

			 /// vpu register f24
			 0x40400000,
			 0x41500000,
			 0x40000000,
			 0x40800000,
			 0x42000000,
			 0x41c80000,
			 0x41f00000,
			 0x41000000,

			 /// vpu register f25
			 0x41f00000,
			 0x41700000,
			 0x41e00000,
			 0x41100000,
			 0x41c80000,
			 0x41300000,
			 0x40400000,
			 0x40400000,

			 /// vpu register f26
			 0x40800000,
			 0x41f00000,
			 0x41900000,
			 0x40e00000,
			 0x41980000,
			 0x40a00000,
			 0x41a00000,
			 0x41800000,

			 /// vpu register f27
			 0x41980000,
			 0x40400000,
			 0x41900000,
			 0x41600000,
			 0x41d00000,
			 0x40400000,
			 0x41d00000,
			 0x41d80000,

			 /// vpu register f28
			 0x41500000,
			 0x41200000,
			 0x41a80000,
			 0x41a00000,
			 0x41f00000,
			 0x41f00000,
			 0x41e00000,
			 0x41000000,

			 /// vpu register f29
			 0x41980000,
			 0x41d80000,
			 0x41f80000,
			 0x41b80000,
			 0x40400000,
			 0x40c00000,
			 0x41b00000,
			 0x41800000,

			 /// vpu register f30
			 0x41100000,
			 0x41880000,
			 0x41f80000,
			 0x41e00000,
			 0x41a00000,
			 0x41e80000,
			 0x41c00000,
			 0x41500000,

			 /// vpu register f31
			 0x41800000,
			 0x41d00000,
			 0x41200000,
			 0x40a00000,
			 0x41a80000,
			 0x41900000,
			 0x41700000,
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
		"fsgnjx.s f28, f7, f27\n"                             ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f15, f12, f14\n"                            ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f4, f13, f16\n"                             ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f21, f11, f18\n"                            ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f17, f29, f15\n"                            ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f19, f21, f26\n"                            ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f13, f3, f13\n"                             ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f29, f26, f24\n"                            ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f26, f30, f19\n"                            ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f15, f30, f24\n"                            ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f18, f14, f12\n"                            ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f29, f23, f10\n"                            ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f14, f9, f25\n"                             ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f26, f8, f8\n"                              ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f23, f25, f29\n"                            ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f18, f16, f18\n"                            ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f23, f29, f1\n"                             ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f17, f26, f24\n"                            ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f28, f20, f22\n"                            ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f18, f17, f8\n"                             ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f21, f15, f20\n"                            ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f25, f19, f0\n"                             ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f9, f15, f22\n"                             ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f29, f30, f12\n"                            ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f2, f10, f11\n"                             ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f14, f24, f2\n"                             ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f20, f4, f27\n"                             ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f26, f7, f21\n"                             ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f25, f8, f3\n"                              ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f17, f3, f28\n"                             ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f20, f0, f20\n"                             ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f5, f20, f7\n"                              ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f15, f20, f3\n"                             ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f6, f3, f5\n"                               ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f17, f23, f11\n"                            ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f6, f10, f7\n"                              ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f14, f22, f24\n"                            ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f7, f23, f7\n"                              ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f18, f15, f1\n"                             ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f17, f25, f8\n"                             ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f19, f1, f1\n"                              ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f10, f7, f1\n"                              ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f29, f3, f8\n"                              ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f17, f23, f7\n"                             ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f25, f1, f17\n"                             ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f22, f2, f4\n"                              ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f17, f21, f22\n"                            ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f19, f3, f8\n"                              ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f30, f10, f25\n"                            ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f21, f19, f17\n"                            ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f28, f14, f21\n"                            ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f17, f29, f10\n"                            ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f5, f23, f11\n"                             ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f9, f15, f27\n"                             ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f30, f7, f24\n"                             ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f4, f7, f20\n"                              ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f29, f0, f2\n"                              ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f25, f15, f28\n"                            ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f26, f14, f5\n"                             ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f13, f20, f26\n"                            ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f22, f9, f20\n"                             ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f18, f22, f19\n"                            ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f28, f2, f18\n"                             ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f4, f9, f19\n"                              ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f11, f20, f20\n"                            ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f16, f14, f24\n"                            ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f27, f30, f9\n"                             ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f12, f12, f4\n"                             ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f7, f25, f6\n"                              ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f21, f21, f0\n"                             ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f19, f16, f9\n"                             ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f24, f11, f27\n"                            ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f24, f0, f27\n"                             ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f15, f1, f16\n"                             ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f24, f15, f22\n"                            ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f25, f9, f12\n"                             ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f6, f1, f23\n"                              ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f27, f19, f13\n"                            ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f3, f26, f18\n"                             ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f2, f18, f26\n"                             ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f2, f25, f21\n"                             ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f8, f5, f11\n"                              ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f8, f0, f6\n"                               ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f10, f26, f1\n"                             ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f19, f16, f17\n"                            ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f28, f18, f26\n"                            ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f22, f25, f22\n"                            ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f30, f21, f18\n"                            ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f10, f27, f5\n"                             ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f17, f22, f19\n"                            ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f11, f16, f20\n"                            ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f7, f19, f12\n"                             ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f20, f1, f6\n"                              ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f30, f2, f29\n"                             ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f28, f23, f14\n"                            ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f20, f24, f20\n"                            ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f22, f14, f14\n"                            ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f18, f23, f13\n"                            ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjx.s f12, f28, f16\n"                            ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
